/* tc-hppa.h -- Header file for the PA */

/* Copyright (C) 1989, 1993 Free Software Foundation, Inc.

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

/* Please refrain from exposing the world to the internals of tc-hppa.c
   when this file is included.  This means only declaring exported functions,
   (please PARAMize them!) not exporting structures and data items which
   are used solely within tc-hppa.c, etc.

   Also refrain from adding any more object file dependent code, there is 
   already far too much object file format dependent code in this file.
   In theory this file should contain only exported functions, structures
   and data declarations common to all PA assemblers.  */

#ifndef _TC_HPPA_H
#define _TC_HPPA_H

#ifndef TC_HPPA
#define TC_HPPA	1
#endif

#define TARGET_ARCH bfd_arch_hppa

/* FIXME.  The lack of a place to put things which are both target cpu
   and target format dependent makes hacks like this necessary.  */
#ifdef OBJ_ELF
#include "../bfd/elf32-hppa.h"
#define TARGET_FORMAT "elf32-hppa"
#endif

#ifdef OBJ_SOM
#include "../bfd/som.h"
#define TARGET_FORMAT "som"
#endif

/* FIXME.  Why oh why aren't these defined somewhere globally?  */
#ifndef FALSE
#define FALSE   (0)
#define TRUE    (!FALSE)
#endif

/* Local labels have an "L$" prefix.  */
#define LOCAL_LABEL(name) ((name)[0] == 'L' && (name)[1] == '$')
#define FAKE_LABEL_NAME "L$0\001"
#define ASEC_NULL (asection *)0

/* Labels are not required to have a colon for a suffix.  */
#define LABELS_WITHOUT_COLONS

/* FIXME.  This should be static and declared in tc-hppa.c, but 
   pa_define_label gets used outside of tc-hppa.c via tc_frob_label.
   Should also be PARAMized, but symbolS isn't available here.  */
extern void pa_define_label ();

/* FIXME.  Types not available here, so they can't be PARAMized.  */
extern void parse_cons_expression_hppa ();
extern void cons_fix_new_hppa ();
extern int hppa_force_relocation ();

#define tc_frob_label(sym) pa_define_label (sym)

/* The PA does not need support for either of these.  */
#define tc_crawl_symbol_chain(headers) {;}
#define tc_headers_hook(headers) {;}

#define RELOC_EXPANSION_POSSIBLE
#define MAX_RELOC_EXPANSION 5

/* FIXME.  More things which are both HPPA and ELF specific.  There is 
   nowhere to put such things.  */
#ifdef OBJ_ELF
#define elf_tc_final_processing	elf_hppa_final_processing
void elf_hppa_final_processing PARAMS ((void));
#endif

/* The PA needs to parse field selectors in .byte, etc.  */

#define TC_PARSE_CONS_EXPRESSION(EXP, NBYTES) \
  parse_cons_expression_hppa (EXP)
#define TC_CONS_FIX_NEW cons_fix_new_hppa

/* On the PA, an equal sign often appears as a condition or nullification
   completer in an instruction.  This can be detected by checking the
   previous character, if the character is a comma, then the equal is
   being used as part of an instruction.  */
#define TC_EQUAL_IN_INSN(C, PTR)	((C) == ',')

/* Similarly for an exclamation point.  It is used in FP comparison
   instructions and as an end of line marker.  When used in an instruction
   it will always follow a comma.  */
#define TC_EOL_IN_INSN(PTR)	(is_end_of_line[*(PTR)] && (PTR)[-1] == ',')

#define tc_fix_adjustable hppa_fix_adjustable

/* Because of the strange PA calling conventions, it is sometimes
   necessary to emit a relocation for a call even though it would
   normally appear safe to handle it completely within GAS.  */
#define TC_FORCE_RELOCATION(FIXP) hppa_force_relocation (FIXP)

/* If a symbol is imported, but never used, then the symbol should
   *not* end up in the symbol table.  Likewise for absolute symbols
   with local scope.  */
#define tc_frob_symbol(sym,punt) \
    if ((S_GET_SEGMENT (sym) == &bfd_und_section && sym->sy_used == 0) \
	|| (S_GET_SEGMENT (sym) == &bfd_abs_section \
	    && (sym->bsym->flags & BSF_EXPORT) == 0)) \
      punt = 1

#ifdef OBJ_ELF
/* Arggg.  The generic BFD ELF code always adds in the section->vma
   to non-common symbols.  This is a lose on the PA.  */
#define obj_frob_symbol(sym,punt) \
    if (S_GET_SEGMENT (sym) != &bfd_com_section \
	&& S_GET_SEGMENT (sym) != &bfd_und_section) \
      S_SET_VALUE ((sym), (S_GET_VALUE (sym) - (sym)->bsym->section->vma))

#endif
#endif /* _TC_HPPA_H */
