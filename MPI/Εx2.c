#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <math.h>

struct MinData {
    int min;
    int index;
};

int main(int argc, char **argv) 
{
    int rank, p, n;
    int i, j;
    int root = 0;
    int *A, *A_loc;
    int *B, *B_loc;

    MPI_Init(&argc, &argv);                // Αρχικοποιήση MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Τρέχουσα process id
    MPI_Comm_size(MPI_COMM_WORLD, &p);   // Πλήθος διεργασιων

    // Διάβασμα n για μέγεθος πίνακα
    if (rank == root) 
    {
        do
        {
            printf("Give n for size nxn of matrix A: ");
            scanf("%d", &n);
        }
        while(p != n); // ελεγχις για επεξεργαστές ισούνται με n
    }

    // Η διεργασία 0 στέλνει σε όλες τις άλλες τον αριθμό n
    MPI_Bcast(&n, 1, MPI_INT, root, MPI_COMM_WORLD); 
    // Δέσμευση πίνακων
    A = (int*) malloc (n*n*sizeof(int)); assert(A != NULL);
    A_loc = (int *) malloc (n*sizeof(int)); assert(A_loc != NULL); 

    if (rank == root) 
    {
        // Αν ο χρήστης έδωσε παράμετρο
        if (argc != 2) 
        {
            printf("Usage: %s <input.txt>\n", argv[0]);
            return 1;
        }
        
        // Άνοιγμα αρχείου 
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) 
        {
            printf("Error opening file.\n");
            return 1;
        }

        // Διάβασμα απο αρχείο σε Α πίνακα
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) 
                fscanf(file, "%d", &A[i*n + j]);

        printf("Matrix A:\n");
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                printf("%d ",A[i*n + j]);
            }
            printf("\n");
        }

        fclose(file);
    }
         
    MPI_Scatter(A, n, MPI_INT, A_loc, n, MPI_INT, root, MPI_COMM_WORLD);

    int flag = 1;
    double sum = 0;
    // Αθρόιζω τις τιμές εκτός διαγωνίου τοπικά
    for(i=0; i<n; i++)
        if(i != rank)
            sum += fabs(A_loc[i]);
    // printf("Process %d %d %.2f\n", rank, A_loc[rank], sum);

    // Κάθε διεργασία καθορίζει αν ο πίνακας είναι δεσπόζων
    if(A_loc[rank] < sum)
    {
        printf("Process %d says is not dominant\n", rank);
        flag = 0;
    }

    // Η root μαζέυει όλες τις flag και κάνει AND μεταξύ τους
    int global_flag;
    MPI_Reduce(&flag, &global_flag, 1, MPI_INT, MPI_LAND, root, MPI_COMM_WORLD);

    if(rank == root)
    {
        if (global_flag == 1)
            printf("yes\n");
        else
        {
            printf("no\n");
            MPI_Finalize(); // Τέλος MPI
            return 0;     //Τερματισμός προγράμματος
        }
    }

    // Β Ερώτημα
    float max = fabs(A_loc[rank]);
    float global_max;
    MPI_Reduce(&max,&global_max,1,MPI_FLOAT,MPI_MAX,root,MPI_COMM_WORLD);
    if(rank == 0)
    {
        printf("Max of diagonal: %.2f\n",global_max);
    }

    // Γ Ερώτημα
    MPI_Bcast(&global_max, 1, MPI_INT, root, MPI_COMM_WORLD); 

    B = (int*) malloc (n*n*sizeof(int)); assert(B != NULL);
    B_loc = (int *) malloc (n*sizeof(int)); assert(B_loc != NULL); 

    for(i=0;i<n;i++)
    {
        if(i == rank)
            B_loc[i] = global_max;
        else
        {
            B_loc[i] = global_max - fabs(A_loc[i]);
        }
    }
    MPI_Gather(B_loc, n, MPI_INT, B, n, MPI_INT, root, MPI_COMM_WORLD);

    if(rank == root)
    {
        printf("Matrix B:\n");
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                printf("%d ",B[i*n + j]);
            }
            printf("\n");
        }
    }

    // Δ Ερώτημα
    int min = B_loc[0];
    int index = 0;
    for (int i = 1; i < n; i++) {
        if (B_loc[i] < min) {
            min = B_loc[i];
            index = i;
        }
    }
    
    struct MinData in,out;
    in.min = min;
    in.index = rank*n + index;

    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);

    if (rank == root) {
        int min_i = out.index / n;
        int min_j = out.index % n;
        printf("Minimum value in B: %d\n", out.min);
        printf("Index of minimum in B: (%d,%d)\n",min_i,min_j);
    }

    free(A);
    free(A_loc);
    free(B);
    free(B_loc);
    MPI_Finalize();
    return 0;
}