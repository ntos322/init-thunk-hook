#pragma once
#include "../range/range.hpp"

namespace thunk {

	namespace hook {
		
		auto setup_thunk_hook() -> bool;
		auto thread_thunk_hook(int a1, __int64(__fastcall* a2)(__int64), __int64 a3) -> __int64;

		inline __int64(*g_original_thunk)(int, __int64(__fastcall*)(__int64), __int64);

	}

}