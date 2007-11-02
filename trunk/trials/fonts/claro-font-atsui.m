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

#include <Carbon/Carbon.h>
#include <Cocoa/Cocoa.h>

#include <cairo/cairo.h>
#include <cairo/cairo-atsui.h>

#include <claro/base.h>
#include <claro/graphics.h>

#include "claro-font-vtable.h"

// I know my ADD error checking here might seem a bit excessive..but ghost bugs waste man hours.


// I should add a hashtable of cached fonts into the system design at some point for performance reasons.
typedef struct
{
	//FcConfig * config;
	claro_list_t * font_families;
} claro_atsui_backend;

// helpers

static void _claro_atsui_font_pattern_destroy(void * obj)
{
	claro_font_pattern_t * pattern = (claro_font_pattern_t *)obj;
	NSFontDescriptor *fdesc = (NSFontDescriptor *)pattern->native;

	[fdesc release];
}

static void _claro_atsui_font_destroy(void * obj)
{
	claro_font_t * font = (claro_font_t *)obj;
	NSFont *fnt = (NSFont *)font->native;

	[fnt release];
}

static bool _claro_atsui_list_families(claro_atsui_backend * backend)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSFontManager *fontManager = [NSFontManager sharedFontManager];
	NSArray *fontFamilies = [fontManager availableFontFamilies];
	int i;
	
	NSArray *sortedArray = [fontFamilies sortedArrayUsingSelector:@selector(caseInsensitiveCompare:)];
	
	for ( i = 0; i < [sortedArray count]; i++ )
	{
		NSString *s = [sortedArray objectAtIndex: i];
		claro_list_append(backend->font_families, sstrdup([s UTF8String]));
	}
	
	[pool release];
	
	return TRUE;
}

static claro_font_pattern_t * _claro_atsui_make_pattern(NSFontDescriptor * desc)
{
	claro_font_pattern_t * pattern;

	pattern = (claro_font_pattern_t *)smalloc(sizeof(claro_font_pattern_t));

	claro_type_init(pattern, _claro_atsui_font_pattern_destroy);
	
	if (desc == NULL)
	{
		desc = [NSFontDescriptor fontDescriptorWithFontAttributes: nil];
	}
	
	pattern->native = (void *)desc;

	/*if(fc_pattern == NULL)
		pattern->native = (void *)FcPatternCreate();	
	else
	{
		FcPatternReference(fc_pattern);
		pattern->native = (void *)fc_pattern;
	}*/

	return pattern;
}

static claro_font_t * _claro_atsui_make_font(NSFontDescriptor * desc)
{
	claro_font_t * font;
	NSFont *ft;

	g_assert(desc != NULL);
	
	font = (claro_font_t *)smalloc(sizeof(claro_font_t));
	claro_type_init(font, _claro_atsui_font_destroy);

	ft = [NSFont fontWithDescriptor: desc size: [desc pointSize]];
	font->native = (void *)ft;

	return font;
}

// vtable

static claro_font_backend_t * claro_atsui_init()
{
	claro_atsui_backend * atsui_backend;
	gboolean res;

	atsui_backend = (claro_atsui_backend *)smalloc(sizeof(claro_atsui_backend));

	//g_assert(FcInit());

	/*atsui_backend->config = FcConfigGetCurrent();
	g_assert(atsui_backend->config != NULL);*/
	
	atsui_backend->font_families = claro_list_create();
	res = _claro_atsui_list_families(atsui_backend);
	g_assert(res == TRUE);		  

	return (claro_font_backend_t *)atsui_backend;
}

static void claro_atsui_cleanup(claro_font_backend_t * backend)
{
	int i, len;
	claro_atsui_backend * atsui_backend = (claro_atsui_backend *)backend;
	
	g_return_if_fail(atsui_backend != NULL);

	len = claro_list_count(atsui_backend->font_families);

	for(i = 0; i < len; i++)
		sfree(claro_list_get_item(atsui_backend->font_families, i));

	claro_list_destroy(atsui_backend->font_families);

	sfree(atsui_backend);  

	//FcFini(); 
}

static claro_list_t * claro_atsui_get_font_families(claro_font_backend_t * backend)
{
	claro_atsui_backend * atsui_backend = (claro_atsui_backend *)backend;
	
	g_return_val_if_fail(atsui_backend != NULL, NULL);
	
	return atsui_backend->font_families;
}
   
claro_font_t * claro_atsui_load_font(claro_font_backend_t * backend, claro_font_pattern_t * pattern, const char * lang_id)
{
	claro_atsui_backend * atsui_backend = (claro_atsui_backend *)backend;
	NSFontDescriptor * font_desc;
	
	claro_font_t * font;	
	
	g_return_val_if_fail(atsui_backend != NULL, NULL);
	g_return_val_if_fail(pattern != NULL, NULL);
	// lang_id can be unspecified

	font_desc = (NSFontDescriptor *)pattern->native;

	// FIXME: use lang_id!
   
	font = _claro_atsui_make_font(font_desc);

	return font;
}

static claro_fontset_t * claro_atsui_load_fontset(claro_font_backend_t * backend, claro_font_pattern_t * pattern, const char * lang_id)
{
	//TODO
	return NULL;
}

// Takes a Claro font and creates a Cairo font object for use with a Cairo context.
static cairo_font_face_t * claro_atsui_cairo_font_create(claro_font_backend_t * backend, claro_font_t * font)
{
	cairo_font_face_t * font_face;
	NSFont *ft;

	g_return_val_if_fail(backend != NULL, NULL);
	g_return_val_if_fail(font != NULL, NULL);
	
	ft = (NSFont *)font->native;
	
	font_face = cairo_atsui_font_face_create_for_atsu_font_id( [ft _atsFontID] );
	/*font_face = cairo_ft_font_face_create_for_pattern
		((FcPattern *)((claro_font_pattern_t *)font->native)->native);	 */

	return font_face;
}
   
// Sets the widget's font.
static bool_t claro_atsui_set_widget_font(claro_font_backend_t * backend, widget_t * widget, claro_font_t * font)
{
	return TRUE;
	// Not dependent on fontconfig..deal with later.
}

static claro_font_pattern_t * claro_atsui_font_get_font_pattern(claro_font_t * font)
{
	claro_font_pattern_t * pattern;
	NSFont *ft;
	NSFontDescriptor *fd;

	g_return_val_if_fail(font != NULL, NULL);
	
	ft = (NSFont *)font->native;
	fd = [ft fontDescriptor];

	pattern = _claro_atsui_make_pattern( fd );

	return pattern;	   
}
   
static int claro_atsui_fontset_count(claro_fontset_t * fontset)
{
	return 0;
	/*FcFontSet * fc_fontset;
	
	g_return_val_if_fail(fontset != NULL, -1);

	fc_fontset = (FcFontSet *)fontset->native;

	return fc_fontset->nfont;*/
}

static claro_font_t * claro_atsui_fontset_get_item(claro_fontset_t * fontset, int i)
{
	return NULL;
	/*FcFontSet * fc_fontset;
	FcPattern * fc_pattern;

	g_return_val_if_fail(fontset != NULL, NULL);

	fc_fontset = (FcFontSet *)fontset->native;

	g_return_val_if_fail(i < 0 || i > fc_fontset->nfont, NULL);

	fc_pattern = fc_fontset->fonts[i];	
	
	g_return_val_if_fail(fc_pattern != NULL, NULL);

	return _claro_atsui_make_font(fc_pattern);*/
}

static void claro_atsui_fontset_foreach(claro_fontset_t * fontset, claro_fontset_foreach_func * foreach_func, void * arg)
{
	/*FcFontSet * fc_fontset;
	int i;

	g_return_if_fail(fontset != NULL);
	g_return_if_fail(foreach_func != NULL);
	
	fc_fontset = (FcFontSet *)fontset->native;

	for(i = 0; i < fc_fontset->nfont; i++)
	{
		FcPattern * fc_pattern = fc_fontset->fonts[i]; 
		foreach_func(_claro_atsui_make_font(fc_pattern), arg);
	}*/
}

static claro_font_pattern_t * claro_atsui_pattern_create()
{
	return _claro_atsui_make_pattern(NULL);
}

static const char * claro_atsui_get_family(claro_font_pattern_t * pattern)
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

static int claro_atsui_get_size(claro_font_pattern_t * pattern)
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
	
static int claro_atsui_get_weight(claro_font_pattern_t * pattern)
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

static int claro_atsui_get_slant(claro_font_pattern_t * pattern)
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

static int claro_atsui_get_decoration(claro_font_pattern_t * pattern)
{
	g_return_val_if_fail(pattern != NULL, -1);

	return -1;
}

static void claro_atsui_set_family(claro_font_pattern_t * pattern, const char * family)
{
	NSFontDescriptor *old_desc = (NSFontDescriptor *)pattern->native;
	NSFontDescriptor *new_desc;
	NSString *family_str;
	
	family_str = [[NSString alloc] initWithUTF8String:family];
	
	new_desc = [old_desc fontDescriptorWithFamily: family_str];
	
	[family_str release];
	[old_desc release];
	
	pattern->native = (void *)new_desc;
}

static void claro_atsui_set_size(claro_font_pattern_t * pattern, int size)
{
	NSFontDescriptor *old_desc = (NSFontDescriptor *)pattern->native;
	NSFontDescriptor *new_desc;
	
	new_desc = [old_desc fontDescriptorWithSize: size];
	
	[old_desc release];
	
	pattern->native = (void *)new_desc;
}
	
static void claro_atsui_set_weight(claro_font_pattern_t * pattern, int weight)
{
	// [old_desc fontDescriptorWithSymbolicTraits: [old_desc symbolicTraits]&(0xffffffff^NSFontBoldTrait) | (weight==cFontWeightBold ? NSFontBoldTrait : 0)];
}

static void claro_atsui_set_slant(claro_font_pattern_t * pattern, int slant)
{
}

static void claro_atsui_set_decoration(claro_font_pattern_t * pattern, int decoration)
{
}


cgraphics_font_vtable _cgraphics_font_vtable =
{
	claro_atsui_init,
	claro_atsui_cleanup,
	claro_atsui_get_font_families,
	claro_atsui_load_font,
	claro_atsui_load_fontset,
	claro_atsui_cairo_font_create,
	claro_atsui_set_widget_font,
	claro_atsui_font_get_font_pattern,
	claro_atsui_fontset_count,
	claro_atsui_fontset_get_item,
	claro_atsui_fontset_foreach,
	claro_atsui_pattern_create,	 
	claro_atsui_get_family,
	claro_atsui_get_size,
	claro_atsui_get_weight,
	claro_atsui_get_slant,
	claro_atsui_get_decoration,
	claro_atsui_set_family,
	claro_atsui_set_size,
	claro_atsui_set_weight,
	claro_atsui_set_slant,
	claro_atsui_set_decoration
};



