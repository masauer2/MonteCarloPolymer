#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_BEADS 25
#define NUM_CHAINS 3
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
    Chain c[NUM_BEADS];
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
    char line[NUM_BEADS*NUM_CHAINS+2];
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
    while (fgets(line, sizeof(line), file) != NULL) {
        // Parse line into atom symbol and coordinates
        if (sscanf(line, "%2s %lf %lf %lf", atoms[num_atoms].symbol, 
                   &atoms[num_atoms].x, &atoms[num_atoms].y, &atoms[num_atoms].z) == 4) {
            num_atoms++;
            if (num_atoms >= max_atoms) {
                printf("Maximum number of atoms exceeded\n");
                break;
            }
        }
    }

    // Close the file
    fclose(file);

    // Print the atoms

    System s;
    printf("Atomic coordinates:\n");
    for (int i = 0; i < num_atoms; i++) {
        int curr_chain = floor(i/NUM_BEADS)
        s.c[curr_chain].b[i].crd[0] = atoms[i].x;
        s.c[curr_chain].b[i].crd[1] = atoms[i].y;
        s.c[curr_chain].b[i].crd[2] = atoms[i].z;
        s.c[curr_chain].b[i].type = atoms[i].symbol;
        //printf("%s %.6lf %.6lf %.6lf\n", atoms[i].symbol, atoms[i].x, atoms[i].y, atoms[i].z);
    }
    
    return 0;
}


