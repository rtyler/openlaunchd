/**
 * main.c - System Starter main
 * Wilfredo Sanchez | wsanchez@opensource.apple.com
 * $Apple$
 **
 * Copyright (c) 1999-2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Portions Copyright (c) 1999 Apple Computer, Inc.  All Rights
 * Reserved.  This file contains Original Code and/or Modifications of
 * Original Code as defined in and that are subject to the Apple Public
 * Source License Version 1.1 (the "License").  You may not use this file
 * except in compliance with the License.  Please obtain a copy of the
 * License at http://www.apple.com/publicsource and read it before using
 * this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON- INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 **/

#include <unistd.h>
#include <crt_externs.h>
#include <fcntl.h>
#include <CoreFoundation/CoreFoundation.h>
#include "Log.h"
#include "SystemStarter.h"

#define LOGINWINDOW_HINT_FILE "/var/run/.systemStarterRunning"

/* Command line options */
int gDebugFlag     = 0;
int gVerboseFlag   = 0;
int gSafeBootFlag  = 0;
int gNoRunFlag     = 0;

static void __attribute__((__noreturn__)) usage(const char *aProgram)
{
    error(CFSTR("usage: %s [-vxdDrqng?] [ <action> [ <item> ] ]\n"
		"\t<action>: action to take (start|stop|restart); default is start\n"
		"\t<item>  : name of item to act on; default is all items\n"
		"options:\n"
		"\t-g: (ignored)\n"
		"\t-r: (ignored)\n"
		"\t-v: verbose (text mode) startup\n"
		"\t-x: safe mode startup\n"
		"\t-d: print debugging output\n"
		"\t-D: print debugging output and dependencies\n"
		"\t-q: be quiet (disable debugging output)\n"
		"\t-n: don't actually perform action on items (pretend mode)\n"
		"\t-?: show this help\n"),
	  aProgram);
    exit(1);
}

int main (int argc, char *argv[])
{
    char* aProgram = argv[0];

    /* Open log facility */
    initLog();

    /**
     * Handle command line.
     **/
    {
        char c;
        while ((c = getopt(argc, argv, "gvxirdDqn?")) != -1) {
            switch (c) {
                case 'v':
                    gVerboseFlag   = 1;
                    break;
                case 'x':
                    gSafeBootFlag = 1;
                    break;
                case 'g':
                case 'r':
                    break;
                case 'd':
                    gDebugFlag    = 1;
                    break;
                case 'D':
                    gDebugFlag    = 2;
                    break;
                case 'q':
                    gDebugFlag    = 0;
                    break;
                case 'n':
                    gNoRunFlag    = 1;
                    break;
                case '?':
                    usage(aProgram);
                    break;
                default:
                    warning(CFSTR("ignoring unknown option '-%c'\n"), c);
                    break;
            }
        }
	argc -= optind;
	argv += optind;
    }

    if (argc > 2) usage(aProgram);

    if (!gNoRunFlag && (getuid() != 0))
      {
        error(CFSTR("you must be root to run %s\n"), aProgram);
        exit(1);
      }

    {
        int    fd, aStatus = 0;
	Action anAction = kActionStart;

	CFStringRef aService = NULL;

        if (argc > 0)
	  {
	    CFStringRef anActionArg =
	      CFStringCreateWithCString(kCFAllocatorDefault, argv[0], kCFStringEncodingUTF8);

	         if (CFEqual(anActionArg, CFSTR("start"  ))) anAction = kActionStart  ;
	    else if (CFEqual(anActionArg, CFSTR("stop"   ))) anAction = kActionStop   ;
	    else if (CFEqual(anActionArg, CFSTR("restart"))) anAction = kActionRestart;
	    else usage(aProgram);

	    CFRelease(anActionArg);
	  }

        if (argc >= 2)
	  aService = CFStringCreateWithCString(kCFAllocatorDefault, argv[1], kCFStringEncodingUTF8);

	if ((fd = open(LOGINWINDOW_HINT_FILE, O_RDWR|O_CREAT, 0666)) >= 0) {
		close(fd);
	} else {
		warning(CFSTR("couldn't create: %s\n"), LOGINWINDOW_HINT_FILE);
	}
					
	if (aService == NULL)
		daemon(0, 0);

	aStatus = system_starter(anAction, aService);

        if (aService) CFRelease(aService);

	exit(aStatus);
    }
}
