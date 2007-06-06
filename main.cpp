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

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/convenience.hpp"

namespace fs = boost::filesystem;

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
	virtual void on_function(FunctionData::ptr param)
	{
#if 1
		//if (param->npath_complexity > 4)
		{
			std::cout
				<< param->filename << ":"
				<< param->line_number << ": mccabe="
				<< param->cyclomatic_complexity << " npath="
				<< param->npath_complexity << " "
				<< std::endl;
				//<< param.npath_complexity_2 << " "
		}
#endif
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

typedef std::vector<fs::path> filelist_t;
filelist_t list;

void find_file( const fs::path& dir_path)
{
	if ( !fs::exists( dir_path ) )
	{
		std::cout << "doesn't exist " << dir_path.native_directory_string() << std::endl;
	}

	if ( fs::is_directory( dir_path ))
	{

		fs::directory_iterator end_itr;
		for(fs::directory_iterator itr( dir_path );
			itr != end_itr;
			++itr )
		{
			if ( fs::is_directory( *itr ) )
			{
				find_file(*itr);
			}
			else
			{
         	
				//if (itr->leaf().rfind(".o") != std::string::npos)
				if (fs::extension(*itr) == ".gcno")
				{
					std::cout << itr->normalize().string() << std::endl;
					list.push_back(itr->normalize());
				}
			}
		}
	}
	else
	{
		list.push_back(dir_path);
	}
}

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
	find_file(".");

	for(filelist_t::iterator pos = list.begin(); pos != list.end(); ++pos)
	{
//		std::cout << pos->string() << std::endl;
		reader.open(pos->string().c_str());
	}

	a.report();
//  	while (argv[optind])
//  		reader.open(argv[optind++]);
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

