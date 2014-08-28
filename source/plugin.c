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
