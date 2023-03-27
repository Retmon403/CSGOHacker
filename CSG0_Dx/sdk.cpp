#include "sdk.hpp"
#include "Hacker.hpp"

Engine* engine = nullptr;

PDWORD _Find(DWORD modbase, const char* name) {
    typedef PDWORD(*_CreateInterface)(const char* name, int* returnCode);
    _CreateInterface pfunc = (_CreateInterface)GetProcAddress((HMODULE)modbase, "CreateInterface");
    if (!pfunc) {
        printf("funciton -> CreateInterface -> nullptr!\n");
        return 0;
    }
    int returnCode = 0;
    PDWORD pclass = pfunc(name, &returnCode);
    if (pclass && !returnCode) {
        return pclass;
    }
    return 0;
}

bool FindInterface() {

    engine = (Engine*)_Find(g_dwEngineAdr, "VEngineClient014");
    if (!engine) {
        printf("find engine_class failed!\n");
        return false;
    }


    return true;
}