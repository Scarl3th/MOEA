/* Data initializtion routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to initialize a population randomly */
void initialize_pop (population *pop, problem_instance *pi)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        initialize_ind (&(pop->ind[i]), pi);
    }
    return;
}

/* Function to initialize an individual randomly */
void initialize_ind (individual *ind, problem_instance *pi)
{
    
    int j, sum=0;
    if (nbin!=0)
    {
        for (j=0; j<nbin; j++)
        {
            if (randomperc() <= 0.5)
            {
                ind->gene[j] = 0;
            }
            else
            {
                ind->gene[j] = 1;
                sum += 1;
            }
        }
    }
    /*
    if(sum > pi->max_carts){
        ind->constr_violation = sum- pi->max_carts;
    }
    else{
        ind->constr_violation = 0;
    }
    */
    ind->rank = 0;
    ind->crowd_dist = 0;
    evaluate_ind(ind,pi);

    return;
}
