def readData(filename='LinkAnalyticsData/Moria_1.graph'):
    MG = nx.MultiGraph()
    with open(filename,'r') as f:
        for line in f:
            tokens = line.strip().split(' ')
            # Adding nodes
            MG.add_node(long(token[0]),nType=int(token[1]))
            MG.add_node(long(token[2]),nType=int(token[3]))
            # Adding edges
            MG.add_edge(long(token[0]),long(token[2]),  # to and from
                    days=int(token[4]),                 # days
                    calls=int(token[5]),                # calls
                    secs=int(token[6]),                 # call duration
                    texts=long(token[7]))               # texts
            print line
    return MG
