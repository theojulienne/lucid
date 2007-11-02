#include <string.h>

#include <glib.h>
#include <enchant/enchant.h>

/*
	We will add the UI for the Enchant-based spell checking plugin using just Lucid code- because its 
	main target will be the X11 port, but it is just as usable on the two other unix ports. (Cocoa and GTK.)
*/

static void _describe_cb(const char * const lang_tag,
				       const char * const provider_name,
				       const char * const provider_desc,
				       const char * const provider_file,
				       void * user_data)
{
	g_print("%s: %s [%s]\n", provider_name, provider_desc, lang_tag);
}

static void test_correct(EnchantDict * dict)
{
	const char * s1 = "incantation";
	const char * s2 = "currse"; 
	g_print("\n\"%s\" is spelled %s\n", s1, enchant_dict_check (dict, s1, strlen(s1)) ? "wrong": "right");
	g_print("\"%s\" is spelled %s\n", s2, enchant_dict_check (dict, s2, strlen(s2)) ? "wrong" : "right");
}

static void test_suggestions(EnchantDict * dict)
{
	const char * s1 = "incorecrtly";
	size_t n_sugg;
	char ** suggs = enchant_dict_suggest(dict, s1, strlen(s1), &n_sugg);
	int i;

	g_print("\nSuggestions for \"%s\":\n", s1);

	for(i = 0; i < n_sugg; i++)
		g_print("%s\n", suggs[0]);

	g_strfreev(suggs);
}

int main(int argc, char ** argv)
{
	EnchantBroker * broker = enchant_broker_init();
	EnchantDict * dict;

	g_print("Available dictionaries: \n\n");

	//I don't even think this feature is neccessary. (?)
	enchant_broker_list_dicts(broker, _describe_cb, NULL);
	
	dict = enchant_broker_request_dict(broker, "en_US");
	
	if(! dict)
		g_error("%s\n", enchant_broker_get_error(broker));

	test_correct(dict);
	test_suggestions(dict);

	enchant_broker_free_dict(broker, dict);
	enchant_broker_free(broker);
	return 0;
}

