#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ATOMS 75
#define MAX_LINE_LENGTH 77

typedef struct {
    char symbol[3];
    double x, y, z;
} Atom;

int main() {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    Atom atoms[MAX_ATOMS];
    int num_atoms = 0;

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
            if (num_atoms >= MAX_ATOMS) {
                printf("Maximum number of atoms exceeded\n");
                break;
            }
        }
    }

    // Close the file
    fclose(file);

    // Print the atoms
    printf("Atomic coordinates:\n");
    for (int i = 0; i < num_atoms; i++) {
        printf("%s %.6lf %.6lf %.6lf\n", atoms[i].symbol, atoms[i].x, atoms[i].y, atoms[i].z);
    }
    
    return 0;
}


