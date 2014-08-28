//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include <pimpa.h>
#include <npapi/npapi.h>
#include <npapi/npfunctions.h>
#include <npapi/npruntime.h>

PA_EXPORT(NPError)
NPP_New (NPMIMEType type, NPP plugin, uint16_t mode, int16_t argc, char* argn[], char* argv[], NPSavedData* saved)
{
	(void) mode;
	(void) saved;

	NPN_SetValue(plugin, NPPVpluginWindowBool, false);

	void* object = PA_New(type, argc, argn, argv);

	if (object == NULL) {
		return NPERR_GENERIC_ERROR;
	}

	plugin->pdata = object;

	return NPERR_NO_ERROR;
}

PA_EXPORT(NPError)
NPP_Destroy (NPP plugin, NPSavedData** saved)
{
	(void) saved;

	if (PA_Destroy(plugin->pdata)) {
		return NPERR_NO_ERROR;
	}
	else {
		return NPERR_GENERIC_ERROR;
	}
}

PA_EXPORT(NPError)
NPP_SetWindow (NPP plugin, NPWindow* window)
{
	(void) plugin;
	(void) window;

	return NPERR_GENERIC_ERROR;
}

PA_EXPORT(NPError)
NPP_NewStream (NPP plugin, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype)
{
	(void) plugin;
	(void) type;
	(void) stream;
	(void) seekable;
	(void) stype;

	return NPERR_GENERIC_ERROR;
}

PA_EXPORT(NPError)
NPP_DestroyStream (NPP plugin, NPStream* stream, NPReason reason) {
	(void) plugin;
	(void) stream;
	(void) reason;

  return NPERR_GENERIC_ERROR;
}

PA_EXPORT(int32_t)
NPP_WriteReady (NPP plugin, NPStream* stream)
{
	(void) plugin;
	(void) stream;

  return 0;
}

PA_EXPORT(int32_t)
NPP_Write (NPP plugin, NPStream* stream, int32_t offset, int32_t len, void* buffer)
{
	(void) plugin;
	(void) stream;
	(void) offset;
	(void) len;
	(void) buffer;

  return 0;
}

PA_EXPORT(void)
NPP_StreamAsFile (NPP plugin, NPStream* stream, const char* fname)
{
	(void) plugin;
	(void) stream;
	(void) fname;
}

PA_EXPORT(void)
NPP_Print (NPP plugin, NPPrint* platformPrint)
{
	(void) plugin;
	(void) platformPrint;
}

PA_EXPORT(int16_t)
NPP_HandleEvent (NPP plugin, void* event)
{
	(void) plugin;
	(void) event;

  return 0;
}

PA_EXPORT(void)
NPP_URLNotify (NPP plugin, const char* url, NPReason reason, void* notifyData)
{
	(void) plugin;
	(void) url;
	(void) reason;
	(void) notifyData;
}

PA_EXPORT(NPError)
NPP_GetValue (NPP plugin, NPPVariable type, void* value)
{
	LOG("NPP_GetValue: %d\n", type);

	switch (type) {
		case NPPVpluginScriptableNPObject: {
			NPObject* object = PA_Interface(plugin);
			NPN_RetainObject(object);

			*((NPObject**) value) = object;
		} break;

		default:
			return NPERR_INVALID_PARAM;
	}

  return NPERR_GENERIC_ERROR;
}

PA_EXPORT(NPError)
NPP_SetValue (NPP plugin, NPNVariable variable, void* value)
{
	(void) plugin;
	(void) variable;
	(void) value;

  return NPERR_GENERIC_ERROR;
}
