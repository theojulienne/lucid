/* Claro Graphics - an abstraction layer for native UI libraries
 * 
 * $Id$
 * 
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * See the LICENSE file for more details.
 */


#ifndef _CLARO_BASE_CONFIG_H
#define _CLARO_BASE_CONFIG_H

#ifdef _MAC
#include "egconf-mac.h"
#include "ac-config-mac.h"

// mac's libc doesn't support environ in libraries directly
#include <crt_externs.h>
#define environ (*_NSGetEnviron())

// this function isn't available on mac, see oscompat.c for basic wrapper of getpwent
int getpwent_r( struct passwd *pwd, char *buffer, int bufsize, struct passwd **result );
#endif

#ifdef _WIN32
#include "egconf-mingw.h"
#include "winconfig.h"
#endif


#endif
