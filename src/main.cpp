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

void MakeSplits(graph *G) {

    char cmd[32];
    while(fscanf(G->sfp, "%31s", cmd) == 1) {

        if(!strcmp(cmd, "Split")) {

            int x,y;
            fscanf(G->sfp, "%d %d", &x, &y);

            G->SplitEdge(G->GetEdge(x), G->GetEdge(y));

        } else {

            break;
        }
    }
}

void MakePoints(graph *G) {

    char cmd[32];
    while(fscanf(G->pfp, "%31s", cmd) == 1) {

        // std::cout << cmd << '\n';

        if(!strcmp(cmd, "Id:")) {

            int id;
            double x,y;
            fscanf(G->pfp, "%d %lf %lf", &id, &x, &y);
    
            // debug
            // std::cout << "what \n";
            // std::cout << id << " " << x << " " << y << "\n";

            G->MakePoint(id, x, y);

        } else {

            break;
        }
    }
}

void PrintGraph(graph *G) {
    // function to print required output

    fprintf(G->ofp, "Edges:\n");
    for(int i = 0; i < G->GetEdgeCount(); i++) {

        fprintf(G->ofp, "%d\n", G->GetEdge(i)->ind);
    }


    fprintf(G->ofp, "Halfedges:\n");
    for(int i = 0; i < G->GetEdgeCount(); i++) {

        pHalfEdge temp = G->GetEdge(i)->t1;

        fprintf(G->ofp, 
            "startVertexIndex=%d endVertexIndex=%d nextEdge=%d previousEdge=%d faceIndex=%d edgeIndex=%d\n", 
            temp->tail->ind, temp->head->ind, temp->nxt->parent->ind, temp->prev->parent->ind, temp->f->ind, temp->parent->ind);

        temp = temp->twin;

        fprintf(G->ofp, 
            "startVertexIndex=%d endVertexIndex=%d nextEdge=%d previousEdge=%d faceIndex=%d edgeIndex=%d\n", 
            temp->tail->ind, temp->head->ind, temp->nxt->parent->ind, temp->prev->parent->ind, temp->f->ind, temp->parent->ind);
    }
    
    std::cout << "please end this 'DCEL' nightmare.\n";

    fprintf(G->ofp, "Faces:\n");
    for(int i = 0; i < G->GetFaceCount(); i++) {
    
        pHalfEdge temp;
        pFace f;

        if(f = G->GetFace(i)) {
            
            temp = f->side;

            fprintf(G->ofp, "FaceIndex:%d Edges ", f->ind);
            do {

                fprintf(G->ofp, "%d->", temp->parent->ind);

                temp = temp->nxt;

            } while(temp != f->side);

            fseek(G->ofp, -2, SEEK_CUR);
            fprintf(G->ofp, " \n");

        }
    }

    for(auto p : G->pointArr)
        fprintf(G->ofp, "Id: %d %d\n", p->id, p->f->ind);
}


int main(int argc, char *argv[]) {

    // ifp: file pointer to input file
    // sfp: file pointer to split file
    // pfp: file pointer to point file
    // ofp: file pointer to output file
    FILE *ifp, *ofp, *sfp, *pfp;

    ifp = fopen(argv[1], "r");
    sfp = fopen(argv[2], "r");
    pfp = fopen(argv[3], "r");
    ofp = fopen(argv[4], "w");

    // creating dcel workspace
    graph *G = new graph(ifp, sfp, pfp, ofp);

    // creating square
    pPoly testQuad;
    testQuad = new poly;

    // calling necessary functions
    MakeQuad(testQuad, G);
    MakeSplits(G);
    MakePoints(G);

    // printing result
    PrintGraph(G);

    fcloseall();

    return 0;
}
