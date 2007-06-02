/* NPath complexity analyser for C++.
   Copyright (C) 2007  Eddy Pronk

Gnocchi is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

Gnocchi is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include <iostream>
#include "gcov_reader.hpp"
#include "analyser.hpp"
#include "reporter.hpp"

extern "C"
{

#include "config.h"
#include "system.h"
#include "version.h"
#include <getopt.h>

}

class report_printer : public reporter
{
public:
	virtual void on_function(const std::string& fn, const FunctionData& param)
	{
		std::cout
			<< param.cyclomatic_complexity << " "
			<< param.npath_complexity << " "
			//<< param.npath_complexity_2 << " "
			<< fn << std::endl;

	}
};

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
	// FIXME unlock_std_streams ();

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

	report_printer r;
	Analyser a(r);
	gcov_reader reader(a);
	while (argv[optind])
		reader.open(argv[optind++]);
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
  printf ("gnocchi %s\n", version_string);
  printf ("Copyright (C) 2007 Eddy Pronk.\n");
  printf ("This is free software; see the source for copying conditions.\n"
  	  "There is NO warranty; not even for MERCHANTABILITY or \n"
	  "FITNESS FOR A PARTICULAR PURPOSE.\n\n");
}

