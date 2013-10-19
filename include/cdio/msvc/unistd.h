/* -*- c -*-
    Copyright (C) Natalia Portillo <claunia@claunia.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if defined (_MSC_VER) || defined (_XBOX)
#ifndef _UNISTD_H_
#define _UNISTD_H_

#ifndef _WINDOWS_H_
#include <windows.h>
#endif

typedef SSIZE_T ssize_t;
typedef unsigned int mode_t;

#endif
#endif