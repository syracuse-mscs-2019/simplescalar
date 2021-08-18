/* tc-ppc.h -- Header file for tc-ppc.c.
   Copyright (C) 1994 Free Software Foundation, Inc.
   Written by Ian Lance Taylor, Cygnus Support.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to
   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. */

#define TC_PPC

#ifndef BFD_ASSEMBLER
 #error PowerPC support requires BFD_ASSEMBLER
#endif

/* The target BFD architecture.  */
#define TARGET_ARCH (ppc_arch ())
extern enum bfd_architecture ppc_arch PARAMS ((void));

/* The target BFD format.  */
#ifdef OBJ_COFF
#define TARGET_FORMAT "aixcoff-rs6000"
#endif
#ifdef OBJ_ELF
#define TARGET_FORMAT "elf32-powerpc"
#endif

/* Permit temporary numeric labels.  */
#define LOCAL_LABELS_FB

/* $ is used to refer to the current location.  */
#define DOLLAR_DOT

/* Strings do not use backslash escapes.  */
#define NO_STRING_ESCAPES

#ifdef OBJ_COFF
/* When using COFF, we determine whether or not to output a symbol
   based on sy_tc.output, not on the name.  */
#define LOCAL_LABEL(name) 0
#endif
#ifdef OBJ_ELF
/* When using ELF, local labels start with '.'.  */
#define LOCAL_LABEL(name) (name[0] == '.' \
			   && (name[1] == 'L' || name[1] == '.'))
#define FAKE_LABEL_NAME ".L0\001"
#endif

/* Set the endianness we are using.  Default to big endian.  */
#ifndef TARGET_BYTES_BIG_ENDIAN
#ifndef TARGET_BYTES_LITTLE_ENDIAN
#define TARGET_BYTES_BIG_ENDIAN
#endif
#endif

#ifdef TARGET_BYTES_BIG_ENDIAN
#define PPC_BIG_ENDIAN 1
#else
#define PPC_BIG_ENDIAN 0
#endif

/* We don't need to handle .word strangely.  */
#define WORKING_DOT_WORD

/* We set the fx_done field appropriately in md_apply_fix.  */
#define TC_HANDLES_FX_DONE

#ifdef OBJ_COFF

/* Declarations needed when generating XCOFF code.  XCOFF is an
   extension of COFF, used only on the RS/6000.  Rather than create an
   obj-xcoff, we just use obj-coff, and handle the extensions here in
   tc-ppc.  */

/* We need to keep some information for symbols.  */
struct ppc_tc_sy
{
  /* We keep a few linked lists of symbols.  */
  struct symbol *next;
  /* Non-zero if the symbol should be output.  The RS/6000 assembler
     only outputs symbols that are external or are mentioned in a
     .globl or .lglobl statement.  */
  int output;
  /* The symbol class.  */
  int class;
  /* The real name, if the symbol was renamed.  */
  char *real_name;
  /* For a csect symbol, the subsegment we are using.  This is zero
     for symbols that are not csects.  */
  subsegT subseg;
  /* For a csect or common symbol, the alignment to use.  */
  int align;
  /* For a function symbol, a symbol whose value is the size.  The
     field is NULL if there is no size.  */
  struct symbol *size;
  /* For a csect symbol, the last symbol which has been defined in
     this csect, or NULL if none have been defined so far.  For a .bs
     symbol, the referenced csect symbol.  */
  struct symbol *within;
};

#define TC_SYMFIELD_TYPE struct ppc_tc_sy

/* We need an additional auxent for function symbols.  */
#define OBJ_COFF_MAX_AUXENTRIES 2

/* Square and curly brackets are permitted in symbol names.  */
#define LEX_BR 3

/* Canonicalize the symbol name.  */
#define tc_canonicalize_symbol_name(name) ppc_canonicalize_symbol_name (name)
extern char *ppc_canonicalize_symbol_name PARAMS ((char *));

/* Get the symbol class from the name.  */
#define tc_symbol_new_hook(sym) ppc_symbol_new_hook (sym)
extern void ppc_symbol_new_hook PARAMS ((struct symbol *));

/* Set the symbol class of a label based on the csect.  */
#define tc_frob_label(sym) ppc_frob_label (sym)
extern void ppc_frob_label PARAMS ((struct symbol *));

/* TOC relocs requires special handling.  */
#define tc_fix_adjustable(fixp) ppc_fix_adjustable (fixp)
extern int ppc_fix_adjustable PARAMS ((struct fix *));

/* We need to set the section VMA.  */
#define tc_frob_section(sec) ppc_frob_section (sec)
extern void ppc_frob_section PARAMS ((asection *));

/* Finish up the symbol.  */
#define tc_frob_symbol(sym, punt) punt = ppc_frob_symbol (sym)
extern int ppc_frob_symbol PARAMS ((struct symbol *));

/* Finish up the file.  */
#define tc_frob_file() ppc_frob_file ()
extern void ppc_frob_file PARAMS ((void));

#endif /* OBJ_COFF */
