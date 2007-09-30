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

void Analyser::add_edge(int src, int dest)
{
	boost::add_edge(src, dest, graph_);
}

void Analyser::clear()
{
	graph_.clear();
}

void Analyser::calculate_npath_2(FunctionData::ptr data)
{
	clear_vertex(0, graph_);
	//remove_vertex(0, graph_);

//  	string filename = data->name + string(".dot");
//  	ofstream os(filename.c_str());
//  	write_graphviz(os, graph_);

	vector<Parent> parents(num_vertices(graph_));
	vector<Vertex> complexity(num_vertices(graph_));
	depth_first_search(graph_, visitor(npath_counter(
										   parents,
										   complexity,
										   data->cyclomatic_complexity_e)));
	data->npath_complexity_e = complexity[1];
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

void Analyser::annotate_file(FunctionData::ptr data, const std::string& filename, const vector<Vertex>& complexity)
{
	std::multimap<int,int>::iterator pos = data->block_map.begin();
	
	for(;pos != data->block_map.end(); ++pos)
	{
		if(data->annotation[pos->first] < complexity[pos->second])
			data->annotation[pos->first] = complexity[pos->second];
	}
}

void Analyser::calculate_npath(FunctionData::ptr data)
{
	if(num_vertices(graph_) > 2)
	{
		clear_vertex(num_vertices(graph_) - 1, graph_);
		//remove_vertex(num_vertices(graph_) - 1, graph_);
	}
  
	vector<Parent> parents(num_vertices(graph_));
	vector<Vertex> complexity(num_vertices(graph_));
			
	depth_first_search(graph_, visitor(npath_counter(
										   parents,
										   complexity,
										   data->cyclomatic_complexity)));
	data->npath_complexity = complexity[1];

//  	for(int i = 0; i < complexity.size(); ++i)
//  		cout << i << " " << complexity[i] << endl;

 	string filename = data->name + string(".dot");
 	ofstream os(filename.c_str());
 	write_graphviz(os, graph_);

	annotate_file(data, data->filename.string(), complexity);
}

void Analyser::process(FunctionData::ptr data)
{
	if(num_vertices(graph_))
	{
		calculate_npath_2(data);
		calculate_npath(data);
		functions.insert(std::make_pair(data->npath_complexity, data));
	}
}

void Analyser::report(int npath_threshold)
{
	std::map<int, FunctionData::ptr>::iterator pos = functions.begin();
	std::map<int, FunctionData::ptr>::iterator end = functions.end();

	for(; pos != end; ++pos)
	{
		if(pos->second->npath_complexity > npath_threshold)
			reporter_.on_function(pos->second);
	}
}
