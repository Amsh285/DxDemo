#pragma once

#include <array>
#include <assert.h>
#include <bitset>
#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <omp.h>
#include <optional>
#include <queue>
#include <set>
#include <shared_mutex>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <variant>
#include <vector>
#include <Windows.h>
#include <windowsx.h>
#include <wrl/client.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>
#include <DirectXColors.h>

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

template<typename T>
inline void SafeRelease(T& ptr)
{
    if (ptr != NULL)
    {
        ptr->Release();
        ptr = NULL;
    }
}

// These Macros overwrite stl functions for some reason.
// Look if there is a better solution later.

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <Vendor/ska/flat_hash_map.hpp>
