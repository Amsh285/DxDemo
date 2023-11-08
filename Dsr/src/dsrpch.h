#pragma once

#include <assert.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
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
