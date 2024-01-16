#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <math.h>

// Δομή για να βρούμε την θέση του ελάχιστου στοιχείου του Β
struct MinData {
    int min;
    int loc;
};

int main(int argc, char **argv) 
{
    int rank, p, n, num, n_loc;
    int i, j, k;
    int root = 0;
    int *A, *A_loc;
    int *B, *B_loc;

    MPI_Init(&argc, &argv);                // Αρχικοποιήση MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Τρέχουσα process id
    MPI_Comm_size(MPI_COMM_WORLD, &p);   // Πλήθος διεργασιων

    // Διάβασμα n για μέγεθος πίνακα Α απο κεντρικό επεξεργαστή
    if (rank == root) 
    {
        do
        {
            printf("Give n for size nxn of matrix A: ");
            scanf("%d", &n);
        }
        while(n % p !=0); // Το n πρέπει να είναι ακέραιο πολλαπλάσιο του p
    }

    // Η διεργασία 0 στέλνει σε όλες τις άλλες τον αριθμό n του πίνακα
    MPI_Bcast(&n, 1, MPI_INT, root, MPI_COMM_WORLD); 

    num = n/p;  // Οι γραμμές του πίνακα Α που παίρνει κάθε διεργασία
    n_loc = num*n; // Το μέγεθος του τοπικού πίνακα κάθε διεργασίας 

    // Δέσμευση πίνακων Α και τοπικού Α_loc
    A = (int*) malloc (n*n*sizeof(int)); assert(A != NULL);
    A_loc = (int *) malloc (n_loc*sizeof(int)); assert(A_loc != NULL); 

    if (rank == root) 
    {
        // Αν ο χρήστης δεν έδωσε παράμετρο για αρχείο, τερμάτισε το πρόγραμμα
        if (argc != 2) 
        {
            printf("Usage: %s <input.txt>\n", argv[0]);
            return 1;
        }
        
        // Άνοιγμα αρχείου από γραμμή εντολών
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) 
        {
            printf("Error opening file\n");
            return 1;
        }

        // Διάβασμα απο αρχείο σε "δισδιάστατο" Α πίνακα
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
        // Κλείσιμο αρχείου
        fclose(file);
    }

    // Ο κεντρικός επεξεργαστής μοιράζει σε ΄ίσα τμήματα n_loc του Α σε όλες τις διεργασίες 
    MPI_Scatter(A, n_loc, MPI_INT, A_loc, n_loc, MPI_INT, root, MPI_COMM_WORLD);
    
    // A Ερώτημα

    //Πίνακας που κρατάει τις τιμές τις διαγωνίου του A_loc 
    int* diag;  
    // Πίνακας που κρατάει τα αθροίσματα για την γραμμή που βρίσκεται κάθε διαγώνιος 
    double* sum;

    // Δέσμευση πινάκων σύμφωνα με τον αριθμό των γραμμών που παίρνει κάθε διεργασία
    diag = (int*) malloc (num*sizeof(int)); assert(diag != NULL);
    sum = (double*) malloc (num*sizeof(double)); assert(sum != NULL);

    // Επανάληψη που αρχικοποιεί την diag με τις τιμές των διαγωνίων των num γραμμών του Α κάθε διεργασίας
    i = 0;
    for(k = (rank*num); k < (rank*num) + n_loc; k+=n+1){
        diag[i] = A_loc[k];
        i++;
    }

    // Αθροίζω τις τιμές των στοιχείων των γραμμών που βρίσκεται κάθε διαγώνιος 
    for(k = 0; k < num; k++){
        sum[k] = 0;
        for(i = 0; i < n; i++){
            if(A_loc[k*n + i]!=diag[k])
                sum[k] += fabs(A_loc[k*n + i]);
        }
    }

    // Τύπωμα για κάθε process των τιμών τις διαγωνίου και του sum για κάθε γραμμή που της αναλογεί
    for(int k = 0; k < num; k++)
        printf("Process %d: Line:%d Diagonal:%d Sum:%.2f\n",rank,k+rank*num,diag[k],sum[k]);
    
    //Κάθε διεργασία καθορίζει αν ο πίνακας είναι δεσπόζων
    int flag = 1;
    for(k = 0; k < num; k++)
        if(diag[k] <= sum[k])
        {
            printf("Process %d says is not dominant\n", rank);
            flag = 0;
        }

    // Η root μαζέυει όλες τις flag και κάνει AND μεταξύ τους (αν μια flag = 0, τότε global_flag = 0)
    int global_flag;
    MPI_Reduce(&flag, &global_flag, 1, MPI_INT, MPI_LAND, root, MPI_COMM_WORLD);

    // Η κεντρική επεξεργασία τυπώνει αν είναι δεσπόζων
    if(rank == root)
    {
        if (global_flag)
            printf("yes\n"); 
        else
            printf("no\n");
    }
    // Η διεργασία 0 ενημερώενει την τιμή της global_flag και στις υπόλοιοπες διεργασίες
    MPI_Bcast(&global_flag, 1, MPI_INT, root, MPI_COMM_WORLD); 
    if (!global_flag) 
    {
        // Αποδέσμευση πινάκων και τερματισμός προγράμματος
        free(A);
        free(A_loc);
        free(diag);
        free(sum);
        MPI_Finalize();
        return 0;
    }

    // Β Ερώτημα

    // Κάθε διεργασία ελέγχει ποια διαγώνιος της είναι η μέγιστη
    float max = fabs(diag[0]);
    for(k = 1; k < num; k++)
        if(max < fabs(diag[k]))
             max = fabs(diag[k]);

    float global_max;
    // Η root μαζεύει όλα τα τοπικά max των διεργασιών και βρίσκει την μεγαλύτερη τιμή
    MPI_Reduce(&max,&global_max,1,MPI_FLOAT,MPI_MAX,root,MPI_COMM_WORLD);\
    // 
    if(rank == root)
        printf("Max of diagonal: %.2f\n",global_max);

    // Γ Ερώτημα
    // Η root στέλνει σε όλες τις διεργασίες την τιμή του global_max για τους έπειτα υπολογισμούς
    MPI_Bcast(&global_max, 1, MPI_INT, root, MPI_COMM_WORLD); 

    // Δέσμευση πινάκων Β και τοπικού B_loc
    B = (int*) malloc (n*n*sizeof(int)); assert(B != NULL);
    B_loc = (int *) malloc (n_loc*sizeof(int)); assert(B_loc != NULL); 

    // Για κάθε γραμμή του Α που έχει κάθε διεργασία ενημερώνεται με τιμές ο Β_loc
    for(k = 0; k < num; k++){
        for(i = 0; i < n; i++){
           if(A_loc[k*n + i] == diag[k])
                B_loc[k*n + i] = global_max;
           else
                B_loc[k*n + i] = global_max - fabs(A_loc[k*n + i]);
        }
    }
    // Ο root συγκεντρώνει τους B_loc και τα συγκεντρώνει/κολλάει στον B
    MPI_Gather(B_loc, n_loc, MPI_INT, B, n_loc, MPI_INT, root, MPI_COMM_WORLD);

    // Τύπωμα του Β πίνακα
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
    int l = 0;      // σε ποια γραμμή του B_loc
    int index = 0;  // σε ποια στήλη βρίσκεται 
    // Εύρεση ελάχιστου και στην θέση που βρίσκεται
    for(k = 0; k < num; k++)
        for(i = 0; i < n; i++)
            if (B_loc[k*n + i] < min) {
                min = B_loc[k*n + i];
                index = i; 
                l = k;          
            }
    // Σε ποια γραμμή του πίνακα Β βρίσκεται (κάθε διεργασία ξεκινάει απο αυτην την γραμμή και έπειτα)
    int line = rank*num + l; 
    struct MinData in,out;
    // Αρχικοποιήση του ελάχιστου και της θέση που βρίσκεται
    in.min = min;
    in.loc = (line)*n + index;

    // Ο root βρίσκει την ελάχιστη τιμή του in.min και αποθηκεύει/κρατάει και την τιμή του in.index 
    // και τα βάζει στο out 
    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);

    if (rank == root) {
        // Πράξη για να χωρίσουμε τις τιμές των (i,j) (i == line, j == index)
        int min_i = out.loc / n;
        int min_j = out.loc % n;
        printf("Minimum value in B: %d\n", out.min);
        printf("Index of minimum in B: (%d,%d)\n",min_i,min_j);
    }

    // Αποδέσμευση πινάκων και τερματισμός προγράμματος
    free(A);
    free(A_loc);
    free(diag);
    free(sum);
    free(B);
    free(B_loc);
    MPI_Finalize();
    return 0;
}
