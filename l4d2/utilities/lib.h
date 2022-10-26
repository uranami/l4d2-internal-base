#pragma once

#if _WIN32 || _WIN64
    #define WINDOWS
    #if _WIN64
        #define ENVIRONMENT64
    #else
        #define ENVIRONMENT32
    #endif
// Check GCC
#elif __GNUC__
    #if __x86_64__ || __ppc64__
         #define ENVIRONMENT64
    #else
         #define ENVIRONMENT32
    #endif
#endif

#include <Windows.h>
#include <format>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>

#include "address/address.h"
#include "console/console.h"
#include "module_info/module_info.h"
#include "string/string.h"
#include "memory/memory.h"
#include "string/string.h"
#include "hash/hash.h"