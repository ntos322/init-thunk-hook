#include "hook.hpp"

using namespace thunk::range;

namespace thunk::hook {

	/* functionality: swaps the "Kernel32ThreadInitThunkFunction" .data pointer in ntdll.dll, the actual BaseThreadInitThunk is exported in kernel32.dll - but ntdll stores it as a qword. */
	auto setup_thunk_hook() -> bool {
	
		auto ntdll_base = (std::uintptr_t)GetModuleHandleA("ntdll.dll");

		if (ntdll_base) {

			/*
			   NOTE:
			   this address will (probably) ONLY work on my version of ntdll.dll, if you want to test this out for yourself, you can find the
			   "Kernel32ThreadInitThunkFunction" qword in the RtlUserThreadStart function.
			   or you can straight up detour BaseThreadInitThunk function exported from kernel32.
			*/

			/* first off, rebase the address (since my ntdll in ida is rebased to 0x180000000) */

			std::uintptr_t address = 0x18016BFF0; //stored as a seperate variable for easier editing/reading
			auto rebased_address = (std::uintptr_t)(address - 0x180000000) + ntdll_base;

			/* store the original function */
			g_original_thunk = *(decltype(g_original_thunk)*)(rebased_address);

			/* now we swap (hook) the qword */
			*(void**)(rebased_address) = (void*)&thread_thunk_hook;

			return true;

		}

		return true;

	}

	auto thread_thunk_hook(int a1, __int64(__fastcall* a2)(__int64), __int64 a3) -> __int64 {

		auto start_address = (std::uintptr_t)a2;
		if (start_address) {

			if (!g_memory_range->is_in_range(start_address)) {

				auto name = g_memory_range->get_module_name(start_address);
				std::printf("caught sussy thread start at 0x%X (%s)\n", start_address, name.c_str());

				/* make sure to exit the thread to make sure it doesnt continue execution */
				ExitThread(-0);

				return 0;

			}

		}

		/* make sure to return the original qword, let the legitimate threads run gng !!!! */
		return g_original_thunk(a1, a2, a3);

	}

}