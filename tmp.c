#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_BEADS 25
#define NUM_CHAINS 1
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
    Bead b[NUM_BEADS*NUM_CHAINS];
} Chain;

typedef struct {
    Chain c[NUM_CHAINS];
    double temp;
    double eMat[NUM_BEADS*NUM_CHAINS][NUM_BEADS*NUM_CHAINS];
    double eVec[NUM_BEADS*NUM_CHAINS];
} System;






int main(){
printf("Souvik is the GOAT");

}
