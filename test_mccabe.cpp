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
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "reporter.hpp"
#include "analyser.hpp"

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

using namespace boost;
using boost::bind;

#define ADD(x,y) add_edge(#x,#y)

class test : public reporter
{
    adjacency_list <vecS, vecS, undirectedS> G;
	//GraphvizDigraph G;
public:
	Analyser analyzer_;
	test() : analyzer_(*this)
	{
	}
	virtual void on_function(FunctionData::ptr param)
	{
		std::cout << "on_function" << std::endl;
	}
	void run()
	{
		call(bind(&test::t_connected, this));
		call(bind(&test::page_308_fig_1, this));
		call(bind(&test::page_310_fig_1, this));
 		call(bind(&test::page_310_fig_2, this));
 		call(bind(&test::page_310_fig_3, this));
 		call(bind(&test::page_310_fig_4, this));
 		call(bind(&test::page_310_fig_5, this));
 		call(bind(&test::page_311_fig_1, this));
 		call(bind(&test::page_311_fig_2, this));
	}

	int connected()
	{
#if 0
		typedef graph_traits<GraphvizGraph>::vertex_descriptor Vertex;
		
		std::vector<int> component(num_vertices(G)), discover_time(num_vertices(G));
		std::vector<default_color_type> color(num_vertices(G));
		std::vector<Vertex> root(num_vertices(G));
		int num = strong_components(G, &component[0], 
									root_map(&root[0]).
									color_map(&color[0]).
									discover_time_map(&discover_time[0]));
#endif
		std::vector<int> component(num_vertices(G));
		int num = connected_components(G, &component[0]);
		return num;
	}
private:
	void call(boost::function<int()> f)
	{
		FunctionData::ptr data(new FunctionData);
//		std::cout << data->cyclomatic_complexity << std::endl;
		analyzer_.clear();
		G.clear(); // = GraphvizDigraph();
		analyzer_.set_data(data);
		int expected = f();
//		analyzer_.add_edge(boost::num_vertices(analyzer_.graph_) - 1, 0); // add extra edge 
		//std::cout << "connected" << connected() << std::endl;
		analyzer_.do_calculate_npath();
		if(data->cyclomatic_complexity != expected)
		{
			std::cout << "fail" << std::endl;
			std::cout << data->cyclomatic_complexity << " != " << expected << std::endl;
		}
		else
			std::cout << "ok" << std::endl;

	}
	void add_edge(int from, int to)
	{
		analyzer_.add_edge(from - 1, to - 1);
		boost::add_edge(from - 1, to - 1, G);
	}

	void add_edge(const std::string& from, const std::string& to)
	{
		add_edge(boost::lexical_cast<char>(from) - 'a' + 1,
				 boost::lexical_cast<char>(to) - 'a' + 1);
	}

	int t_connected()
	{
		add_edge(1, 2);
		add_edge(2, 5);
		add_edge(5, 1);
		add_edge(3, 6);
		return 0;
	}

	int page_308_fig_1()
	{
		ADD(a,b);
		ADD(a,c);
		ADD(a,d);
		ADD(d,c);
		ADD(b,e);
		ADD(e,b);
		ADD(e,a);
		ADD(c,f);
		ADD(e,f);
		return 9 - 6 + 2;
	}

	int page_310_fig_1()
	{
		add_edge(1,2);
		add_edge(2,3);
		add_edge(3,1); // extra
		return 2;
	}

	int page_310_fig_2()
	{
		add_edge(1,2);
		add_edge(1,3);
		add_edge(2,6);
		add_edge(3,4);
		add_edge(3,5);
		add_edge(4,7);
		add_edge(6,7);
		add_edge(5,8);
		add_edge(7,8);
		return 3;
	}

	int page_310_fig_3()
	{
		add_edge(1,2);
		add_edge(1,3);
		add_edge(2,6);
		add_edge(3,4);
		add_edge(4,4);
		add_edge(4,5);
		add_edge(5,6);
		add_edge(6,7);
		return 3;
	}

	int page_310_fig_4()
	{
		add_edge(1,2);
		add_edge(2,3);
		add_edge(2,4);
		add_edge(4,2);
		add_edge(3,6);
		add_edge(6,7);
		add_edge(7,8);
		add_edge(7,9);
		add_edge(9,7);
		add_edge(8,9);
		add_edge(9,10);
		add_edge(4,5);
		add_edge(5,10);
		return 5;
	}
	int page_310_fig_5()
	{
		add_edge(1,2);
		add_edge(2,3);
		add_edge(3,7);
		add_edge(7,2);
		add_edge(7,8);
		add_edge(8,12);
		add_edge(2,4);
		add_edge(4,5);
		add_edge(4,6);
		add_edge(5,6);
		add_edge(5,9);
		add_edge(9,9);
		add_edge(9,10);
		add_edge(10,11);
		add_edge(6,11);
		add_edge(11,12);
		return 6;
	}
	int page_311_fig_1()
	{
		add_edge(1,2);
		add_edge(1,3);
		add_edge(1,4);
		add_edge(1,5);
		add_edge(1,6);
		add_edge(1,7);
		add_edge(2,8);
		add_edge(3,8);
		add_edge(4,8);
		add_edge(5,8);
		add_edge(6,8);
		add_edge(7,2);
		add_edge(7,11);
		add_edge(11,12);
		add_edge(8,9);
		add_edge(8,10);
		add_edge(9,10);
		add_edge(10,12);
		return 8;
	}

	int page_311_fig_2()
	{
		add_edge(1,2);
		add_edge(1,3);
		add_edge(2,3);
		add_edge(2,4);
		add_edge(4,5);
		add_edge(4,6);
		add_edge(6,3);
		add_edge(5,9);
		add_edge(6,7);
		add_edge(7,9);
		add_edge(6,8);
		add_edge(8,9);
		add_edge(9,10);
		add_edge(10,11);
		add_edge(10,12);
		add_edge(11,12);
		add_edge(12,13);
		add_edge(3,13);
		return 8;
	}

};

int main()
{
// 	boost::GraphvizDigraph G;
// 	read_graphviz("fig2.dot", G);
	test().run();
	return 0;
}
