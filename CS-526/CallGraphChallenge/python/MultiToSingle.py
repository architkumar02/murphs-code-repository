def MultiToSingle2(graph=g,attr='calls'):
    for n, nbrsdict in graph.adjacency_iter():
        for nbr,keydict in nbrsdict.items():
            for key,eattr in keydict.items():
                if attr in eattr:
                    print n,nbr,eattr[attr]
                    
def MultiToSingle(G=g,attr='calls'):
    return [(u,v,edata[attr]) for u,v,edata in G.edges(data=True) if attr in edata]
MultiToSingle(g)
