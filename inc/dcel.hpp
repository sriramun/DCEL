#ifndef _DCEL_H_
#define _DCEL_H_

#include <iostream>
#include <fstream>

#include <vector>


extern int // global index counter
    edgeNum,
    faceNum,
    vertNum;


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

// vectors to store vertices, edges and faces
extern std::vector<pVertex> vertArr; 
extern std::vector<pEdge> edgeArr; 
extern std::vector<pFace> faceArr; 
    
// function to create a new vertex
pVertex MakeVertex (double, double);

// function to create a new edge
pEdge MakeEdge (pHalfEdge, pVertex, pVertex);

#endif