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
#define PRAGMA_WARN_DLL_BEGIN _Pragma("warning(push)") \
_Pragma("warning(disable : 4251 4275)")
#define PRAGMA_WARN_DLL_END _Pragma("warning(pop)")
#else
#define PRAGMA_WARN_DLL_BEGIN
#define PRAGMA_WARN_DLL_END
#endif
