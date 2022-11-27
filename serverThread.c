#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<stdlib.h>
#include<pthread.h>

typedef struct threadDataStruct{
    int num;
    float feel;
} mojaTh;

void *serve_client( void *ptr );

    int read_size;
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
    char client_message[2000];
    pthread_t thread1;
    char *threadMessage = "Thread 1";
    char *threadMessage2 = "Thread 2";
    int iret1;
     
int main(int argc , char *argv[])
{
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = INADDR_ANY;

    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Binding server failed. System error");
        return 1;
    }
    puts("Bind done");
     
    listen(socket_desc , 3);
     
    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    
    while( client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c) ) {
        if (client_sock < 0)
        {
            perror("Accept failed");
            return 1;
        } else { 
            pthread_create( &thread1, NULL, serve_client, (void *)threadMessage );
        }

    }
    // pthread_exit(NULL);
    return 0;
}

void *serve_client( void *ptr )
{
    if(read_size == 0){
        puts("Client disconnected");
        fflush(stdout);
    } else if(read_size == -1) {
        perror("recv failed");
    }
    while( read_size = recv(client_sock , client_message , 2000 , 0)   )
    {
        client_message[read_size]=0x00;
        int lenghtOfDataHeader = client_message[0] - '0';
        int i = 1;
        char data[2000];

        while ( i <= lenghtOfDataHeader ) {
            data[i-1] = client_message[i];
            i++;
        }

        FILE* fp = fopen(data, "w");
        printf("\n=========\n");
        printf("Header: %s ", data);
        int j = 0;
        while ( client_message[i] != NULL ){
            data[j] = client_message[i];
            j++; 
            i++;
        }
        data[j] = '\n';
        if( fp == NULL ) {
            printf("Neuspjesno otvaranje fajla");
        } else{ 
            fprintf(fp, data);
        }
        fclose(fp);
        printf("\n=========\n");
        printf("Data: %s ", data);
        printf("\n=========\n");
        // printf("client_message: %s ", client_message);
        // printf("strlen client message&: %li ", strlen(client_message));

        // printf("Server get: ");
        // printf("%c\n", client_message[0]);
        send(client_sock , client_message , strlen(client_message),0);
        for ( int o = 0; o < read_size; o++ ){
            client_message[o] = '\0';
            data[o] = '\0';
        }
    }

    printf("kraj");
}