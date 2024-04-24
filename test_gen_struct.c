#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_BEADS 25
#define NUM_CHAINS 3
#define BOND_LENGTH 1.0
#define BOX_X 10.0
#define BOX_Y 10.0
#define BOX_Z 10.0
#define MIN_DISTANCE 1.5 // Minimum distance between beads of different chains

typedef struct {
    double crd[3];
} Bead;

typedef struct {
    Bead beads[NUM_BEADS];
} Chain;

double random_double(double min, double max) {
    return min + (rand() / (RAND_MAX / (max - min)));
}

void random_unit_vector(double *vec) {
    double theta = random_double(0, 2 * M_PI);
    double phi = random_double(0, M_PI);
    vec[0] = sin(phi) * cos(theta);
    vec[1] = sin(phi) * sin(theta);
    vec[2] = cos(phi);
}

// Calculate Euclidean distance between two beads
double distance(Bead a, Bead b) {
    return sqrt(pow(a.crd[0] - b.crd[0], 2) + pow(a.crd[1] - b.crd[1], 2) + pow(a.crd[2] - b.crd[2], 2));
}

// Check if a new chain overlaps with existing chains
int check_overlap(Chain *chains, int num_chains, Chain *new_chain) {
    for (int i = 0; i < num_chains; i++) {
        for (int j = 0; j < NUM_BEADS; j++) {
            for (int k = 0; k < NUM_BEADS; k++) {
                if (distance(chains[i].beads[j], new_chain->beads[k]) < MIN_DISTANCE) {
                    return 1; // Overlap detected
                }
            }
        }
    }
    return 0; // No overlap
}

// Generate a chain with random initial point and direction, ensuring no overlap
int generate_chain(Chain *chains, int num_chains, FILE *file) {
    Chain new_chain;
    double direction[3];
    random_unit_vector(direction);

    int attempts = 0;
    int max_attempts = 100; // Limit attempts to prevent infinite loops
    do {
        new_chain.beads[0].crd[0] = random_double(0, BOX_X);
        new_chain.beads[0].crd[1] = random_double(0, BOX_Y);
        new_chain.beads[0].crd[2] = random_double(0, BOX_Z);

        for (int i = 1; i < NUM_BEADS; i++) {
            new_chain.beads[i].crd[0] = new_chain.beads[i-1].crd[0] + BOND_LENGTH * direction[0];
            new_chain.beads[i].crd[1] = new_chain.beads[i-1].crd[1] + BOND_LENGTH * direction[1];
            new_chain.beads[i].crd[2] = new_chain.beads[i-1].crd[2] + BOND_LENGTH * direction[2];
        }

        if (++attempts > max_attempts) return 0; // Failed to place chain without overlap
    } while (check_overlap(chains, num_chains, &new_chain));

    // Copy new chain to global chains array and write to file
    chains[num_chains] = new_chain;
    for (int i = 0; i < NUM_BEADS; i++) {
        fprintf(file, "B %.5f %.5f %.5f\n", new_chain.beads[i].crd[0], new_chain.beads[i].crd[1], new_chain.beads[i].crd[2]);
    }
    return 1;
}

int main() {
    srand(time(NULL)); // Initialize random seed
    FILE *file;
    char *filename = "example.xyz";
    Chain chains[NUM_CHAINS]; // Array to store all chains

    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    fprintf(file, "%d\n", NUM_BEADS * NUM_CHAINS);
    fprintf(file, "POLYMER WITH CHAINS %d\n", NUM_CHAINS);

    for (int i = 0; i < NUM_CHAINS; i++) {
        if (!generate_chain(chains, i, file)) {
            fprintf(stderr, "Failed to place all chains without overlap after multiple attempts.\n");
            fclose(file);
            return EXIT_FAILURE;
        }
    }

    fclose(file);
    return 0;
}
