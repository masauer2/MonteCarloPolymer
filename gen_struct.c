#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_BEADS 10      // Total number of beads per chain
#define NUM_CHAINS 3      // Number of chains
#define BOND_LENGTH 1    // Length of each bond
#define SUBCHAIN_LENGTH 5 // Length of each subchain of type 'b'

// Simulation box dimensions
#define BOX_X 100.0
#define BOX_Y 100.0
#define BOX_Z 100.0

typedef struct {
    double crd[3];
    int type;
} Bead;

typedef struct {
    Bead b[NUM_BEADS*NUM_CHAINS];
} Chain;

// Function to generate a random double between min and max
double random_double(double min, double max) {
    return min + (rand() / (RAND_MAX / (max - min)));
}

// Generate the chain with random initial point within the box
void gen_chain(FILE *file) {
    double init[3] = {random_double(0, BOX_X), random_double(0, BOX_Y), random_double(0, BOX_Z)};
    int subchain_start1 = 20;  // Start of first subchain of type 'b'
    int subchain_start2 = 60;  // Start of second subchain of type 'b'
    int subchain_start3 = 100; // Start of third subchain of type 'b'
    
    for (int i = 0; i < NUM_BEADS; i++) {
        double z_offset = i * BOND_LENGTH;
        if ((i >= subchain_start1 && i < subchain_start1 + SUBCHAIN_LENGTH) || 
            (i >= subchain_start2 && i < subchain_start2 + SUBCHAIN_LENGTH) ||
            (i >= subchain_start2 && i < subchain_start3 + SUBCHAIN_LENGTH) ) {
            fprintf(file, "B %.5f %.5f %.5f\n", init[0], init[1], init[2] + z_offset);  // Type 'b' beads
        } else {
            fprintf(file, "A %.5f %.5f %.5f\n", init[0], init[1], init[2] + z_offset);  // Type 'a' beads
        }
    }
}

int main() {
    srand(time(NULL)); // Initialize random seed
    FILE *file;
    char *filename = "example.xyz";

    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }
    
    fprintf(file, "%d\n", NUM_BEADS * NUM_CHAINS);
    fprintf(file, "POLYMER WITH CHAINS %d\n", NUM_CHAINS);
    for (int i = 0; i < NUM_CHAINS; i++) {
        gen_chain(file);
    }
    fclose(file);
    return 0;
}
