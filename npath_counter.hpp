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

class npath_counter : public boost::dfs_visitor<>
{
public:
    npath_counter(std::vector<Vertex>& pp, std::vector<Vertex>& qq)
		: parent(pp), complexity(qq), end_state(parent.size() - 1) {}

    template <class Edge, class Graph>
    void tree_edge(Edge e, const Graph& g)
	{
		if(parent[target(e, g)] == 0)
			parent[target(e, g)] = source(e, g);
    }
    template <class Edge, class Graph>
    void back_edge(Edge e, const Graph& g)
	{
		if(parent[target(e, g)] == 0)
			parent[target(e, g)] = source(e, g);
		complexity[source(e, g)] = 1;
		complexity[target(e, g)] += 1;
    }
    template <class Edge, class Graph>
    void forward_or_cross_edge(Edge e, const Graph& g)
	{
		complexity[source(e, g)] += complexity[target(e, g)];
    }
    template <class Vertex, class Graph>
    void finish_vertex(Vertex u, const Graph&)
	{
		if(complexity[u] == 0)
			complexity[u] = 1;

		if(u != 0)
			complexity[parent[u]] += complexity[u];
    }
private:
	std::vector<Vertex>& parent;
	std::vector<Vertex>& complexity;
	Vertex end_state;
};

#endif
