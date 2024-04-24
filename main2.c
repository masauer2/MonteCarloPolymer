#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char atom_type;
    double x, y, z;
} Atom;


typedef struct {
    int length;
    Atom *atoms;
} Chain;


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
    Chain *chains = malloc(num_chains * sizeof(Chain));
    for (int i = 0; i < num_chains; i++) {
        chains[i].length = chain_length;
        chains[i].atoms = malloc(chain_length * sizeof(Atom));
    }

    char atom_type;
    double x, y, z;
    int atom_count = 0;
    while (fscanf(file, "%c %lf %lf %lf\n", &atom_type, &x, &y, &z) == 4) {
        int chain_index = atom_count / chain_length;
        int atom_index = atom_count % chain_length;
        chains[chain_index].atoms[atom_index].atom_type = atom_type;
        chains[chain_index].atoms[atom_index].x = x;
        chains[chain_index].atoms[atom_index].y = y;
        chains[chain_index].atoms[atom_index].z = z;
        atom_count++;
    }

    fclose(file);
    
    // Output the data to check
    for (int i = 0; i < num_chains; i++) {
        printf("Chain %d:\n", i + 1);
        for (int j = 0; j < chain_length; j++) {
            printf("%c (%.5f, %.5f, %.5f)\n",
                   chains[i].atoms[j].atom_type,
                   chains[i].atoms[j].x,
                   chains[i].atoms[j].y,
                   chains[i].atoms[j].z);
        }
    }

    // Free memory
    for (int i = 0; i < num_chains; i++) {
        free(chains[i].atoms);
    }

    free(chains);
    return 0;
}









