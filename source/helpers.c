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

static void
_deallocate (NPObject* self)
{
	NPN_MemFree(self);
}

static void
_invalidate (NPObject* self)
{
	(void) self;
}

static bool
_hasMethod (NPObject* self, NPIdentifier name)
{
	(void) self;
	(void) name;

	return false;
}

static bool
_invoke (NPObject* self, NPIdentifier name, const NPVariant* argv, uint32_t argc, NPVariant* result)
{
	(void) self;
	(void) name;
	(void) argv;
	(void) argc;
	(void) result;

	return false;
}

static bool
_invokeDefault (NPObject* self, const NPVariant* argv, uint32_t argc, NPVariant* result)
{
	(void) self;
	(void) argv;
	(void) argc;
	(void) result;

	return false;
}

static bool
_hasProperty (NPObject* self, NPIdentifier name)
{
	(void) self;
	(void) name;

	return false;
}

static bool
_getProperty (NPObject* self, NPIdentifier name, NPVariant* result)
{
	(void) self;
	(void) name;
	(void) result;

	return false;
}

static bool
_setProperty (NPObject* self, NPIdentifier name, const NPVariant* value)
{
	(void) self;
	(void) name;
	(void) value;

	return false;
}

static bool
_removeProperty (NPObject* self, NPIdentifier name)
{
	(void) self;
	(void) name;

	return false;
}

static bool
_enumerate (NPObject* self, NPIdentifier** result, uint32_t* count)
{
	(void) self;
	(void) result;
	(void) count;

	return false;
}

static bool
_construct (NPObject* self, const NPVariant* argv, uint32_t argc, NPVariant* result)
{
	(void) self;
	(void) argv;
	(void) argc;
	(void) result;

	return false;
}

NPObject*
PA_Object (NPClass* klass, void* data)
{
	PAObject* object = NPN_MemAlloc(sizeof(PAObject));

	if (klass->structVersion == 0) {
		klass->structVersion = NP_CLASS_STRUCT_VERSION;
	}

	if (!klass->deallocate)     klass->deallocate     = _deallocate;
	if (!klass->invalidate)     klass->invalidate     = _invalidate;
	if (!klass->hasMethod)      klass->hasMethod      = _hasMethod;
	if (!klass->invoke)         klass->invoke         = _invoke;
	if (!klass->invokeDefault)  klass->invokeDefault  = _invokeDefault;
	if (!klass->hasProperty)    klass->hasProperty    = _hasProperty;
	if (!klass->getProperty)    klass->getProperty    = _getProperty;
	if (!klass->setProperty)    klass->setProperty    = _setProperty;
	if (!klass->removeProperty) klass->removeProperty = _removeProperty;
	if (!klass->enumerate)      klass->enumerate      = _enumerate;
	if (!klass->construct)      klass->construct      = _construct;

	object->super._class = klass;
	object->super.referenceCount = 0;

	object->magic = 0xDEADCA7;
	object->data  = data;

	return (NPObject*) object;
}

void*
PA_Private (NPObject* pointer)
{
	PAObject* object = (PAObject*) pointer;

	if (object->magic != 0xDEADCA7) {
		return NULL;
	}

	return object->data;
}

void*
PA_Self (NPP plugin)
{
	return plugin->pdata;
}
