# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "global.h"
# include "rand.h"

void printInd(individual *ind){

    int i,j;

    printf("Variables: \n");
    for (i = 0; i < nbin; i++)
    {
        printf("%d ",ind->gene[i]);
    }
    printf("\n");
    printf("Rank: %d\nConst_vio: %d\nObj1: %f\nObj2: %f\nCrow_d: %f\n",
        ind->rank,ind->constr_violation,ind->obj[0],ind->obj[1], ind->crowd_dist);

}