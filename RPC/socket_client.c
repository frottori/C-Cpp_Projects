#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void func1(int sockfd);
void func2(int sockfd);
void func3(int sockfd, int n);

int main(int argc, char *argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // HOSTNAME AND PORT
    if (argc < 3) 
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(1);
    }

    // socket()
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) error("ERROR, no such host\n");
    portno = atoi(argv[2]);

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if (server->h_addr_list[0] != NULL) 
        bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    else 
        error("ERROR, no address associated with hostname\n");
    serv_addr.sin_port = htons(portno);

    // connect()
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) error("ERROR connecting");

    int flag = 1, choice, n, i;
    int* y;

    while(flag)
    {
        do
        {
            printf("Give choice (1,2,3): ");
            scanf("%d", &choice);
        } while(choice < 1 || choice > 3);

        // Send the choice to the server
        if (send(sockfd, &choice, sizeof(int), 0) < 0)  error("ERROR sending choice to server");   

        // Read n length and y array
        printf("Give length n of y: ");
        scanf("%d", &n);

        // Dynamic Allocation for y
        y = (int* ) malloc (n*sizeof(int)); assert( y != NULL);
        for(i = 0; i < n; i++)
        {
            printf("Give y[%d]: ",i);
            scanf("%d", &y[i]);
        }
        // Send length n to server
        if (send(sockfd, &n, sizeof(int), 0) < 0) error("ERROR sending length n ");
        // Send array y to server
        if (send(sockfd, y, n*sizeof(int), 0) < 0) error("ERROR sending array y");

        switch(choice)
        {
            case 1: func1(sockfd); break;
            case 2: func2(sockfd); break;
            case 3: func3(sockfd, n); break;
        }

        do
        {
            printf("Do you want to continue? (0 or 1): ");
            scanf("%d", &flag);
        } while (flag < 0 || flag > 1);
        
        // Send the flag to the server
        if (send(sockfd, &flag, sizeof(int), 0) < 0) error("ERROR sending flag to server");
    }

    free(y);
    close(sockfd);
    return 0;
}

void func1(int sockfd)
{
    double res;
    // Receive Average from server
    if (recv(sockfd, &res, sizeof(double), 0) < 0) error("ERROR receiving double num from server");
    printf("Average: %lf\n",res);
}

void func2(int sockfd)
{
    int res[2];
    // Receive Max and Min values from server
    if (recv(sockfd, res, sizeof(res), 0) < 0) error("ERROR receiving double num from client");
    printf("Max: %d Min: %d\n",res[0], res[1]);
}

void func3(int sockfd, int n)
{
    double a;

    // Read doouble a 
    printf("Give double num a : ");
    scanf("%lf", &a);
     // Send double a to server
    if (send(sockfd, &a, sizeof(double), 0) < 0) error("ERROR sending double num");
    
    // Dynamic allocation for res
    double *res = (double*) malloc(n*sizeof(double));
    if (res == NULL) error("ERROR allocating memory for res");

    // Receive prod a*y double array from server
    if (recv(sockfd, res, n*sizeof(double), 0) < 0) 
    {
        error("ERROR receiving");
        free(res); 
    }

    printf("Prod a*y array: ");
    for (int i = 0; i < n; i++)
        printf("%lf ", res[i]);
    printf("\n");

    free(res);
}