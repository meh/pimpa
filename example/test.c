//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include <pimpa.h>
#include <stdlib.h>

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

	Private* self = (Private*) malloc(sizeof(Private));
	self->number = 42;

	return self;
}

PA_EXPORT(bool)
PA_Destroy (void* pointer)
{
	free(pointer);

	return true;
}

struct NPClass Class = {
	.allocate = Class_allocate
};

PA_EXPORT(NPObject*)
PA_Interface (NPP plugin)
{
	NPObject* object = NPN_CreateObject(plugin, &Class);
	NPN_RetainObject(object);

	return object;
}
