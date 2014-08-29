pimpa
=====
**pima** is library to help develop clean scriptable-only NPAPI plugins in C,
mostly targeted at (Overwolf)[http://overwolf.com].

Callbacks
---------
**pimpa** simplifies the plugin interface reducing it to just a handful of
callbacks dedicated to initializing the plugin and providing the main interface
entry point.

### `PA_Metadata`

`PA_Metadata` is used to retrieve basic information about the plugin to be
provided to the browser.

It should return a `PAMetadata*` type, the struct is composed of 4 fields:

- `name` should contain the name of the plugin, example: `"Test Plugin"`.
- `description` should contain the description of the plugin, example: `"Test Plugin, for real"`.
- `version` should contain the version of the plugin, example: `"1.0.0.0"`.
- `mime` should contain the mime types, for example: `"application/x-tes-plugint:tst:Test Plugin"`

Keep in mind on windows this information has to be provided through a `.rc` file.

### `PA_Initialize`

`PA_Initialize` is called once when the plugin is loaded, here you can do
plugin global initializations.

Remember to return `true` if the initialization was successful, or `false`
otherwise.

### `PA_Shutdown`

`PA_Shutdown` is called when the plugin is unloaded by the browser, cleanup
your mess here.

Again, remember to return `true` if the initialization was successful, or
`false` otherwise.

### `PA_New`

`PA_New` is called when an instance of the plugin is created, each `<embed>`
with the plugin you have will create an instance.

`PA_New` gets passed 4 parameters:

- `const char* mime` the string containing the mime type for the `<embed>`.
- `int argc` the number of attributes of the `<embed>` element
- `char* argn[]` the names of the attributes
- `char* argv[]` the values of the attributes

`PA_New` shall return a pointer to the private data for the instance, returning
`NULL` to mark a failure.

### `PA_Destroy`

`PA_Destroy` is called when the plugin instance is being destroyed, it receives
the private data returned by `PA_New` as parameter, clean it up.

### `PA_Interface`

`PA_Interface` is used to return the main interface entry point, the plugin
instance is passed as parameter and it has to return an `NPObject*`, `NULL`
signifies a failure.

Helpers
-------
There are few helpers to deal with the interface side of the plugin.

### `PA_Object`

`PA_Object` is used to allocate an `NPObject*`, it's usually called from the
`.allocate` method on the `NPClass`.

It takes three parameters:

- `NPClass* klass` the class of the object.
- `NPP plugin` the instance of the plugin.
- `void* data` the private data for the object.

`PA_Object` takes care of allocating enough space for it all and returns a
proper `NPObject*`.

### `PA_Private`

`PA_Private` is used to retrieve the private data from an `NPObject*` created
with `PA_Object`.

### `PA_Plugin`

`PA_Plugin` is used to retrieve the plugin instance from an `NPObject*` created
with `PA_Object`.

### `PA_Self`

`PA_Self` is used to retrieve the private data from a plugin instance (aka the
actual pimpa instance).
