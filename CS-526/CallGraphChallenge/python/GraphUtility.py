import networkx as nx
def readData(filename='LinkAnalyticsData/UTK_problem/Moria_1.graph'):
    """ Creates a MultiGraph of the formated data supplied by filename """
    MG = nx.MultiGraph()
    with open(filename,'r') as f:
        for line in f:
            token = line.strip().split(' ')
            # Adding nodes
            MG.add_node(long(token[0]),nType=int(token[1]))
            MG.add_node(long(token[2]),nType=int(token[3]))
            # Adding edges
            MG.add_edge(long(token[0]),long(token[2]),  # to and from
                    days=int(token[4]),                 # days
                    calls=int(token[5]),                # calls
                    secs=int(token[6]),                 # call duration
                    texts=long(token[7]))               # texts
    return MG
                    
def MultiToSingle(G,attr='calls'):
    """ Converts a Multigraph to a single bidirectional weighted graph """
    """ attr values are calls, secs, texts, days """
    g = nx.Graph(weight=attr)
    for u,v,edata in G.edges(data=True):
        g.add_node(u)
        g.add_node(v)
        g.add_edge(u,v,weight=edata[attr])
    return g

def GetAttr(G,attr='calls'):
    """ Returns a list of the of the values of the attribute in the graph"""
    """ attr values are calls, secs, texts, days """
    return [edata[attr] for u,v,edata in G.edges(data=True) if attr in edata]
