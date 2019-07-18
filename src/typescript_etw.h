#pragma once
#define WINVER 0x0601 // Support Win7 or later
#define UNICODE

// The below intrinsics are needed on debug builds currently
// See https://docs.microsoft.com/en-us/cpp/intrinsics/intrinsics-available-on-all-architectures
#include <string.h>
#pragma intrinsic(memset, wcslen)

#include <Windows.h>
#include <TraceLoggingProvider.h>
#include <TraceLoggingActivity.h>
#include <evntprov.h>
#include <winmeta.h>
#include <node_api.h>

bool LoadNapiFunctions();

// Declare or define the NAPI method pointers?
#ifdef NAPI_IMPL
  #define DECL_NAPI_IMPL(fn_name) decltype(&fn_name) p##fn_name
#else
  #define DECL_NAPI_IMPL(fn_name) extern decltype(&fn_name) p##fn_name
#endif


DECL_NAPI_IMPL(napi_module_register);
DECL_NAPI_IMPL(napi_create_function);
DECL_NAPI_IMPL(napi_set_named_property);
DECL_NAPI_IMPL(napi_get_cb_info);
DECL_NAPI_IMPL(napi_typeof);
DECL_NAPI_IMPL(napi_get_value_string_utf16);
//DECL_NAPI_IMPL(napi_add_env_cleanup_hook); // Not available on Node.js 8

constexpr UINT64 TSSERVER_KEYWORD_PERF = 0x01;

// TraceLogging quickstart: https://msdn.microsoft.com/en-us/library/windows/desktop/dn904627(v=vs.85).aspx
TRACELOGGING_DECLARE_PROVIDER(g_hMyProvider);
// The above basically becomes: extern const TraceLoggingHProvider g_hMyProvider;

namespace typescript_etw {
	void InitEtw();
	void CleanupEtw(void *arg);
	napi_value LogEvent(napi_env env, napi_callback_info args);
	napi_value LogErrEvent(napi_env env, napi_callback_info args);
	napi_value LogPerfEvent(napi_env env, napi_callback_info args);
	napi_value LogInfoEvent(napi_env env, napi_callback_info args);
	napi_value LogStartCommand(napi_env env, napi_callback_info args);
	napi_value LogStopCommand(napi_env env, napi_callback_info args);
	napi_value LogStartUpdateProgram(napi_env env, napi_callback_info args);
	napi_value LogStopUpdateProgram(napi_env env, napi_callback_info args);
	napi_value LogStartUpdateGraph(napi_env env, napi_callback_info args);
	napi_value LogStopUpdateGraph(napi_env env, napi_callback_info args);
	napi_value LogStartResolveModule(napi_env env, napi_callback_info args);
	napi_value LogStopResolveModule(napi_env env, napi_callback_info args);
	napi_value LogStartParseSourceFile(napi_env env, napi_callback_info args);
	napi_value LogStopParseSourceFile(napi_env env, napi_callback_info args);
	napi_value LogStartReadFile(napi_env env, napi_callback_info args);
	napi_value LogStopReadFile(napi_env env, napi_callback_info args);
	napi_value LogStartBindFile(napi_env env, napi_callback_info args);
	napi_value LogStopBindFile(napi_env env, napi_callback_info args);
	napi_value LogStartScheduledOperation(napi_env env, napi_callback_info args);
	napi_value LogStopScheduledOperation(napi_env env, napi_callback_info args);
} // namespace typescript_etw
