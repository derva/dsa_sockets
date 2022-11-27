#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>

int main(int argc , char *argv[])
{
    int sock;
    printf("argv[1]: %s\n", argv[1]);
    FILE *fpForRead = fopen(argv[1], "r");
    if( fpForRead == NULL){
        printf("Nisam uspio otvoriti file\n");
    } else{
        printf("Uspio sam otvoriti file\n");
    }
    char messageForRead[2000],ch; 
    int k = 0;
    while( (ch = fgetc(fpForRead)) != EOF){
        // printf("uso u while\n");
        // printf("%c", ch);
        messageForRead[k] = ch;
        k++;
    }
    printf("message for read %s\n", messageForRead);
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    int read_size;
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    //keep communicating with server
    int i =0;
    while(i < 1 )
    {
        printf("Enter message : ");
        // scanf("%s" , message);
	    // gets(message);
         
        //Send some data
        if( send(sock , messageForRead , strlen(messageForRead) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        if( (read_size=recv(sock , server_reply , 2000 , 0)) < 0)
        {
            puts("recv failed");
            break;
        }
        gets(message);
        puts("Server reply :");
	    server_reply[read_size]=0x00;
        puts(server_reply);
        i++;
    }
     
    close(sock);
    return 0;
}
