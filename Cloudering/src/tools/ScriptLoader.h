//#pragma once
//#include <iostream>
//#include  <windows.h>
//#include <string>
//#include "../core/Script.h"
//
//
////full on chatGPT . . . I don't understand this xD
//class ScriptLoader {
//public:
//    struct ScriptHandle {
//        Script* instance = nullptr;
//        HMODULE libHandle = nullptr;
//
//        using DestroyFunc = void (*)(Script*);
//        DestroyFunc destroyFunc = nullptr;
//
//        ~ScriptHandle() {
//            if (instance && destroyFunc) {
//                destroyFunc(instance);
//            }
//            if (libHandle) {
//                FreeLibrary(libHandle);
//            }
//        }
//    };
//
//    static ScriptHandle* LoadScript(const std::string& dllPath) {
//        HMODULE lib = LoadLibraryA(dllPath.c_str());
//        if (!lib) {
//            std::cerr << "Failed to load DLL: " << dllPath << std::endl;
//            return nullptr;
//        }
//
//        using CreateFunc = Script * (*)();
//        using DestroyFunc = void (*)(Script*);
//
//        auto createScript = (CreateFunc)GetProcAddress(lib, "CreateScript");
//        auto destroyScript = (DestroyFunc)GetProcAddress(lib, "DestroyScript");
//
//        if (!createScript || !destroyScript) {
//            std::cerr << "Failed to get CreateScript or DestroyScript." << std::endl;
//            FreeLibrary(lib);
//            return nullptr;
//        }
//
//        Script* instance = createScript();
//        if (!instance) {
//            std::cerr << "Script instance creation failed." << std::endl;
//            FreeLibrary(lib);
//            return nullptr;
//        }
//
//        ScriptHandle* handle = new ScriptHandle();
//        handle->instance = instance;
//        handle->libHandle = lib;
//        handle->destroyFunc = destroyScript;
//
//        return handle;
//    }
//};