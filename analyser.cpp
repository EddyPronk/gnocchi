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
#include <iomanip>
#include "analyser.hpp"

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/graphviz.hpp>

#include "npath_counter.hpp"
#include "reporter.hpp"

using namespace boost;
using namespace std;

void Analyser::calculate_npath_2(const Graph& g, foobar& f)
{
	Graph graph(g);
	clear_vertex(0, graph);
	//remove_vertex(0, graph_);

//  	string filename = data->name + string(".dot");
//  	ofstream os(filename.c_str());
//  	write_graphviz(os, graph_);

	vector<Parent> parents(num_vertices(graph));
	vector<long long> complexity(num_vertices(graph));
	depth_first_search(graph, visitor(npath_counter(
										  parents,
										  complexity)));
										  ///f.cyclomatic_complexity_e)));
	f.npath_complexity_e = complexity[1];
	f.cyclomatic_complexity_e = num_edges(graph) - num_vertices(graph) + 2;
//  	for(int i = 0; i < complexity.size(); ++i)
//  		cout << i << " " << complexity[i] << endl;
}

#if 0
void Analyser::print_file(FunctionData::ptr data, const std::string& filename, const vector<Vertex>& complexity)
{
	cout << "print_file" << endl;
	ifstream is(filename.c_str());
	ofstream os((filename + ".gcov").c_str());

	os << "        -:    0:Source:" << filename << endl
	   << "        -:    0:Graph:" << filename << endl;
		// << "        -:    0:Data:before.gcda" << endl
		// << "        -:    0:Runs:1" << endl
		// << "        -:    0:Programs:1" << endl;
	
	assert(!is.fail());
	int lineno = 1;
	while(!is.eof())
 	{
 		char buffer[1024];
 		is.getline(buffer, 1024);
		std::multimap<int,int>::iterator pos = data->block_map.lower_bound(lineno);
		unsigned int npath = 0;
		bool has_prefix = false;
		if(pos != data->block_map.upper_bound(lineno))
		{
			has_prefix = true;
			npath = complexity[pos->second];
			++pos;
		}
		for(;pos != data->block_map.upper_bound(lineno); ++pos)
		{
			npath = std::max(npath, complexity[pos->second]);
		}
		string prefix = "-";
		if(has_prefix)
			prefix = lexical_cast<string>(npath);


		os << setw(9) << prefix << ":" << setw(5) << right << lineno << ":"
		   << buffer << endl;
 		++lineno;
	}
}
#endif

void Analyser::calculate_npath(const gcov_reader& reader, const Graph& g, foobar& f)
{
	Graph graph(g);
	clear_vertex(0, graph);

	if(num_vertices(graph) > 2)
	{
		clear_vertex(num_vertices(graph) - 1, graph);
		//remove_vertex(num_vertices(graph_) - 1, graph_);
	}
  
	vector<Parent> parents(num_vertices(graph));
	vector<long long> complexity(num_vertices(graph));
			
	depth_first_search(graph, visitor(npath_counter(
										  parents,
										  complexity)));
	f.cyclomatic_complexity = num_edges(graph) - num_vertices(graph) + 2;
	f.npath_complexity = complexity[1];

//  	for(int i = 0; i < complexity.size(); ++i)
//  		cout << i << " " << complexity[i] << endl;

//  	string filename = data->name + string(".dot");
//  	ofstream os(filename.c_str());
//  	write_graphviz(os, graph_);

	//annotate_file(annotation, data, data->filename.string(), complexity);
	on_complexity_calculated(reader, f, complexity);
}

void Analyser::process(const gcov_reader& reader)
{
//	cout << reader.function().name << endl;

	foobar baz;
	baz.function = reader.function();

	if(num_vertices(reader.graph()))
	{
		calculate_npath_2(reader.graph(), baz);
		calculate_npath(reader, reader.graph(), baz);
//		functions.insert(std::make_pair(data->npath_complexity, data));
		functions.insert(baz);
	}
}

void Analyser::report(int npath_threshold)
{
	function_index::iterator pos = functions.begin();
	function_index::iterator end = functions.end();

	for(; pos != end; ++pos)
	{
		//	if(pos->npath_complexity > npath_threshold)
			reporter_.on_function(*pos);
	}
}
