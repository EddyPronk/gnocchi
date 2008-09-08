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

#ifndef NPATH_COUNTER_HPP
#define NPATH_COUNTER_HPP

#include <boost/graph/depth_first_search.hpp>

struct Parent
{
	Parent()
		: defined_(false)
		, parent_(0)
	{
	}
	operator int()
	{
		return parent_;
	}
	void set(int parent)
	{
		if(!defined_)
		{
			parent_ = parent;
			defined_ = true;
		}
	}
	bool defined_;
	int parent_;
};

class npath_counter : public boost::dfs_visitor<>
{
public:
    npath_counter(std::vector<Parent>& pp, std::vector<long long>& qq)
		: parent(pp)
		, complexity(qq)
		, end_state(parent.size() - 1)
		, edges(0)
		, vertices(0L)
	{}

	int cabe()
	{
		return edges - vertices + 2;
	}
    template <class Edge, class Graph>
    void tree_edge(Edge e, const Graph& g)
	{
		parent[target(e, g)].set(source(e, g));
		++edges;
		//std::cout << "edges[" << edges << "]" << std::endl;
    }
    template <class Edge, class Graph>
    void back_edge(Edge e, const Graph& g)
	{
		complexity[source(e, g)] = 1;
		parent[target(e, g)].set(source(e, g));
		++edges;
		//	std::cout << "edges[" << edges << "]" << std::endl;
    }
    template <class Edge, class Graph>
    void forward_or_cross_edge(Edge e, const Graph& g)
	{
		//std::cout << "C " << complexity[source(e, g)] << " + " << complexity[target(e, g)] << " = ";
		complexity[source(e, g)] += complexity[target(e, g)];
		//std::cout << complexity[source(e, g)] << std::endl;
		++edges;
//		std::cout << "edges[" << edges << "]" << std::endl;
    }
    template <class Vertex, class Graph>
    void finish_vertex(Vertex u, const Graph&)
	{
		++vertices;
#if 0
		if(u == 0)
		{
			cabe = edges - vertices + 2;
			std::cout << "cabe = edges[" << edges << "] - vertices[" << vertices << "] + 2 = " << cabe << std::endl;
		}
#endif
		if(complexity[u] == 0)
			complexity[u] = 1;

		if(u != 0)
			if(parent[u].defined_)
			{
				//	std::cout << "C " << complexity[parent[u]] << " + " << complexity[u] << " = ";
				complexity[parent[u]] += complexity[u];
				//std::cout << complexity[parent[u]] << std::endl;
			}
    }
private:
	std::vector<Parent>& parent;
	//std::vector<Vertex>& complexity;
	std::vector<long long>& complexity;
	Vertex end_state;
	int edges;
	int vertices;
};

#endif
