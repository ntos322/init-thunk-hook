#include "range.hpp"

namespace thunk::range {

	/* functionality: whitelist a module, returns true if the procedure was successfull */
	auto memory_range::add_whitelist(const char* module_name) -> bool {

		auto module_base = (std::uintptr_t)GetModuleHandleA(module_name);
		if (!module_base)
			return false;

		// do some proper error checking here?? 
		auto nt_headers = (IMAGE_NT_HEADERS*)((std::uintptr_t)((IMAGE_DOS_HEADER*)module_base)->e_lfanew + module_base);

		// insert the module base and size into our map 
		m_whitelisted_modules.insert({ module_base, nt_headers->OptionalHeader.SizeOfImage }); return true;
	}

	/* functionality: checks whether an addr is in the range of one of your modules in the map, returns true if in range */
	auto memory_range::is_in_range(std::uintptr_t address) -> bool {

		// iterate the whitelisted module map and check if the address matches atleast one of the elements
		for (const auto& mod : m_whitelisted_modules)
		{
			if (address >= mod.first && address <= mod.first + mod.second)
				return true;
		}

		return false;

	}

	/* functionality: get the module name from an address (for debugging purposes i guess?) */
	auto memory_range::get_module_name(std::uintptr_t address) -> std::string {

		//https://stackoverflow.com/questions/6924195/get-dll-path-at-runtime ??

		HMODULE module_handle;
		char module_name[MAX_PATH];

		if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCSTR)(address),
			&module_handle)) {

			GetModuleFileNameA(module_handle, module_name, MAX_PATH);

			return module_name;
		}

		return "";

	}

}