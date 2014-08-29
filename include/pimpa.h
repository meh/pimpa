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

#if defined(XP_UNIX)
/* GCC 3.3 and later support the visibility attribute. */
#	if defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#		define PA_VISIBILITY_DEFAULT __attribute__((visibility("default")))
#	elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#		define PA_VISIBILITY_DEFAULT __global
#	else
#		define PA_VISIBILITY_DEFAULT
#	endif
#
#	define PA_EXPORT(__type) PA_VISIBILITY_DEFAULT __type
#elif defined(XP_WIN)
#	define PA_EXPORT(__type) __type __declspec(dllexport)
#endif

typedef struct PAMetadata {
	const char* name;
	const char* description;
	const char* version;
	const char* mime;
} PAMetadata;

PA_EXPORT(PAMetadata*) PA_Metadata (void);

PA_EXPORT(bool) PA_Initialize (void);
PA_EXPORT(bool) PA_Shutdown (void);

PA_EXPORT(void*) PA_New (const char* mime, int argc, char* argn[], char* argv[]);
PA_EXPORT(bool) PA_Destroy (void* self);

PA_EXPORT(NPObject*) PA_Interface (NPP plugin);

PA_EXPORT(NPObject*) PA_Object (NPClass* klass, void* data);
PA_EXPORT(void*) PA_Private (NPObject* object);
PA_EXPORT(void*) PA_Self (NPP plugin);

#endif
