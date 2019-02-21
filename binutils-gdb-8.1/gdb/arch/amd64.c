/* Copyright (C) 2017-2018 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "common-defs.h"
#include "amd64.h"
#include "x86-xstate.h"
#include <stdlib.h>

#include "../features/i386/64bit-avx.c"
#include "../features/i386/64bit-avx512.c"
#include "../features/i386/64bit-core.c"
#include "../features/i386/64bit-linux.c"
#include "../features/i386/64bit-mpx.c"
#include "../features/i386/64bit-pkeys.c"
#include "../features/i386/64bit-segments.c"
#include "../features/i386/64bit-sse.c"

#include "../features/i386/x32-core.c"

/* Create amd64 target descriptions according to XCR0.  If IS_X32 is
   true, create the x32 ones.  If IS_LINUX is true, create target
   descriptions for Linux.  */

target_desc *
amd64_create_target_description (uint64_t xcr0, bool is_x32, bool is_linux)
{
  target_desc *tdesc = allocate_target_description ();

#ifndef IN_PROCESS_AGENT
  set_tdesc_architecture (tdesc, is_x32 ? "i386:x64-32" : "i386:x86-64");

  if (is_linux)
    set_tdesc_osabi (tdesc, "GNU/Linux");
#endif

  long regnum = 0;

  if (is_x32)
    regnum = create_feature_i386_x32_core (tdesc, regnum);
  else
    regnum = create_feature_i386_64bit_core (tdesc, regnum);

  regnum = create_feature_i386_64bit_sse (tdesc, regnum);
  if (is_linux)
    regnum = create_feature_i386_64bit_linux (tdesc, regnum);
  regnum = create_feature_i386_64bit_segments (tdesc, regnum);

  if (xcr0 & X86_XSTATE_AVX)
    regnum = create_feature_i386_64bit_avx (tdesc, regnum);

  if ((xcr0 & X86_XSTATE_MPX) && !is_x32)
    regnum = create_feature_i386_64bit_mpx (tdesc, regnum);

  if (xcr0 & X86_XSTATE_AVX512)
    regnum = create_feature_i386_64bit_avx512 (tdesc, regnum);

  if ((xcr0 & X86_XSTATE_PKRU) && !is_x32)
    regnum = create_feature_i386_64bit_pkeys (tdesc, regnum);

  return tdesc;
}
