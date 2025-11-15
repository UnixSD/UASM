/*  Copyright (C) 2025 UnixSD

   This file is part of UASM.

   UASM is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   UASM is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with UASM.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once 
#define __IFA__(event) // %IF
#define __ELSE__(event) // %ELSE
#define __INCL__(path) // %INCLUDE
#define __ENDIF__ // %ENDIF
#define __DEF__(macro) // %DEF
#define __UNDEF__(macro) // %UNDEF
#define __ELIF__(event) \ // %ELIF

#define __COMMENT__ // ;
#define __NEWLINE__ // \ 
#define __ADDSUB__(event) // () for %if (mov eax, ebx) == ...

std::vector<std::vector<char>> __def_buffer[1000][100]; // limit for macro is 1000
std::vector<std::vector<<char>> __incl_buffer[1000][1000]; // The limit for included files is 1000
