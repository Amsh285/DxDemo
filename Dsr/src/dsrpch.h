#pragma once

#include <assert.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
#include <Windows.h>
#include <windowsx.h>
#include <wrl/client.h>

#include <d3d11.h>


template<typename T>
inline void SafeRelease(T& ptr)
{
    if (ptr != NULL)
    {
        ptr->Release();
        ptr = NULL;
    }
}
