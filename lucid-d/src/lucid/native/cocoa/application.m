#include <Cocoa/Cocoa.h>

#ifndef AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER
void CPSEnableForegroundOperation( ProcessSerialNumber* psn );
#endif

NSAutoreleasePool *arpool = nil;

@interface LucidApplication : NSObject
{
}

- (void)applicationWillFinishLaunching:(NSNotification *) not;
- (void)applicationDidFinishLaunching:(NSNotification *) not;

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender;

@end

@implementation LucidApplication
- (void) applicationWillFinishLaunching: (NSNotification *) not
{
	/* */
}

- (void) applicationDidFinishLaunching: (NSNotification *) not
{
	/* */
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
	NSLog( @"Woof!" );
}
@end

LucidApplication *lapp = nil;

void lucid_native_application_init( )
{
	NSApplicationLoad( );
	
	arpool = [[NSAutoreleasePool alloc] init];
	
	[NSApplication sharedApplication];
	
	[NSApp setMainMenu:[[NSMenu alloc] init]];
	
	lapp = [[LucidApplication alloc] init];
	[NSApp setDelegate: lapp];
	
	// we don't really care about <10.3, but it seemed right to allow the 'new' and 'old' way anyway
#ifdef AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER

	ProcessSerialNumber psn = { 0, kCurrentProcess }; 
	OSStatus returnCode = TransformProcessType(& psn, kProcessTransformToForegroundApplication);
	if( returnCode != 0) {
	    NSLog(@"Could not bring the application to front. Error %d", returnCode);
	}

#else
	ProcessSerialNumber myProc, frProc;
	Boolean sameProc;

	if ( GetFrontProcess( &frProc ) == noErr && GetCurrentProcess( &myProc ) == noErr )
	{
		if ( SameProcess( &frProc, &myProc, &sameProc ) == noErr && !sameProc )
		{
			CPSEnableForegroundOperation( &myProc );
		}
		
		SetFrontProcess( &myProc );
	}
#endif
}

void lucid_native_application_run( )
{
	[NSApp run];
}