//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include <pimpa.h>
#include <stdlib.h>

typedef struct PAObject {
	NPObject super;
	int      magic;
	void*    data;
} PAObject;

PA_EXPORT(NPObject*)
PA_Object (NPClass* klass, void* data)
{
	PAObject* object = malloc(sizeof(PAObject));

	object->super._class = klass;
	object->super.referenceCount = 0;

	object->magic = 0xDEADCA7;
	object->data  = data;

	return (NPObject*) object;
}

PA_EXPORT(void*)
PA_Private (NPObject* pointer)
{
	PAObject* object = (PAObject*) pointer;

	if (object->magic != 0xDEADCA7) {
		return NULL;
	}

	return object->data;
}

PA_EXPORT(void*)
PA_Self (NPP plugin)
{
	return plugin->pdata;
}
