/* Routines for storing population data into files */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to print the information of a population in a file */
void report_pop (population *pop, FILE *fpt)
{
    int i, j;
    for (i=0; i<popsize; i++)
    {
        for (j=0; j<2; j++)
        {
            fprintf(fpt,"%f\t",pop->ind[i].obj[j]);
        }
        fprintf(fpt,"%d\t",pop->ind[i].constr_violation);
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
                if(pop->ind[i].gene[j] == 1){
                    fprintf(fpt,"x%d\t",j+1);
                }
            }
        }

        fprintf(fpt,"%d\t",pop->ind[i].rank);
        fprintf(fpt,"%e\n",pop->ind[i].crowd_dist);
    }
    return;
}

/* Function to print the information of feasible and non-dominated population in a file */
void report_feasible (population *pop, FILE *fpt)
{
    int i, j;
    for (i=0; i<popsize; i++)
    {
        if(pop->ind[i].constr_violation == 0){
            for (j=0; j<2; j++)
            {
                fprintf(fpt,"%f\t",pop->ind[i].obj[j]);
            }
            fprintf(fpt,"%d\t",pop->ind[i].constr_violation);
            if (nbin!=0)
            {
                for (j=0; j<nbin; j++)
                {
                    if(pop->ind[i].gene[j] == 1){
                        fprintf(fpt,"x%d\t",j+1);
                    }
                }
            }

            fprintf(fpt,"%d\t",pop->ind[i].rank);
            fprintf(fpt,"%e\n",pop->ind[i].crowd_dist);
        }
    }
    return;
}
