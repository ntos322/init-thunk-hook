#include "hook/hook.hpp"

using namespace thunk;

std::vector<std::string>g_legitimate_modules = {
    "kernelbase.dll", //kernelbase does some thread stuff on application exit
    "ntdll.dll", //idk when but ntdll definitely creates threads
    "kernel32.dll" //unsure if kernel32.dll creates any threads, but just to be safe
    //you can whitelist more modules here...
};

int main()
{
    /* because ntdll.dll isnt loaded by default (depending on compiler configuration) */
    if (!LoadLibraryA("ntdll.dll"))
        return 0;

    range::g_memory_range = std::make_unique<range::memory_range>();

    /* iterate the legitimate module name vector and add every element to the range whitelist */
    for (auto& module_name : g_legitimate_modules) {

        if (!range::g_memory_range->add_whitelist(module_name.c_str()))
            return 0;
    }

    /* since we cant really add a nullptr to a vector, can we? :P */
    if (!range::g_memory_range->add_whitelist(nullptr))
        return 0;

    /* init the thunk hook */
    if (!hook::setup_thunk_hook())
        return 0;


    /* to keep the program running */
    while (true)
        Sleep(1000);

}

