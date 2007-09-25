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

static void print_version (void);

class file_processor : public reporter
{
	typedef vector<fs::path> filelist_t;
	typedef map<fs::path, fs::path> filemap_t;

	virtual void on_function(FunctionData::ptr param)
	{
 		filemap_t::iterator found = map.find(param->filename);
		
 		if (found != map.end())
 		{
			cout << found->second.string();
 		}
		else
		{
			cout << param->filename.string();
		}
		
#if 0
		cout  << ":" << param->line_number << ":"
			  << " mccabe=" << param->cyclomatic_complexity
			  << " npath=" << param->npath_complexity 
			  << " mccabe=" << param->cyclomatic_complexity_e
			  << " npath=" << param->npath_complexity_e 
			  << endl;
#else
		long long npath_delta = param->npath_complexity_e - param->npath_complexity;
		assert(npath_delta >= 0);
		int cyclomatic_delta = param->cyclomatic_complexity_e - param->cyclomatic_complexity;
		assert(cyclomatic_delta >= 0);
		
		cout  << ":" << param->line_number << ":"
			  << " mccabe=" << param->cyclomatic_complexity;
		if(cyclomatic_delta > 0)	
			cout << "(+" << cyclomatic_delta << ")";
		cout << " npath=" << param->npath_complexity;
		if(npath_delta > 0)	
			cout << "(+" << npath_delta << ") ";
		cout << endl;
#endif

}

	filelist_t filelist;
	filemap_t map;
public:
	void find_file( const fs::path& dir_path)
	{
		if ( !fs::exists( dir_path ) )
		{
			cout << "doesn't exist " << dir_path.native_directory_string() << endl;
			return;
		}

		try
		{
			if ( fs::is_directory( dir_path ))
			{

				fs::directory_iterator end_itr;
				for(fs::directory_iterator itr( dir_path );
					itr != end_itr;
					++itr )
				{
//					std::cout << itr->string() << " " << itr->leaf() << std::endl;
					try
					{
						map[itr->leaf()] = *itr;
					}
					catch(const std::exception& e)
					{
						cout << "FIXME " << e.what() << endl;
					}

					if ( fs::is_directory( *itr ) )
					{
						find_file(*itr);
					}
					else
					{
						if (fs::extension(*itr) == ".gcno")
						{
							filelist.push_back(*itr);
						}
					}
				}
			}
			else
			{
				filelist.push_back(dir_path);
			}
		}
		catch(const std::exception& e)
		{
			cout << "FIXME " << e.what() << endl;
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
	fs::path::default_name_check( fs::native );

	try
	{
		po::options_description generic("Generic options");
		int threshold(0);

		generic.add_options()
			("version,v", "print version string")
			("help,h", "produce help message")
			("verbose", "be verbose");

		po::options_description config("Configuration");
		config.add_options()
			("threshold,t", po::value<int>(&threshold)->default_value(1), 
			 "npath threshold");

		config.add_options()
			("annotate", "Write gcov style annotated source files.");

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

		if (options.count("help"))
		{
			cout << visible << "\n";
			return 0;
		}

		if (options.count("version"))
		{
			print_version();
			return 0;
		}

		//report_printer r;
		file_processor processor;
		Analyser a(processor);
		gcov_reader reader(a, options);

		if (options.count("input-file"))
		{
			processor.process(options["input-file"].as< vector<string> >());
		}
		else
		{
			cerr << "gnocchi: no input files" << endl;
			return 1;
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
	cout << "gnocchi " << version_string;
	printf ("Copyright (C) 2007 Eddy Pronk.\n");
	printf ("This is free software; see the source for copying conditions.\n"
			"There is NO warranty; not even for MERCHANTABILITY or \n"
			"FITNESS FOR A PARTICULAR PURPOSE.\n\n");
}

