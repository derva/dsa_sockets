#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
     
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
        }
        puts("Connection accepted");

        if( fork() == 0) {
            if(read_size == 0){
                puts("Client disconnected");
                fflush(stdout);
            } else if(read_size == -1) {
                perror("recv failed");
            }
            while( read_size = recv(client_sock , client_message , 2000 , 0)   )
            {
                client_message[read_size]=0x00;
                // printf("Kraj client poruke: %s ", client_message[read_size]);
                // printf("Client poruka je: %s ", client_message);
                int lenghtOfDataHeader = client_message[0] - '0';
                // printf("lenghtOfData: %i ", lenghtOfDataHeader);
                int i = 1;
                char data[2000];

                while ( i <= lenghtOfDataHeader ) {
                    data[i-1] = client_message[i];
                    // printf("+");
                    i++;
                }

                FILE* fp = fopen(data, "w");
                printf("\n=========\n");
                printf("Zaglavlje: %s ", data);
                int j = 0;
                while ( client_message[i] != NULL ){
                    // printf("-");
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
                printf("client_message: %s ", client_message);
                printf("strlen client message&: %li ", strlen(client_message));

                printf("Server get: ");
                // client_message[read_size]=0x00;
                printf("%c\n", client_message[0]);
                send(client_sock , client_message , strlen(client_message),0);
            }
            
            // if(read_size == 0){
            //     puts("Client disconnected");
            //     fflush(stdout);
            // } else if(read_size == -1) {
            //     perror("recv failed");
            // }
        }
    }
    return 0;
}
