#import <AppKit/AppKit.h>

void test_correct(NSSpellChecker * checker)
{
	NSString * s1 = [[NSString alloc] initWithUTF8String: "incantation"];
	NSString * s2 = [[NSString alloc] initWithUTF8String: "currse"]; 
	NSRange range = [checker checkSpellingOfString: s1, startingAt: 0];

	//- (NSArray *)guessesForWord:(NSString *)word

}/*	printf("\n\"%s\" is spelled %s\n", s1, enchant_dict_check (dict, s1, strlen(s1)) ? "wrong": "right");
	printf("\"%s\" is spelled %s\n", s2, enchant_dict_check (dict, s2, strlen(s2)) ? "wrong" : "right");
}

static void test_suggestions(NSSpellChecker * checker)
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
*/

int main(int argc, char ** argv)
{
	NSAutoreleasePool * local_pool = [[NSAutoreleasePool alloc] init];

	NSSpellChecker * checker = [NSSpellChecker sharedSpellChecker];

	test_correct(checker);	

	[local_pool release];

	return 0;
}

