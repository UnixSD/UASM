/*  Copyright (C) 2025 UnixSD

   This file is part of UASM.

   Unix Engine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Unix Engine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with UASM.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include "preproc.h"
#include <fstream>
#if __cplusplus < 202002L
#   include <filesystem>
#   define __CPP17__
namespace fs = std::filesystem

void  requireINCL(char file);
#endif
using namespace std;

void requireINCL(ifstream file);
void requireINCLBuffer() const;
void requireFunc() const; 
