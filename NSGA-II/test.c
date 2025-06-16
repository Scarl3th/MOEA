# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>

# include "global.h"
# include "rand.h"

int mainn(int argc, char **argv){

    problem_instance * pi = malloc (sizeof(problem_instance));
    population *parent_pop;
    population *child_pop;
    population *mixed_pop;

    if (argc<2)
    {
        printf("\n Usage ./nsga2r instance_route random_seed popsize ngen nobj pcross_bin pmut_bin\n./nsga2r 0.123 BOSCPchiquito.dat 100 100 2 0.6 0.01\n");
        exit(1);
    }

    char * instance_route = argv[2];
    readInputFile(instance_route, pi);

    popsize = atoi(argv[3]);
    if (popsize<4 || (popsize%4)!= 0){
        printf("\n population size read is : %d",popsize);
        printf("\n Wrong population size entered, hence exiting \n");
        exit (1);
    }
    ngen = atoi(argv[4]);
    if (ngen<1){
        printf("\n number of generations read is : %d",ngen);
        printf("\n Wrong nuber of generations entered, hence exiting \n");
        exit (1);
    }
    nobj = atoi(argv[5]);
    if (nobj<1){
        printf("\n number of objectives entered is : %d",nobj);
        printf("\n Wrong number of objectives entered, hence exiting \n");
        exit (1);
    }
    
    nbin = pi->nU - pi->n_medical_centers;

    pcross_bin = atof (argv[6]);
    if (pcross_bin<0.0 || pcross_bin>1.0){
        printf("\n Probability of crossover entered is : %e",pcross_bin);
        printf("\n Entered value of probability of crossover of binary variables is out of bounds, hence exiting \n");
        exit (1);
    }
    pmut_bin = atof (argv[7]);
    if (pmut_bin<0.0 || pmut_bin>1.0){
        printf("\n Probability of mutation entered is : %e",pmut_bin);
        printf("\n Entered value of probability  of mutation of binary variables is out of bounds, hence exiting \n");
        exit (1);
    }

    nbinmut = 0;
    nrealmut = 0;
    nbincross = 0;
    nrealcross = 0;
    parent_pop = (population *)malloc(sizeof(population));
    child_pop = (population *)malloc(sizeof(population));
    mixed_pop = (population *)malloc(sizeof(population));
    allocate_memory_pop (parent_pop, popsize);
    allocate_memory_pop (child_pop, popsize);
    allocate_memory_pop (mixed_pop, 2*popsize);
    randomize();
    initialize_pop(parent_pop,pi);

}