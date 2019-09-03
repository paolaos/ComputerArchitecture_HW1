/*
* Arquitectura de Computadores
* Tarea programada 1
* Paola Ortega - B55204
* Valeria Zamora - B37743
*/

#include <mpi.h>
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
    int row_counter = 0;
    int column_counter = 0;
    int counter = 0;
    int rows = 0, i = 0, j = 0;
    for (rows = 0; rows < rows_amount; rows++){
        // cantidad de filas
        for (i = 0; i < quadrants_amount; i++) {
            // cantidad de cuadrantes
            for (j = 0; j < quadrant_size; j++){
                // cantidad de hectareas por cuadrante
                matrix[rows*rows_amount+(i*quadrant_size+j)] = row_counter + column_counter;
            }
            column_counter++;
        }
        column_counter = 0;
        counter++;
        if (rows > 0 && counter == quadrants_amount){
            row_counter += quadrants_amount;
            counter = 0;
        }
        
    }
}

// Fill the forest
void generate_forest(int* mat, float f0, float f1, float f2, int size) {
    srand((unsigned) time(NULL));
    float random_number = 0.0f;
    int i = 0;
    for(i = 0; i < size; ++i) {
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
    int i = 0;
    int j = 0;
    for(i = 0; i < column_size; ++i) {
        for(j = 0; j < column_size; ++j) {
            printf("%d ", mat[i*column_size+j]);
        }
        printf("\n");
    }
}

// Get quadrant number of a given position (test if this is right)
int getQuadrant(int position, int* quadrants_matrix) {
    return quadrants_matrix[position];
}

void print_results(int* results, int size) {
    int i = 0;
    for (i = 0; i < size; i++) {
        printf("%d ", results[i]);
    }
    printf("\n");
}

int main(int argc,char **argv){
    // Declare variables
	int x = 0, my_id = 0, p = 0, size = 0, total_quadrants = 0;
    int process_size = 0, initial_position = 0, row_size = 0, number_of_quadrants= 0;
    int quadrant_area = 0;
    int * mat = 0;  // forest
    int * process_part = 0;     // part of the forest
    int * process_results = 0;  // array of results of each process
    int * results = 0;           // final results
    int * quadrant_matrix = 0;
    float f0 = 0.0f, f1 = 0.0f, f2 = 0.0f;
    int i = 0;
    int total_results_size = 0;

    MPI_Init(&argc,&argv);

    // Get p           
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    // Get x 
    // p = 4;
    x = greatest_common_factor(p);
    quadrant_area = p*p;
    total_quadrants = x*x;
    row_size = x*p;
    size = row_size * row_size;
    process_size = size / p;
    total_results_size = total_quadrants*3;

    // Get my id
    MPI_Comm_rank(MPI_COMM_WORLD,&my_id);

    // Get initial position of the forest
    initial_position = my_id*process_size;

    // Refence matrix to identify quadrants
    quadrant_matrix = (int *)malloc((size)*(sizeof(int)));
    generate_quadrant_matrix(quadrant_matrix, row_size, x, p);


    // where the process part of the forest will be stored
    process_part = (int *)malloc((process_size)*(sizeof(int)));


    // where the quadrants count of 0's, 1's and 2's is stored
    process_results = (int *)malloc((total_results_size)*(sizeof(int)));
    for(i  = 0; i < total_results_size; i++) {
        process_results[i] = 0;
    }

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

        results = (int *)malloc(total_results_size*(sizeof(int)));
        for(i  = 0; i < total_results_size; i++) {
            results[i] = 0;
        }
    }

    // Send parts to each process
    MPI_Scatter(mat, process_size, MPI_INT, process_part, process_size, MPI_INT, 0, MPI_COMM_WORLD);


    // Add the process part quadrants values
    int actual_quadrant = 0;
    int actual_value = 0;

    for(i = 0; i < process_size; i++) {
        actual_quadrant = getQuadrant(initial_position+i, quadrant_matrix);
        actual_value = process_part[i];
        process_results[actual_quadrant*3+actual_value]++;
    }
    
    // Return the values to process 0    
    MPI_Reduce(process_results, results, total_results_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Get the percentages of each quadrant
    float reforestable_quadrants = 0.0f;
    float total_0 = 0.0f, total_1 = 0.0f, total_2 = 0.0f; 
    if (my_id == 0) {
        print_results(results, total_results_size);
        for (i = 0; i < total_results_size; i+=3) {
            total_0 = (float)results[i]/(float)quadrant_area;
            total_1 = (float)results[i+1]/(float)quadrant_area;
            total_2 = (float)results[i+2]/(float)quadrant_area;
            if (total_0 <= 0.2 && total_2 >= 0.5) {
                reforestable_quadrants += 1.0f;
            }
            printf("Numero de cuadrante: %d\n", i/3);
            printf("\tGran inversion: %f \n", total_0);
            printf("\tAlto costo: %f \n", total_1);
            printf("\tRazonable: %f \n", total_2);

        }
        
        printf("La cantidad de cuadrantes reforestables es: %f\n", reforestable_quadrants);
        printf("El porcentaje del bosque a reforestar es: %f\n", reforestable_quadrants/(float)total_quadrants);
        if (reforestable_quadrants == total_quadrants) {
            printf("Se reforestara todo el bosque, el costo es de 0.\n");
        } else {
            if (reforestable_quadrants/(float)total_quadrants >= 0.5) {
                printf("La cantidad de hectareas reforestadas es mayor al 0.5 del bosque.\n");
            } else {
                printf("No se obtuvo beneficio en el costo de reforestar el bosque.\n");
            }
        }
    }
           
    MPI_Finalize();
    if (my_id == 0) {
        free(mat);
        free(results);
    }
    free(process_part);
    free(process_results);
    return 0;
}
 