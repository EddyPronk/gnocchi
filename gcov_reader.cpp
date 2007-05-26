#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

typedef boost::adjacency_list<> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::vertices_size_type size_type;

extern "C"
{

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "version.h"
#include <getopt.h>

#define IN_GCOV (-1)

#include "gcov-io.h"
#include "gcov-io.c"


}

#include "npath_counter.hpp"

typedef struct tag_format
{
	unsigned tag;
	char const *name;
} tag_format_t;

static int flag_dump_contents = 1;
//static int flag_dump_positions = 0;

static const tag_format_t tag_table[] =
{
  {0, "NOP"},
  {0, "UNKNOWN"},
  {0, "COUNTERS" },
  {GCOV_TAG_FUNCTION, "FUNCTION"},
  {GCOV_TAG_BLOCKS, "BLOCKS"},
  {GCOV_TAG_ARCS, "ARCS"},
  {GCOV_TAG_LINES, "LINES"},
  {GCOV_TAG_OBJECT_SUMMARY, "OBJECT_SUMMARY"},
  {GCOV_TAG_PROGRAM_SUMMARY, "PROGRAM_SUMMARY"},
  {0, NULL}
};

struct classx
{
	std::string func_name;
	std::ofstream os_;
	Graph g;
	classx()
	{
		os_.open("arcs");
		os_ << "digraph G {" << std::endl;
	}
	~classx()
	{
		//os_ << "}" << std::endl;
	}
	void tag_function (const char *, unsigned, unsigned);
	void tag_blocks (const char *, unsigned, unsigned);
	void tag_arcs (const char *, unsigned, unsigned);
	void tag_lines (const char *, unsigned, unsigned);
	void tag_counters (const char *, unsigned, unsigned);
	void tag_summary (const char *, unsigned, unsigned);
	void process_graph()
	{
		if(num_vertices(g))
		{
			int npath = 0;
			int npathpp = 0;
			{
				std::vector<Vertex> parents(boost::num_vertices(g));
				std::vector<Vertex> complexity(boost::num_vertices(g));
				depth_first_search(g, visitor(npath_counter(parents,complexity)));
				npathpp = complexity[0];
			}
			{
//				std::cout << num_vertices(g) << std::endl;
				clear_vertex(num_vertices(g) - 1, g);
				remove_vertex(num_vertices(g) - 1, g);
//				std::cout << num_vertices(g) << std::endl;
				std::vector<Vertex> parents(boost::num_vertices(g));
				std::vector<Vertex> complexity(boost::num_vertices(g));
				depth_first_search(g, visitor(npath_counter(parents,complexity)));
				npath = complexity[0];

				std::string filename = func_name + std::string(".dot");
				std::ofstream os(filename.c_str());
				write_graphviz(os, g);
			}
			std::cout
				<< npath << " "
				<< npathpp << " "
				<< func_name << std::endl;
		}
	}
};

void
classx::tag_function (const char *filename ATTRIBUTE_UNUSED,
	      unsigned tag ATTRIBUTE_UNUSED, unsigned length ATTRIBUTE_UNUSED)
{
  unsigned long pos = gcov_position ();

  gcov_read_unsigned (); // indent
  gcov_read_unsigned (); // checksum

  if (gcov_position () - pos < length)
    {
      const char *name;

	  process_graph();
      name = gcov_read_string ();
	  func_name = name;
	  g.clear();
//      printf (", `%s'", name ? name : "NULL");
      name = gcov_read_string ();
      //printf (" %s", name ? name : "NULL");
      gcov_read_unsigned();
    }
}

void
classx::tag_blocks (const char *filename ATTRIBUTE_UNUSED,
	    unsigned tag ATTRIBUTE_UNUSED, unsigned length ATTRIBUTE_UNUSED)
{
  unsigned n_blocks = GCOV_TAG_BLOCKS_NUM (length);

//  printf (" %u blocks", n_blocks);

  if (flag_dump_contents)
    {
      unsigned ix;

      for (ix = 0; ix != n_blocks; ix++)
	{
	  if (!(ix & 7))
	    {
	      //printf ("\n");
	      //print_prefix (filename, 0, gcov_position ());
	      //printf ("\t\t%u", ix);
	    }
	  //printf (" %04x", gcov_read_unsigned ());
	  gcov_read_unsigned();
	}
    }
}

void
classx::tag_arcs (const char *filename ATTRIBUTE_UNUSED,
				  unsigned tag ATTRIBUTE_UNUSED, unsigned length ATTRIBUTE_UNUSED)
{
//	std::cout << "tag_arcs start" << std::endl;
	unsigned n_arcs = GCOV_TAG_ARCS_NUM (length);

//	printf (" %u arcs", n_arcs);
	if (flag_dump_contents)
    {
		unsigned ix;
		unsigned blockno = gcov_read_unsigned ();

		for (ix = 0; ix != n_arcs; ix++)
		{
			unsigned dst, flags;

			if (!(ix & 3))
			{
//				printf ("\n");
//				print_prefix (filename, 0, gcov_position ());
//				printf ("\ttag_arcs block %u:", blockno);
//				os_ << "[ block " << blockno << "]"  << std::endl;
			}
			dst = gcov_read_unsigned ();
			flags = gcov_read_unsigned ();
			//printf (" %u:%04x", dst, flags);
//			if((flags == 4) or (flags == 1))
			{
				//		std::cout << blockno << " -> " << dst << std::endl;
				add_edge(blockno, dst, g);
			}
		}
    }
}

void
classx::tag_lines (const char *filename ATTRIBUTE_UNUSED,
				   unsigned tag ATTRIBUTE_UNUSED, unsigned length ATTRIBUTE_UNUSED)
{
	if (flag_dump_contents)
    {
		/* unsigned blockno = */ gcov_read_unsigned ();
		char const *sep = NULL;

		while (1)
		{
			/* gcov_position_t position = */ gcov_position ();
			const char *source = NULL;
			unsigned lineno = gcov_read_unsigned ();

			if (!lineno)
			{
				source = gcov_read_string ();
				if (!source)
					break;
				sep = NULL;
			}

			if (!sep)
			{
				//      printf ("\n");
				//print_prefix (filename, 0, position);
				//printf ("\tblock %u:", blockno);
				sep = "";
			}
			if (lineno)
			{
//	      printf ("%s%u", sep, lineno);
				sep = ", ";
			}
			else
			{
//	      printf ("%s`%s'", sep, source);
				sep = ":";
			}
		}
    }
}

void
classx::tag_counters (const char *filename ATTRIBUTE_UNUSED,
	      unsigned tag ATTRIBUTE_UNUSED, unsigned length ATTRIBUTE_UNUSED)
{
	//static const char *const counter_names[] = GCOV_COUNTER_NAMES;
  unsigned n_counts = GCOV_TAG_COUNTER_NUM (length);

//  printf (" %s %u counts", counter_names[GCOV_COUNTER_FOR_TAG (tag)], n_counts);
  if (flag_dump_contents)
    {
      unsigned ix;

      for (ix = 0; ix != n_counts; ix++)
	{
	  gcov_type count;

	  if (!(ix & 7))
	    {
//	      printf ("\n");
//	      print_prefix (filename, 0, gcov_position ());
//	      printf ("\t\t%u", ix);
	    }

	  count = gcov_read_counter ();
//	  printf (" ");
//	  printf (HOST_WIDEST_INT_PRINT_DEC, count);
	}
    }
}

void
classx::tag_summary (const char *filename ATTRIBUTE_UNUSED,
	     unsigned tag ATTRIBUTE_UNUSED, unsigned length ATTRIBUTE_UNUSED)
{
  struct gcov_summary summary;
  unsigned ix;

  gcov_read_summary (&summary);
//  printf (" checksum=0x%08x", summary.checksum);

  for (ix = 0; ix != GCOV_COUNTERS; ix++)
    {
//      printf ("\n");
		//print_prefix (filename, 0, 0);
//      printf ("\t\tcounts=%u, runs=%u", summary.ctrs[ix].num, summary.ctrs[ix].runs);

//      printf (", sum_all=" HOST_WIDEST_INT_PRINT_DEC, (HOST_WIDEST_INT)summary.ctrs[ix].sum_all);
//      printf (", run_max=" HOST_WIDEST_INT_PRINT_DEC, (HOST_WIDEST_INT)summary.ctrs[ix].run_max);
//      printf (", sum_max=" HOST_WIDEST_INT_PRINT_DEC, (HOST_WIDEST_INT)summary.ctrs[ix].sum_max);
    }
}

void
dump_file (const char *filename)
{
	classx object;

	unsigned tags[4];
	unsigned depth = 0;

	if (!gcov_open (filename, 1))
    {
		fprintf (stderr, "%s:cannot open\n", filename);
		return;
    }

	/* magic */
	{
		unsigned magic = gcov_read_unsigned ();
		unsigned version;
		const char *type = NULL;
		int endianness = 0;
		char m[4], v[4];
    
		if ((endianness = gcov_magic (magic, GCOV_DATA_MAGIC)))
			type = "data";
		else if ((endianness = gcov_magic (magic, GCOV_NOTE_MAGIC)))
			type = "note";
		else
		{
			printf ("%s:not a gcov file\n", filename);
			gcov_close ();
			return;
		}
		version = gcov_read_unsigned ();
		GCOV_UNSIGNED2STRING (v, version);
		GCOV_UNSIGNED2STRING (m, magic);
    
// 		printf ("%s:%s:magic `%.4s':version `%.4s'%s\n", filename, type,
// 				m, v, endianness < 0 ? " (swapped endianness)" : "");
		if (version != GCOV_VERSION)
		{
			char e[4];
	
			GCOV_UNSIGNED2STRING (e, GCOV_VERSION);
			printf ("%s:warning:current version is `%.4s'\n", filename, e);
		}
	}

	/* stamp */
	{
		gcov_read_unsigned (); // stamp

//		printf ("%s:stamp %lu\n", filename, (unsigned long)stamp);
	}
  
	while (1)
    {
		gcov_position_t base;
		//gcov_position_t position = gcov_position ();
		unsigned tag, length;
		tag_format_t const *format;
		unsigned tag_depth;
		int error;
		unsigned mask;

		tag = gcov_read_unsigned ();
		if (!tag)
			break;
		length = gcov_read_unsigned ();
		base = gcov_position ();
		mask = GCOV_TAG_MASK (tag) >> 1;
		for (tag_depth = 4; mask; mask >>= 8)
		{
			if ((mask & 0xff) != 0xff)
			{
//				printf ("%s:tag `%08x' is invalid\n", filename, tag);
				break;
			}
			tag_depth--;
		}
		
		for (format = tag_table; format->name; format++)
			if (format->tag == tag)
				goto found;
		format = &tag_table[GCOV_TAG_IS_COUNTER (tag) ? 2 : 1];
	  found:;
		switch(format->tag)
		{
			case GCOV_TAG_FUNCTION:
				break;
//   {GCOV_TAG_BLOCKS, "BLOCKS", classx::tag_blocks},
//   {GCOV_TAG_ARCS, "ARCS", classx::tag_arcs},
//   {GCOV_TAG_LINES, "LINES", classx::tag_lines},
//   {GCOV_TAG_OBJECT_SUMMARY, "OBJECT_SUMMARY", classx::tag_summary},
//   {GCOV_TAG_PROGRAM_SUMMARY, "PROGRAM_SUMMARY", classx::tag_summary},
		}
		if (tag)
		{
			if (depth && depth < tag_depth)
			{
				if (!GCOV_TAG_IS_SUBTAG (tags[depth - 1], tag))
					printf ("%s:tag `%08x' is incorrectly nested\n",
							filename, tag);
			}
			depth = tag_depth;
			tags[depth - 1] = tag;
		}

		//print_prefix (filename, tag_depth, position);
//		printf ("%08x:%4u:%s", tag, length, format->name);

		bool format_proc(true);
		switch(format->tag)
		{
			case GCOV_TAG_FUNCTION:
				object.tag_function(filename, tag, length);
				break;
			case GCOV_TAG_BLOCKS:
				object.tag_blocks(filename, tag, length);
				break;
			case GCOV_TAG_ARCS:
				object.tag_arcs(filename, tag, length);
				break;
			case GCOV_TAG_LINES:
				object.tag_lines(filename, tag, length);
				break;
			case GCOV_TAG_OBJECT_SUMMARY:
				object.tag_counters(filename, tag, length);
				break;
			case GCOV_TAG_PROGRAM_SUMMARY:
				object.tag_summary(filename, tag, length);
				break;
			default:
				format_proc = false;
		}

//		printf ("\n");
		if (flag_dump_contents && format_proc)
		{
			unsigned long actual_length = gcov_position () - base;

			if (actual_length > length)
				printf ("%s:record size mismatch %lu bytes overread\n",
						filename, actual_length - length);
			else if (length > actual_length)
				printf ("%s:record size mismatch %lu bytes unread\n",
						filename, length - actual_length);
		}
		gcov_sync (base, length);
		if ((error = gcov_is_error ()))
		{
			printf (error < 0 ? "%s:counter overflow at %lu\n" :
					"%s:read error at %lu\n", filename,
					(long unsigned) gcov_position ());
			break;
		}
    }
	object.process_graph();
	gcov_close ();
}
