/*  Copyright (C) 2025 UnixSD

   This file is part of UASM.

   UASM is free software: you can redistribute it and/or modify
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

#include <string_view>
#include <iostream>

#if defined(__aarch64__)
#   error "Unsupported architecture"
#endif

#if defined(__x86_64__) || defined(_WIN64)
# define _UASM64_ 
#elif defined(__i386__) || defined(_WIN32)
# define _UASM32_
#elif defined(_M_I86) 
# define _UASM16_
#endif



int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
	{
		string_view arg = argv[i];
		
    if (arg == "-o")
		{

		}
		else if (arg == "-f" && i + 1 < argc)
		{
		  
		}
	}

    return 0;
}
