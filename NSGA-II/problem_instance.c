/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "global.h"
# include "rand.h"


int isAUbication(int name, problem_instance pi){
    if(name <= pi.nU) return 1;
    else return 0;
}
int isASector(int name, problem_instance pi){
    if(name > pi.nU) return 1;
    else return 0;
}

/*

int getNodeId(int name, problem_instance pi){
    int i, j;
    int count=0;
    for (i=0; i<pi.nDepots; i++){
        for (j=0; j<3; j++){
            if(name == pi.depots[i].names[j])
                return count;
        }
        count++;
    }
    for (i=0; i<pi.nClients; i++){
        if(name == pi.clients[i].name)
            return count;
        count++;
    }
    return -1;
}

int getNodeIndex(int name, problem_instance pi){
    int i, j;
    int count=0;
    for (i=0; i<pi.nDepots; i++){
        for (j=0; j<3; j++){
            if(name == pi.depots[i].names[j])
                return count;
        }
        count++;
    }
    count=0; 
    for (i=0; i<pi.nClients; i++){
        if(name == pi.clients[i].name)
            return count;
        count++;
    }
    return -1;
}

*/

void printProblemInstance(problem_instance *pi){

    int i;
    printf("Max Carts: %d\n", pi->max_carts);
    printf("Sectors: %d\n", pi->nS);
    for (i=0; i<pi->nS; i++){
        printf("%d: %d", pi->s[i].id, pi->s[i].n_pop);
        printf("\n");
    }

    printf("Ubications: %d\n",pi->nU);
    printf("N Medical Centers: %d\n",pi->n_medical_centers);
    for (i=0; i<pi->nU; i++){
        printf("%d: %d, %d\n", pi->u[i].id, pi->u[i].c_per_sec, pi->u[i].is_medical_center);
    }

    return;

}


