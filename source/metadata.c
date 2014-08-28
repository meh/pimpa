//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include <npapi/npapi.h>
#include <npapi/npfunctions.h>
#include <npapi/npruntime.h>

#include <pimpa.h>

extern PAMetadata metadata;

PA_EXPORT(char*)
NP_GetPluginVersion (void)
{
	return (char*) metadata.version;
}

PA_EXPORT(const char*)
NP_GetMIMEDescription (void)
{
	return metadata.mime;
}

PA_EXPORT(NPError)
NP_GetValue (void* future, NPPVariable type, void* value)
{
	NPP plugin = (NPP) future;

	switch (type) {
		case NPPVpluginNameString:
			*((char**) value) = (char*) metadata.name;
			break;

		case NPPVpluginDescriptionString:
			*((char**) value) = (char*) metadata.description;
			break;

		case NPPVpluginScriptableNPObject:


		default:
			return NPERR_INVALID_PARAM;
	}

	return NPERR_NO_ERROR;
}
