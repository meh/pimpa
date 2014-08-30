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

	plugin->newp              = NPP_New;
	plugin->destroy           = NPP_Destroy;
	plugin->setwindow         = NPP_SetWindow;
	plugin->newstream         = NPP_NewStream;
	plugin->destroystream     = NPP_DestroyStream;
	plugin->asfile            = NPP_StreamAsFile;
	plugin->writeready        = NPP_WriteReady;
	plugin->write             = NPP_Write;
	plugin->print             = NPP_Print;
	plugin->event             = NPP_HandleEvent;
	plugin->urlnotify         = NPP_URLNotify;
	plugin->getvalue          = NPP_GetValue;
	plugin->setvalue          = NPP_SetValue;
	plugin->gotfocus          = NPP_GotFocus;
	plugin->lostfocus         = NPP_LostFocus;
	plugin->urlredirectnotify = NPP_URLRedirectNotify;
	plugin->clearsitedata     = NPP_ClearSiteData;
	plugin->getsiteswithdata  = NPP_GetSitesWithData;
	plugin->didComposite      = NPP_DidComposite;

	return NPERR_NO_ERROR;
}

#if defined(XP_UNIX)
	NPError
	NP_Initialize (NPNetscapeFuncs* browser, NPPluginFuncs* plugin)
	{
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
#elif defined(XP_WIN)
	NPError OSCALL
	NP_Initialize (NPNetscapeFuncs* browser)
	{
		{
			NPError error = _browser_initialize(browser);

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

	NPError OSCALL
	NP_GetEntryPoints (NPPluginFuncs* plugin)
	{
		{
			NPError error = _plugin_initialize(plugin);

			if (error != NPERR_NO_ERROR) {
				return error;
			}
		}

		return NPERR_NO_ERROR;
	}
#endif

NPError OSCALL
NP_Shutdown (void)
{
	if (PA_Shutdown()) {
		return NPERR_NO_ERROR;
	}

	return NPERR_GENERIC_ERROR;
}

char*
NP_GetPluginVersion (void)
{
	return (char*) PA_Metadata()->version;
}

const char*
NP_GetMIMEDescription (void)
{
	return PA_Metadata()->mime;
}

NPError
NP_GetValue (void* plugin, NPPVariable type, void* value)
{
	(void) plugin;

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
NPN_GetURL (NPP plugin, const char* url, const char* window)
{
	return _browser->geturl(plugin, url, window);
}

NPError
NPN_PostURL (NPP plugin, const char* url, const char* window, uint32_t len, const char* buf, NPBool file)
{
	return _browser->posturl(plugin, url, window, len, buf, file);
}

NPError
NPN_RequestRead (NPStream* stream, NPByteRange* rangeList)
{
	return _browser->requestread(stream, rangeList);
}

NPError
NPN_NewStream (NPP plugin, NPMIMEType type, const char* window, NPStream** stream)
{
	return _browser->newstream(plugin, type, window, stream);
}

int32_t
NPN_Write (NPP plugin, NPStream* stream, int32_t len, void* buffer)
{
	return _browser->write(plugin, stream, len, buffer);
}

NPError
NPN_DestroyStream (NPP plugin, NPStream* stream, NPReason reason)
{
	return _browser->destroystream(plugin, stream, reason);
}

void
NPN_Status (NPP plugin, const char* message)
{
	_browser->status(plugin, message);
}

const char*
NPN_UserAgent (NPP plugin)
{
	return _browser->uagent(plugin);
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
NPN_GetURLNotify (NPP plugin, const char* url, const char* window, void* notifyData)
{
	return _browser->geturlnotify(plugin, url, window, notifyData);
}

NPError
NPN_PostURLNotify (NPP plugin, const char* url, const char* window, uint32_t len, const char* buf, NPBool file, void* notifyData)
{
	return _browser->posturlnotify(plugin, url, window, len, buf, file, notifyData);
}

NPError
NPN_GetValue (NPP plugin, NPNVariable variable, void* ret_value)
{
	return _browser->getvalue(plugin, variable, ret_value);
}

NPError
NPN_SetValue (NPP plugin, NPPVariable variable, void* value)
{
	return _browser->setvalue(plugin, variable, value);
}

void
NPN_InvalidateRect (NPP plugin, NPRect* rect)
{
	_browser->invalidaterect(plugin, rect);
}

void
NPN_InvalidateRegion (NPP plugin, NPRegion region)
{
	_browser->invalidateregion(plugin, region);
}

void
NPN_ForceRedraw (NPP plugin)
{
	_browser->forceredraw(plugin);
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
NPN_CreateObject (NPP plugin, NPClass* klass)
{
	return _browser->createobject(plugin, klass);
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
NPN_Invoke (NPP plugin, NPObject* obj, NPIdentifier methodName, const NPVariant* args, uint32_t argCount, NPVariant* result)
{
	return _browser->invoke(plugin, obj, methodName, args, argCount, result);
}

bool
NPN_InvokeDefault (NPP plugin, NPObject* obj, const NPVariant* args, uint32_t argCount, NPVariant* result)
{
	return _browser->invokeDefault(plugin, obj, args, argCount, result);
}

bool
NPN_Evaluate (NPP plugin, NPObject* obj, NPString* script, NPVariant* result)
{
	return _browser->evaluate(plugin, obj, script, result);
}

bool
NPN_GetProperty (NPP plugin, NPObject* obj, NPIdentifier propertyName, NPVariant* result)
{
	return _browser->getproperty(plugin, obj, propertyName, result);
}

bool
NPN_SetProperty (NPP plugin, NPObject* obj, NPIdentifier propertyName, const NPVariant* value)
{
	return _browser->setproperty(plugin, obj, propertyName, value);
}

bool
NPN_RemoveProperty (NPP plugin, NPObject* obj, NPIdentifier propertyName)
{
	return _browser->removeproperty(plugin, obj, propertyName);
}

bool
NPN_HasProperty (NPP plugin, NPObject* obj, NPIdentifier propertyName)
{
	return _browser->hasproperty(plugin, obj, propertyName);
}

bool
NPN_HasMethod (NPP plugin, NPObject* obj, NPIdentifier propertyName)
{
	return _browser->hasmethod(plugin, obj, propertyName);
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
NPN_PushPopupsEnabledState (NPP plugin, NPBool enabled)
{
	_browser->pushpopupsenabledstate(plugin, enabled);
}

void
NPN_PopPopupsEnabledState (NPP plugin)
{
	_browser->poppopupsenabledstate(plugin);
}

bool
NPN_Enumerate (NPP plugin, NPObject* obj, NPIdentifier* *identifier, uint32_t* count)
{
	return _browser->enumerate(plugin, obj, identifier, count);
}

void
NPN_PluginThreadAsyncCall (NPP plugin, void (*func)(void* ), void* userData)
{
	_browser->pluginthreadasynccall(plugin, func, userData);
}

bool
NPN_Construct (NPP plugin, NPObject* obj, const NPVariant* args, uint32_t argCount, NPVariant* result)
{
	return _browser->construct(plugin, obj, args, argCount, result);
}

NPError
NPN_GetValueForURL (NPP plugin, NPNURLVariable variable, const char* url, char* *value, uint32_t* len)
{
	return _browser->getvalueforurl(plugin, variable, url, value, len);
}

NPError
NPN_SetValueForURL (NPP plugin, NPNURLVariable variable, const char* url, const char* value, uint32_t len)
{
	return _browser->setvalueforurl(plugin, variable, url, value, len);
}

NPError
NPN_GetAuthenticationInfo (NPP plugin, const char* protocol, const char* host, int32_t port, const char* scheme, const char* realm, char* *username, uint32_t* ulen, char* *password, uint32_t* plen)
{
	return _browser->getauthenticationinfo(plugin, protocol, host, port, scheme, realm, username, ulen, password, plen);
}

uint32_t
NPN_ScheduleTimer (NPP plugin, uint32_t interval, NPBool repeat, void (*timerFunc)(NPP plugin, uint32_t timerID))
{
	return _browser->scheduletimer(plugin, interval, repeat, timerFunc);
}

void
NPN_UnscheduleTimer (NPP plugin, uint32_t timerID)
{
	_browser->unscheduletimer(plugin, timerID);
}

NPError
NPN_PopUpContextMenu (NPP plugin, NPMenu* menu)
{
	return _browser->popupcontextmenu(plugin, menu);
}

NPBool
NPN_ConvertPoint (NPP plugin, double sourceX, double sourceY, NPCoordinateSpace sourceSpace, double* destX, double* destY, NPCoordinateSpace destSpace)
{
	return _browser->convertpoint(plugin, sourceX, sourceY, sourceSpace, destX, destY, destSpace);
}

NPBool
NPN_HandleEvent (NPP plugin, void* event, NPBool handled)
{
	return _browser->handleevent(plugin, event, handled);
}

NPBool
NPN_UnfocusInstance (NPP plugin, NPFocusDirection direction)
{
	return _browser->unfocusinstance(plugin, direction);
}

void
NPN_URLRedirectResponse (NPP plugin, void* notifyData, NPBool allow)
{
	_browser->urlredirectresponse(plugin, notifyData, allow);
}

NPError
NPN_InitAsyncSurface (NPP plugin, NPSize* size, NPImageFormat format, void* initData, NPAsyncSurface* surface)
{
	return _browser->initasyncsurface(plugin, size, format, initData, surface);
}

NPError
NPN_FinalizeAsyncSurface (NPP plugin, NPAsyncSurface* surface)
{
	return _browser->finalizeasyncsurface(plugin, surface);
}

void
NPN_SetCurrentAsyncSurface (NPP plugin, NPAsyncSurface* surface, NPRect* changed)
{
	_browser->setcurrentasyncsurface(plugin, surface, changed);
}
