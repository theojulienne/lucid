#ifndef __LUCID_LXML_H__
#define __LUCID_LXML_H__

#include <lbase.hpp>

#if 1 //LT_XML_PARSER_GMARKUP
#include <glib.h>
#endif

class LError;

#ifndef lt_xml_event_func
typedef void lt_xml_event_func(const char * element, const char ** attr_names,
	const char ** attr_values, void * user_data);
#endif

//We'll see how LBase pans out...Not sure if it will make sense to subclass LBase. (Needed for threading)
class LXml: LBase
{
public:
	LXml(lt_xml_event_func * start_func, lt_xml_event_func * end_func, 
		lt_xml_event_func * text_func, void * user_data, void (* val_free_fn) (void *));
	~LXml();

	bool_t ParseBuffer(const char * data, int len, bool_t is_final, LError ** err);
	bool_t ParseFile(const char * file_name, LError ** err);

#if 1 //LT_XML_PARSER_GMARKUP
	static void _gmarkup_parse_start_element(GMarkupParseContext *context,
                          const gchar         *element_name,
                          const gchar        **attribute_names,
                          const gchar        **attribute_values,
                          gpointer             user_data,
                          GError             **error);	
	static void _gmarkup_parse_end_element(GMarkupParseContext *context,
                          const gchar         *element_name,
                          gpointer             user_data,
                          GError             **error);
	static void _gmarkup_parse_text(GMarkupParseContext *context,
                          const gchar         *text,
                          gsize                text_len,  
                          gpointer             user_data,
                          GError             **error);
private:
	GMarkupParseContext * m_impl;
#else
private:
#endif
	void * m_user_data;	
	lt_xml_event_func * m_start_func;
	lt_xml_event_func * m_end_func;
	lt_xml_event_func * m_text_func;
	void (* m_val_free_fn) (void *);
};

#endif

