#include <lucid-base.h>

typedef struct
{
	int depth;
} xml_dump_data_t;

static void start_el(const char *el, const char **attr_names, const char ** attr_vals, void * user_data)
{
	int i;
	xml_dump_data_t * dump_data = (xml_dump_data_t *)user_data;
	
	for (i = 0; i < dump_data->depth; i++)
		printf("  ");

	printf("%s", el);

	for (i = 0; attr_names[i]; i ++)
		printf(" %s='%s'", attr_names[i], attr_vals[i]);

	//printf("\n");

	dump_data->depth += 1;
} 

static void end_el(const char *el, const char **attr_names, const char ** attr_vals, void * user_data)
{
	xml_dump_data_t * dump_data = (xml_dump_data_t *)user_data;
	dump_data->depth -= 1;
}

//TODO - Make it print the text properly as well. (Checking for 'text' that is just white space.)
static void text(const char *el, const char **attr_names, const char ** attr_vals, void * user_data)
{
	//xml_dump_data_t * dump_data = (xml_dump_data_t *)user_data;
	if(el)
	{
		if(el[0] != 32 && el[0] != 9 && el[0] != 10)
			printf(" [%s %d]\n", el, el[0]);
		else
			printf("\n");
	}
}

int main(int argc, char ** argv)
{
	xml_dump_data_t * data = g_new0(xml_dump_data_t, 1);
	LXml * xml = lt_xml_create(start_el, end_el, text, data, free);
	lt_xml_parse_file(xml, "foo.xml", NULL);
	lt_base_unref(LT_BASE(xml));	
	return 0;
}

