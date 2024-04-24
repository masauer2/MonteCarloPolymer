#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_BEADS 25
#define NUM_CHAINS 3
#define BOND_LENGTH 1.0


typedef struct {
    double crd[3];
    int type;
} Bead;

typedef struct {
    Bead b[NUM_BEADS*NUM_CHAINS];
} Chain;

void gen_chain(double* init, FILE *file){
    for(int i=0; i<NUM_BEADS; i++){
        double offset = i*BOND_LENGTH;
		fprintf(file, "B %.5f %.5f %.5f\n", init[0], init[1], init[2]+offset);
	}
}

int main(){
    double init_crd[] = {0.0, 0.0, 0.0};
    FILE *file;
    char *filename = "example.xyz";

    file = fopen(filename, "w");
    fprintf(file, "%d\n", NUM_BEADS*NUM_CHAINS);
    fprintf(file, "%s %d\n", "POLYMER WITH CHAINS", NUM_CHAINS);
	for(int i=0; i<=NUM_CHAINS; i++){    
		gen_chain(init_crd, file);
	}
    fclose(file);
    return 0;
}
