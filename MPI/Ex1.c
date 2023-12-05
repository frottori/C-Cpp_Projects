#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

// Συναρτήσεις που συλλέγει ο κεντρικός επεξεργαστής και τυπώνει τα αποτελέσματα
float print_VAL(float m, char msg[]);
float print_MAX(int max);
void print_D(int* d);

// global μεταβλητές που χρησιμοποιουν όλες οι διεργασίες (και οι συναρτήσεις)
int rank, p, n, num;
int tag1 = 50, tag2 = 60, tag3 = 70;
MPI_Status status;

int main(int argc, char **argv) 
{
    int i, target, choice;
    int *x, *x_loc;
    char msg[20];
    srand(time(NULL));

    MPI_Init(&argc, &argv);                // Αρχικοποιήση MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Τρέχουσα process id
    MPI_Comm_size(MPI_COMM_WORLD, &p);   // Πλήθος διεργασιων

    do
    {
        if(rank == 0) // κεντρικός επεξεργαστής
        {
            do
            {
                printf("Δώσε πλήθος n: ");
                scanf("%d", &n);
            } 
            while(n%p != 0);  //έλεγχος για αν n ακέραιο πολλαπλάσιο του p

            // Δέσμευση πίνακα για n στοιχεία
            x = (int *) malloc(n * sizeof(int)); assert(x != NULL);

            // Εισαγωγή των στοιχείων του πίνακα
            printf("Δώσε τα %d στοιχεία : ", n);
            for(i = 0; i < n; i++)
                scanf("%d", &x[i]);

            num = n/p; i = num;
            // Πλήθος στοιχείων που θα πάρει ο κάθε επεξεργαστής 
            for (target = 1; target < p; target++) 
                MPI_Send(&n, 1, MPI_INT, target, tag1, MPI_COMM_WORLD);
            
            // Tα στοιχεία που αναλογούν σε κάθε επεξεργαστή
            for (target = 1; target < p; target++) 
            {
                MPI_Send(&x[i], num, MPI_INT, target, tag2, MPI_COMM_WORLD);
                i += num;
            }

            // Δέσμευση x_loc για τον κεντρικό επεξεργαστή
            x_loc = (int *) malloc(num * sizeof(int)); assert(x_loc != NULL);
            for(i = 0; i < num; i++)
                x_loc[i] = x[i];
        }
        else // άλλοι επεξεργαστές
        {
            MPI_Recv(&n, 1, MPI_INT, 0, tag1, MPI_COMM_WORLD, &status);
            num = n/p; // πλήθος θέσεων του x_loc
            // Δέσμευση x_loc για τις υπόλοιπες διεργασίες
            x_loc = (int *) malloc(num * sizeof(int)); assert(x_loc != NULL);
            MPI_Recv(&x_loc[0], num, MPI_INT, 0, tag2, MPI_COMM_WORLD, &status);
        }

        // Υπολογισμός τοπικού μέσου όρου και μεγίστου
        int sum = 0;
        int max = x_loc[0];
        for(i = 0; i < num; i++)
        {
            sum += x_loc[i];
            if(x_loc[i] > max)
                max = x_loc[i];
        }      
        float mo = (float) sum/n;
            
        // Όλοι οι επεξεργαστες στέλνουν στον 0 τα αποτελέσματα που βρήκαν
        if(rank != 0)  
        { 
            MPI_Send(&mo, 1, MPI_FLOAT, 0, tag3, MPI_COMM_WORLD);
            MPI_Send(&max, 1, MPI_INT, 0, tag3, MPI_COMM_WORLD);
            // Λαμβάνουν απο τον 0 τον συνολικο μέσο όρο και μέγιστο
            MPI_Recv(&mo, 1, MPI_FLOAT, 0, tag3, MPI_COMM_WORLD,&status);
            MPI_Recv(&max, 1, MPI_INT, 0, tag3, MPI_COMM_WORLD,&status);
        }
        else  // Ο Κεντρικός επεξεργαστής
        { 
            // (α) Υπολογισμός συνολικού μέσου όρου
            strcpy(msg,"Μέσος Όρος"); 
            mo = print_VAL(mo,msg);       
            // (β) Υπολογισμός συνολικού μέγιστου
            max = print_MAX(max);  
            // Στέλνει σε όλους τον συνολικό μέσο όρο και μέγιστο
            for (target = 1; target < p; target++) 
            {
                MPI_Send(&mo, 1, MPI_FLOAT, target, tag3, MPI_COMM_WORLD);
                MPI_Send(&max, 1, MPI_INT, target, tag3, MPI_COMM_WORLD);
            }      
        }
        
        // Υπολογισμός διασποράς (variance) και διανύσματος d
        float var = 0;
        int* d_loc;
        if(rank == 0)
        {
            d_loc = (int *) malloc(n * sizeof(int)); assert(d_loc != NULL);
        }
        else 
        {
            d_loc = (int *) malloc(num * sizeof(int)); assert(d_loc != NULL);
        }
           
        for(i = 0; i < num; i++)
        {
            var += (x_loc[i] - mo)*(x_loc[i] - mo);
            d_loc[i] = fabs((x_loc[i] - max)*(x_loc[i] - max));
        }    
        var = var/n;
        
        // Όλοι οι επεξεργαστες στέλνουν στον 0 τα αποτελέσματα που βρήκαν
        if(rank != 0)  
        {
            MPI_Send(&var, 1, MPI_FLOAT, 0, tag3, MPI_COMM_WORLD);
            MPI_Send(&d_loc[0], num, MPI_INT, 0, tag3, MPI_COMM_WORLD);
            // Λαμβάνουν απο τον 0 την επιλογή του χρήστη
            MPI_Recv(&choice, 1, MPI_INT, 0, tag3, MPI_COMM_WORLD,&status);
            // Αποδεσμευση μνήμης για τις υπόλοιπες διεργασίες
            free(x_loc);
            free(d_loc);
        }
        else // Ο Κεντρικός επεξεργαστής
        {
            // (γ) Υπολογισμός συνολικής διασποράς
            strcpy(msg,"Διασπορά");
            print_VAL(var,msg); 
            // (δ) Τυπώνει το διάνυσμα d
            print_D(d_loc);

            printf("\n------------MENOY------------\n");
            printf("\t1. Συνέχεια\n\t2. Έξοδος\n");
            do
            {
                printf("Επιλογή: ");
                scanf("%d", &choice);
            }
            while(choice != 1 && choice != 2);
            printf("-----------------------------\n\n");
            // Στέλνει σε όλους την επιλογή του χρήστη
            for (target = 1; target < p; target++) 
                MPI_Send(&choice, 1, MPI_INT, target, tag3, MPI_COMM_WORLD); 
            // Αποδεσμευση μνήμης για τον κεντρικό επεξεργαστή
            free(x); 
            free(x_loc);
            free(d_loc);
        }
    }
    while(choice != 2);

    MPI_Finalize(); // Τέλος MPI
    return 0;
}

float print_VAL(float val, char msg[])
{     
    int source;  
    float fin = val;
    printf("\n----------%s----------\n", msg);
    printf("\tΔιεργασία %d: %.2f\n", rank, val);
    for(source = 1; source < p; source++)  
    {
        MPI_Recv(&val, 1, MPI_FLOAT, source, tag3, MPI_COMM_WORLD, &status);
        fin = fin + val;
        printf("\tΔιεργασία %d: %.2f\n", source, val);
    }
    printf("\n\tΣύνολο: %.2f\n", fin);
    return fin;
}

float print_MAX(int max)
{     
    int source;  
    int finmax = max;
    printf("\n-----------Μέγιστος-----------\n");
    printf("\tΔιεργασία %d: %d\n", rank, max);
    for(source = 1; source < p; source++)  
    {
        MPI_Recv(&max, 1, MPI_INT, source, tag3, MPI_COMM_WORLD, &status);
        printf("\tΔιεργασία %d: %d\n", source, max);
        if(max > finmax)
            finmax = max;    
    }
    printf("\n\tΣύνολο: %d\n", finmax);
    return finmax;
}

void print_D(int* d)
{       
    int source; 
    int i = num;
    printf("\n--------Διάνυσμα d[n]--------\n\n");
    for(source = 1; source < p; source++) 
    {
        MPI_Recv(&d[i], num, MPI_INT, source, tag3, MPI_COMM_WORLD,&status);
        i += num;
    }
    for(i = 0; i < n; i++)
        printf("%d ", d[i]);
    printf("\n");
}
