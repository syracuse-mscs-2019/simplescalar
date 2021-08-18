/* ldctor.c -- constructor support routines
   Copyright (C) 1991, 92, 93, 94 Free Software Foundation, Inc.
   By Steve Chamberlain <sac@cygnus.com>
   
This file is part of GLD, the Gnu Linker.

GLD is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GLD is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GLD; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include "bfd.h"
#include "sysdep.h"
#include "bfdlink.h"

#include "ld.h"
#include "ldexp.h"
#include "ldlang.h"
#include "ldmisc.h"
#include "ldgram.h"
#include "ldmain.h"
#include "ldctor.h"

/* The list of statements needed to handle constructors.  These are
   invoked by the command CONSTRUCTORS in the linker script.  */
lang_statement_list_type constructor_list;

/* We keep a list of these structures for each sets we build.  */

struct set_info
{
  struct set_info *next;		/* Next set.  */
  struct bfd_link_hash_entry *h;	/* Hash table entry.  */
  bfd_reloc_code_real_type reloc;	/* Reloc to use for an entry.  */
  size_t count;				/* Number of elements.  */
  struct set_element *elements;		/* Elements in set.  */
};

struct set_element
{
  struct set_element *next;		/* Next element.  */
  asection *section;			/* Section of value.  */
  bfd_vma value;			/* Value.  */
};

/* The sets we have seen.  */

static struct set_info *sets;

/* Add an entry to a set.  H is the entry in the linker hash table.
   RELOC is the relocation to use for an entry in the set.  SECTION
   and VALUE are the value to add.  This is called during the first
   phase of the link, when we are still gathering symbols together.
   We just record the information now.  The ldctor_find_constructors
   function will construct the sets.  */

void
ldctor_add_set_entry (h, reloc, section, value)
     struct bfd_link_hash_entry *h;
     bfd_reloc_code_real_type reloc;
     asection *section;
     bfd_vma value;
{
  struct set_info *p;
  struct set_element *e;
  struct set_element **epp;

  for (p = sets; p != (struct set_info *) NULL; p = p->next)
    if (p->h == h)
      break;

  if (p == (struct set_info *) NULL)
    {
      p = (struct set_info *) xmalloc (sizeof (struct set_info));
      p->next = sets;
      sets = p;
      p->h = h;
      p->reloc = reloc;
      p->count = 0;
      p->elements = NULL;
    }
  else if (p->reloc != reloc)
    einfo ("%P%X: Different relocs used in set %s\n", h->root.string);

  e = (struct set_element *) xmalloc (sizeof (struct set_element));
  e->next = NULL;
  e->section = section;
  e->value = value;

  for (epp = &p->elements; *epp != NULL; epp = &(*epp)->next)
    ;
  *epp = e;

  ++p->count;
}

/* This function is called after the first phase of the link and
   before the second phase.  At this point all set information has
   been gathered.  We now put the statements to build the sets
   themselves into constructor_list.  */

void
ldctor_build_sets ()
{
  lang_statement_list_type *old;
  struct set_info *p;

  old = stat_ptr;
  stat_ptr = &constructor_list;

  lang_list_init (stat_ptr);

  for (p = sets; p != (struct set_info *) NULL; p = p->next)
    {
      struct set_element *e;
      const reloc_howto_type *howto;
      int size;

      /* If the symbol is defined, we may have been invoked from
	 collect, and the sets may already have been built, so we do
	 not do anything.  */
      if (p->h->type == bfd_link_hash_defined)
	continue;

      /* For each set we build:
	   set:
	     .long number_of_elements
	     .long element0
	     ...
	     .long elementN
	     .long 0
	 except that we use the right size instead of .long.  When
	 generating relocateable output, we generate relocs instead of
	 addresses.  */
      howto = bfd_reloc_type_lookup (output_bfd, p->reloc);
      if (howto == (reloc_howto_type *) NULL)
	{
	  einfo ("%P%X: BFD backend does not support reloc %d for set %s\n",
		 (int) p->reloc, p->h->root.string);
	  size = LONG;
	}
      else
	{
	  switch (bfd_get_reloc_size (howto))
	    {
	    case 1: size = BYTE; break;
	    case 2: size = SHORT; break;
	    case 4: size = LONG; break;
	    case 8: size = QUAD; break;
	    default:
	      einfo ("%P%X: Unsupported size %d for set %s\n",
		     bfd_get_reloc_size (howto), p->h->root.string);
	      size = LONG;
	      break;
	    }
	}

      lang_add_assignment (exp_assop ('=', p->h->root.string,
				      exp_nameop (NAME, ".")));
      lang_add_data (size, exp_intop ((bfd_vma) p->count));

      for (e = p->elements; e != (struct set_element *) NULL; e = e->next)
	{
	  if (link_info.relocateable)
	    lang_add_reloc (p->reloc, howto, e->section,
			    (const char *) NULL, exp_intop (e->value));
	  else
	    lang_add_data (size, exp_relop (e->section, e->value));
	}

      lang_add_data (size, exp_intop (0));
    }

  stat_ptr = old;
}
