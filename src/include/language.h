/**
 *  Copyright (C) 2004 Stefan Knöfel
 * 
 *  This file is part of GSwiss.
 *
 *  GSwiss is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  GSwiss is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#if LANGUAGE_EN
#include "l_english.h"
#else
#if LANGUAGE_DE
#include "l_german.h"
#endif
#endif

/* Last check to ensure that there is a language definition */
#ifndef LANGUAGE
#include "l_english.h"
#endif


#define L_VERSIONTEXT_COPYRIGHT "Copyright (C) 2004 Stefan Knöfel\n"
#define L_VERSIONTEXT_VERSION   "GSwiss (%s) %d.%d%c\n"

#define L_TABLEFORMAT      "%4d. (%04d) %-30s %4d %3s %4.1f %5.1f %6.1f\n"
#define L_PAIRINGSFORMAT   "%4d. (%04d) %-17s %4.1f %1c %1d - (%04d) %-17s %4.1f %1c %1d %3s\n"
#define L_PAIRINGSBYE      "      (%04d) %-17s %4.1f %1c %1d            %-17s      \n"
