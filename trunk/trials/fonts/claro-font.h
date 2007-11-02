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

#ifndef CLARO_GRAPHICS_FONT_H
#define CLARO_GRAPHICS_FONT_H

#include <claro/base.h>
#include <claro/graphics.h>

#include <cairo/cairo.h>

// This header file is really a mess and will be rewritten with the new object system.
// And the internal crap will be pulled out of course.

#define CLARO_FLOATING 0x80000000
#define claro_type_HEAD  volatile unsigned int _ref_count; void (* _destroy_func) (void *);

#define claro_type_init(obj, destroy_func) do { (obj)->_ref_count = CLARO_FLOATING; (obj)->_destroy_func = destroy_func; } while(0)

#define claro_type_ref(obj) do { if((obj)->_ref_count & CLARO_FLOATING) { (obj)->_ref_count = 1; } else { (obj)->_ref_count++; } } while(0)

#define claro_type_unref(obj) do { if((obj)->_ref_count == CLARO_FLOATING || (obj)->_ref_count == 1) { if((obj)->_destroy_func) { (obj)->_destroy_func((obj)); } sfree((obj)); } else { (obj)->_ref_count--; } } while(0)

#define cFontSlantNormal 0
#define cFontSlantItalic 1
#define cFontSlantOblique 1

#define cFontWeightNormal 0
#define cFontWeightBold 1

#define cFontDecorationNormal 0
#define cFontDecorationUnderline 1

// all objects to support properties 
    
typedef struct
{
    claro_type_HEAD
    
    void * native;
} claro_font_pattern_t;

// This will be a total mess on Gtk+ because of Pango's over-complicated design.
// Basically we will have to just assume Gtk+ is using FreeType.
typedef struct 
{
    claro_type_HEAD

    // an already matched FcPattern, a ATSUFontId, or a HFONT
    // for Pango, this will be the PangoFont* - then an FT_Face will be pulled out
    void * native;
} claro_font_t;

typedef struct
{
    claro_type_HEAD

    void * native;
} claro_fontset_t;

typedef void claro_fontset_foreach_func(claro_font_t * font, void * arg);

CLFEXP void claro_fonts_init();
CLFEXP void claro_fonts_cleanup();

CLFEXP claro_list_t * claro_get_font_families();

    // Attempts to load the font described by the arguments given. Possibly returning a similar font, or NULL. All can be missing except for family. It will also take the language into account.
CLFEXP claro_font_t * claro_load_font(claro_font_pattern_t * pattern, const char * lang_id);

CLFEXP claro_fontset_t * claro_load_fontset(claro_font_pattern_t * pattern, const char * lang_id);

// Takes a Claro font and creates a Cairo font object for use with a Cairo context.
CLFEXP cairo_font_face_t * claro_cairo_font_create(claro_font_t * font);
   
// Sets the widget's font.
CLFEXP bool_t claro_widget_set_font(widget_t * widget, claro_font_t * font);

CLFEXP claro_font_pattern_t * claro_font_get_font_pattern(claro_font_t * font);

//font functions
CLFEXP claro_font_t * claro_font_ref(claro_font_t * font);
    
CLFEXP void claro_font_unref(claro_font_t * font);
    
CLFEXP claro_fontset_t * claro_fontset_ref(claro_fontset_t * fontset);

CLFEXP void claro_fontset_unref(claro_fontset_t * fontset);

//enumeration
CLFEXP int claro_fontset_count(claro_fontset_t * fontset);

CLFEXP claro_font_t * claro_fontset_get_item(claro_fontset_t * fontset, int i);

CLFEXP void claro_fontset_foreach(claro_fontset_t * fontset, claro_fontset_foreach_func * func,
void * arg);

//pattern functions

// allocation
CLFEXP claro_font_pattern_t * claro_font_pattern_create();

// references
CLFEXP claro_font_pattern_t * claro_font_pattern_ref(claro_font_pattern_t * pattern);

CLFEXP void claro_pattern_unref(claro_font_pattern_t * pattern);

// gets - NULL or -1 means missing
CLFEXP const char * claro_font_pattern_get_family(claro_font_pattern_t * pattern);

CLFEXP int claro_font_pattern_get_size(claro_font_pattern_t * pattern);
    
CLFEXP int claro_font_pattern_get_weight(claro_font_pattern_t * pattern);

CLFEXP int claro_font_pattern_get_slant(claro_font_pattern_t * pattern);

CLFEXP int claro_font_pattern_get_decoration(claro_font_pattern_t * pattern);

//sets - set NULL or -1 to remove
CLFEXP void claro_font_pattern_set_family(claro_font_pattern_t * pattern, const char * family);

CLFEXP void claro_font_pattern_set_size(claro_font_pattern_t * pattern, int size);
    
CLFEXP void claro_font_pattern_set_weight(claro_font_pattern_t * pattern, int weight);

CLFEXP void claro_font_pattern_set_slant(claro_font_pattern_t * pattern, int slant);

CLFEXP void claro_font_pattern_set_decoration(claro_font_pattern_t * pattern, int decoration);

#endif

