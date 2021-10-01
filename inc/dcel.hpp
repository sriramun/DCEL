#ifndef _DCEL_H_
#define _DCEL_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>


typedef struct vertex vertex;
typedef struct halfEdge halfEdge;
typedef struct edge edge;
typedef struct face face;

// VERTICES
typedef struct vertex{

    /* Contents: 

        ind: Index

        x: Abscissa
        y: Ordinate

        ray: Arbitrary halfEdge originating from this
    */

    int ind;
    double x,y;
    halfEdge *ray;

} *pVertex;

// HALF-EDGES
typedef struct halfEdge {

    /* Contents: 

        head: Ending vertex
        tail: Starting vertex

        nxt: Next halfEdge
        prev: Previous halfEdge
        twin: Twin halfEdge

        parent: Overlying edge

        f: Face bounded by this
    */

    vertex *head, *tail;
    halfEdge *nxt, *prev, *twin;
    edge *parent;
    face *f;

} *pHalfEdge;

// EDGES
typedef struct edge {

    /* Contents: 

        ind: Index

        t1,
        t2: Constituent halfEdges 
    */

    int ind;
    halfEdge *t1, *t2;

} *pEdge;

// FACES
typedef struct face {

    /* Contents: 

        ind: Index

        side: Pointer to a bounding halfEdge
    */

    int ind;
    halfEdge *side;

} *pFace;


class graph {
private:

    int // global index counters
        vertNum,
        edgeNum,
        faceNum;

    int MAXN;

    // vectors to store vertices, edges and faces
    std::vector<pVertex> vertArr; 
    std::vector<pEdge> edgeArr; 
    std::vector<pFace> faceArr; 

public:
    // ifp: file pointer to input file
    // sfp: file pointer to split file
    // ofp: file pointer to output file
    FILE *ifp, *ofp, *sfp;
    

public:

    graph();
    graph(FILE*, FILE*, FILE*);

    // function to create a new vertex
    pVertex MakeVertex (double, double);

    // function to create a new edge
    pEdge MakeEdge (pHalfEdge, pVertex, pVertex);

    // functions to split edges
        // split a single edge with a tailing halfEdge and index
    pVertex SplitEdge(pEdge, pHalfEdge, int);
        // split two edges and join their mid-points
    void SplitEdge(pEdge, pEdge);

    // functions to access private data members
    int GetVertCount();
    int GetEdgeCount();
    int GetFaceCount();

    // functions to access private containers
    pVertex GetVert(int);
    pEdge GetEdge(int);
    pFace GetFace(int);

    ~graph();
};

#endif