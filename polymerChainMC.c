#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_BEADS 25
#define BOND_LENGTH 1.0
#define BOND_STIFFNESS 10.0
#define LJ_SIGMA 1.0
#define LJ_EPSILON 0.0
#define MC_STEPS 10000
#define MAX_DISPLACEMENT 0.1
#define WRITE_FREQUENCY 100

typedef struct {
    double x;
    double y;
    double z;
} Vector3D;

typedef struct {
    Vector3D position;
    Vector3D oldPosition;
} Bead;

Bead polymerChain[NUM_BEADS];

double calculateEnergyOld() {
    double energy = 0.0;

    // Calculate bond energy
    for (int i = 0; i < NUM_BEADS - 1; i++) {
        double bondLength = sqrt(pow(polymerChain[i].oldPosition.x - polymerChain[i+1].oldPosition.x, 2) +
                                 pow(polymerChain[i].oldPosition.y - polymerChain[i+1].oldPosition.y, 2) +
                                 pow(polymerChain[i].oldPosition.z - polymerChain[i+1].oldPosition.z, 2));
        double bondEnergy = 0.5 * BOND_STIFFNESS * pow(bondLength - BOND_LENGTH,2);
        energy += bondEnergy;
    }

    // Calculate repulsion energy
    for (int i = 0; i < NUM_BEADS; i++) {
        for (int j = i + 2; j < NUM_BEADS; j++) {
            double distanceSq = pow(polymerChain[i].oldPosition.x - polymerChain[j].oldPosition.x, 2) +
                                pow(polymerChain[i].oldPosition.y - polymerChain[j].oldPosition.y, 2) +
                                pow(polymerChain[i].oldPosition.z - polymerChain[j].oldPosition.z, 2);
            if (distanceSq < 16 * LJ_SIGMA * LJ_SIGMA) {
                double repulsionEnergy = 4.0 * LJ_EPSILON * (pow(LJ_SIGMA,12)/pow(distanceSq,6) - pow(LJ_SIGMA,6)/pow(distanceSq, 3));
                energy += repulsionEnergy;
            }
        }
    }

    return energy;
}

double calculateEnergy() {
    double energy = 0.0;

    // Calculate bond energy
    for (int i = 0; i < NUM_BEADS - 1; i++) {
        double bondLength = sqrt(pow(polymerChain[i].position.x - polymerChain[i+1].position.x, 2) +
                                 pow(polymerChain[i].position.y - polymerChain[i+1].position.y, 2) +
                                 pow(polymerChain[i].position.z - polymerChain[i+1].position.z, 2));
        double bondEnergy = 0.5 * BOND_STIFFNESS * pow(bondLength - BOND_LENGTH,2);
        energy += bondEnergy;
    }

    // Calculate repulsion energy
    for (int i = 0; i < NUM_BEADS; i++) {
        for (int j = i + 2; j < NUM_BEADS; j++) {
            double distanceSq = pow(polymerChain[i].position.x - polymerChain[j].position.x, 2) +
                                pow(polymerChain[i].position.y - polymerChain[j].position.y, 2) +
                                pow(polymerChain[i].position.z - polymerChain[j].position.z, 2);
            if (distanceSq < 16 * LJ_SIGMA * LJ_SIGMA) {
                double repulsionEnergy = 4.0 * LJ_EPSILON * (pow(LJ_SIGMA,12)/pow(distanceSq,6) - pow(LJ_SIGMA,6)/pow(distanceSq, 3));
                energy += repulsionEnergy;
            }
        }
    }

    return energy;
}

void monteCarloStep() {
    // Randomly select a bead
    int beadIndex = rand() % NUM_BEADS;

    // Save the old position
    polymerChain[beadIndex].oldPosition.x = polymerChain[beadIndex].position.x;
    polymerChain[beadIndex].oldPosition.y = polymerChain[beadIndex].position.y;
    polymerChain[beadIndex].oldPosition.z = polymerChain[beadIndex].position.z;

    // Generate a random displacement in spherical coordinates
    double r = (rand() / (double)RAND_MAX) * MAX_DISPLACEMENT;
    double theta   = acos(1.0 - 2.0 * (rand() / (double)RAND_MAX));
    double phi = (rand() / (double)RAND_MAX) * 2.0 * M_PI;
    double dx = r * sin(theta) * cos(phi);
    double dy = r * sin(theta) * sin(phi);
    double dz = r * cos(theta);

    // Update the position
    polymerChain[beadIndex].position.x += dx;
    polymerChain[beadIndex].position.y += dy;
    polymerChain[beadIndex].position.z += dz;

    // Calculate the energy difference
    double oldEnergy = calculateEnergyOld();
    double newEnergy = calculateEnergy();
    double deltaEnergy = newEnergy - oldEnergy;

    // Accept or reject the move based on the Metropolis criterion
    if (deltaEnergy > 0 && exp(-deltaEnergy) < (rand() / (double)RAND_MAX)) {
        // Reject the move
        polymerChain[beadIndex].position.x = polymerChain[beadIndex].oldPosition.x;
        polymerChain[beadIndex].position.y = polymerChain[beadIndex].oldPosition.y;
        polymerChain[beadIndex].position.z = polymerChain[beadIndex].oldPosition.z;
    }
}

int main() {
    // Initialize the polymer chain
    for (int i = 0; i < NUM_BEADS; i++) {
        polymerChain[i].position.x = 0.0;
        polymerChain[i].position.y = 0.0;
        polymerChain[i].position.z = i * BOND_LENGTH;
    }

    // Perform Monte Carlo steps
    for (int step = 0; step < MC_STEPS * NUM_BEADS; step++) {
        monteCarloStep();
        if(step % WRITE_FREQUENCY == 0) {
            printf("%d\nstep = %d\n", NUM_BEADS, step);
            for (int i = 0; i < NUM_BEADS; i++) {
                printf("X  %5.3f %5.3f %5.3f\n", polymerChain[i].position.x, polymerChain[i].position.y, polymerChain[i].position.z);
            }
        }
    }

    return 0;
}