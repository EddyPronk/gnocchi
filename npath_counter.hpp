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
    npath_counter(std::vector<Parent>& pp, std::vector<Vertex>& qq, int& c)
		: cabe(c)
		, parent(pp)
		, complexity(qq)
		, end_state(parent.size() - 1)
		, edges(0)
		, vertices(0)
	{}

    template <class Edge, class Graph>
    void tree_edge(Edge e, const Graph& g)
	{
		parent[target(e, g)].set(source(e, g));
		++edges;
    }
    template <class Edge, class Graph>
    void back_edge(Edge e, const Graph& g)
	{
		complexity[source(e, g)] = 1;
		parent[target(e, g)].set(source(e, g));
		++edges;
    }
    template <class Edge, class Graph>
    void forward_or_cross_edge(Edge e, const Graph& g)
	{
		complexity[source(e, g)] += complexity[target(e, g)];
		++edges;
    }
    template <class Vertex, class Graph>
    void finish_vertex(Vertex u, const Graph&)
	{
		++vertices;
		if(u == 0)
		{
			cabe = edges - vertices + 2;
		}
		if(complexity[u] == 0)
			complexity[u] = 1;

		if(u != 0)
			if(parent[u].defined_)
				complexity[parent[u]] += complexity[u];
    }
	int& cabe;
private:
	std::vector<Parent>& parent;
	std::vector<Vertex>& complexity;
	Vertex end_state;
	int edges;
	int vertices;
};

#endif
