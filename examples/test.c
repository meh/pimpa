//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include <pimpa.h>
#include <stdlib.h>

PAMetadata*
PA_Metadata (void)
{
	static PAMetadata meta = {
		.name        = "Test Plugin",
		.description = "Test Plugin, for real.",
		.version     = "1.0.0.0",
		.mime        = "application/x-test-plugin:tst:Test Plugin",
	};

	return &meta;
}

typedef struct Private {
	int number;
} Private;

PA_EXPORT(bool)
PA_Initialize (void)
{
	return true;
}

PA_EXPORT(bool)
PA_Shutdown (void)
{
	return true;
}

PA_EXPORT(void*)
PA_New (const char* mime, int argc, char* argn[], char* argv[])
{
	(void) mime;
	(void) argc;
	(void) argn;
	(void) argv;

	Private* self = (Private*) NPN_MemAlloc(sizeof(Private));
	self->number  = 42;

	return self;
}

PA_EXPORT(bool)
PA_Destroy (void* pointer)
{
	NPN_MemFree(pointer);

	return true;
}

NPObject*
Class_Allocate (NPP plugin, NPClass* klass)
{
	return PA_Object(klass, PA_Self(plugin));
}

bool
Class_HasMethod (NPObject* self, NPIdentifier name)
{
	(void) self;
	(void) name;

	return false;
}

bool
Class_HasProperty (NPObject* self, NPIdentifier name)
{
	(void) self;
	(void) name;

	return true;
}

bool
Class_GetProperty (NPObject* self, NPIdentifier name, NPVariant* result)
{
	(void) name;

	Private* data = PA_Private(self);
	INT32_TO_NPVARIANT(data->number, *result);

	return true;
}

bool
Class_InvokeDefault (NPObject* self, const NPVariant* args, uint32_t count, NPVariant* result)
{
	(void) args;
	(void) count;

	Private* data = PA_Private(self);
	INT32_TO_NPVARIANT(data->number, *result);

	return true;
}

struct NPClass Class = {
	.allocate      = Class_Allocate,
	.hasMethod     = Class_HasMethod,
	.hasProperty   = Class_HasProperty,
	.getProperty   = Class_GetProperty,
	.invokeDefault = Class_InvokeDefault,
};

PA_EXPORT(NPObject*)
PA_Interface (NPP plugin)
{
	return NPN_CreateObject(plugin, &Class);
}
