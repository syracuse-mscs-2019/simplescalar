/* ELF object file format.
   Copyright (C) 1992, 1993 Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to
   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */


/* HP PA-RISC support was contributed by the Center for Software Science
   at the University of Utah.  */

#ifndef _OBJ_ELF_H
#define _OBJ_ELF_H

#define OBJ_ELF 1

#include <bfd.h>

#define BYTES_IN_WORD 4		/* for now */
#include "../bfd/libelf.h"

/* Use this to keep track of .size expressions that involve differences
   that we can't compute yet.  */
#define OBJ_SYMFIELD_TYPE expressionS *

/* Symbol fields used by the ELF back end.  */
#define ELF_TARGET_SYMBOL_FIELDS int local:1; unsigned long sy_name_offset;

/* Don't change this; change ELF_TARGET_SYMBOL_FIELDS instead.  */
#define TARGET_SYMBOL_FIELDS ELF_TARGET_SYMBOL_FIELDS

#include "targ-cpu.h"

#ifndef FALSE
#define FALSE 0
#define TRUE  !FALSE
#endif

/* should be conditional on address size! */
#define elf_symbol(asymbol) ((elf_symbol_type *)(&(asymbol)->the_bfd))

#define S_GET_SIZE(S) (elf_symbol ((S)->bsym)->internal_elf_sym.st_size)
#define S_SET_SIZE(S,V) \
  (elf_symbol((S)->bsym)->internal_elf_sym.st_size = (V))

#define S_GET_ALIGN(S) (elf_symbol ((S)->bsym)->internal_elf_sym.st_value)
#define S_SET_ALIGN(S,V) \
  (elf_symbol ((S)->bsym)->internal_elf_sym.st_value = (V))

extern asection *gdb_section;

#define obj_frob_file()	elf_frob_file()

extern void elf_frob_file PARAMS ((void));
extern void elf_file_symbol PARAMS ((char *));

extern void obj_elf_section PARAMS ((int));
extern void obj_elf_previous PARAMS ((int));
extern void obj_elf_version PARAMS ((int));

/* BFD wants to write the udata field, which is a no-no for the
   globally defined sections.  */
#define obj_sec_sym_ok_for_reloc(SEC)	((SEC)->owner != 0)

/* When setting one symbol equal to another, by default we probably
   want them to have the same "size", whatever it means in the current
   context.  */
#define OBJ_COPY_SYMBOL_ATTRIBUTES(DEST,SRC) \
  S_SET_SIZE ((DEST), S_GET_SIZE (SRC))

/* Stabs go in a separate section.  */
#define SEPARATE_STAB_SECTIONS

/* We need 12 bytes at the start of the section to hold some initial
   information.  */
extern void obj_elf_init_stab_section PARAMS ((segT));
#define INIT_STAB_SECTION(seg) obj_elf_init_stab_section (seg)

/* For now, always set ECOFF_DEBUGGING for a MIPS target.  */
#ifdef TC_MIPS
#define ECOFF_DEBUGGING
#endif

#ifdef ECOFF_DEBUGGING

/* If we are generating ECOFF debugging information, we need some
   additional fields for each symbol.  */
#undef TARGET_SYMBOL_FIELDS
#define TARGET_SYMBOL_FIELDS \
  ELF_TARGET_SYMBOL_FIELDS \
  struct efdr *ecoff_file; \
  struct localsym *ecoff_symbol; \
  valueT ecoff_extern_size;

/* We smuggle stabs in ECOFF rather than using a separate section.
   The Irix linker can not handle a separate stabs section.  */
#undef SEPARATE_STAB_SECTIONS
#undef INIT_STAB_SECTION
#define OBJ_PROCESS_STAB(what, string, type, other, desc) \
  ecoff_stab ((what), (string), (type), (other), (desc))

#define OBJ_GENERATE_ASM_LINENO(filename, lineno) \
  ecoff_generate_asm_lineno ((filename), (lineno))

#endif /* ECOFF_DEBUGGING */

extern void elf_frob_symbol PARAMS ((struct symbol *));
#define obj_frob_symbol(symp, punt) elf_frob_symbol (symp)

#endif /* _OBJ_ELF_H */
