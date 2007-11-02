/* Claro Graphics - an abstraction layer for native UI libraries
 * 
 * $Id$
 * 
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * See the LICENSE file for more details.
 */

#include "../../src/claro/base/eglib/src/glib.h"

#include <windows.h>

#include <cairo/cairo.h>
#include <cairo/cairo-win32.h>

#include <claro/base.h>
#include <claro/graphics.h>

#include "claro-font-vtable.h"

// I know my ADD error checking here might seem a bit excessive..but ghost bugs waste man hours.


// I should add a hashtable of cached fonts into the system design at some point for performance reasons.
typedef struct
{
	//FcConfig * config;
	claro_list_t * font_families;
} claro_win32_backend;

// helpers

static void _claro_win32_font_pattern_destroy(void * obj)
{
	claro_font_pattern_t * pattern = (claro_font_pattern_t *)obj;
	
	// free native
	g_free( pattern->native );
}

static void _claro_win32_font_destroy(void * obj)
{
	claro_font_t * font = (claro_font_t *)obj;
	
	// free natives
	DeleteObject( (HFONT)font->native );
}

wchar_t * utf8_to_utf16(const char * utf8, int utf8_len)
{
    int len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8,
utf8_len, NULL, 0);
    wchar_t * buf; 
    if(len < 0)
        return NULL;
    buf = malloc(sizeof(wchar_t) * len);
    len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8, -1,
buf, len);
    if(len < 0)
    {
        free(buf);
        return NULL;
    }
    return buf;
}

char * utf16_to_utf8(const wchar_t * utf16, int utf16_len)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, utf16, utf16_len, NULL, 0,
NULL, NULL);
    char * buf;
    if(len < 0)
        return NULL;
    buf = malloc(len);
    len = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, buf, len, NULL,
NULL);
    if(len < 0)
    {
        free(buf);
        return NULL;
    }
    return buf;
}

#define g_utf16_to_utf8(a,b,c,d,e) utf16_to_utf8(a,b)
#define g_utf8_to_utf16(a,b,c,d,e) utf8_to_utf16(a,b)

int CALLBACK _claro_win32_font_family_enum_callback( ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm, DWORD FontType, LPARAM lParam )
{
	claro_win32_backend * backend = (claro_win32_backend *) lParam;
	GError *err = NULL;
	
	unsigned char *str = g_utf16_to_utf8( &lpelf->elfFullName, -1, NULL, NULL, NULL );
	
	//g_print( "(%p) %s\n", err, str );
	claro_list_append(backend->font_families, str);
	
	// no free, because we let list have it.
}

int _claro_win32_list_sort_string(const char * value1, const char * value2, void * user_arg)
{
	printf( "%s ~ %s\n", value1, value2 );
	return g_strcasecmp( value1, value2 );
}

static bool _claro_win32_list_families(claro_win32_backend * backend)
{
	HDC dc = GetDC( NULL );
	
	EnumFontFamilies( dc, NULL, _claro_win32_font_family_enum_callback, (LPARAM)backend );
	
	claro_list_sort( backend->font_families, _claro_win32_list_sort_string, NULL );
	
	return TRUE;
}

#ifndef CLEARTYPE_QUALITY
#define CLEARTYPE_QUALITY 5
#endif

static claro_font_pattern_t * _claro_win32_make_pattern(LOGFONT * desc)
{
	claro_font_pattern_t * pattern;

	pattern = (claro_font_pattern_t *)smalloc(sizeof(claro_font_pattern_t));

	claro_type_init(pattern, _claro_win32_font_pattern_destroy);
	
	if (desc == NULL)
	{
		desc = g_new0( LOGFONT, 1 );
		desc->lfCharSet = DEFAULT_CHARSET;
		desc->lfOutPrecision = OUT_OUTLINE_PRECIS;
		desc->lfClipPrecision = CLIP_DEFAULT_PRECIS;
		desc->lfQuality = ANTIALIASED_QUALITY | CLEARTYPE_QUALITY;
		desc->lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		//desc = [NSFontDescriptor fontDescriptorWithFontAttributes: nil];
	}
	
	pattern->native = (void *)desc;

	return pattern;
}

static claro_font_t * _claro_win32_make_font(LOGFONT * desc)
{
	claro_font_t * font;
	HFONT ft;

	g_assert(desc != NULL);
	
	font = (claro_font_t *)smalloc(sizeof(claro_font_t));
	claro_type_init(font, _claro_win32_font_destroy);

	ft = CreateFontIndirect( desc );
	font->native = (void *)ft;

	return font;
}

// vtable

static claro_font_backend_t * claro_win32_init()
{
	claro_win32_backend * win32_backend;
	gboolean res;

	win32_backend = (claro_win32_backend *)smalloc(sizeof(claro_win32_backend));
	
	win32_backend->font_families = claro_list_create();
	res = _claro_win32_list_families(win32_backend);
	g_assert(res == TRUE);		  

	return (claro_font_backend_t *)win32_backend;
}

static void claro_win32_cleanup(claro_font_backend_t * backend)
{
	int i, len;
	claro_win32_backend * win32_backend = (claro_win32_backend *)backend;
	
	g_return_if_fail(win32_backend != NULL);

	len = claro_list_count(win32_backend->font_families);

	for(i = 0; i < len; i++)
		sfree(claro_list_get_item(win32_backend->font_families, i));

	claro_list_destroy(win32_backend->font_families);

	sfree(win32_backend);
}

static claro_list_t * claro_win32_get_font_families(claro_font_backend_t * backend)
{
	claro_win32_backend * win32_backend = (claro_win32_backend *)backend;
	
	g_return_val_if_fail(win32_backend != NULL, NULL);
	
	return win32_backend->font_families;
}
   
claro_font_t * claro_win32_load_font(claro_font_backend_t * backend, claro_font_pattern_t * pattern, const char * lang_id)
{
	claro_win32_backend * win32_backend = (claro_win32_backend *)backend;
	LOGFONT * font_desc;
	
	claro_font_t * font;	
	
	g_return_val_if_fail(win32_backend != NULL, NULL);
	g_return_val_if_fail(pattern != NULL, NULL);
	// lang_id can be unspecified

	font_desc = (LOGFONT *)pattern->native;

	// FIXME: use lang_id!
   
	font = _claro_win32_make_font(font_desc);
	
	return font;
}

static claro_fontset_t * claro_win32_load_fontset(claro_font_backend_t * backend, claro_font_pattern_t * pattern, const char * lang_id)
{
	//TODO
	return NULL;
}

// Takes a Claro font and creates a Cairo font object for use with a Cairo context.
static cairo_font_face_t * claro_win32_cairo_font_create(claro_font_backend_t * backend, claro_font_t * font)
{
	cairo_font_face_t * font_face;
	
	g_return_val_if_fail(backend != NULL, NULL);
	g_return_val_if_fail(font != NULL, NULL);
	
	font_face = cairo_win32_font_face_create_for_hfont( (HFONT)font->native );
	
	return font_face;
}
   
// Sets the widget's font.
static bool_t claro_win32_set_widget_font(claro_font_backend_t * backend, widget_t * widget, claro_font_t * font)
{
	return TRUE;
	// Not dependent on fontconfig..deal with later.
}

static claro_font_pattern_t * claro_win32_font_get_font_pattern(claro_font_t * font)
{
	claro_font_pattern_t * pattern;

	g_return_val_if_fail(font != NULL, NULL);
	
	//pattern = _claro_win32_make_pattern( fd );
	pattern = NULL;

	return pattern;	   
}
   
static int claro_win32_fontset_count(claro_fontset_t * fontset)
{
	return 0;
	/*FcFontSet * fc_fontset;
	
	g_return_val_if_fail(fontset != NULL, -1);

	fc_fontset = (FcFontSet *)fontset->native;

	return fc_fontset->nfont;*/
}

static claro_font_t * claro_win32_fontset_get_item(claro_fontset_t * fontset, int i)
{
	return NULL;
	/*FcFontSet * fc_fontset;
	FcPattern * fc_pattern;

	g_return_val_if_fail(fontset != NULL, NULL);

	fc_fontset = (FcFontSet *)fontset->native;

	g_return_val_if_fail(i < 0 || i > fc_fontset->nfont, NULL);

	fc_pattern = fc_fontset->fonts[i];	
	
	g_return_val_if_fail(fc_pattern != NULL, NULL);

	return _claro_win32_make_font(fc_pattern);*/
}

static void claro_win32_fontset_foreach(claro_fontset_t * fontset, claro_fontset_foreach_func * foreach_func, void * arg)
{
	/*FcFontSet * fc_fontset;
	int i;

	g_return_if_fail(fontset != NULL);
	g_return_if_fail(foreach_func != NULL);
	
	fc_fontset = (FcFontSet *)fontset->native;

	for(i = 0; i < fc_fontset->nfont; i++)
	{
		FcPattern * fc_pattern = fc_fontset->fonts[i]; 
		foreach_func(_claro_win32_make_font(fc_pattern), arg);
	}*/
}

static claro_font_pattern_t * claro_win32_pattern_create()
{
	return _claro_win32_make_pattern(NULL);
}

static const char * claro_win32_get_family(claro_font_pattern_t * pattern)
{
	/*FcPattern * fc_pattern;
	FcResult res;
	FcChar8 * family;

	g_return_val_if_fail(pattern != NULL, NULL);

	fc_pattern = (FcPattern *)pattern->native;

	res = FcPatternGetString (fc_pattern, FC_FAMILY, 0, &family);

	if(res != FcResultMatch)
		return NULL;
	else
		return (const char *)family;*/
	return "foo";
}

static int claro_win32_get_size(claro_font_pattern_t * pattern)
{
	/*FcPattern * fc_pattern;
	FcResult res;
	double size;

	g_return_val_if_fail(pattern != NULL, -1);

	fc_pattern = (FcPattern *)pattern->native;

	res = FcPatternGetDouble (fc_pattern, FC_SIZE, 0, &size);

	if(res != FcResultMatch)
		return -1;
	else
		return (int)1024 * size;*/
	return -1;
}
	
static int claro_win32_get_weight(claro_font_pattern_t * pattern)
{
	/*FcPattern * fc_pattern;
	FcResult res;
	int weight;

	g_return_val_if_fail(pattern != NULL, -1);

	fc_pattern = (FcPattern *)pattern->native;

	res = FcPatternGetInteger(fc_pattern, FC_WEIGHT, 0, &weight);

	if(res != FcResultMatch)
		return -1;
	else
	{
		if(weight == FC_WEIGHT_NORMAL)
			return cFontWeightNormal;
		else if(weight == FC_WEIGHT_BOLD)
			return cFontWeightBold;
		else
			return -1;
	}*/
	return -1;
}

static int claro_win32_get_slant(claro_font_pattern_t * pattern)
{
	/*FcPattern * fc_pattern;
	FcResult res;
	int slant;

	g_return_val_if_fail(pattern != NULL, -1);

	fc_pattern = (FcPattern *)pattern->native;

	res = FcPatternGetInteger(fc_pattern, FC_SLANT, 0, &slant);

	if(res != FcResultMatch)
		return -1;
	else
	{
		if(slant == FC_SLANT_ROMAN)
			return cFontSlantNormal;
		else if(slant == FC_SLANT_ITALIC)
			return cFontSlantItalic;
		else if(slant == FC_SLANT_OBLIQUE)
			return cFontSlantOblique;
		else
			return -1;
	}	*/
	return -1;
}

static int claro_win32_get_decoration(claro_font_pattern_t * pattern)
{
	g_return_val_if_fail(pattern != NULL, -1);

	return -1;
}

static void claro_win32_set_family(claro_font_pattern_t * pattern, const char * family)
{
	LOGFONT *desc = (LOGFONT *)pattern->native;
	gunichar2 *str = g_utf8_to_utf16( family, strlen(family)>=32?31:strlen(family), NULL, NULL, NULL );
	
	memcpy( desc->lfFaceName, str, sizeof(TCHAR)*32 );
	
	g_free( str );
	/*NSFontDescriptor *old_desc = (NSFontDescriptor *)pattern->native;
	NSFontDescriptor *new_desc;
	NSString *family_str;
	
	family_str = [[NSString alloc] initWithUTF8String:family];
	
	new_desc = [old_desc fontDescriptorWithFamily: family_str];
	
	[family_str release];
	[old_desc release];
	
	pattern->native = (void *)new_desc;*/
}

static void claro_win32_set_size(claro_font_pattern_t * pattern, int size)
{
	/*NSFontDescriptor *old_desc = (NSFontDescriptor *)pattern->native;
	NSFontDescriptor *new_desc;
	
	new_desc = [old_desc fontDescriptorWithSize: size];
	
	[old_desc release];
	
	pattern->native = (void *)new_desc;*/
}
	
static void claro_win32_set_weight(claro_font_pattern_t * pattern, int weight)
{
	// [old_desc fontDescriptorWithSymbolicTraits: [old_desc symbolicTraits]&(0xffffffff^NSFontBoldTrait) | (weight==cFontWeightBold ? NSFontBoldTrait : 0)];
}

static void claro_win32_set_slant(claro_font_pattern_t * pattern, int slant)
{
}

static void claro_win32_set_decoration(claro_font_pattern_t * pattern, int decoration)
{
}


cgraphics_font_vtable _cgraphics_font_vtable =
{
	claro_win32_init,
	claro_win32_cleanup,
	claro_win32_get_font_families,
	claro_win32_load_font,
	claro_win32_load_fontset,
	claro_win32_cairo_font_create,
	claro_win32_set_widget_font,
	claro_win32_font_get_font_pattern,
	claro_win32_fontset_count,
	claro_win32_fontset_get_item,
	claro_win32_fontset_foreach,
	claro_win32_pattern_create,	 
	claro_win32_get_family,
	claro_win32_get_size,
	claro_win32_get_weight,
	claro_win32_get_slant,
	claro_win32_get_decoration,
	claro_win32_set_family,
	claro_win32_set_size,
	claro_win32_set_weight,
	claro_win32_set_slant,
	claro_win32_set_decoration
};



