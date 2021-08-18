/* Copyright (C) 1991 Free Software Foundation, Inc.

This file is part of GLD, the Gnu Linker.

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

#include <stdio.h>
#include "bfd.h"
#include "sysdep.h"

#include "ld.h"
#include "ldver.h"
#include "ldemul.h"
#include "ldmain.h"

void
ldversion (noisy)
     int noisy;
{
  fprintf(stdout,"ld version 2.5.2 (with BFD %s)\n", BFD_VERSION);

  if (noisy) 
  {
    ld_emulation_xfer_type **ptr = ld_emulations;
    
    printf("  Supported emulations:\n");
    while (*ptr) 
    {
      printf("   %s \n", (*ptr)->emulation_name);
      ptr++;
    }
  }
}

void
help ()
{
  printf ("\
Usage: %s [-o output] objfile...\n\
Options:\n\
       [-A architecture] [-b input-format] [-Bstatic]\n\
       [-c MRI-commandfile] [-d | -dc | -dp]\n\
       [-defsym symbol=expression] [-e entry] [-F] [-Fformat]\n",
	  program_name);
  puts ("\
       [-format input-format] [-g] [-G size] [-help] [-i]\n\
       [-l archive] [-L searchdir] [-M] [-Map mapfile]\n\
       [-m emulation] [-N | -n] [-noinhibit-exec]\n\
       [-oformat output-format] [-R filename] [-relax]");
  puts ("\
       [-r | -Ur] [-S] [-s] [-sort-common] [-T commandfile]\n\
       [-Ttext textorg] [-Tdata dataorg] [-Tbss bssorg] [-t]\n\
       [-traditional-format] [-u symbol] [-V] [-v] [-version]\n\
       [-warn-common] [-X] [-x] [-y symbol]\n\
       [-( archives -)] [--start-group archives --end-group]");
}
