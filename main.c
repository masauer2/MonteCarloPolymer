#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TWO_TO_THE_POWER_OF_ONE_SIXTH 1.12246204831
#define BOND_LENGTH 1.0
#define BOND_STIFFNESS 10.0
#define LJ_SIGMA 1.0
#define LJ_EPSILON 20
#define MC_STEPS 10000
#define MAX_DISPLACEMENT 0.1
#define WRITE_FREQUENCY 100
#define MC_STEP_SIZE 1

typedef struct {
    char atom_type;
    double x, y, z;
} Atom;

typedef struct {
    int length;
    Atom *atoms;
} Chain;

typedef struct {
    Chain *chains;
    double temp;
    double **eMat;
    double *eVec;
} System;

int get_chain_num(int index, int chainLength){
    return floor(index/chainLength);
}

void bonded_interaction(System *s, int i,  int j, int chainLength){
    int chainNum1 = get_chain_num(i, chainLength);
    int atomIndex1 = i % chainLength;

    int chainNum2 = get_chain_num(j, chainLength);
    int atomIndex2 = j % chainLength;

    double bondLength = sqrt(pow(s->chains[chainNum1].atoms[atomIndex1].x - s->chains[chainNum2].atoms[atomIndex2].x, 2) +
                                 pow(s->chains[chainNum1].atoms[atomIndex1].y - s->chains[chainNum1].atoms[atomIndex2].y, 2) +
                                 pow(s->chains[chainNum1].atoms[atomIndex1].z - s->chains[chainNum1].atoms[atomIndex2].z, 2));
    double bondEnergy = 0.5 * BOND_STIFFNESS * pow(bondLength - BOND_LENGTH,2);
    s->eMat[i][j] += bondEnergy;
    
}

void angle_interaction(System *s,  int i,  int j, int chainLength){
    int chainNum1 = get_chain_num(i, chainLength);
    int atomIndex1 = i % chainLength;

    int chainNum2 = get_chain_num(j, chainLength);
    int atomIndex2 = j % chainLength;

    double repulsionEnergy;
    double bondLengthSq = pow(s->chains[chainNum1].atoms[atomIndex1].x - s->chains[chainNum2].atoms[atomIndex2].x, 2) +
                                 pow(s->chains[chainNum1].atoms[atomIndex1].y - s->chains[chainNum1].atoms[atomIndex2].y, 2) +
                                 pow(s->chains[chainNum1].atoms[atomIndex1].z - s->chains[chainNum1].atoms[atomIndex2].z, 2);
    if (bondLengthSq < TWO_TO_THE_POWER_OF_ONE_SIXTH * LJ_EPSILON) {
        repulsionEnergy = 4.0 * LJ_EPSILON * (pow(LJ_SIGMA,12)/pow(bondLengthSq,6) - pow(LJ_SIGMA,6)/pow(bondLengthSq, 3)) + LJ_EPSILON;
    }
    else{
        repulsionEnergy = 0.0;
    }
    s->eMat[i][j] += repulsionEnergy;
    
    
}

void nonbonded_interaction(System *s, int i,  int j, int chainLength){

    int chainNum1 = get_chain_num(i, chainLength);
    int atomIndex1 = i % chainLength;

    int chainNum2 = get_chain_num(j, chainLength);
    int atomIndex2 = j % chainLength;
    
    double nonBondedEnergy;
    double bondLengthSq = pow(s->chains[chainNum1].atoms[atomIndex1].x - s->chains[chainNum2].atoms[atomIndex2].x, 2) +
                                 pow(s->chains[chainNum1].atoms[atomIndex1].y - s->chains[chainNum1].atoms[atomIndex2].y, 2) +
                                 pow(s->chains[chainNum1].atoms[atomIndex1].z - s->chains[chainNum1].atoms[atomIndex2].z, 2);

    if (bondLengthSq < 16 * LJ_SIGMA * LJ_SIGMA) {
        nonBondedEnergy = 4.0 * LJ_EPSILON * (pow(LJ_SIGMA,12)/pow(bondLengthSq,6) - pow(LJ_SIGMA,6)/pow(bondLengthSq, 3));
    }
    else{
        nonBondedEnergy = 0.0;
    }
    s->eMat[i][j] += nonBondedEnergy;
   
   

}

void initialize_energy_matrix(System *s, int nChains, int chainLength){
    for (int i = 0; i < (nChains * chainLength) - 1 ; i++) {
        for (int j = i+1; j < (nChains * chainLength); j++){
            s->eMat[i][j] = 0;
            if((j - i == 1) && (get_chain_num(i, chainLength) == get_chain_num(j, chainLength))){bonded_interaction(s, i,  j, chainLength);}
            if((j - i == 2) && (get_chain_num(i, chainLength) == get_chain_num(j, chainLength))){angle_interaction(s, i,  j, chainLength);}
            if((j - i > 2)){nonbonded_interaction(s, i,  j, chainLength);}
            s->eMat[j][i] = s->eMat[i][j]; 
            }
        }

}

void print_energy_matrix(System *s){
    for (int i = 0; i < 30 ; i++) {
        for (int j = 0; j < 30 ; j++) {
            printf("%f ", s->eMat[i][j]);
        }
        printf("\n");
    }
}

int main() {
    FILE *file = fopen("example.xyz", "r");

    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    int num_atoms;
    int num_chains;
    fscanf(file, "%d\n", &num_atoms); // Read the number of atoms
    fscanf(file, "POLYMER WITH CHAINS %d\n", &num_chains); // Read the number of chains
    int chain_length = num_atoms / num_chains;


    // Create an array of chains
    System sys;
    sys.chains = (Chain*) malloc(num_chains * sizeof(Chain));
    
    for (int i = 0; i < num_chains; i++) {
        sys.chains[i].length = chain_length;
        sys.chains[i].atoms = (Atom*) malloc(chain_length * sizeof(Atom));
    }

    sys.eMat = (double**) malloc(num_chains * chain_length*sizeof(double*));
    for(int i =0; i < num_chains*chain_length;i++){
        sys.eMat[i] = (double*) malloc(num_chains*chain_length*sizeof(double));
    }

    char atom_type;
    double x, y, z;
    int atom_count = 0;
    while (fscanf(file, "%c %lf %lf %lf\n", &atom_type, &x, &y, &z) == 4) {
        int chain_index = atom_count / chain_length;
        int atom_index = atom_count % chain_length;
        sys.chains[chain_index].atoms[atom_index].atom_type = atom_type;
        sys.chains[chain_index].atoms[atom_index].x = x;
        sys.chains[chain_index].atoms[atom_index].y = y;
        sys.chains[chain_index].atoms[atom_index].z = z;
        atom_count++;
    }

    fclose(file);
    
    initialize_energy_matrix(&sys, num_chains, chain_length);
    print_energy_matrix(&sys);  

    // Free memory
    for (int i = 0; i < num_chains; i++) {
        free(sys.chains[i].atoms);
    }
    free(sys.chains);
    free(sys.eMat);

    return 0;
}









