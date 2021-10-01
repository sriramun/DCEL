#ifndef _DCEL_H_
#define _DCEL_H_

#include <stdio.h>
#include <stdlib.h>

extern int // global index counter
    edgeNum,
    faceNum,
    vertNum;

extern const int
    MAXN; // index limit


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

// arrays to store vertices, edges and faces
extern pVertex *vertArr; 
extern pEdge *edgeArr;
extern pFace *faceArr;

// function to create a new vertex
pVertex MakeVertex (double, double);

// function to create a new edge
pEdge MakeEdge (pHalfEdge, pVertex, pVertex);

#endif