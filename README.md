# DCEL

## Assumptions and Format
Capping number of vertices, edges, and faces at 1000.

Halfedges are outputted in an order corresponding to the time of creation of their parent edges (possible duplicates).

An "outside" extraneous face is counted and outputted.

Split command values in the split file are assumed to be prefixed with "Split "

Point command values in the point file are assumed to be prefixed with "Id: "
