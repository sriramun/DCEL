#include "../inc/dcel.hpp"

int edgeNum = 0,
    faceNum = 0,
    vertNum = 0;

std::vector<pVertex> vertArr; 
std::vector<pEdge> edgeArr; 
std::vector<pFace> faceArr; 

pVertex MakeVertex(double x, double y) {
    // x: abscissa
    // y: ordinate

    // create required vertex
    pVertex newVert = new vertex;

    // data assignment
    newVert->x = x;
    newVert->y = y;
    newVert->ray = NULL;
    newVert->ind = vertNum;

    // update global vertex array and index
    vertArr.push_back(newVert);
    vertNum++;

    return newVert;
}

pEdge MakeEdge(pHalfEdge h, pVertex P, pVertex Q) {
    // h: previous halfEdge
    // P,Q: vertices to be connected

    // create constituent (twin) halfEdges
    pHalfEdge 
        h1 = new halfEdge,
        h2 = new halfEdge;


    // let h1 be halfEdge originating from Q
    // and h2 from P

    h1->twin = h2;
    h1->head = P;
    h1->tail = Q;
        
    h2->twin = h1;
    h2->head = Q;
    h2->tail = P;

    if(h == NULL) {
        // if first edge being created

        // create an 'outside' face
        pFace f1 = new face;

        P->ray = h2;
        Q->ray = h1;

        // h1 & h2 are cyclic
        h1->nxt = h2;
        h1->prev = h2;

        h2->nxt = h1;
        h2->prev = h1;

        // and bound f1
        h1->f = f1;
        h2->f = f1;

        // // replaced faceNum with '0'
        // // subject to change

        // data assignment (face)
        f1->ind = 0;
        f1->side = h1; 

        // update global face array and index
        faceArr.push_back(f1);
        faceNum++;

    } else {

        if(Q->ray) {
            // if edge splits an existing face.

            // graph must grow from the initial edge (ie, h must be NULL only once per execution)
            // hence, non-nullity of Q->ray implies a face split


            // create a new face
            pFace
                f1 = new face;

            // data assignment and updation
            h1->nxt = h->nxt;
            h->nxt->prev = h1;

            h->nxt = h2;
            h2->prev = h;
            
            // cycle through h1 and update f-member to new face
            pHalfEdge temp = h1;
            while(temp->head != Q) {
                
                temp->f = f1;
                temp = temp->nxt;
            }
            // final temp must point to h1->prev
            // ergo h2->nxt must point to temp->twin

            f1->side = temp;
            temp->nxt = h1;
            h1->prev = temp;

            temp->f = f1;

            h2->nxt = temp->twin;
            temp->twin->prev = h2;

            h2->f = temp->twin->f;

            f1->ind = faceNum;

            // update global face array and index
            faceArr.push_back(f1);
            faceNum++;
        }

        else {
            // if edge does not split
            // when Q is new

            h1->nxt = h->nxt;
            
            if(h->nxt)
                h->nxt->prev = h1;

            h->nxt = h2;
            h2->prev = h;

            // h2 loops back to h1
            h2->nxt = h1;
            h1->prev = h2;

            h1->f = h->f;
            h2->f = h->f;

            Q->ray = h1;
        }
    }

    // create required edge
    pEdge newEdge = new edge;

    // data assignment
    h1->parent = newEdge;
    h2->parent = newEdge;
    newEdge->t1 = h1;
    newEdge->t2 = h2;
    newEdge->ind = edgeNum;

    // update global edge array and index
    edgeArr.push_back(newEdge);
    edgeNum++;

    return newEdge;
}

