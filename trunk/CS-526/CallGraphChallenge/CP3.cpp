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
#include <stdlib.h>

#include "args.hpp"
using namespace boost;

// Helper Function
bool StrToBool(std::string s){
    int n = strtol(s.c_str(),NULL,10);
    if (n == 0)
        return true;
    else
        return false;
}


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

// Define a Vertex
struct Vertex{
    long int vertex_id;
    bool vertex_type;
};

// Define an Edge
struct Edge{
    int days;
    int calls;
    int secs;
    int texts;
};


int main(int argc, char **argv) {
    using namespace std;

    // Input arguments
    parse_args(argc,argv);
    std::ifstream datafile(filename.c_str());

    if (!datafile) {
        std::cerr <<"Could not open "<<filename<< std::endl;
        print_usage(argc, argv);
        return EXIT_FAILURE;
    }
    cout<<"Reading data from "<<filename<<endl;
    
    // Creating the type of my graph
    typedef boost::adjacency_list <
        boost::setS,
        boost::setS,
        boost::undirectedS,
        Vertex,
        Edge
            > CallGraph;
    typedef CallGraph::vertex_descriptor VertexID;
    typedef CallGraph::edge_descriptor EdgeID;

    // Creating and filling the graph
    CallGraph graph;
    for (std::string line; std::getline(datafile, line);) {
        // breaking up the input line. Format is:
        // V_A Type_A V_B Type_B Days Calls Secs Texts 
        char_delimiters_separator < char >sep(false, "", " ");
        tokenizer <> line_toks(line, sep);
        tokenizer <>::iterator i = line_toks.begin();

        // Adding to the Vertexes
        VertexID vID = boost::add_vertex(graph);
        VertexID uID = boost::add_vertex(graph);
        
        /*
        for(int j = 0; j < 8; j++){
            std::string s = *(i++);
            std::cout<<s<<" ";

        }
        std::cout<<std::endl;
        */
        graph[vID].vertex_id = strtol((*(i++)).c_str(),NULL,10);
        graph[vID].vertex_type = StrToBool(((*i++)).c_str()); 
        graph[uID].vertex_id = strtol((*(i++)).c_str(),NULL,10);
        graph[uID].vertex_type = StrToBool((*(i++)).c_str()); 

        // Adding the Edges
        EdgeID edge;
        bool ok;
        boost::tie(edge,ok) = boost::add_edge(uID,vID,graph);
        if (ok){
            graph[edge].days = strtol((*(i++)).c_str(),NULL,10);
            graph[edge].calls = strtol((*(i++)).c_str(),NULL,10);
            graph[edge].secs = strtol((*(i++)).c_str(),NULL,10);
            graph[edge].texts = strtol((*(i++)).c_str(),NULL,10);

        }
    }
    return 0;
}
