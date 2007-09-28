#include <lucid-base.h>

static void start_el(const char *el, const char **attr_names, const char ** attr_vals, void * user_data)
{
	int i, * depth = (int *)user_data;
	
	for (i = 0; i < * depth; i++)
		printf("  ");

	printf("%s", el);

	for (i = 0; attr_names[i]; i ++)
		printf(" %s='%s'", attr_names[i], attr_vals[i]);

	printf("\n");

	* depth += 1;
} 

static void end_el(const char *el, const char **attr_names, const char ** attr_vals, void * user_data)
{
	* (int *)user_data -= 1;
}

//TODO - Make it print the text properly as well. (Checking for 'text' that is just white space.)
static void text(const char *el, const char **attr_names, const char ** attr_vals, void * user_data)
{
}

int main(int argc, char ** argv)
{
	LXml * xml = lt_xml_create(start_el, end_el, text, malloc(sizeof(int)), free);
	lt_xml_parse_file(xml, "foo.xml", NULL);
	lt_base_unref(LT_BASE(xml));	
	return 0;
}

