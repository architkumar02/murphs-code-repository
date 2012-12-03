//=======================================================================
// Link Analytics Challenge Problem 3
// CS 526, Fall 2012, Group 11
// Matthew J. Urffer (matthew.urffer@gmail.com)
// 
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <map>

#include "args.hpp"
using namespace boost;

/**
 * Calculates the number of hops this node is from another node
 * (Similar to Kevin Bacon Number)
 */
template <typename DistanceMap>
class number_recorder : public default_bfs_visitor{
    public:
        number_recorder(DistanceMap dist) : d(dist) { }

        template <typename Edge, typename Graph>
            void tree_edge(Edge e, const Graph& g) const{
                typename graph_traits<Graph>::vertex_descriptor
                    u = source(e, g), v = target(e, g);
                d[v] = d[u] + 1;
            }
    private:
        DistanceMap d;
};

// Convenience function
template < typename DistanceMap >
    number_recorder<DistanceMap>
record_number(DistanceMap d)
{
    return number_recorder < DistanceMap > (d);
}


int main(int argc, char **argv) {
    using namespace std;

    // Input arguments
    parse_args(argc,argv);
    std::ifstream datafile(filename.c_str());
    
    if (!datafile) {
        std::cerr << "No "<<filename<< std::endl;
        print_usage(argc, argv);
        return EXIT_FAILURE;
    }
    cout<<"Reading data from "<<filename<<endl;

    typedef adjacency_list < vecS, vecS, undirectedS, property < vertex_name_t,
            std::string >, property < edge_name_t, std::string > > Graph;
    Graph g;

    typedef property_map < Graph, vertex_name_t >::type actor_name_map_t;
    actor_name_map_t actor_name = get(vertex_name, g);
    typedef property_map < Graph, edge_name_t >::type movie_name_map_t;
    movie_name_map_t connecting_movie = get(edge_name, g);

    typedef graph_traits < Graph >::vertex_descriptor Vertex;
    typedef std::map < std::string, Vertex > NameVertexMap;
    NameVertexMap actors;

    for (std::string line; std::getline(datafile, line);) {
        char_delimiters_separator < char >sep(false, "", ";");
        tokenizer <> line_toks(line, sep);
        tokenizer <>::iterator i = line_toks.begin();
        std::string actors_name = *i++;
        NameVertexMap::iterator pos;
        bool inserted;
        Vertex u, v;
        tie(pos, inserted) = actors.insert(std::make_pair(actors_name, Vertex()));
        if (inserted) {
            u = add_vertex(g);
            actor_name[u] = actors_name;
            pos->second = u;
        } else
            u = pos->second;

        std::string movie_name = *i++;

        tie(pos, inserted) = actors.insert(std::make_pair(*i, Vertex()));
        if (inserted) {
            v = add_vertex(g);
            actor_name[v] = *i;
            pos->second = v;
        } else
            v = pos->second;

        graph_traits < Graph >::edge_descriptor e;
        tie(e, inserted) = add_edge(u, v, g);
        if (inserted)
            connecting_movie[e] = movie_name;

    }

    std::vector < int >number(num_vertices(g));

    Vertex src = actors["Kevin Bacon"];
    number[src] = 0;

    breadth_first_search(g, src,
            visitor(record_number(&number[0])));

    graph_traits < Graph >::vertex_iterator i, end;
    for (tie(i, end) = vertices(g); i != end; ++i) {
        std::cout << actor_name[*i] << " has a Bacon number of "
            << number[*i] << std::endl;
    }

    return 0;
}
