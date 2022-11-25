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
    if (socket_desc == -1) {
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
                perror("Recieve failed");
            }
            while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0   )
            {
                char data[2000];
                char finalData[2000];
                printf("Data prije %s", data);
                printf("Read size %i", read_size);
                client_message[read_size]=0x00;
                int lenghtOfDataHeader = client_message[0] - '0';
                int i = 1;
                printf("Length of data %i", lenghtOfDataHeader);

                while ( i <= lenghtOfDataHeader ) {
                    data[i-1] = client_message[i];
                    i++;
                }
                printf("data poslije %s\n", data);

                //FILE* fp = fopen("/home/dervis/Documents/fileZaTest.txt", "w");
                sprintf(finalData, "%s.txt", data);
                printf("Data.txt: %s ", finalData);
                FILE* fp = fopen(finalData, "w");
                printf("\n=========\n");
                printf("Zaglavlje: %s ", data);
                int j = 0;
                while ( client_message[i] != 0x00 ){
                    data[j] = client_message[i];
                    j++; 
                    i++;
                }
                if( fp == NULL ) {
                    printf("Neuspjesno otvaranje fajla");
                } else{ 
                    // printf("Upiso u fajl");
                    fprintf(fp, data);
                }
                fclose(fp);
                printf("\n=========\n");
                printf("Data: %s ", data);
                printf("\n=========\n");
                send(client_sock , client_message , strlen(client_message),0);
                for ( int o = 0; o < read_size; o++ ){
                    client_message[o] = '\0';
                    data[o] = '\0';
                }         
            }
        }
    }
    return 0;
}
