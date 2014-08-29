//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#ifndef _PIMPA_H
#define _PIMPA_H

#ifdef __unix
#define XP_UNIX 1
#endif

#include <npapi/npapi.h>
#include <npapi/npfunctions.h>
#include <npapi/npruntime.h>

typedef struct PAMetadata {
	const char* name;
	const char* description;
	const char* version;
	const char* mime;
} PAMetadata;

PAMetadata* PA_Metadata (void);

bool PA_Initialize (void);
bool PA_Shutdown (void);

void* PA_New (const char* mime, int argc, char* argn[], char* argv[]);
bool PA_Destroy (void* self);

NPObject* PA_Interface (NPP plugin);

NPObject* PA_Object (NPClass* klass, void* data);
void* PA_Private (NPObject* object);
void* PA_Self (NPP plugin);

#endif
