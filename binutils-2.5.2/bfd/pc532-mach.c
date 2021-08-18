/* BFD back-end for Mach3/532 a.out-ish binaries.
   Copyright (C) 1990, 1991, 1992, 1994 Free Software Foundation, Inc.

This file is part of BFD, the Binary File Descriptor library.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* Written by Ian Dall
 *            19-Apr-94
 *
 * Formerly part of aout-pc532-mach.c. Split out to allow more
 * flexibility with multiple formats.
 *
 */
/* This architecture has N_TXTOFF and N_TXTADDR defined as if
 * N_HEADER_IN_TEXT, but the a_text entry (text size) does not include the
 * space for the header. So we have N_HEADER_IN_TEXT defined to
 * 1 and specially define our own N_TXTSIZE
 */

#define N_HEADER_IN_TEXT(x) 1
#define N_TXTSIZE(x) ((x).a_text)


#define TEXT_START_ADDR 0x10000       /* from old ld */
#define PAGE_SIZE 0x1000       /* from old ld,  032 & 532 are really 512/4k */

/* Use a_entry of 0 to distinguish object files from OMAGIC executables */
#define N_TXTADDR(x) \
  (N_MAGIC(x) == OMAGIC ? \
   ((x).a_entry < TEXT_START_ADDR? 0: TEXT_START_ADDR): \
   (N_MAGIC(x) == NMAGIC? TEXT_START_ADDR: \
    TEXT_START_ADDR + EXEC_BYTES_SIZE))

#define	SEGMENT_SIZE	PAGE_SIZE

#define N_SHARED_LIB(x) 0
#define SEGMENT_SIZE PAGE_SIZE
#define DEFAULT_ARCH bfd_arch_ns32k
  
#define MY(OP) CAT(pc532machaout_,OP)

#define TARGETNAME "a.out-pc532-mach"

#include "bfd.h"
#include "sysdep.h"
#include "libaout.h"
#include "libbfd.h"
#include "aout/aout64.h"

/* We can`t use the MYNS macro here for cpp reasons too subtle for me -- IWD */

#define MY_bfd_reloc_type_lookup ns32kaout_bfd_reloc_type_lookup

/* libaout doesn't use NAME for these ... */

#define MY_get_section_contents aout_32_get_section_contents

/* Forward declaration. Defined in aout-target.h */

static boolean MY(set_sizes)();
     
static CONST struct aout_backend_data MY(backend_data) =
{
  0,				/* zmagic contiguous */
  1,				/* text incl header */
  0,
  0,				/* text vma? */
  MY(set_sizes),
  1,				/* exec header is not counted */
};
     
#define MY_backend_data &MY(backend_data)
     
#define MYNSX(OP) CAT(ns32kaout_,OP)
CONST struct reloc_howto_struct *
MYNSX(bfd_reloc_type_lookup)
  PARAMS((bfd *abfd AND
	  bfd_reloc_code_real_type code));

boolean
MYNSX(write_object_contents)
  PARAMS((bfd *abfd));

static boolean
MY(write_object_contents) (abfd)
bfd *abfd;
{
  struct external_exec exec_bytes;
  struct internal_exec *execp = exec_hdr (abfd);
  
#if CHOOSE_RELOC_SIZE
  CHOOSE_RELOC_SIZE(abfd);
#else
  obj_reloc_entry_size (abfd) = RELOC_STD_SIZE;
#endif
  
  BFD_ASSERT (bfd_get_arch (abfd) == bfd_arch_ns32k);
  switch (bfd_get_mach (abfd))
    {
    case 32032:
      N_SET_MACHTYPE (*execp, M_NS32032);
      break;
    case 32532:
    default:
      N_SET_MACHTYPE (*execp, M_NS32532);
      break;
    }
  N_SET_FLAGS (*execp, aout_backend_info (abfd)->exec_hdr_flags);
  
  WRITE_HEADERS(abfd, execp);
  
  return true;
}

#define MY_write_object_contents MY(write_object_contents)

#include "aout-target.h"
