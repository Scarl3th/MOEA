/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Routine to evaluate objective function values and constraints for a population */
void evaluate_pop (population *pop, problem_instance *pi)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        evaluate_ind(&(pop->ind[i]),pi);
    }
    return;
}

/* Routine to evaluate objective function values and constraints for an individual */
/*void evaluate_ind (individual *ind)
{
    int j;
    test_problem (ind->xreal, ind->xbin, ind->gene, ind->obj, ind->constr);
    if (ncon==0)
    {
        ind->constr_violation = 0.0;
    }
    else
    {
        ind->constr_violation = 0.0;
        for (j=0; j<ncon; j++)
        {
            if (ind->constr[j]<0.0)
            {
                ind->constr_violation += ind->constr[j];
            }
        }
    }
    return;
}*/


/* Routine to evaluate objective function values and constraints for an individual */
void evaluate_ind (individual *ind, problem_instance *pi)
{
    /*Acá la evaluación completa. Deben setearse los valores de obj y constr_violation. */
    /*
    void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
    {
    obj[0] = pow(xreal[0],2.0);
    obj[1] = pow((xreal[0]-2.0),2.0);
    return;
    }
    */
    int i,j,sum=0;
    int *personas_atendidas = malloc(pi->nS*sizeof(int));
    double overlap = 0;

    for (i = 0; i < pi->nS; i++)
    {
        personas_atendidas[i] = 0;
    }
    

    /*Calcular cantidad de personas no atendidas por sector*/
    for (i = 0; i < pi->nU; i++)
    {
        /*Atendidas por carritos*/
        if(i < nbin){
            if(ind->gene[i] == 1){
                for (j = 0; j < pi->nS; j++)
                {
                    if(pi->coverage_matrix[j][i] == 1){
                        personas_atendidas[j] = min(pi->s[j].n_pop,personas_atendidas[j]+pi->u[i].c_per_sec);
                    }
                }
            } 
        }
        
        /*Atendidas por centros medicos*/
        else{
            for (j = 0; j < pi->nS; j++)
            {
                if(pi->coverage_matrix[j][i] == 1){
                    personas_atendidas[j] = min(pi->s[j].n_pop,personas_atendidas[j]+pi->u[i].c_per_sec);
                }
            }
        }
    }
    
    /*Sumamos catnidad de personas no atendidas total*/
    for (i = 0; i < pi->nS; i++)
    {
        sum += personas_atendidas[i];
    }

    /*Asignamos cantidad de personas no atendidas*/
    ind->obj[0] = pi->total_pop - sum;
    
    /*Calcular nivel total de overlapping*/
    for (i = 0; i < pi->nU; i++)
    {
        /*Overlapping de carritos*/
        if(i < nbin){
            if(ind->gene[i] == 1){

                /*Con otros carritos*/
                for (j = i+1; j < nbin; j++)
                {
                    if(ind->gene[j] == 1){
                        if(pi->overlap_matrix[i][j] > 0){overlap += pi->overlap_matrix[i][j];
                            /*printf("carrito %d y carrito %d se hacen overlapping\n",i+1,j+1);*/
                        }
                    }
                }
                /*Con centros medicos*/
                for (j = nbin; j < pi->nU; j++)
                {
                    if(pi->overlap_matrix[i][j] > 0){overlap += pi->overlap_matrix[i][j];
                        /*printf("carrito %d y cm %d se hacen overlapping\n",i+1,j+1);*/
                    }
                }
            }
        }

        /*Overlapping de centros médicos*/
        else{
            for (j = i+1; j < pi->nU; j++)
            {
                if(pi->overlap_matrix[i][j] > 0){overlap += pi->overlap_matrix[i][j];
                    /*printf("cm %d y cm %d se hacen overlapping\n",i+1,j+1);*/
                }
            }
        }
    }
    
    ind->obj[1] = overlap;
    
    /*Calcular cantidad de 1s sobrantes segun maxima cantidad de carritos*/
    sum = 0;
    if (nbin!=0)
    {
        for (j = 0; j<nbin; j++)
        {
            
            if(ind->gene[j] == 1){sum += 1;
                /*printf("carrito %d seleccionado\n",j+1);*/
            }
        }
    }
    if((sum + pi->n_medical_centers) > pi->max_carts){
        ind->constr_violation = sum - (pi->max_carts - pi->n_medical_centers);
        /*printf("A reparar, pues %d > %d\n",sum, pi->max_carts);*/
        repair_ind(ind,pi);
    }
    else{
        ind->constr_violation = 0;
    }
    /*printInd(ind);*/
    /*printf("\n\n\n");*/
    
    return;
}

/*Reparar individuo infactible*/
void repair_ind(individual *ind, problem_instance *pi){

    int *positions = malloc(((pi->max_carts - pi->n_medical_centers)+ind->constr_violation)*sizeof(int));
    int i,j=0,tmp;

    for (i = 0; i < nbin; i++)
    {
        if(ind->gene[i] == 1){
            positions[j] = i;
            j++;
        }
    }

    for (i = ((pi->max_carts - pi->n_medical_centers)+ind->constr_violation) - 1; i > 0; i--) {
            j = (int)(rndreal(0,i+1));
            if(j == i+1){j--;}
            tmp = positions[i];
            positions[i] = positions[j];
            positions[j] = tmp;
     }

    for (i = 0; i < ind->constr_violation; i++) {
            ind->gene[positions[i]] = 0;
    }

    evaluate_ind(ind,pi);
    
    return;
}
