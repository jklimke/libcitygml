/* -*-c++-*- libcitygml - Copyright (c) 2010 Joachim Pouderoux, BRGM
*
* This file is part of libcitygml library
* http://code.google.com/p/libcitygml
*
* libcitygml is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 2.1 of the License, or
* (at your option) any later version.
*
* libcitygml is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*/

#pragma once

#ifdef _MSC_VER
// MSVC defines
#define PRAGMA_WARN_DLL_BEGIN _Pragma("warning(push)") \
_Pragma("warning(disable : 4251 4275)")
#define PRAGMA_WARN_DLL_END _Pragma("warning(pop)")

#define PRAGMA_NO_DEPRECATION_BEGIN _Pragma("warning(push)") \
_Pragma("warning(disable : 4996)")
#define PRAGMA_NO_DEPRECATION_END _Pragma("warning(pop)") \

#elif defined(__GNUC__)
// GCC defines
#define PRAGMA_WARN_DLL_BEGIN
#define PRAGMA_WARN_DLL_END

#define PRAGMA_NO_DEPRECATION_BEGIN _Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#define PRAGMA_NO_DEPRECATION_END _Pragma("GCC diagnostic pop") \

#else
// LLVM defines
#define PRAGMA_WARN_DLL_BEGIN
#define PRAGMA_WARN_DLL_END

#define PRAGMA_NO_DEPRECATION_BEGIN _Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#define PRAGMA_NO_DEPRECATION_END _Pragma("clang diagnostic pop") \

#endif
