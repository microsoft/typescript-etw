#include "typescript_etw.h"

/*
 * To keep size & dependencies down, this build has some "interesting" configuration.
 *  - This builds with the /NODEFAULTLIB option (i.e. CRT is unavailable) to exclude all the default library dependencies
 *  - Which means needing to define DllMain to do the work of the CRT version and enter it directly (/ENTRY:DllMain)
 *  - This means basics such as new & delete, or std::string & std::vector need to be implemented if needed
 *  - Also, don't allocate over a page (4kb) of locals in a function, else you'll get an unresolved external symbol for _chkstk
 *    - See https://stackoverflow.com/questions/7420927/unresolved-symbols-when-linking-without-c-runtime-with-vs2010
 *  - This also requires knowing how some of the funky ETW and Node.js macros work to avoid potentical issues:
 *    - See details on NAPI_MODULE below. This is mimiced in the process attach event in DllMain to initialize
 *    - ETW initialization (TRACELOGGING_DEFINE_PROVIDER) _appears_ to be a constant expression, (so hopefully does require CRT initialization)
 *  - Exceptions and runtime type information are also disabled for simplicity and size.
 *
 * Result: A skeleton native module compiled is around 15kb, instead of around 100kb when statically linking in the CRT.
 */

namespace typescript_etw {

	// TODO: Is this safe if initialized multiple times in a process? May need to ensure InitEtw is only called once.
	// See https://nodejs.org/dist/latest-v10.x/docs/api/addons.html#addons_context_aware_addons
	napi_value Init(napi_env env, napi_value exports) {
		InitEtw();
		
		auto register_fn = [&](const char *name, napi_callback cb) {
			napi_value fn;
			napi_status status;
			status = pnapi_create_function(env, name, 0, cb, nullptr, &fn);
			if (status == napi_ok) {
				status = pnapi_set_named_property(env, exports, name, fn);
			}
			return status == napi_ok;
		};

		// Short-circuiting evaluation for any failures
		// Note: napi_add_env_cleanup_hook requires Node.js 10.2 or later. See https://nodejs.org/dist/latest-v10.x/docs/api/n-api.html#n_api_napi_add_env_cleanup_hook
		// Commented out cleanup hook for now, as VS Code uses an Electron which uses Node 8.9.3 (files at https://nodejs.org/dist/v8.9.3/)
		bool success = /* pnapi_add_env_cleanup_hook(env, CleanupEtw, nullptr) == napi_ok && */
			register_fn("logEvent", LogEvent) &&
			register_fn("logStartCommand", LogStartCommand) &&
			register_fn("logStopCommand", LogStopCommand) &&
			register_fn("logStartUpdateProgram", LogStartUpdateProgram) &&
			register_fn("logStopUpdateProgram", LogStopUpdateProgram) &&
			register_fn("logStartUpdateGraph", LogStartUpdateGraph) &&
			register_fn("logStopUpdateGraph", LogStopUpdateGraph) &&
			register_fn("logStartResolveModule", LogStartResolveModule) &&
			register_fn("logStopResolveModule", LogStopResolveModule) &&
			register_fn("logStartParseSourceFile", LogStartParseSourceFile) &&
			register_fn("logStopParseSourceFile", LogStopParseSourceFile) &&
			register_fn("logStartReadFile", LogStartReadFile) &&
			register_fn("logStopReadFile", LogStopReadFile) &&
			register_fn("logStartBindFile", LogStartBindFile) &&
			register_fn("logStopBindFile", LogStopBindFile) &&
			register_fn("logStartScheduledOperation", LogStartScheduledOperation) &&
			register_fn("logStopScheduledOperation", LogStopScheduledOperation) &&
			register_fn("logErrEvent", LogErrEvent) &&
			register_fn("logInfoEvent", LogInfoEvent) &&
			register_fn("logPerfEvent", LogPerfEvent);

		if (!success) return nullptr; // TODO: Set exception? Just fails silently for now (but would likely crash if called).

		return exports;
	}

	// NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

	/* The above usual macro expands in the preprocessor to the below. Allocating the registration 
	 * function directly in the C-runtime initialization section (".CRT$XCU") is a little "awkward", 
	 * and does make it hard to build native add-ons that don't link against the CRT.

	extern "C" {
		static napi_module _module = 
		{ 
			1, 
			0, 
			"c:\\src\\github\\typescript-etw\\main.cc", 
			Init, 
			"typescript-etw", 
			0, 
	        {0}, 
		}; 
		static void __cdecl _register_typescript_etw(void); 
		__declspec(dllexport, allocate(".CRT$XCU")) void(__cdecl * _register_typescript_etw_)(void) = _register_typescript_etw;
		static void __cdecl _register_typescript_etw(void) { 
			napi_module_register(&_module);
		}
	}

	*/

	// Do the work the macro would if we were using the C-runtime library to initialize (avoiding about 75KB of binary)
	extern "C" {
		static napi_module _module;

		bool WINAPI DllMain(
			_In_ HINSTANCE hinstDLL,
			_In_ DWORD     fdwReason,
			_In_ LPVOID    lpvReserved) {

			if (fdwReason == DLL_PROCESS_ATTACH)
			{
				LoadNapiFunctions(); // Custom "delay load" implementation of the Node functions.
				_module = {
					NAPI_MODULE_VERSION,
					0,
					__FILE__,
					Init,
					"typescript-etw",
					0,
					{0},
				};

				pnapi_module_register(&_module);
				return true;
			}
		}
	}
}  // namespace typescript_etw
