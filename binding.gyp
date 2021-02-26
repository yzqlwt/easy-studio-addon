{
    "targets": [
        {
            "target_name": "addon",
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions", "-std=c++20" ],
            "sources": [
                "src/addon.cc",
                "src/http/httplib.h",
                "src/utils/Tools.h",
                "src/utils/Tools.cpp",
                "src/utils/PlistParser.h",
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS'],
        }
    ]
}
