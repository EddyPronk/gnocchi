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

#define YAFFUT_MAIN
#include "yaffut/yaffut.h"

#include <stdexcept>
#include <cassert>
#include "analyser.hpp"
#include "npath_counter.hpp"

#include <vector>

using namespace std;

struct Fixture
{
	Graph g;
	vector<Parent> parents;
	vector<Vertex> complexity;

	void calc ()
	{
		parents.resize(num_vertices(g));
		complexity.resize(num_vertices(g));
			
		long long cyclomatic_complexity = 0;
		depth_first_search(g, visitor(npath_counter(
											   parents,
											   complexity,
											   cyclomatic_complexity
											   )));
	}
 };

TEST(Fixture, empty)
{
	add_edge(1, 2, g); // fixme: fails if empty
	calc();
	EQUAL(1, complexity[0]);
}

TEST(Fixture, simple2)
{
	add_edge(0, 1, g);
	
	add_edge(1, 2, g);
	add_edge(2, 1, g);
	add_edge(0, 3, g);

	calc();
	EQUAL(2, complexity[0]);
}

TEST(Fixture, simple3)
{
	add_edge(0, 1, g);
	add_edge(1, 0, g);
	add_edge(0, 2, g);

	calc();
	EQUAL(2, complexity[0]);
}

TEST(Fixture, diamond)
{
	add_edge(0, 1, g);
	add_edge(0, 2, g);
	add_edge(1, 3, g);
	add_edge(2, 3, g);

	calc();
	EQUAL(2, complexity[0]);
}

TEST(Fixture, simple)
{
	add_edge(0, 1, g);
	add_edge(1, 0, g);

	calc();
	EQUAL(1, complexity[0]);
}
