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
#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/program_options.hpp>

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

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace std;

class report_printer : public reporter
{
public:
	virtual void on_function(FunctionData::ptr param)
	{
		cout
			<< param->filename << ":"
			<< param->line_number << ": mccabe="
			<< param->cyclomatic_complexity << " npath="
			<< param->npath_complexity << " "
			<< endl;
	}
};

static void print_version (void);

class file_processor
{

	typedef vector<fs::path> filelist_t;
	filelist_t filelist;
public:
	void find_file( const fs::path& dir_path)
	{
		if ( !fs::exists( dir_path ) )
		{
			cout << "doesn't exist " << dir_path.native_directory_string() << endl;
			return;
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
         	
					//if (itr->leaf().rfind(".o") != string::npos)
					if (fs::extension(*itr) == ".gcno")
					{
						cout << itr->normalize().string() << endl;
						filelist.push_back(itr->normalize());
					}
				}
			}
		}
		else
		{
			filelist.push_back(dir_path);
		}
	}
	void process_file(const fs::path& path)
	{
		if (!fs::exists(path))
		{
			cout << "doesn't exist " << path.native_directory_string() << endl;
			return;
		}
		
		if(fs::is_directory(path))
			find_file(path);
		else
			filelist.push_back(path);

	}
	void process(const vector<string>& v)
	{
		for_each(v.begin(), v.end(), boost::bind(&file_processor::process_file, this, _1));
	}

	void invoke(boost::function<void(fs::path)> f)
	{
		for_each(filelist.begin(), filelist.end(), f);
	}
};

int main(int ac, char* av[])
{
	// FIXME unlock_std_streams ();

	try
	{
		po::options_description generic("Generic options");
		int threshold(0);

		generic.add_options()
			("version,v", "print version string")
			("help,h", "produce help message");

		po::options_description config("Configuration");
		config.add_options()
			("threshold,t", po::value<int>(&threshold)->default_value(200), 
			 "npath threshold");

		po::options_description hidden("Hidden options");

		hidden.add_options()
			("input-file", po::value< vector<string> >(), "input file");
		po::positional_options_description p;
		p.add("input-file", -1);

		po::options_description cmdline_options;
		cmdline_options.add(generic).add(config).add(hidden);

		po::options_description visible("Allowed options");
		visible.add(generic).add(config);

		po::variables_map options;
		store(po::command_line_parser(ac, av).
			  options(cmdline_options).positional(p).run(), options);
		notify(options);

		if (options.count("help")) {
			cout << visible << "\n";
			return 0;
		}

		if (options.count("version"))
		{
			print_version();
			return 0;
		}

		report_printer r;
		Analyser a(r);
		gcov_reader reader(a);
		file_processor processor;

		if (options.count("input-file"))
		{
			processor.process(options["input-file"].as< vector<string> >());
		}

		processor.invoke(boost::bind(&gcov_reader::open, &reader, _1));

		a.report(threshold);
	}
	catch(exception& e)
	{
		cout << e.what() << "\n";
		return 1;
	}    
	return 0;
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

