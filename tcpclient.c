/* tcp_ client.c */
/* Programmed by Adarsh Sethi */
/* Sept. 19, 2019 */

#include <stdio.h>      /* for standard I/O functions */
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for memset, memcpy, and strlen */
#include <netdb.h>      /* for struct hostent and gethostbyname */
#include <sys/socket.h> /* for socket, connect, send, and recv */
#include <netinet/in.h> /* for sockaddr_in */
#include <unistd.h>     /* for close */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define STRING_SIZE 1024

int main(void)
{

   int sock_client; /* Socket used by client */

   struct sockaddr_in server_addr;                  /* Internet address structure that
                                        stores server address */
   struct hostent *server_hp;                       /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE] = "localhost"; /* Server's hostname */
   unsigned short server_port = 64441;              /* Port number used by server (remote port) */

   char sentence[STRING_SIZE]; /* send message */
   int receivedHeader;         /* received header */
   unsigned int msg_len;       /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */

   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
   {
      perror("Client: can't open stream socket");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information 
            unless you want to specify a specific local port
            (in which case, do it the same way as in udpclient.c).
            The local address initialization and binding is done automatically
            when the connect function is called later, if the socket has not
            already been bound. */

   /* initialize server address information */

   //printf("Enter hostname of server: ");
   //scanf("%s", server_hostname);
   if ((server_hp = gethostbyname(server_hostname)) == NULL)
   {
      perror("Client: invalid server hostname");
      close(sock_client);
      exit(1);
   }

   //printf("Enter port number for server: ");
   //scanf("%hu", &server_port);

   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
          server_hp->h_length);
   server_addr.sin_port = htons(server_port);

   /* connect to the server */

   if (connect(sock_client, (struct sockaddr *)&server_addr,
               sizeof(server_addr)) < 0)
   {
      perror("Client: can't connect to server");
      close(sock_client);
      exit(1);
   }

   /* user interface */

   //printf("Please input a sentence:\n");
   //scanf("%s", sentence);
   strcpy(sentence, "test2.txt");
   msg_len = strlen(sentence) + 1;

   /* send message */

   bytes_sent = send(sock_client, sentence, msg_len, 0);

   /* get response from server */
   int fp = open("testoutput.txt",O_WRONLY|O_CREAT, 0777);
   int n = 0;
   while (1)
   {
      receivedHeader = 0;
      int count = 0;
      int seqNum = 0;
      bytes_recd = recv(sock_client, &receivedHeader, 4, 0);
      if (bytes_recd > 0)
      {
         receivedHeader = ntohl(receivedHeader);
         count = (receivedHeader >> 16 & 0x0000FFFF);
         seqNum = receivedHeader & 0x0000FFFF;
         if (count == 0)
         {
	   //end of transmission packet
	   printf("End of Transmission Packet with sequence number %d recieved with %d data bytes\n",seqNum,count);
	   exit(0);
         }
      }
      char line[1024];
      memset(&line,0,sizeof(line));
      bytes_recd = recv(sock_client, &line, count, 0);
      if (bytes_recd > 0)
      {
         printf("Packet %d recieved with %d data bytes\n",seqNum,count);
	 if(write(fp,line,strlen(line))<0){
	   printf("error with writing");
	 }
      }
   }
   /* close the socket */
   close(sock_client);
}
