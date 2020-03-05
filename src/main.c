/*
 * @project simple http server
 * @file main.c
 * @author Soundwave
 * @date Mar 6, 2020
 * @brief simple http server
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 4096

char * hypertext(FILE *file){
	char *header = "HTTP/1.1 200 OK\r\n\n";
	char buffer[BUFFER_SIZE];
	size_t size = strlen(header);
	char* hypertext_ = malloc(size+1);
	strcat(hypertext_, header);
	if(file == NULL){fprintf(stderr, "http_server: problem with file"); exit(EXIT_FAILURE);}
	while(fgets(buffer, BUFFER_SIZE, file) != NULL){
		size+=BUFFER_SIZE;
		hypertext_ = realloc(hypertext_, size);
		if(hypertext_ == NULL){fprintf(stderr, "http_server: allocating memory error");exit(EXIT_FAILURE);}
		strcat(hypertext_, buffer);
	}
	return hypertext_;
}


int main(int argc, char **argv){
	FILE* index_file = fopen("examples/index.html","r");
	int server_fd, new_socket;
	long valread;
	struct sockaddr_in address;
	const int addrlen = sizeof(address);
	char *message = hypertext(index_file);

	if((server_fd = socket(AF_INET, SOCK_STREAM,0)) == 0){fprintf(stderr, "http_server: problem with socket");return 1;}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));

	if(bind(server_fd, (struct sockaddr*)(&address),sizeof(address))<0){ //typical type
		fprintf(stderr,"http_server: problem with binding");
		return 1;
	}

	if(listen(server_fd,10) < 0){
		fprintf(stderr, "http_server: problem with listeing");
		return 1;
	}
	while(1){
		printf("\n\t\t\t Waitining for new connection\t\t\t\n\n");
		if( (new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))<0 ){
			fprintf(stderr, "http_server: problem with accept");
			return 1;
		}
		char buffer[BUFFER_SIZE] = {0};
		valread = read(new_socket, buffer, BUFFER_SIZE);
		printf("Message_in:   %s\n", buffer);
		write(new_socket, message, strlen(message));
		printf("message sent");
		close(new_socket);
	}

	return 0;
}

