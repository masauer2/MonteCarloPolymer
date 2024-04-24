#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ATOMS 25

typedef struct {
    char symbol[3];
    double x, y, z;
} Atom;

int main() {
    FILE *file;
    int array[MAX_SIZE];
    int num_elements = 0;
    int i;


    // Open the file for reading
    file = fopen("initial.xyz", "r");
    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }


    // Read x y z from the file into the array
    while (fscanf(file, "%d", &array[NUM_BEADS]) == 1) {
        num_elements++;
        if (NUM_BEADS >= MAX_SIZE) {
            printf("Array size exceeded maximum\n");
            break;
        }
    }


    // Close the file
    fclose(file);

    // Print the elements of the array
    printf("Array elements:\n");
    for (i = 0; i < num_elements; i++) {
        printf("%d\n", array[i]);
    }

    return 0;
}
