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
    pVertex SplitEdge(pEdge, pHalfEdge, int);
    void SplitEdge(pEdge, pEdge);

    int GetVertCount();
    int GetEdgeCount();
    int GetFaceCount();

    pVertex GetVert(int);
    pEdge GetEdge(int);
    pFace GetFace(int);

    ~graph();
};

#endif