#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list< 
	boost::mapS, boost::vecS, boost::bidirectionalS,
	boost::property<boost::vertex_color_t, boost::default_color_type,
					boost::property<boost::vertex_degree_t, int,
									boost::property<boost::vertex_in_degree_t, int,
													boost::property<boost::vertex_out_degree_t, int> > > >
	> Graph;

#endif
