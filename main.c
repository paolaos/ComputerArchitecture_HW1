/*
* Arquitectura de Computadores
* Tarea programada 1
* Paola Ortega - B55204
* Valeria Zamora - B37743
*/


// #include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>


int greatest_common_factor(int a)
{
    bool found = false;
    int ans = a-1;
    
    while (!found && a > 1) {
        if(a % ans == 0) {
            found = true;
        } else {
            ans--;
        }
    }
    // Its a prime number
    if (ans <= 1) {
        ans = a; 
    }
    return ans;
}

void generate_quadrant_matrix(int* matrix, int rows_amount, int quadrants_amount, int quadrant_size) {
    /*
    for (int rows = 0; rows < rows_amount; rows++){
        // cantidad de filas
        for (int i = 0; i < quadrants_amount; i++) {
            // cantidad de cuadrantes
            for (int j = 0; j < quadrant_size; j++){
                // cantidad de hectareas por cuadrante
                matrix[rows*rows_amount+(i*rows_amount+j)] = i;
            }
        }
    }
    */
    
    
    
}

// Fill the forest
void generate_forest(int* mat, float f0, float f1, float f2, int size) {
    srand((unsigned) time(NULL));
    float random_number = 0.0f;
    for(int i = 0; i < size; ++i) {
        random_number = (float)rand() / RAND_MAX;
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

void print_forest(int* mat, int column_size) {
    printf("El bosque creado es: \n");
    for(int i = 0; i < column_size; ++i) {
        for(int j = 0; j < column_size; ++j) {
            printf("%d ", mat[i*column_size+j]);
        }
        printf("\n");
    }
}

// Funcion para contar la cantidad de 0, 1 y 2 un cuadrante especifico.
void addProcessPart(){

}

// Get quadrant number of a given position (test if this is right)
/*
int getQuadrant(int position, int quadrants_number, int quadrant_side_size, int process_init_row, int process_init_col) {
    int quadrant_row = process_init_row / quadrants_number;
    int quadrant_col = process_init_col % quadrants_number;

    int quadrant_number  = (quadrant_row * quadrants_number + quadrant_col);

    return quadrant_number;
}
*/ 

int main(int argc,char **argv){
    // Declare variables
	int x = 0, my_id = 0, p = 0, size = 0, total_quadrants = 0;
    int process_size = 0, initial_position = 0, initial_row = 0, initial_column = 0, row_size = 0, number_of_quadrants= 0;
    int quadrant_area = 0;
    int * mat = 0;  // forest
    int * process_part = 0;     // part of the forest
    int * process_results = 0;  // array of results of each process
    int * results = 0;           // final results
    int * quadrant_matrix = 0;
    float f0 = 0.0f, f1 = 0.0f, f2 = 0.0f;

    //MPI_Init(&argc,&argv);

    // Get p           
   // MPI_Comm_size(MPI_COMM_WORLD,&p);

    // Get x 
    x = greatest_common_factor(p);
    quadrant_area = p*p;
    total_quadrants = x*x;
    row_size = x*p;
    size = row_size * row_size;
    process_size = size / p;
 
    // Get my id
    //MPI_Comm_rank(MPI_COMM_WORLD,&my_id);

    // Get initial position of the forest
    initial_position = my_id*process_size;

    initial_row = (int)(initial_position / row_size);
    initial_column = initial_position % row_size;

    // Refence matrix to identify quadrants
    quadrant_matrix = (int *)malloc((size)*(sizeof(int)));
    generate_quadrant_matrix(quadrant_matrix, row_size, x, p);
    print_forest(quadrant_matrix, row_size);

    // where the process part of the forest will be stored
    // process_part = (int *)malloc((process_size)*(sizeof(int)));

    // where the quadrants count of 0's, 1's and 2's is stored
    // process_results = (int *)malloc((x*3)*(sizeof(int)));

    // Initialize forest
    if (my_id == 0){
        // Create forest
        mat = (int *)malloc((size)*(sizeof(int)));

        // Get distribution
        bool valid_distribution = false;
        while(!valid_distribution) {
        printf("Ingrese la distribución que desea utilizar para generar el bosque. Los valores deben estar entre 0.0 y 1.0, los tres deben sumar 1.0\n");

        printf("Ingrese la proporcion deseada para la categoria 0: ");
        scanf("%f", &f0);

        printf("Ingrese la proporcion deseada para la categoria 1: ");
        scanf("%f", &f1);

        printf("Ingrese la proporcion deseada para la categoria 2: ");
        scanf("%f", &f2);

        float total = f0+f1+f2;
        if(total == 1) {
            valid_distribution = true;
        }
    }

        // print general forest info
        printf("El bosque tiene %d cuadrantes y %d hectareas por cuadrante.\n", total_quadrants, quadrant_area);
        printf("El area total del bosque es de %d hectareas.\n", size);

       // Fill the forest
        generate_forest(mat, f0, f1, f2, size);
        print_forest(mat, row_size);

        // results = (int *)malloc((x*3)*(sizeof(int)));
    }


    // Send parts to each process
    //MPI_SCATTER(mat, process_size, MPI_INT, process_part, process_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Recorrer todo el process_part

    // [#cuadrante, #0, #1, #2, siguiente] 

    // en cada index calcula en cual cuadrante esta y suma a la estructura del cuadrante correspodiente

    // devuelve los valores de cada uno de los cuadrantes
    /*
    for (int i = 0; i < (x*3); ++x) {
        int sum = i*sizeof(int);
        // MPI_Reduce(process_results + sum, (results + sum), 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    */

    // Get the percentages of each quadrant
    /*
    int reforestable_quadrants = 0;
    float total_0 = 0f, total_1 = 0f, total_2 = 0f; 
    if (my_id == 0) {
        for (int i = 0; i < (x*3); i+=3) {
            total_0 = results[i]/p;
            total_1 = results[i+1]/p;
            total_2 = results[i+2]/p;
            if (total_0 <= 0.2 && total_2 >= 0.5) {
                reforestable_quadrants += 1;
            }
        }
        printf("La cantidad de cuadrantes reforestables es: %d\n", reforestable_quadrants);
        if (reforestable_quadrants == x) {
            printf("Se reforestara todo el bosque.\n");
        } else {
            if (reforestable_quadrants/x >= 0.5) {
                printf("La cantidad de hectareas reforestadas es mayor al 0.5 del bosque.\n");
            }
        }
    }
    */

    // Wait
	// MPI_Barrier(MPI_COMM_WORLD);

           
    // MPI_Finalize();
    if (my_id == 0) {
        free(mat);
        free(results);
    }
    free(process_part);
    free(process_results);
    return 0;
}
 