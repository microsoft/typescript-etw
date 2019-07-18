{
  "targets": [
    {
      "target_name": "typescript-etw",
      "sources": [
        "src/crt.cc",
        "src/main.cc",
        "src/typescript_etw.cc",
        "src/win_delay_load_hook.cc"
      ],
      "win_delay_load_hook": "false", # We have a custom win_delay_load_hook, so don't include the default one
      "msvs_settings": {
        "VCCLCompilerTool": {
          "BufferSecurityCheck": "false", # Disabling this since it depends on CRT
          "DebugInformationFormat": 3,    # Generate a PDB
          "ExceptionHandling": 0,         # No C++ exception handling
          "RuntimeTypeInfo": "false"      # No runtime types
        },
        "VCLinkerTool": {
          "EntryPointSymbol": "DllMain",       # /ENTRY:DllMain (see notes in main.cc)
          "IgnoreAllDefaultLibraries": "true", # /NODEFAULTLIB (see notes in main.cc)
          "ImageHasSafeExceptionHandlers": "false"
        }
      }
    },
    {
        "target_name": "copy_binary",
        "type":"none",
        "dependencies" : [ "typescript-etw" ],
        "copies":
        [
          {
              'destination': '<(module_root_dir)/bin/<(target_arch)',
              'files': ['<(module_root_dir)/build/Release/typescript-etw.node']
          }
        ]
    }
  ]
}
