#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <claro/base.h>
#include <claro/graphics.h>

#include "claro-font.h"

object_t * w, *l, *c;

static void window_closed( object_t * obj, event_t * event )
{
	claro_shutdown();
}

static void enum_fonts(object_t * l)
{
    claro_list_t * list;
    int i, len;

    list = claro_get_font_families();
    len = claro_list_count(list);
    
    for(i = 0; i < len; i++)
        listbox_append_row(l, (char *)claro_list_get_item(list, i));
}

static void draw_cairo(cairo_t * cr, cairo_font_face_t * font_face)
{    
    cairo_set_font_face(cr, font_face);
    cairo_set_font_size (cr, 90.0);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, 300, 300);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to (cr, 10.0, 135.0);
    cairo_show_text (cr, "Hello");

    cairo_move_to (cr, 70.0, 165.0);
    cairo_text_path (cr, "void");
    cairo_set_source_rgb (cr, 0.5, 0.5, 1);
    cairo_fill_preserve (cr);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_set_line_width (cr, 2.56);
    cairo_stroke (cr);

    /* draw helping lines */
    cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
    cairo_arc (cr, 10.0, 135.0, 5.12, 0, 2*M_PI);
    cairo_close_path (cr);
    cairo_arc (cr, 70.0, 165.0, 5.12, 0, 2*M_PI);
    cairo_fill (cr);
}

static void font_changed(object_t * object, event_t * event)
{
    canvas_redraw(c);
}

static void draw_with_font(object_t * obj, event_t * event)
{
    list_item_t * item;
    const char * str; 
    claro_font_pattern_t * pattern;
    claro_font_t * font;
    cairo_font_face_t * cairo_font;
    cairo_t * cr;

    cr = canvas_get_cairo_context(c);

    if(!cr)
        return;

    item = listbox_get_selected(l);

	if (!item)
		return;

    str = (const char *)item->data[0];

    pattern = claro_font_pattern_create();
    claro_font_pattern_set_family(pattern, str);

    font = claro_load_font(pattern, NULL);    
    if(font == NULL)
    {
       printf("ERROR: Couldn't load font \"%s\".\n", str);      
       return;    
    }

    cairo_font = claro_cairo_font_create(font);       
    draw_cairo(cr, cairo_font);
    cairo_font_face_destroy(cairo_font);

    claro_font_unref(font);
    claro_font_pattern_unref(pattern);
}

int main(int argc, char ** argv)
{
    bounds_t *b;

    claro_base_init();
    claro_graphics_init();
    claro_fonts_init();

    w = window_widget_create( 0, new_bounds( 0, 0, 550, 300 ), 0 );
	object_addhandler( w, "destroy", window_closed );
	window_set_title( w, "Font test" );    
    
    l = listbox_widget_create( w, new_bounds( 300, 0, 250, 300 ), 0 );
    object_addhandler(l, "selected", font_changed);

    c = canvas_widget_create(w, new_bounds(0, 0, 300, 300), 0);
    object_addhandler(c, "redraw", draw_with_font);       

    enum_fonts(l);

 //   widget_show(c);
    window_show( w );

    claro_loop();

    claro_fonts_cleanup();
    return 0;
}

