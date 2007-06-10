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

void Analyser::calculate_npath_2()
{
	clear_vertex(0, graph_);
	remove_vertex(0, graph_);

// 	string filename = func_name + string(".dot");
// 	ofstream os(filename.c_str());
// 	write_graphviz(os, graph_);

	vector<Parent> parents(num_vertices(graph_));
	vector<Vertex> complexity(num_vertices(graph_));
	int cyclomatic_complexity = 0;
	depth_first_search(graph_, visitor(npath_counter(parents, complexity, cyclomatic_complexity)));
	data_->npath_complexity_2 = complexity[0];
}

void Analyser::calculate_npath()
{
	if(num_vertices(graph_) > 2)
	{
		clear_vertex(num_vertices(graph_) - 1, graph_);
		remove_vertex(num_vertices(graph_) - 1, graph_);
	}
  
	vector<Parent> parents(num_vertices(graph_));
	vector<Vertex> complexity(num_vertices(graph_));
			
	depth_first_search(graph_, visitor(npath_counter(
										   parents,
										   complexity,
										   data_->cyclomatic_complexity
										   )));
	data_->npath_complexity = complexity[0];

// 	string filename = func_name + string(".simple.dot");
// 	ofstream os(filename.c_str());
// 	write_graphviz(os, graph_);
}

void Analyser::process(FunctionData::ptr data)
{
	data_ = data;

	if(num_vertices(graph_))
	{
		calculate_npath_2();
		calculate_npath();
		functions.insert(std::make_pair(data_->npath_complexity, data_));
	}
}

void Analyser::report()
{
	std::map<int, FunctionData::ptr>::iterator pos = functions.begin();
	std::map<int, FunctionData::ptr>::iterator end = functions.end();

	for(; pos != end; ++pos)
	{
		if(pos->second->npath_complexity > 4)
			reporter_.on_function(pos->second);
	}
	
}
