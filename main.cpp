/* Dump a gcov file, for debugging use.
   Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.
   Contributed by Nathan Sidwell <nathan@codesourcery.com>

Gcov is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

Gcov is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gcov; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "gcov_reader.hpp"

extern "C"
{

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "version.h"
#include <getopt.h>
}


//static void print_prefix (const char *, unsigned, gcov_position_t);
static void print_usage (void);
static void print_version (void);
extern int main (int, char **);

static const struct option options[] =
{
  { "help",                 no_argument,       NULL, 'h' },
  { "version",              no_argument,       NULL, 'v' },
  { "long",                 no_argument,       NULL, 'l' },
  { "positions",	    no_argument,       NULL, 'o' },
  { 0, 0, 0, 0 }
};

int
main (int argc ATTRIBUTE_UNUSED, char **argv)
{
	int opt;

	/* Unlock the stdio streams.  */
	unlock_std_streams ();

	while ((opt = getopt_long (argc, argv, "hlpv", options, NULL)) != -1)
    {
		switch (opt)
		{
			case 'h':
				print_usage ();
				break;
			case 'v':
				print_version ();
				break;
			case 'l':
				//flag_dump_contents = 1;
				break;
			case 'p':
				//flag_dump_positions = 1;
				break;
			default:
				fprintf (stderr, "unknown flag `%c'\n", opt);
		}
    }

	while (argv[optind])
		dump_file (argv[optind++]);
	return 0;
}

static void
print_usage (void)
{
  printf ("Usage: gcov-dump [OPTION] ... gcovfiles\n");
  printf ("Print coverage file contents\n");
  printf ("  -h, --help           Print this help\n");
  printf ("  -v, --version        Print version number\n");
  printf ("  -l, --long           Dump record contents too\n");
  printf ("  -p, --positions      Dump record positions\n");
}

static void
print_version (void)
{
	//printf ("gcov-dump (GCC) %s\n", version_string);
  printf ("Copyright (C) 2006 Free Software Foundation, Inc.\n");
  printf ("This is free software; see the source for copying conditions.\n"
  	  "There is NO warranty; not even for MERCHANTABILITY or \n"
	  "FITNESS FOR A PARTICULAR PURPOSE.\n\n");
}

