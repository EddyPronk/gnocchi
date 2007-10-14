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

#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
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
using boost::lexical_cast;
static void print_version (void);

struct block_index
{
	typedef std::multimap<int,int> block_map_t;
	block_map_t block_map_;
	string filename_;

	const string& source_filename()
	{
		filename_ = "foo.c";
		return filename_;
	}

	void on_line_inserted(int lineno ,int blockno)
	{
		block_map_.insert(make_pair(lineno, blockno));
	}

	std::string prefix_with_block_number(int lineno)
	{
		string prefix;
		std::multimap<int,int>::iterator pos = block_map_.lower_bound(lineno);
		if(pos != block_map_.upper_bound(lineno))
		{
			prefix = lexical_cast<std::string>(pos->second);
			++pos;
		}
		for(;pos != block_map_.upper_bound(lineno); ++pos)
		{
			prefix += "," + lexical_cast<std::string>(pos->second);
		}
		return prefix;
	}
};

struct npath_annotator
{
	block_index index;
	map<int,int> annotation;
	string filename_;
	npath_annotator(block_index& i) : index(i)
	{
	}

	const string& source_filename() { return filename_; }
	void annotate_file(const gcov_reader& reader, const foobar& f, const std::vector<Vertex>& complexity)
	{
		if(f.function.filename != "/home/epronk/gnocchi/trunk/gcov_reader.cpp")
			return;

		filename_ = reader.function().filename.string();
		std::multimap<int,int>::const_iterator pos = reader.block_map().begin();
	
		for(;pos != reader.block_map().end(); ++pos)
		{
			if(annotation[pos->first] < complexity[pos->second])
			{
				annotation[pos->first] = complexity[pos->second];
			}
		}
	}
	std::string prefix_with_npath(int lineno)
	{
		if(annotation.find(lineno) != annotation.end())
			return lexical_cast<std::string>(annotation.find(lineno)->second);
		else
			return "";
	}
};

void print_file(const std::string& filename, boost::function< std::string(int) > prefix)
{
	ifstream is(filename.c_str());
	ofstream os((filename + ".gcov").c_str());

	os << "        -:    0:Source:" << filename << endl;

	int lineno = 1;
	while(!is.eof())
 	{
 		char buffer[1024];
 		is.getline(buffer, 1024);
		
		os << setw(9) << prefix(lineno) << ":" << setw(5) << right << lineno << ":"
		   << buffer << endl;
 		++lineno;
	}
}


class file_processor : public reporter
{
	Analyser analyser_;

	typedef vector<fs::path> filelist_t;
	typedef map<fs::path, fs::path> filemap_t;

	filelist_t filelist;
	filemap_t map;

public:
	file_processor()
		: analyser_(*this)
	{
	}

private:

	virtual void on_function(const foobar& param)
	{
 		filemap_t::iterator found = map.find(param.function.filename);
		
 		if (found != map.end())
 		{
			cout << found->second.string();
 		}
		else
		{
			cout << param.function.filename.string();
		}
		
		long long npath_delta = param.npath_complexity_e - param.npath_complexity;
		int cyclomatic_delta = param.cyclomatic_complexity_e - param.cyclomatic_complexity;
		
		cout  << ":" << param.function.line_number << ":"
			  << " mccabe=" << param.cyclomatic_complexity;
		if(cyclomatic_delta > 0)	
			cout << "(+" << cyclomatic_delta << ")";
		cout << " npath=" << param.npath_complexity;
		if(npath_delta > 0)	
			cout << "(+" << npath_delta << ") ";
		cout << endl;

		assert(npath_delta >= 0);
		assert(cyclomatic_delta >= 0);
	}

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

	int filelist_size()
	{
		return filelist.size();
	}
	void annotate_with_block_numbers()
	{
		if(filelist.size() == 1)
		{
			string filename = (filelist.begin())->string();

			gcov_reader reader(analyser_);
			block_index index;
			reader.on_line_insert.connect(boost::bind(&block_index::on_line_inserted, &index, _1, _2));
			reader.open(filename);

			print_file(index.source_filename(), boost::bind(&block_index::prefix_with_block_number, &index, _1));
		}
		else
		{
			// error
		}
	}

	void annotate_with_npath()
	{
		if(filelist.size() == 1)
		{
			string filename = (filelist.begin())->string();

			gcov_reader reader(analyser_);
			block_index index;
			npath_annotator annotator(index);
			analyser_.on_complexity_calculated.connect(boost::bind(&npath_annotator::annotate_file, &annotator, _1, _2, _3));
			reader.open(filename);
			print_file(annotator.source_filename(), boost::bind(&npath_annotator::prefix_with_npath, &annotator, _1));
		}
		else
		{
			// error
		}
	}

	void build_filelist(const vector<string>& files)
	{
		BOOST_FOREACH(string s, files)
		{
			process_file(s);
		}
	}

	void report()
	{
		BOOST_FOREACH(fs::path filename, filelist)
		{
			gcov_reader file(analyser_);
			file.open(filename);
		}

		analyser_.report(1); // threshold);
	}
};


int main(int ac, char* av[])
{
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
			("annotate-npath", "Write gcov style annotated source files with npath")
			("annotate-block", "Write gcov style annotated source files with block no.");

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

		file_processor processor;

		if (options.count("input-file"))
		{
			processor.build_filelist(options["input-file"].as< vector<string> >());
		}
		else
		{
			cerr << "gnocchi: no input files" << endl;
			return 1;
		}

		if(options.count("annotate-block"))
		{
			processor.annotate_with_block_numbers();
		}
		else if(options.count("annotate-npath"))
		{
			processor.annotate_with_npath();
		}
		else
		{
			processor.report();
		}
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

