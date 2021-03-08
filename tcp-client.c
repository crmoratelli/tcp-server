/* Adaptado de https://www.thegeekstuff.com/2011/12/c-socket-programming/?utm_source=feedburner */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, 0,sizeof(recvBuff));
    
    /*Cria o Socket */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    } 

	/* Configura o IP de destino e porta na estrutura sockaddr_in */
    memset(&serv_addr, 0, sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        perror("inet_pton");
        return 1;
    } 

	/* Conecta ao servidor. */
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
    	perror("connect");
       	return 1;
    } 

	/* Aguarda o recebimento de dados do servidor. 
	 * Enquanto n for maior que 0. */
    while ( (n = recv(sockfd, recvBuff, sizeof(recvBuff)-1, 0)) > 0)
    {
		/* Coloca null no final da string. */
        recvBuff[n] = '\0';
        if(fputs(recvBuff, stdout) == EOF)
        {
            perror("fputs");
        }
    } 

    return 0;
}
