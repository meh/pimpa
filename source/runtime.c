//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include <pimpa.h>
#include <stdlib.h>
#include <stddef.h>

#define MAJOR_VERSION(x) ((((uint32_t) (x)) & 0xff00) >> 8)
#define MINOR_VERSION(x) ((((uint32_t) (x)) & 0x00ff) >> 8)

static NPNetscapeFuncs* _browser = NULL;

static NPError
_browser_initialize (NPNetscapeFuncs* browser)
{
	if (browser == NULL) {
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	if (MAJOR_VERSION(browser->version) > NP_VERSION_MAJOR) {
		return NPERR_INCOMPATIBLE_VERSION_ERROR;
	}

	if (browser->size < sizeof(_browser)) {
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	_browser = browser;

	return NPERR_NO_ERROR;
}

static NPError
_plugin_initialize (NPPluginFuncs* plugin)
{
	if (MAJOR_VERSION(plugin->version) > NP_VERSION_MAJOR) {
		return NPERR_INCOMPATIBLE_VERSION_ERROR;
	}

	if (plugin->size < (offsetof(NPPluginFuncs, setvalue) + sizeof(void*))) {
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	plugin->newp          = NPP_New;
	plugin->destroy       = NPP_Destroy;
	plugin->setwindow     = NPP_SetWindow;
	plugin->newstream     = NPP_NewStream;
	plugin->destroystream = NPP_DestroyStream;
	plugin->asfile        = NPP_StreamAsFile;
	plugin->writeready    = NPP_WriteReady;
	plugin->write         = NPP_Write;
	plugin->print         = NPP_Print;
	plugin->event         = NPP_HandleEvent;
	plugin->urlnotify     = NPP_URLNotify;
	plugin->getvalue      = NPP_GetValue;
	plugin->setvalue      = NPP_SetValue;

	return NPERR_NO_ERROR;
}

#ifdef XP_UNIX
	PA_EXPORT(NPError)
	NP_Initialize (NPNetscapeFuncs* browser, NPPluginFuncs* plugin)
	{
		LOG("NP_Initialize: called\n");

		{
			NPError error = _browser_initialize(browser);

			if (error != NPERR_NO_ERROR) {
				return error;
			}
		}

		{
			NPError error = _plugin_initialize(plugin);

			if (error != NPERR_NO_ERROR) {
				return error;
			}
		}

		{
			if (!PA_Initialize()) {
				return NPERR_GENERIC_ERROR;
			}
		}

		return NPERR_NO_ERROR;
	}
#else
	PA_EXPORT(NPError)
	NP_Initialize (NPNetscapeFuncs* browser)
	{
		{
			NPError error = _initialize(browser);

			if (error != NPERR_NO_ERROR) {
				return error;
			}
		}

		{
			if (!PA_Initialize()) {
				return NPERR_GENERIC_ERROR;
			}
		}

		return NPERR_NO_ERROR;
	}
#endif

PA_EXPORT(NPError)
NP_Shutdown (void)
{
	LOG("NP_Shutdown: called\n");

	if (PA_Shutdown()) {
		return NPERR_NO_ERROR;
	}

	return NPERR_GENERIC_ERROR;
}

PA_EXPORT(char*)
NP_GetPluginVersion (void)
{
	LOG("NP_GetPluginVersion: %s\n", PA_Metadata()->version);

	return (char*) PA_Metadata()->version;
}

PA_EXPORT(const char*)
NP_GetMIMEDescription (void)
{
	LOG("NP_GetMIMEDescription: %s\n", PA_Metadata()->mime);

	return PA_Metadata()->mime;
}

PA_EXPORT(NPError)
NP_GetValue (void* plugin, NPPVariable type, void* value)
{
	(void) plugin;

	LOG("NP_GetValue: %d\n", type);

	switch (type) {
		case NPPVpluginNameString:
			*((char**) value) = (char*) PA_Metadata()->name;
			break;

		case NPPVpluginDescriptionString:
			*((char**) value) = (char*) PA_Metadata()->description;
			break;

		case NPPVpluginWindowBool:
			*((NPBool*) value) = false;
			break;

		default:
			return NPERR_INVALID_PARAM;
	}

	return NPERR_NO_ERROR;
}

NPError
NPN_GetURL (NPP instance, const char* url, const char* window)
{
	return _browser->geturl(instance, url, window);
}

NPError
NPN_PostURL (NPP instance, const char* url, const char* window, uint32_t len, const char* buf, NPBool file)
{
	return _browser->posturl(instance, url, window, len, buf, file);
}

NPError
NPN_RequestRead (NPStream* stream, NPByteRange* rangeList)
{
	return _browser->requestread(stream, rangeList);
}

NPError
NPN_NewStream (NPP instance, NPMIMEType type, const char* window, NPStream** stream)
{
	return _browser->newstream(instance, type, window, stream);
}

int32_t
NPN_Write (NPP instance, NPStream* stream, int32_t len, void* buffer)
{
	return _browser->write(instance, stream, len, buffer);
}

NPError
NPN_DestroyStream (NPP instance, NPStream* stream, NPReason reason)
{
	return _browser->destroystream(instance, stream, reason);
}

void
NPN_Status (NPP instance, const char* message)
{
	return _browser->status(instance, message);
}

const char*
NPN_UserAgent (NPP instance)
{
	return _browser->uagent(instance);
}

void*
NPN_MemAlloc (uint32_t size)
{
	return _browser->memalloc(size);
}

void
NPN_MemFree (void* ptr)
{
	_browser->memfree(ptr);
}

uint32_t
NPN_MemFlush (uint32_t size)
{
	return _browser->memflush(size);
}

void
NPN_ReloadPlugins (NPBool reloadPages)
{
	_browser->reloadplugins(reloadPages);
}

NPError
NPN_GetURLNotify (NPP instance, const char* url, const char* window, void* notifyData)
{
	return _browser->geturlnotify(instance, url, window, notifyData);
}

NPError
NPN_PostURLNotify (NPP instance, const char* url, const char* window, uint32_t len, const char* buf, NPBool file, void* notifyData)
{
	return _browser->posturlnotify(instance, url, window, len, buf, file, notifyData);
}

NPError
NPN_GetValue (NPP instance, NPNVariable variable, void* ret_value)
{
	return _browser->getvalue(instance, variable, ret_value);
}

NPError
NPN_SetValue (NPP instance, NPPVariable variable, void* value)
{
	return _browser->setvalue(instance, variable, value);
}

void
NPN_InvalidateRect (NPP instance, NPRect* rect)
{
	_browser->invalidaterect(instance, rect);
}

void
NPN_InvalidateRegion (NPP instance, NPRegion region)
{
	_browser->invalidateregion(instance, region);
}

void
NPN_ForceRedraw (NPP instance)
{
	_browser->forceredraw(instance);
}

NPIdentifier
NPN_GetStringIdentifier (const NPUTF8* name)
{
	return _browser->getstringidentifier(name);
}

void
NPN_GetStringIdentifiers (const NPUTF8** names, int32_t nameCount, NPIdentifier* identifiers)
{
	_browser->getstringidentifiers(names, nameCount, identifiers);
}

NPIdentifier
NPN_GetIntIdentifier (int32_t intid)
{
	return _browser->getintidentifier(intid);
}

bool
NPN_IdentifierIsString (NPIdentifier identifier)
{
	return _browser->identifierisstring(identifier);
}

NPUTF8*
NPN_UTF8FromIdentifier (NPIdentifier identifier)
{
	return _browser->utf8fromidentifier(identifier);
}

int32_t
NPN_IntFromIdentifier (NPIdentifier identifier)
{
	return _browser->intfromidentifier(identifier);
}

NPObject*
NPN_CreateObject (NPP instance, NPClass* klass)
{
	return _browser->createobject(instance, klass);
}

NPObject*
NPN_RetainObject (NPObject* obj)
{
	return _browser->retainobject(obj);
}

void
NPN_ReleaseObject (NPObject* obj)
{
	_browser->releaseobject(obj);
}

bool
NPN_Invoke (NPP instance, NPObject* obj, NPIdentifier methodName, const NPVariant* args, uint32_t argCount, NPVariant* result)
{
	return _browser->invoke(instance, obj, methodName, args, argCount, result);
}

bool
NPN_InvokeDefault (NPP instance, NPObject* obj, const NPVariant* args, uint32_t argCount, NPVariant* result)
{
	return _browser->invokeDefault(instance, obj, args, argCount, result);
}

bool
NPN_Evaluate (NPP instance, NPObject* obj, NPString* script, NPVariant* result)
{
	return _browser->evaluate(instance, obj, script, result);
}

bool
NPN_GetProperty (NPP instance, NPObject* obj, NPIdentifier propertyName, NPVariant* result)
{
	return _browser->getproperty(instance, obj, propertyName, result);
}

bool
NPN_SetProperty (NPP instance, NPObject* obj, NPIdentifier propertyName, const NPVariant* value)
{
	return _browser->setproperty(instance, obj, propertyName, value);
}

bool
NPN_RemoveProperty (NPP instance, NPObject* obj, NPIdentifier propertyName)
{
	return _browser->removeproperty(instance, obj, propertyName);
}

bool
NPN_HasProperty (NPP instance, NPObject* obj, NPIdentifier propertyName)
{
	return _browser->hasproperty(instance, obj, propertyName);
}

bool
NPN_HasMethod (NPP instance, NPObject* obj, NPIdentifier propertyName)
{
	return _browser->hasmethod(instance, obj, propertyName);
}

void
NPN_ReleaseVariantValue (NPVariant* variant)
{
	_browser->releasevariantvalue(variant);
}

void
NPN_SetException (NPObject* obj, const NPUTF8* message)
{
	_browser->setexception(obj, message);
}

void
NPN_PushPopupsEnabledState (NPP instance, NPBool enabled)
{
	_browser->pushpopupsenabledstate(instance, enabled);
}

void
NPN_PopPopupsEnabledState (NPP instance)
{
	_browser->poppopupsenabledstate(instance);
}

bool
NPN_Enumerate (NPP instance, NPObject* obj, NPIdentifier* *identifier, uint32_t* count)
{
	return _browser->enumerate(instance, obj, identifier, count);
}

void
NPN_PluginThreadAsyncCall (NPP instance, void (*func)(void* ), void* userData)
{
	_browser->pluginthreadasynccall(instance, func, userData);
}

bool
NPN_Construct (NPP instance, NPObject* obj, const NPVariant* args, uint32_t argCount, NPVariant* result)
{
	return _browser->construct(instance, obj, args, argCount, result);
}

NPError
NPN_GetValueForURL (NPP instance, NPNURLVariable variable, const char* url, char* *value, uint32_t* len)
{
	return _browser->getvalueforurl(instance, variable, url, value, len);
}

NPError
NPN_SetValueForURL (NPP instance, NPNURLVariable variable, const char* url, const char* value, uint32_t len)
{
	return _browser->setvalueforurl(instance, variable, url, value, len);
}

NPError
NPN_GetAuthenticationInfo (NPP instance, const char* protocol, const char* host, int32_t port, const char* scheme, const char* realm, char* *username, uint32_t* ulen, char* *password, uint32_t* plen)
{
	return _browser->getauthenticationinfo(instance, protocol, host, port, scheme, realm, username, ulen, password, plen);
}

uint32_t
NPN_ScheduleTimer (NPP instance, uint32_t interval, NPBool repeat, void (*timerFunc)(NPP instance, uint32_t timerID))
{
	return _browser->scheduletimer(instance, interval, repeat, timerFunc);
}

void
NPN_UnscheduleTimer (NPP instance, uint32_t timerID)
{
	_browser->unscheduletimer(instance, timerID);
}

NPError
NPN_PopUpContextMenu (NPP instance, NPMenu* menu)
{
	return _browser->popupcontextmenu(instance, menu);
}

NPBool
NPN_ConvertPoint (NPP instance, double sourceX, double sourceY, NPCoordinateSpace sourceSpace, double* destX, double* destY, NPCoordinateSpace destSpace)
{
	return _browser->convertpoint(instance, sourceX, sourceY, sourceSpace, destX, destY, destSpace);
}

NPBool
NPN_HandleEvent (NPP instance, void* event, NPBool handled)
{
	return _browser->handleevent(instance, event, handled);
}

NPBool
NPN_UnfocusInstance (NPP instance, NPFocusDirection direction)
{
	return _browser->unfocusinstance(instance, direction);
}

void
NPN_URLRedirectResponse (NPP instance, void* notifyData, NPBool allow)
{
	_browser->urlredirectresponse(instance, notifyData, allow);
}

NPError
NPN_InitAsyncSurface (NPP instance, NPSize* size, NPImageFormat format, void* initData, NPAsyncSurface* surface)
{
	return _browser->initasyncsurface(instance, size, format, initData, surface);
}

NPError
NPN_FinalizeAsyncSurface (NPP instance, NPAsyncSurface* surface)
{
	return _browser->finalizeasyncsurface(instance, surface);
}

void
NPN_SetCurrentAsyncSurface (NPP instance, NPAsyncSurface* surface, NPRect* changed)
{
	_browser->setcurrentasyncsurface(instance, surface, changed);
}
