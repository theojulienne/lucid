#import <Foundation/Foundation.h>

int main(int argc, char ** argv)
{
	[NSProcessInfo initializeWithArguments:argv count:argc environment:NULL];

	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSMutableDictionary * dict = [NSMutableDictionary dictionaryWithCapacity: 3];

	[dict setObject:[NSNumber numberWithInt:1] forKey:@"one"];
	[dict setObject:[NSNumber numberWithInt:2] forKey:@"two"];
	[dict setObject:[NSNumber numberWithInt:3] forKey:@"three"];

	NSLog(@"one = %@\n", [dict objectForKey:@"one"]);	

	[pool release];
	return 0;
}
