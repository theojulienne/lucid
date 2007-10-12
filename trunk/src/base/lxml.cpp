#include <lucid-base.h>

// Simple SAX XML Parser - Currently using GMarkupParser.

#define LT_XML_PARSER_BUFF_SIZE 4096

extern "C" 
{

LXml * lt_xml_create(lt_xml_event_func * start_func, lt_xml_event_func * end_func, 
		lt_xml_event_func * text_func, void * user_data, void (* val_free_fn) (void *));
bool_t lt_xml_parse_buffer(LXml * self, const char * data, int len, bool_t is_final, LError ** err);
bool_t lt_xml_parse_file(LXml * self, const char * file_name, LError ** err);

}

#if 0 //LT_XML_PARSER_GMARKUP

void LXml::_gmarkup_parse_start_element(GMarkupParseContext *context,
                          const gchar         *element_name,
                          const gchar        **attribute_names,
                          const gchar        **attribute_values,
                          gpointer             user_data,
                          GError             **error)
{
	g_return_if_fail(user_data != NULL);
	LXml * xml = static_cast<LXml *>(user_data);
	if(xml->m_start_func)
		xml->m_start_func(element_name, attribute_names, attribute_values, xml->m_user_data);	
}

void LXml::_gmarkup_parse_end_element(GMarkupParseContext *context,
                          const gchar         *element_name,
                          gpointer             user_data,
                          GError             **error)
{
	g_return_if_fail(user_data != NULL);
	LXml * xml = static_cast<LXml *>(user_data);
	if(xml->m_end_func)
		xml->m_end_func(element_name, NULL, NULL, xml->m_user_data);
}

void LXml::_gmarkup_parse_text(GMarkupParseContext *context,
                          const gchar         *text,
                          gsize                text_len,  
                          gpointer             user_data,
                          GError             **error)
{
	g_return_if_fail(user_data != NULL);
	char * zero_term_text = NULL;
	LXml * xml = static_cast<LXml *>(user_data);
	//FIXME- Wtf is up with the random white space 'text' callbacks. 
	/*	
	if(g_ascii_isspace(text[0]))
		return;	
	*/
	if(text && text_len > 0)
	{
		//FIXME - This is NOT safe to do, but can be much faster.
		zero_term_text = (char *)g_realloc((void *)text, text_len + 1);
		if(zero_term_text)
			zero_term_text[text_len] = '\0';
		if(xml->m_text_func)
			xml->m_text_func(zero_term_text, NULL, NULL, xml->m_user_data);	
	}
}
   
static GMarkupParser _gmarkup_parser = 
{
	LXml::_gmarkup_parse_start_element, 
	LXml::_gmarkup_parse_end_element, 
	LXml::_gmarkup_parse_text, 
	NULL,
	NULL
};

//Errors will be propogated once you try to do something.
LXml::LXml(lt_xml_event_func * start_func, lt_xml_event_func * end_func, 
		lt_xml_event_func * text_func, void * user_data, void (* val_free_fn) (void *))
{
	this->m_impl = g_markup_parse_context_new(&_gmarkup_parser, 
		(GMarkupParseFlags)0, (void *)this, NULL);
	this->m_user_data = user_data;
	this->m_start_func = start_func;
	this->m_end_func = end_func;
	this->m_text_func = text_func;
	this->m_val_free_fn = val_free_fn;
}

LXml::~LXml()
{
	if(this->m_impl)
		g_markup_parse_context_free(this->m_impl);
	if(this->m_user_data)
		this->m_val_free_fn(this->m_user_data);
		
}

bool_t LXml::ParseBuffer(const char * data, int len, bool_t is_final, LError ** err)
{
	GError * g_err = NULL;
	gboolean ret;

	g_return_val_if_fail(this->m_impl != NULL, FALSE);
	g_return_val_if_fail(data != NULL, FALSE);
	
	ret = g_markup_parse_context_parse(this->m_impl, data, len > 0 ? len : strlen(data), &g_err);
			
	if(! ret)
	{
		//TODO - Set an error, even if GLib doesn't.
		if(err)
			;	
		return FALSE;
	}

	if(is_final)
	{
		ret = g_markup_parse_context_end_parse(this->m_impl, &g_err);

		if(! ret)
		{
			//TODO - Set an error, even if GLib doesn't.
			if(err)
				;	
			return FALSE;
		}
	}
	return TRUE;
}

#elif 1 //LT_XML_PARSER_EXPAT

void LXml::_expat_start_element(void *data, const char *el, const char **attr)
{
	g_return_if_fail(data != NULL);
	LXml * xml = static_cast<LXml *>(data);
	if(xml->m_start_func)
	{
		if(attr)
		{
			LArray<const char *> names(NULL), values(NULL);
			for(int i = 0; attr[i]; i += 2)
			{
				names.Append(attr[i]);
				values.Append(attr[i + 1]);
			}
			names.Append(NULL);
			values.Append(NULL);
			xml->m_start_func(el, names.GetData(), values.GetData(), xml->m_user_data);
		}
		else
			xml->m_start_func(el, NULL, NULL, xml->m_user_data);
	}
}

void LXml::_expat_end_element(void * data, const char * el)
{
	g_return_if_fail(data != NULL);
	LXml * xml = static_cast<LXml *>(data);
	if(xml->m_end_func)
	{
		xml->m_end_func(el, NULL, NULL, xml->m_user_data);
	}
}

void LXml::_expat_text(void * data, const char * text, int text_len)
{
	g_return_if_fail(data != NULL);
	char * zero_term_text = NULL;
	LXml * xml = static_cast<LXml *>(data);
	//FIXME- Wtf is up with the random white space 'text' callbacks. 
		
	/*if(g_ascii_isspace(text[0]))
		return;	
	*/

	if(text && text_len > 0)
	{
		//FIXME - This is NOT safe to do, but can be much faster.
		zero_term_text = g_strndup(text, text_len);
		if(xml->m_text_func)
			xml->m_text_func(zero_term_text, NULL, NULL, xml->m_user_data);	
		if(zero_term_text)
			g_free(zero_term_text);
	}
}

LXml::LXml(lt_xml_event_func * start_func, lt_xml_event_func * end_func, 
		lt_xml_event_func * text_func, void * user_data, void (* val_free_fn) (void *))
{
	this->m_impl = XML_ParserCreate(NULL);
	this->m_user_data = user_data;
	this->m_start_func = start_func;
	this->m_end_func = end_func;
	this->m_text_func = text_func;
	this->m_val_free_fn = val_free_fn;
	if(this->m_impl)
	{
		XML_SetUserData(this->m_impl, (void *)this);
		XML_SetElementHandler(this->m_impl, 
			LXml::_expat_start_element, LXml::_expat_end_element);
		XML_SetCharacterDataHandler(this->m_impl, LXml::_expat_text);
	}
}

LXml::~LXml()
{
	if(this->m_impl)
		XML_ParserFree(this->m_impl);
	if(this->m_user_data)
		this->m_val_free_fn(this->m_user_data);
		
}

bool_t LXml::ParseBuffer(const char * data, int len, bool_t is_final, LError ** err)
{
	g_return_val_if_fail(this->m_impl != NULL, FALSE);
	g_return_val_if_fail(data != NULL, FALSE);
	
	if(! XML_Parse(this->m_impl, data, len, is_final))
	{
		return FALSE;
	}
	else
		return TRUE;
}

#endif

//FIXME - Make me incrementally read the file for performance reasons.
bool_t LXml::ParseFile(const char * file_name, LError ** err)
{
	GError * g_err = NULL;
	char * contents;
	gsize len;
	bool_t ret;

	ret = g_file_get_contents(file_name, &contents, &len, &g_err);

	if(! ret)
	{
		//TODO - Set an error, even if GLib doesn't.
		if(err)
			;	
		return FALSE;	
	}
	ret = this->ParseBuffer((const char *)contents, len, TRUE, err);
	g_free(contents);
	return ret;	
}

LXml * lt_xml_create(lt_xml_event_func * start_func, lt_xml_event_func * end_func, 
		lt_xml_event_func * text_func, void * user_data, void (* val_free_fn) (void *))
{
	LT_NEW_CPP(LXml, start_func, end_func, text_func, user_data, val_free_fn);
}

bool_t lt_xml_parse_buffer(LXml * self, const char * data, int len, bool_t is_final, LError ** err)
{
	LT_RET_CALL_CPP(ParseBuffer, FALSE, data, len, is_final, err);
}

bool_t lt_xml_parse_file(LXml * self, const char * file_name, LError ** err)
{
	LT_RET_CALL_CPP(ParseFile, FALSE, file_name, err);
}

