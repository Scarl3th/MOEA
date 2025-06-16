/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "global.h"
# include "rand.h"


void findDef(FILE *f, char *def) {
    char word[1024];
    /* assumes no word exceeds length of 1023 */
    while (fscanf(f, " %1023s", word)) {
        if(strcmp(word,def) == 0) break;
    }
}

void removeSemicolon(char *line){
    strtok(line, ";");
}

int countWords(char *line){
    int words;
    char linet[65638], *token;
    strcpy(linet, line);

    words = 0;
    token = strtok(linet, " ");

    while( token != NULL ) {
        words ++;
        token = strtok(NULL, " ");
    }
   return words;
}

void readMaxCarts(FILE *f, problem_instance *pi) {
    int debug=0, maxCarts;

    char line[1024];
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    if (debug) printf("Line: %s\n", line);
    removeSemicolon(line);
    maxCarts = atoi(line);
    if (debug) printf("Max Carts: %d\n", maxCarts);
    pi->max_carts = maxCarts;
}


void readUbications(FILE *f, problem_instance *pi) {
    int debug=0, id=0;
    char *token;

    char line[65638];
    fgets(line, sizeof(line), f);

    if (debug) printf("Line: %s\n", line);
    removeSemicolon(line);
    if (debug) printf("Line: %s\n", line);
    pi->nU=countWords(line);
    printf("nU: %d \n",pi->nU);
    pi->n_medical_centers = 0;
    /*nbin = pi->nU;*/

    pi->u=malloc(pi->nU*sizeof(ubication));

    if (debug) printf("nU: %d\n", pi->nU);

    token=strtok(line, " ");
    while( token != NULL ) {
        if (debug) printf("%s\n", token);
        pi->u[id].id=atoi(token);
        pi->u[id].is_medical_center = 0;
        token=strtok(NULL, " ");
        id++;
    }
    if (debug) {
        for ( id=0; id<pi->nU; id++ )
            printf ("%d, %d\n", pi->u[id].id, pi->u[id].id);
    }
    if (debug) getchar();
}

void readSectors(FILE *f, problem_instance *pi) {
    int debug=0, id=0;
    char *token;

    char line[4096];
    fgets(line, sizeof(line), f);

    if (debug) printf("Line: %s\n", line);
    removeSemicolon(line);
    if (debug) printf("Line: %s\n", line);
    pi->nS=countWords(line);
    printf("nS: %d \n",pi->nS);

    pi->s=malloc(pi->nS*sizeof(sector));

    if (debug) printf("nP: %d\n", pi->nS);

    token=strtok(line, " ");
    while( token != NULL ) {
        if (debug) printf("%s\n", token);
        pi->s[id].id=atoi(token);
        token=strtok(NULL, " ");
        id++;
    }
    if (debug) {
        for ( id=0; id<pi->nS; id++ )
            printf ("%d, %d\n", pi->s[id].id, pi->s[id].id);
    }
    if (debug) getchar();
}

void readCapacity(FILE *f, problem_instance *pi) {
    int debug=0;
    int idNode;
    char *id, *c_per_sec;
    char line[1024];

   fgets(line, sizeof(line), f);

    while (1){
        fgets(line, sizeof(line), f);
        if (debug) printf("Line: %s\n", line);

        id=strtok(line, " ");
        if (strchr(id, ';') != NULL) break;

        if (isAUbication(atoi(id), *pi)){
            c_per_sec = strtok(NULL, " ");
            idNode = atoi(id)-1;
            if (debug) printf("id: %s, idNode: %d demand: %s\n", id, idNode, c_per_sec);
            pi->u[idNode].c_per_sec = atoi(c_per_sec);
        }
    }
}

void readMedicalCenters(FILE *f, problem_instance *pi) {
    int debug=0;
    int idNode;
    char *id, *is_medical_center;
    char line[1024];

   fgets(line, sizeof(line), f);

    while (1){
        fgets(line, sizeof(line), f);
        if (debug) printf("Line: %s\n", line);

        id=strtok(line, " ");
        if (strchr(id, ';') != NULL) break;

        if (isAUbication(atoi(id), *pi)){
            is_medical_center = strtok(NULL, " ");
            idNode = atoi(id)-1;
            if (debug) printf("id: %s, idNode: %d demand: %s\n", id, idNode, is_medical_center);
            pi->u[idNode].is_medical_center = atoi(is_medical_center);
            pi->n_medical_centers++;
        }
    }
}

void readPopulations(FILE *f, problem_instance *pi) {
    int debug=0;
    int idNode, total=0;
    char *id, *n_pop;
    char line[1024];

   fgets(line, sizeof(line), f);

    while (1){
        fgets(line, sizeof(line), f);
        if (debug) printf("Line: %s\n", line);

        id=strtok(line, " ");
        if (strchr(id, ';') != NULL) break;

        if (isASector(atoi(id), *pi)){
            n_pop = strtok(NULL, " ");
            idNode = atoi(id)-(pi->nU+1);
            if (debug) printf("id: %s, idNode: %d demand: %s\n", id, idNode, n_pop);
            pi->s[idNode].n_pop = atoi(n_pop);
            total += atoi(n_pop);
        }
    }
    pi->total_pop = total;
}

void readCoverageMatrix(FILE *f, problem_instance *pi) {
    int debug=0, i,j;
    int idFrom, idTo;
    char *from, *to, *coverage;
    char line[1024];

    pi->coverage_matrix = (int **)malloc(pi->nS * sizeof(int*));
    for(i=0; i<pi->nS; i++)
        pi->coverage_matrix[i] = (int *)malloc(pi->nU * sizeof(int));

    /* Inicializar valores en 0 */ 
    for (i = 0; i < pi->nS; i++)
    {
        for (j = 0; j < pi->nU; j++)
        {
            pi->coverage_matrix[i][j] = 0;
        } 
    }
    
    fgets(line, sizeof(line), f);

    while (1){
        fgets(line, sizeof(line), f);
        if (debug) printf("Line: %s\n", line);

        from=strtok(line, " ");
        if (strchr(from, ';') != NULL) break;

        to  = strtok(NULL, " ");
        coverage = strtok(NULL, " ");
        idFrom = atoi(from) - ((pi->nU)+1);
        idTo = atoi(to) - 1;
        /*printf("idFrom: %d, idTo: %d\n",idFrom,idTo);*/

        if (debug) printf("from: %s, to:%s , coverage: %s\n", from, to, coverage);
        pi->coverage_matrix[idFrom][idTo] = atoi(coverage);
    }
}

void readOverlapMatrix(FILE *f, problem_instance *pi) {
    int debug=0, i,j;
    int idFrom, idTo;
    char *from, *to, *overlap;
    char line[1024];

    pi->overlap_matrix = (double **)malloc(pi->nU * sizeof(double*));
    for(i=0; i<pi->nU; i++)
        pi->overlap_matrix[i] = (double *)malloc(pi->nU * sizeof(double));

    /* Inicializar valores en 0 */ 
    for (i = 0; i < pi->nU; i++)
    {
        for (j = 0; j < pi->nU; j++)
        {
            pi->overlap_matrix[i][j] = 0;
        } 
    }
    
    fgets(line, sizeof(line), f);

    while (1){
        fgets(line, sizeof(line), f);
        if (debug) printf("Line: %s\n", line);

        from=strtok(line, " ");
        if (strchr(from, ';') != NULL) break;

        to  = strtok(NULL, " ");
        overlap = strtok(NULL, " ");
        idFrom = atoi(from) - 1;
        idTo = atoi(to) - 1;

        if (debug) printf("from: %s, to:%s , coverage: %s\n", from, to, overlap);
        pi->overlap_matrix[idFrom][idTo] = atof(overlap);
    }
}


int readInputFile(char* filePath, problem_instance *pi) {
    int debug=0;
    FILE* fh=fopen(filePath, "r");

    /*check if file exists*/
    if ( fh==NULL ){
        printf("File does not exists %s", filePath);
        return 0;
    }

    if(debug) printf("Reading: %s \n", filePath);

    findDef(fh, "U:=");
    readUbications(fh, pi);
    if(debug) printf("End readUbications! \n");

    findDef(fh, "S:=");
    readSectors(fh, pi);
    if(debug) printf("End readSectors! \n");

    findDef(fh, "maxU:=");
    readMaxCarts(fh, pi);
    if(debug) printf("End readMaxCarts! \n");

    findDef(fh, "k:=");
    readCapacity(fh, pi);
    if(debug) printf("End readCapacity! \n");

    findDef(fh, "m:=");
    readMedicalCenters(fh, pi);
    if(debug) printf("End readMedicalCenters!\n");

    findDef(fh, "p:=");
    readPopulations(fh, pi);
    if(debug) printf("End readPopulations! \n");

    findDef(fh, "a:=");
    readCoverageMatrix(fh, pi);
    if(debug) printf("End readCoverageMatrix! \n");

    findDef(fh, "o:=");
    readOverlapMatrix(fh, pi);
    if(debug) printf("End readOverlapMatrix! \n");

    fclose(fh);
    if(debug) printf("End Reading! \n");

    /*printProblemInstance(pi);*/
    if(debug) getchar();

    printf("Reader exiting successfully\n");
    return 0;
}


