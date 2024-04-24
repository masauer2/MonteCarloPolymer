#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define NUM_BEADS 125
#define NUM_CHAINS 10
#define BOND_LENGTH 1.0
#define BOND_STIFFNESS 10.0
#define LJ_SIGMA 1.0
#define LJ_EPSILON 0.0
#define MC_STEPS 10000
#define MAX_DISPLACEMENT 0.1
#define WRITE_FREQUENCY 100

typedef struct {
    double crd[3];
    int type;
} Bead;

typedef struct {
    Bead b[NUM_BEADS];
} Chain;

typedef struct {
    Chain c[NUM_CHAINS];
    double temp;
    double eMat[NUM_BEADS*NUM_CHAINS][NUM_BEADS*NUM_CHAINS];
    double eVec[NUM_BEADS*NUM_CHAINS];
} System;


typedef struct {
    char symbol[3];
    double x, y, z;
} Atom;

int main() {
    FILE *file;
    //char line[NUM_BEADS*NUM_CHAINS+2];
    char line[100];
    Atom atoms[NUM_BEADS*NUM_CHAINS];
    int num_atoms = 0;
    int max_atoms = NUM_BEADS*NUM_CHAINS;
    int tmp;

    // Open the .xyz file for reading
    file = fopen("example.xyz", "r");
    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }
 
    // Read atoms from the file
    //sizeof(line)
    // != NULL
    while (fgets(line, sizeof(line), file)) {
        // Parse line into atom symbol and coordinates
        if (sscanf(line, "%2s %lf %lf %lf", &atoms[num_atoms].symbol, &atoms[num_atoms].x, &atoms[num_atoms].y, &atoms[num_atoms].z) == 4) {
            num_atoms++;
        }
    }

    // Close the file
    fclose(file);

    // Print the atoms

    System s;
    printf("Atomic coordinates:\n");
    for (int i = 0; i < num_atoms; i++) {
        int curr_chain = floor(i/NUM_BEADS);
        s.c[curr_chain].b[i-NUM_BEADS*curr_chain].crd[0] = atoms[i].x;
        s.c[curr_chain].b[i-NUM_BEADS*curr_chain].crd[1] = atoms[i].y;
        s.c[curr_chain].b[i-NUM_BEADS*curr_chain].crd[2] = atoms[i].z;
        if(strcmp(atoms[i].symbol,"A") == 0){
            s.c[curr_chain].b[i-NUM_BEADS*curr_chain].type = 0;
        }
        else if(strcmp(atoms[i].symbol,"B") == 0){
            s.c[curr_chain].b[i-NUM_BEADS*curr_chain].type = 1;
        }
        else{
            s.c[curr_chain].b[i-NUM_BEADS*curr_chain].type = -1;
            printf("Error reading atom type. Should be A or B.");
        }
       
        printf("%d %.6lf %.6lf %.6lf\n", s.c[curr_chain].b[i-NUM_BEADS*curr_chain].type, s.c[curr_chain].b[i-NUM_BEADS*curr_chain].crd[0], s.c[curr_chain].b[i-NUM_BEADS*curr_chain].crd[1], s.c[curr_chain].b[i-NUM_BEADS*curr_chain].crd[2]);
        //printf("%s\n", s.c[curr_chain].b[i-NUM_BEADS*curr_chain].type);
    }
    
    return 0;
}


