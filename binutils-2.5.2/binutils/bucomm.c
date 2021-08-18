/* bucomm.c -- Bin Utils COMmon code.
   Copyright (C) 1991, 92, 93, 94 Free Software Foundation, Inc.

   This file is part of GNU Binutils.

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

/* We might put this in a library someday so it could be dynamically
   loaded, but for now it's not necessary.  */

#include "bfd.h"
#include "sysdep.h"
#include "libiberty.h"
#include "bucomm.h"

#ifdef ANSI_PROTOTYPES
#include <stdarg.h>
#else
#include <varargs.h>
#endif

char *target = NULL;		/* default as late as possible */

/* Error reporting */

char *program_name;

void
bfd_nonfatal (string)
     CONST char *string;
{
  CONST char *errmsg = bfd_errmsg (bfd_get_error ());

  if (string)
    fprintf (stderr, "%s: %s: %s\n", program_name, string, errmsg);
  else
    fprintf (stderr, "%s: %s\n", program_name, errmsg);
}

void
bfd_fatal (string)
     CONST char *string;
{
  bfd_nonfatal (string);
  xexit (1);
}

#ifdef ANSI_PROTOTYPES
void
fatal (const char *format, ...)
{
  va_list args;

  fprintf (stderr, "%s: ", program_name);
  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);
  putc ('\n', stderr);
  xexit (1);
}
#else
void 
fatal (va_alist)
     va_dcl
{
  char *Format;
  va_list args;

  fprintf (stderr, "%s: ", program_name);
  va_start (args);
  Format = va_arg (args, char *);
  vfprintf (stderr, Format, args);
  va_end (args);
  putc ('\n', stderr);
  xexit (1);
}
#endif

/* After a false return from bfd_check_format_matches with
   bfd_get_error () == bfd_error_file_ambiguously_recognized, print the possible
   matching targets.  */

void
list_matching_formats (p)
     char **p;
{
  fprintf(stderr, "%s: Matching formats:", program_name);
  while (*p)
    fprintf(stderr, " %s", *p++);
  fprintf(stderr, "\n");
}

/* Display the archive header for an element as if it were an ls -l listing:

   Mode       User\tGroup\tSize\tDate               Name */

void
print_arelt_descr (file, abfd, verbose)
     FILE *file;
     bfd *abfd;
     boolean verbose;
{
  struct stat buf;

  if (verbose)
    {
      if (bfd_stat_arch_elt (abfd, &buf) == 0)
	{
	  char modebuf[11];
	  char timebuf[40];
	  time_t when = buf.st_mtime;
	  CONST char *ctime_result = (CONST char *) ctime (&when);

	  /* POSIX format:  skip weekday and seconds from ctime output.  */
	  sprintf (timebuf, "%.12s %.4s", ctime_result + 4, ctime_result + 20);

	  mode_string (buf.st_mode, modebuf);
	  modebuf[10] = '\0';
	  /* POSIX 1003.2/D11 says to skip first character (entry type).  */
	  fprintf (file, "%s %d/%d %6ld %s ", modebuf + 1,
		   buf.st_uid, buf.st_gid, buf.st_size, timebuf);
	}
    }

  fprintf (file, "%s\n", bfd_get_filename (abfd));
}
