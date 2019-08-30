/*
* Tarea programada 1
* Paola Ortega - 
* Valeria Zamora - B37743
*/


#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


int greatest_common_factor(int a)
{
    bool found = false;
    int ans = a-1;
    
    while (!found && a > 1) {
        // Busca el mayor numero entero que divide el numero a y el resto es cero
        if(a % ans == 0) {
            found = true;
        } else {
            ans--;
        }
    }
    if (ans <= 1) {
        ans = a; 
    }
    return ans;
}

// Fill the forest
void generate_forest(int* mat, float f0, float f1, float f2, int size) {
    srand((unsigned) time(NULL));
    float random_number = 0.0f;
    for(int i = 0; i < size; ++i) {
        random_number = (float)rand() / RAND_MAX;
        printf("%f\n",random_number);
        if(random_number < f0) {
            mat[i] = 0;
        } else {
            if (random_number < f1) {
                mat[i] = 1;
            } else {
                mat[i] = 2;
            }
        }
    } 
}

// Funcion para contar la cantidad de 0, 1 y 2 un cuadrante especifico.

int main(int argc,char **argv){
    // Declare variables
	int x = 0, my_id = 0, p = 0, size = 0;
    int process_size = 0, initial_position = 0, initial_row = 0, initial_column = 0, row_size = 0, number_of_quadrants= 0;
    int quadrant_area = 0;
    int quadrants_in_a_process = 0;
    int * mat = 0;
    int * process_part = 0;
    float f0, f1, f2;

    MPI_Init(&argc,&argv);

    // Get p           
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    // Get x 
    x = greatest_common_factor(p);
    quadrant_area = p*p;
    row_size = x*p;
    size = row_size * row_size;
    process_size = size/p;
    
    
 
    // Get my id
    MPI_Comm_rank(MPI_COMM_WORLD,&my_id);

    // Get initial position of the forest
    initial_position = my_id*process_size;
    initial_row = (int)(initial_position / row_size;
    initial_column = initial_position % row_size;


    // Initialize 
    if (myid == 0){
       // Create forest
       mat = (int *)malloc((size)*(sizeof(int)));

       // Get distribution
       bool valid_distribution = false;
       while(!valid_distribution) {
        printf("Enter distribution you want to use to generate the forest. Values must be from 0.0 to 1.0 and must add up 1.0\n");

        printf("Enter the percentage for value 0:");
        scanf("%f", &f0);

        printf("Enter the percentage for value 1:)");
        scanf("%f", &f1);

        printf("Enter the percentage for value 2:");
        scanf("%f", &f2);

        float total = f0+f1+f2;
        if(total == 1) {
            valid_distribution = true;
        }
       }

       // Fill the forest
        generate_forest(mat, f0, f1, f2, size);
    }


    // Send parts to each process
    MPI_SCATTER(mat, process_size, MPI_INT, process_part, process_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Recorrer todo el process_part

    // [#cuadrante, #0, #1, #2, siguiente] 

    // en cada index calcula en cual cuadrante esta y suma a la estructura del cuadrante correspodiente

    // loop que devuelva los valores de cada uno de los cuadrantes


    // Wait
	MPI_Barrier(MPI_COMM_WORLD);

           
    MPI_Finalize();
    return 0;
}
 