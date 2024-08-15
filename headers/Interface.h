#pragma once

#include <Windows.h>

typedef PVOID(*InterfaceInstanceFn)();

struct InterfaceEntry {
    InterfaceInstanceFn CreateFunction;
    PCSTR Name;
    InterfaceEntry* Next;
};

template<typename T>
__forceinline T* CreateInterface(const char* moduleName, const char* name) {
    // Also can find export using GetProcAddress and call - CreateInterface("SchemaSystem_001", nullptr);
    const HMODULE schemaSystemModuleHandle = GetModuleHandle(moduleName);
    const auto createInterfaceAddress = reinterpret_cast<ULONGLONG>(GetProcAddress(schemaSystemModuleHandle, "CreateInterface"));
    const ULONGLONG interfaces = createInterfaceAddress + *reinterpret_cast<int*>(createInterfaceAddress + 3) + 7;
    for (const InterfaceEntry* current = *reinterpret_cast<InterfaceEntry**>(interfaces); current; current = current->Next)
        if (strcmp(current->Name, name) == 0)
            return reinterpret_cast<T*>(current->CreateFunction());

    return nullptr;
}
