{
    "targets": [
        {
            "target_name": "addon",
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions", "-std=c++17" ],
            "sources": [
                "<!@(node -p \"require('fs').readdirSync('./src').map(f=>'src/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('./src/utils').map(f=>'src/utils/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('./src/http').map(f=>'src/http/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('./src/csd').map(f=>'src/csd/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('./src/csd/flatbuffers').map(f=>'src/csd/flatbuffers/'+f).join(' ')\")",
                "<!@(node -p \"require('fs').readdirSync('./src/csd/reader').map(f=>'src/csd/reader/'+f).join(' ')\")",
            ],
            'configurations': {
                'Debug': {
                    'msvs_settings': {
                        'VCCLCompilerTool': {
                            'AdditionalOptions': ['/EHsc'] # 启用c++异常
                        },
                    },
                },
                'Release': {
                    'msvs_settings': {
                        'VCCLCompilerTool': {
                            'AdditionalOptions': ['/EHsc']  # 启用c++异常
                        },
                    },
                },
            },
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS'],
            "conditions":[
            ["OS=='win'", {
                "copies":[
                    { 
                        'destination': './build/Release/assets',
                        'files':[
                            "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./assets').join(' ');\")",
                        ]
                    }
                ]
            }]
        ],
        }
    ]
}
