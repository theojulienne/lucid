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


#include <glib.h>

#include "claro-font-vtable.h"

extern cgraphics_font_vtable _cgraphics_font_vtable;
cgraphics_font_vtable * _claro_font_vtable = &_cgraphics_font_vtable;

// Not sure how I want to handle this yet..
static claro_font_backend_t * _backend = NULL;

CLFEXP void claro_fonts_init()
{
    if(_backend == NULL)
        _backend = _claro_font_vtable->init();
    g_assert(_backend != NULL);
}

CLFEXP void claro_fonts_cleanup()
{
    _claro_font_vtable->cleanup(_backend);
    _backend = NULL;
}

CLFEXP claro_list_t * claro_get_font_families()
{
   return _claro_font_vtable->get_font_families(_backend); 
}

CLFEXP claro_font_t * claro_load_font(claro_font_pattern_t * pattern, const char * lang_id)
{
    return _claro_font_vtable->load_font(_backend, pattern, lang_id);
}

CLFEXP claro_fontset_t * claro_load_fontset(claro_font_pattern_t * pattern, const char * lang_id)
{
    return _claro_font_vtable->load_fontset(_backend, pattern, lang_id);
}

// Takes a Claro font and creates a Cairo font object for use with a Cairo context.
CLFEXP cairo_font_face_t * claro_cairo_font_create(claro_font_t * font)
{
    return _claro_font_vtable->cairo_font_create(_backend, font);
}
   
// Sets the widget's font.
CLFEXP bool_t claro_widget_set_font(widget_t * widget, claro_font_t * font)
{
    return _claro_font_vtable->widget_set_font(_backend, widget, font);
}

CLFEXP claro_font_pattern_t * claro_font_get_font_pattern(claro_font_t * font)
{
    return _claro_font_vtable->font_get_font_pattern(font);
}

//font functions
CLFEXP claro_font_t * claro_font_ref(claro_font_t * font)
{   
    g_return_val_if_fail(font != NULL, NULL);
    claro_type_ref(font);
    return font;
}
    
CLFEXP void claro_font_unref(claro_font_t * font)
{
    g_return_if_fail(font != NULL);
    claro_type_unref(font);
}
    
CLFEXP claro_fontset_t * claro_fontset_ref(claro_fontset_t * fontset)
{
    g_return_val_if_fail(fontset != NULL, NULL);
    claro_type_ref(fontset);
    return fontset;
}

CLFEXP void claro_fontset_unref(claro_fontset_t * fontset)
{
    g_return_if_fail(fontset != NULL);
    claro_type_unref(fontset);
}  

//enumeration
CLFEXP int claro_fontset_count(claro_fontset_t * fontset)
{
    return _claro_font_vtable->fontset_count(fontset);
}

CLFEXP claro_font_t * claro_fontset_get_item(claro_fontset_t * fontset, int i)
{
    return _claro_font_vtable->fontset_get_item(fontset, i);
}

CLFEXP void claro_fontset_foreach(claro_fontset_t * fontset, claro_fontset_foreach_func * func,
void * arg)
{
    _claro_font_vtable->fontset_foreach(fontset, func, arg);
}

//pattern functions

// allocation
CLFEXP claro_font_pattern_t * claro_font_pattern_create()
{
    return _claro_font_vtable->font_pattern_create();
}

// references
CLFEXP claro_font_pattern_t * claro_font_pattern_ref(claro_font_pattern_t * pattern)
{
    g_return_val_if_fail(pattern != NULL, NULL);
    claro_type_ref(pattern);
    return pattern;
}

CLFEXP void claro_font_pattern_unref(claro_font_pattern_t * pattern)
{
    g_return_if_fail(pattern != NULL);
    claro_type_unref(pattern);   
}

// gets - NULL or -1 means missing
CLFEXP const char * claro_font_pattern_get_family(claro_font_pattern_t * pattern)
{
    return _claro_font_vtable->font_pattern_get_family(pattern);
}

CLFEXP int claro_font_pattern_get_size(claro_font_pattern_t * pattern)
{
    return _claro_font_vtable->font_pattern_get_size(pattern);
}
   
CLFEXP int claro_font_pattern_get_weight(claro_font_pattern_t * pattern)
{
    return _claro_font_vtable->font_pattern_get_weight(pattern);
}

CLFEXP int claro_font_pattern_get_slant(claro_font_pattern_t * pattern)
{
    return _claro_font_vtable->font_pattern_get_slant(pattern);
}

CLFEXP int claro_font_pattern_get_decoration(claro_font_pattern_t * pattern)
{
    return _claro_font_vtable->font_pattern_get_decoration(pattern);
}

//sets - set NULL or -1 to remove
CLFEXP void claro_font_pattern_set_family(claro_font_pattern_t * pattern, const char * family)
{
    _claro_font_vtable->font_pattern_set_family(pattern, family);
}

CLFEXP void claro_font_pattern_set_size(claro_font_pattern_t * pattern, int size)
{
    _claro_font_vtable->font_pattern_set_size(pattern, size);
}
    
CLFEXP void claro_font_pattern_set_weight(claro_font_pattern_t * pattern, int weight)
{
    _claro_font_vtable->font_pattern_set_weight(pattern, weight);
}

CLFEXP void claro_font_pattern_set_slant(claro_font_pattern_t * pattern, int slant)
{
    _claro_font_vtable->font_pattern_set_slant(pattern, slant);
}

CLFEXP void claro_font_pattern_set_decoration(claro_font_pattern_t * pattern, int decoration)
{
    _claro_font_vtable->font_pattern_set_decoration(pattern, decoration);
}


