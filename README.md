# @microsoft/typescript-etw

**@microsoft/typescript-etw** is a native Node.js module for logging ETW events for the TypeScript project.

## Compatibility

ETW is a Windows technology, so this module only works on Windows (Win7 or later). The module is built using the N-API interface and works on Node.js 8.9 or later. (See https://nodejs.org/docs/v8.9.0/api/n-api.html)

## Installing

Install from npm via: `npm install @microsoft/typescript-etw`

Since this dependency only supports Windows, consider using the `--save-optional` flag to add this as an optional dependency or `--no-save` to omit this as a listed dependency altogether.

The module will build from source using node-gyp.

If you would like to take advantage of types for this package information, install them from DefinitelyTyped via: `npm install @types/microsoft__typescript-etw --save-dev`

This will allow you to compile your code with types even if the optional dependency was not installed.

## Usage

This module is designed to be used as an optional dependency. Require it inside of a try-catch block in order to handle the situation where it is not found. For example:

```javascript
var etw;
try {
    etw = require('typescript-etw');
} catch (e) {
    // Optional package not found
    etw = undefined;
}
```

Then you can log events conditionally:

```javascript
if (etw) etw.logInfoEvent('message');
```

## Misc

See the `notes.txt` file for details on the implementation, TODOs, etc.

## Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.opensource.microsoft.com.

When you submit a pull request, a CLA bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., status check, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.
