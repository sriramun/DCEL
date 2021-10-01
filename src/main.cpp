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


void MakeQuad(pPoly q1, FILE *ifp) {

    // function to create a quadrilateral polygon


    // 4 vertices, 4 edges
    q1->v = (pVertex *) calloc(4, sizeof(pVertex)); 
    q1->e = (pEdge *) calloc(4, sizeof(pEdge));

    // input vertices from file
    for(int i = 0; i < 4; i++) {

        double x,y;
        fscanf(ifp, "%lf %lf", &x, &y);

        q1->v[i] = MakeVertex(x, y);
    }

    // make edges in a clock-wise manner
    // v0->(first edge) v1->v2->v3->(split-face) v0
    q1->e[0] = MakeEdge(NULL, q1->v[0], q1->v[1]);
    for(int i = 1; i < 4; i++) {

        q1->e[i] = MakeEdge(q1->e[i-1]->t2, q1->v[i], q1->v[(i+1)%4]);
    }
}

void PrintQuad(pPoly q1, FILE *ofp) {

    // function to print a quadrilateral's details

    fprintf(ofp, "Edges:\n");

    pHalfEdge temp = q1->e[0]->t2;

    do {
        
        fprintf(ofp, "%d\n", temp->parent->ind);
        temp = temp->nxt;

    } while(temp != q1->e[0]->t2);

    fprintf(ofp, "Halfedges:\n");
    do {

        fprintf(ofp, 
            "startVertexIndex=%d endVertexIndex=%d nextEdge=%d previousEdge=%d faceIndex=%d edgeIndex=%d\n", 
            temp->tail->ind, temp->head->ind, temp->nxt->parent->ind, temp->prev->parent->ind, temp->f->ind, temp->parent->ind);

        temp = temp->nxt;

    } while(temp != q1->e[0]->t2);

    // iterating through faceArr
    fprintf(ofp, "Faces:\n");
    for(int i = 1; i < faceNum; i++) {
        if(faceArr[i]) {
            
            // fprint decremented face index
            // since faceArr[0] points to 'outside' face
            fprintf(ofp, "FaceIndex:%d Edges ", faceArr[i]->ind -1);

            temp = faceArr[i]->side;
            do {

                fprintf(ofp, "%d->", temp->parent->ind);
                temp = temp->prev;

            } while(temp != faceArr[i]->side);

            // erase leading arrow
            fseek(ofp, -2, SEEK_CUR);
            fprintf(ofp, " \n");
        }
    }
}

int main(int argc, char *argv[]) {

    // ifp: file pointer to input file
    // ofp: file pointer to output file
    FILE *ifp, *ofp;

    ifp = fopen(argv[1], "r");
    ofp = fopen(argv[2], "w");


    // creating square
    pPoly testQuad;
    testQuad = new poly;

    // and calling necessary functions
    MakeQuad(testQuad, ifp);
    PrintQuad(testQuad, ofp);

    fcloseall();

    return 0;
}