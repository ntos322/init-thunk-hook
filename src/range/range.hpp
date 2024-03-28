#pragma once
#include <Windows.h>
#include <iostream>

#include <unordered_map>

namespace thunk {

    namespace range {

        class memory_range {
        private:

            /* this map will hold all whitelisted dlls (and their sizes) that are allowed to create threads */
            std::unordered_map<std::uintptr_t, std::size_t>m_whitelisted_modules;

        public:

            auto add_whitelist(const char* module_name) -> bool;
            auto is_in_range(std::uintptr_t address) -> bool;

            auto get_module_name(std::uintptr_t address) -> std::string;

        };

        inline std::unique_ptr<memory_range>g_memory_range;

    }

}