#include "../inc/dcel.hpp"


graph::graph() {

    MAXN = 1000;

    vertNum = 0;
    edgeNum = 0;
    faceNum = 0;

    vertArr.resize(MAXN);
    edgeArr.resize(MAXN);
    faceArr.resize(MAXN);
}
graph::graph(FILE *ifp, FILE *sfp, FILE *ofp) : graph::graph() {

    this->ifp = ifp;
    this->sfp = sfp;
    this->ofp = ofp;
}

pVertex graph::MakeVertex(double x, double y) {
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
    vertArr[vertNum] = newVert;
    newVert->ind = vertNum;
    vertNum++;

    return newVert;
}

pEdge graph::MakeEdge(pHalfEdge h, pVertex P, pVertex Q) {
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
        faceArr[faceNum] = f1;
        f1->ind = faceNum;
        faceNum++;

    } else {

        if(Q->ray) {
            // if edge splits an existing face.

            // graph must grow from the initial edge (ie, h must be NULL only once per execution)
            // hence, non-nullity of Q->ray implies a face split


            pFace newF = new face;

            // make halfEdges of the new edge the checkpoint for faces
            newF->side = h2;
            h->f->side = h1;

            // reordering data to accommodate new edge
            h1->nxt = h->nxt;
            h->nxt->prev = h1;

            h->nxt = h2;
            h2->prev = h;

            // traversing cycle through h1
            // to find required data for h2
            pHalfEdge temp = h1;
            while(temp->head != Q) {
                temp = temp->nxt;
            }
            h1->f = temp->f;

            h2->nxt = temp->nxt;
            temp->nxt->prev = h2;

            temp->nxt = h1;
            h1->prev = temp;

            // assigning faces to halfEdges of cycle through h2
            temp = h2;
            while(temp->head != P) {
                temp->f = newF;
                temp = temp->nxt;
            }

            faceArr[faceNum] = newF;
            newF->ind = faceNum;
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
    edgeArr[edgeNum] = newEdge;
    newEdge->ind = edgeNum;
    edgeNum++;

    return newEdge;
}


pVertex graph::SplitEdge(pEdge e, pHalfEdge h, int ind) {

    // creating a new vertex with mid-point co-ordinates
    pVertex v;
    v = MakeVertex((e->t1->tail->x + e->t1->head->x)/2, (e->t1->tail->y + e->t1->head->y)/2);

    pEdge se = new edge;
    se->ind = ind;
    edgeArr[ind] = se;

    pHalfEdge
        h1 = new halfEdge,
        h2 = new halfEdge;

    // making sure face checkpoints remain twins
    if(e->t1->f->side == e->t1) {

        e->t1->f->side = h->nxt->twin;
        e->t2->f->side = h1;
    }
    
    // reordering data to accommodate new edge

    h1->f = h->nxt->f;
    h1->head = v;
    h1->nxt = h->nxt;
    h1->parent = e;
    h1->prev = h;
    h1->tail = h->head;
    h1->twin = h->nxt->twin;

    h2->f = h->nxt->twin->f;
    h2->head = v;
    h2->nxt = h->nxt->twin;
    h2->parent = se;
    h2->prev = h->nxt->twin->prev;
    h2->tail = h->nxt->head;
    h2->twin = h->nxt;

    e->t1 = h->nxt->twin;
    e->t2 = h1;

    se->t1 = h2;
    se->t2 = h->nxt;

    v->ray = h->nxt;

    h->nxt->tail = v;
    h->nxt->twin->tail = v;

    h->nxt->parent = se;



    h->nxt->twin->prev->nxt = h2;
    h->nxt->twin->prev = h2;

    h->nxt->prev = h1;
    h->nxt = h1;

    return v;
}

void graph::SplitEdge(pEdge e1, pEdge e2) {

    // making use of the index argument
    // to follow naming convention

    pVertex v1 = SplitEdge(e1, e1->t2->prev, edgeNum+1);
    pVertex v2 = SplitEdge(e2, e2->t1->prev, edgeNum+2);


    MakeEdge(e1->t2, v1, v2);

    edgeNum+=2;
}

int graph::GetVertCount() {
    return this->vertNum;
}
int graph::GetEdgeCount() {
    return this->edgeNum;
}
int graph::GetFaceCount() {
    return this->faceNum;
}

pVertex graph::GetVert(int ind) {

    if(ind < 0 or ind > vertNum) {
        std::cerr << "vertArr out of bounds!";
        return NULL;
    }

    return vertArr[ind];
}
pEdge graph::GetEdge(int ind) {

    if(ind < 0 or ind > edgeNum) {
        std::cerr << ind << " " << edgeNum << " edgeArr out of bounds!\n";
        return NULL;
    }

    return edgeArr[ind];
}
pFace graph::GetFace(int ind) {

    if(ind < 0 or ind > faceNum) {
        std::cerr << "faceArr out of bounds!";
        return NULL;
    }

    return faceArr[ind];
}

graph::~graph() {

}