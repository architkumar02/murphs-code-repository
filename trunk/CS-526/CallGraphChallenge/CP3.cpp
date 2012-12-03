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
template < typename DistanceMap > number_recorder<DistanceMap> record_number(DistanceMap d){
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


    // New Properties
    struct texts_t { typedef edge_property_tag kind;}
    struct calls_t { typedef edge_property_tag kind;}
    struct days_t { typedef edge_property_tag kind;}
    struct secs_t { typedef edge_property_tag kind;}

    // Creating Property Tags
    typedef property<texts_t,int> Texts;
    typedef property<calls_t,int,Texts> Calls;
    typedef property<days_t,int,Calls> Days;
    typedef property<secs_t,double,Days> EdgeProperty;
    typedef property<vertex_name_t,int> VertexProperty;

    // Setting up the graph
    typedef adjacency_list < vecS, vecS, undirectedS, VertexProperty, EdgeProperty > Graph;
    Graph g;

    // Accessors for the graph properties
    typedef property_map < Graph, vertex_name_t>::type vertex_number =  get(vertex_name, g);
    typedef property_map < Graph, texts_t >::type texts = get(texts_t(),G); 
    typedef property_map < Graph, calls_t >::type texts = get(calls_t(),G); 
    typedef property_map < Graph, days_t >::type texts = get(days_t(),G); 
    typedef property_map < Graph, secs_t >::type texts = get(secs_t(),G); 
    

    typedef graph_traits < Graph >::vertex_descriptor Vertex;
    typedef std::map < std::string, Vertex > NameVertexMap;
    NameVertexMap actors;

    for (std::string line; std::getline(datafile, line);) {
        char_delimiters_separator < char >sep(false, "", " ");
        tokenizer <> line_toks(line, sep);
        tokenizer <>::iterator i = line_toks.begin();
        std::string actors_name = *i++;
        NameVertexMap::iterator pos;
        bool inserted;
        Vertex u, v;
        tie(pos, inserted) = actors.insert(std::make_pair(actors_name, Vertex()));
        if (inserted) {
            u = add_vertex(g);
            vertex_number[u] = actors_name;
            pos->second = u;
        } else
            u = pos->second;

        std::string movie_name = *i++;

        tie(pos, inserted) = actors.insert(std::make_pair(*i, Vertex()));
        if (inserted) {
            v = add_vertex(g);
            vertex_number[v] = *i;
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
        std::cout << vertex_number[*i] << " has a Bacon number of "
            << number[*i] << std::endl;
    }

    return 0;
}
