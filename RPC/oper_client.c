#include "oper.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include <pthread.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void add_prog_1(char *host, int choice, int clientsockfd)
{
	CLIENT *clnt;
	double  *result_1;
	in1  avg_1_arg;
	out1  *result_2;
	in1  maxmin_1_arg;
	out2  *result_3;
	in2  proday_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, ADD_PROG, ADD_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	int n, i;
	int* y;
	// Receive n length from socket client
    if (recv(clientsockfd, &n, sizeof(int), 0) < 0) error("ERROR receiving length from client");
	printf("CHOICE: %d \nReceived n: %d\n",choice, n);

	// Dynamic allocation for y
    y = (int*) malloc (n*sizeof(int)); 
    if (y == NULL) error("ERROR allocating memory for y");

    // Receive array y from client
    if (recv(clientsockfd, y, n*sizeof(int), 0) < 0) 
    {
        error("ERROR receiving array from client");
        free(y); 
    }

	printf("Received y: ");
    for (i = 0; i < n; i++)
        printf("%d ", y[i]);
    printf("\n");

	if(choice == 1)
	{
		avg_1_arg.y.y_len = n;
		avg_1_arg.y.y_val = (int*) malloc(n*sizeof(int)); 
		if (avg_1_arg.y.y_val == NULL) error("ERROR allocating memory for y");

		// Copy values to argument for RPC server
		for(i = 0; i < n; i++)
			avg_1_arg.y.y_val[i] = y[i];

		result_1 = avg_1(&avg_1_arg, clnt);
		if (result_1 == (double *) NULL) 
		{
			clnt_perror (clnt, "call failed");
		}
		else
		{
			// send avg to the socket client
			if (send(clientsockfd, result_1, sizeof(double), 0) < 0) error("ERROR sending double num");
		}
		free(avg_1_arg.y.y_val);
	}
	else if (choice == 2) 
	{
		maxmin_1_arg.y.y_len = n;
		maxmin_1_arg.y.y_val = (int*) malloc(n*sizeof(int)); 
		if (maxmin_1_arg.y.y_val == NULL) error("ERROR allocating memory");

		for(i = 0; i < n; i++)
			maxmin_1_arg.y.y_val[i] = y[i];
		
		result_2 = maxmin_1(&maxmin_1_arg, clnt);
		if (result_2 == (out1 *) NULL) 
		{
			clnt_perror (clnt, "call failed");
		}
		else
		{	
			// Send maxmin[2] to the socket client 
			if (send(clientsockfd, result_2->maxmin, sizeof(result_2->maxmin), 0) < 0) error("ERROR sending array");
		}
		free(maxmin_1_arg.y.y_val);
	}
	else
	{
		double a;
		// Receive double num a from socket client
		if (recv(clientsockfd, &a, sizeof(double), 0) < 0) error("ERROR receiving double num from client");
		printf("Received a: %lf\n",a);

		proday_1_arg.a = a;
		proday_1_arg.y.y_len = n;
		proday_1_arg.y.y_val = (int*) malloc(n*sizeof(int)); 
		if (proday_1_arg.y.y_val == NULL) error("ERROR allocating memory");
    
		for(i = 0; i < n; i++)
			proday_1_arg.y.y_val[i] = y[i];

		result_3 = proday_1(&proday_1_arg, clnt);
		if (result_3 == (out2 *) NULL) 
		{
			clnt_perror (clnt, "call failed");
		}
		else
		{
			// Send the double array of prod to the socket client
			if (send(clientsockfd, result_3->pr.pr_val, n*sizeof(double), 0) < 0) error("ERROR sending array");
		}
		free(proday_1_arg.y.y_val);
	}

	free(y);
	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

// struct to pass arguments to pthread
typedef struct {
    int clientsockfd;
    char* host;
} thread_args_t;

#define N 5
void* client_handler(void* args);

int main (int argc, char *argv[])
{
	char *host;

	if (argc < 3) {
		printf ("usage: %s server_host PORT\n", argv[0]);
		exit (1);
	}
	host = argv[1];

	int sockfd, clientsockfd, portno, clilen, n;
    struct sockaddr_in serv_addr, cli_addr;

	// socket()
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) error("ERROR opening socket");

    // sockaddr_in struct for bind()
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[2]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

	// bind()
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR on binding");

    // listen() for N clients
    if (listen(sockfd, N) == -1) error("ERROR on listen");

	// accept() and communication begins
    while(1) 
    {
        clilen = sizeof(cli_addr);
        clientsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (clientsockfd < 0) error("ERROR on accept");
        
        printf("Client connected...\n");

		pthread_t thread;
       	thread_args_t args;
	   	args.clientsockfd = clientsockfd;
	   	args.host = host;

	   	if (pthread_create(&thread, NULL, client_handler, (void*) &args) != 0) error("ERROR pthread_create");
    }

	exit (0);
}

void* client_handler(void* args) 
{
    thread_args_t* thread_args = (thread_args_t*) args;
    int clientsockfd = thread_args->clientsockfd;
    char* host = thread_args->host;
    int flag = 1, choice;

    while(flag)
    {
       // Receive the choice from the client
        if (recv(clientsockfd, &choice, sizeof(int), 0) < 0) error("ERROR receiving choice from client");

		// RPC Function call
		add_prog_1 (host, choice, clientsockfd);       

        // Receive the flag from the client (to exit program)
        if (recv(clientsockfd, &flag, sizeof(int), 0) < 0) error("ERROR receiving flag from client");
    }

    close(clientsockfd);
    pthread_exit(NULL);
}