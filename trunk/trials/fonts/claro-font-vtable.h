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

#ifndef CLARO_GRAPHICS_FONT_VTABLE_H
#define CLARO_GRAPHICS_FONT_VTABLE_H

#include <claro/base.h>
#include <claro/graphics.h>

#include <cairo/cairo.h>

#include "claro-font.h"

// not an object - just a structure for your crud..
typedef void claro_font_backend_t;

typedef struct
{
    //main functions

    // Initializes the font system and retrieves the list of available font families.
    // A font mapping cache should be created and stored in the backend object.
    // should g_assert on failure. (FUBAR)
    claro_font_backend_t * (* init) ();

    // Cleans up all allocated memory by the font system and unrefs any fonts in the font cache. 
    void (* cleanup) (claro_font_backend_t * backend);

    // Returns a global list of family names in UTF-8. DO NOT MODIFY.
    // They MUST be sorted case-insenstive alphabetically. 
    claro_list_t * (* get_font_families) (claro_font_backend_t * backend);

    // Attempts to load the font described by the arguments given. Possibly returning a similar font, or NULL. All can be missing except for family. It will also take the language into account.
    claro_font_t * (* load_font) (claro_font_backend_t * backend, claro_font_pattern_t * pattern, const char * lang_id);

    claro_fontset_t * (* load_fontset) (claro_font_backend_t * backend, claro_font_pattern_t * pattern, const char * lang_id);

     // Takes a Claro font and creates a Cairo font object for use with a Cairo context.
    cairo_font_face_t * (* cairo_font_create) (claro_font_backend_t * backend, claro_font_t * font);
   
    // Sets the widget's font.
    bool_t (* widget_set_font) (claro_font_backend_t * backend, widget_t * widget, claro_font_t * font);

    claro_font_pattern_t * (* font_get_font_pattern) (claro_font_t * font);

    // enumeration
    int (* fontset_count) (claro_fontset_t * fontset);

    claro_font_t * (* fontset_get_item) (claro_fontset_t * fontset, int i);

    void (* fontset_foreach) (claro_fontset_t * fontset, claro_fontset_foreach_func * func,
void * arg);

    //pattern functions

    // allocation
    claro_font_pattern_t * (* font_pattern_create) ();

    // gets - NULL or -1 means missing
    const char * (* font_pattern_get_family) (claro_font_pattern_t * pattern);

    int (* font_pattern_get_size) (claro_font_pattern_t * pattern);
    
    int (* font_pattern_get_weight) (claro_font_pattern_t * pattern);

    int (* font_pattern_get_slant) (claro_font_pattern_t * pattern);

    int (* font_pattern_get_decoration) (claro_font_pattern_t * pattern);

    //sets - set NULL or -1 to remove
    void (* font_pattern_set_family) (claro_font_pattern_t * pattern, const char * family);

    void (* font_pattern_set_size) (claro_font_pattern_t * pattern, int size);
    
    void (* font_pattern_set_weight) (claro_font_pattern_t * pattern, int weight);

    void (* font_pattern_set_slant) (claro_font_pattern_t * pattern, int slant);

    void (* font_pattern_set_decoration) (claro_font_pattern_t * pattern, int decoration);
    
} cgraphics_font_vtable;

#endif

