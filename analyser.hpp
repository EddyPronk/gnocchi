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

#ifndef ANALYSER_HPP_
#define ANALYSER_HPP_

#include <set>
#include <vector>
#include "graph.hpp"
#include "reporter.hpp"
#include "function_data.hpp"
#include <boost/signal.hpp>
#include "gcov_reader.hpp"

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::vertices_size_type size_type;

class Analyser
{
public:
	Analyser(reporter& r)
		: reporter_(r)
	{
	}
	void process(const gcov_reader&);
	void calculate_npath(const gcov_reader&, const Graph&, foobar&);
	void calculate_npath_2(const Graph&, foobar&);
	void report(int /*npath_threshold*/);

    typedef boost::signal<void (const gcov_reader&, foobar&, std::vector<long long>)>  line_insert_signal_type;
	line_insert_signal_type on_complexity_calculated;
private:
	reporter& reporter_;	
	typedef std::set<foobar> function_index;
	function_index functions;
};

#endif
