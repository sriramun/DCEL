#include "../inc/dcel.hpp"

// POLYGONS
typedef struct {

    /* Contents: 

        v: Array of pVertices
        
        e: Array of pEdges
    */

    pVertex *v;
    pEdge *e;

} poly, *pPoly;


void MakeQuad(pPoly q1, graph *G) {

    // function to create a quadrilateral polygon


    // 4 vertices, 4 edges
    q1->v = (pVertex *) calloc(4, sizeof(pVertex)); 
    q1->e = (pEdge *) calloc(4, sizeof(pEdge));

    // input vertices from file
    for(int i = 0; i < 4; i++) {

        double x,y;
        fscanf(G->ifp, "%lf %lf", &x, &y);

        q1->v[i] = G->MakeVertex(x, y);
    }

    // make edges in a clock-wise manner
    // v0->(first edge) v1->v2->v3->(split-face) v0
    q1->e[0] = G->MakeEdge(NULL, q1->v[0], q1->v[1]);
    for(int i = 1; i < 4; i++) {

        q1->e[i] = G->MakeEdge(q1->e[i-1]->t2, q1->v[i], q1->v[(i+1)%4]);
    }
}

void PrintQuad(pPoly q1, graph *G) {

    // function to print a quadrilateral's details

    fprintf(G->ofp, "Edges:\n");

    pHalfEdge temp = q1->e[0]->t2;

    do {
        
        fprintf(G->ofp, "%d\n", temp->parent->ind);
        temp = temp->nxt;

    } while(temp != q1->e[0]->t2);

    fprintf(G->ofp, "Halfedges:\n");
    do {

        fprintf(G->ofp, 
            "startVertexIndex=%d endVertexIndex=%d nextEdge=%d previousEdge=%d faceIndex=%d edgeIndex=%d\n", 
            temp->tail->ind, temp->head->ind, temp->nxt->parent->ind, temp->prev->parent->ind, temp->f->ind, temp->parent->ind);

        temp = temp->nxt;

    } while(temp != q1->e[0]->t2);

    // iterating through faceArr
    fprintf(G->ofp, "Faces:\n");
    for(int i = 1; i < G->GetFaceCount(); i++) {

        pFace f;

        if(f = G->GetFace(i)) {
            
            // fprint decremented face index
            // since faceArr[0] points to 'outside' face
            fprintf(G->ofp, "FaceIndex:%d Edges ", f->ind -1);

            temp = f->side;
            do {

                fprintf(G->ofp, "%d->", temp->parent->ind);
                temp = temp->prev;

            } while(temp != f->side);

            // erase leading arrow
            fseek(G->ofp, -2, SEEK_CUR);
            fprintf(G->ofp, " \n");
        }
    }
}

int main(int argc, char *argv[]) {

    // ifp: file pointer to input file
    // ofp: file pointer to output file
    FILE *ifp, *ofp;

    ifp = fopen(argv[1], "r");
    ofp = fopen(argv[2], "w");

    // creating dcel workspace
    graph *G = new graph(ifp, ofp);

    // creating square
    pPoly testQuad;
    testQuad = new poly;

    // and calling necessary functions
    MakeQuad(testQuad, G);
    PrintQuad(testQuad, G);

    fcloseall();

    return 0;
}