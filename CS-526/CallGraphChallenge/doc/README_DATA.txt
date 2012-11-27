Link Analytics Data README
The following details the structure of the data you will be working with for the final project of this course:

The first set of files are for two different cities: Moria and Standelf
from month 1:
Moria_1.graph
Standelf_1.graph

Each file is space-delimited with no header or trailer. Each line in these
files is formatted as follows:

A A_type B B_type num_days num_calls num_secs num_texts

The A and B identifiers are up to 15 digits long and will not fit in a
32-bit type, so you will need to use a 64-bit type (or string) to store
these identifiers.

The second set of files are for the same cities:
Moria_2.graph
Standelf_2.graph

These files are formatted identically to the first set. These files
were constructed as follows:
- Take the original network for month 2 for the respective city and
relabel all the nodes in a 1-1 and onto way (call this function f(x))
- Remove 25% of the edges at random - each edge is a separate Bernoulli
trial and is deleted from the graph with probability .25

There are several additional files for month 2:
Moria_2.edges
Standelf_2.edges

Each line in these files is an edge in space-delimited form:

f(A) f(B)

There are two possibilities:
1) The edge (f(A),f(B)) existed in the original network for month 2 and it
was removed
2) This is a randomly generated edge that did not exist in the original
network

You must decide which category each edge (f(A),f(B)) falls into by assigning
it either a 1 (if you think the edge was removed), or a 0 (if you think
the edge was randomly generated). Your final answer should be two files
where you simply append a 0 or 1 to each line in the two .edges files.
Please maintain the order of the edges (sorted ascending by f(A)).

In the interest of transparency, there are two more files that hold
the information about which edges were in the original month 2 network
and were removed, and which edges were randomly generated:
Moria_2.solution
Standelf_2.solution

These are formatted as follows

A/D X Y f(X) f(Y)

The A indicates that it was a randomly added edge, and the D indicates
that it was deleted. X and Y are the IDs for this node in month 1, and
f(X) and f(Y) are the IDs for this node in month 2. These two files
are encrypted with a password that will be provided to you at the end
of the challenge so you can verify your results and hopefully gain
an even deeper understanding of these networks.