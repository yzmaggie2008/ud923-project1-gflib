#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h> 
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdio.h>
#include "gfclient.h"


typedef struct gfcrequest_t{
	char *server;
	char *path;
	unsigned short port;
	int status;
	void (*headerfunc)(void*, size_t, void *);
	void *headerarg;
	void (*writefunc)(void*, size_t, void *);
	void *writearg;
	size_t bytesreceived;
}gfcrequest_t;

struct student
{
   char name[20];
   int roll;
   float marks;
};

gfcrequest_t *gfc_create(){
	/*
	 * This function must be the first one called as part of
	 * request.  It returns a gfcrequest_t handle which should be
	 * passed into all subsequent library calls pertaining to
	 * this requeest.
	 */
	gfcrequest_t *gfr = (gfcrequest_t *)malloc(sizeof(*gfr));
	memset(gfr, 0, sizeof *gfr);
	return gfr;
}

void gfc_set_server(gfcrequest_t *gfr, char* server){

	/*
	 * Sets the server to which the request will be sent.
	 */

	gfr->server = (char *)malloc(sizeof(server));
	if (gfr->server == NULL){
		perror("ERROR SETTING SERVER\n");

	}
 	strcpy(gfr->server, server);
}

void gfc_set_path(gfcrequest_t *gfr, char* path){

	/*
	 * Sets the path of the file that will be requested.
	 */
	gfr->path = malloc(sizeof(path));
	if (gfr->path == NULL){
		perror("ERROR SETTING PATH\n");

	}
	strcpy(gfr->path, path);
}

void gfc_set_port(gfcrequest_t *gfr, unsigned short port){
	/*
	 * Sets the port over which the request will be made.
	 */
	if (gfr == NULL){
		perror("GFR was NULL in set port\n");
	}
	gfr->port =  port;
}

void gfc_set_headerfunc(gfcrequest_t *gfr, void (*headerfunc)(void*, size_t, void *)){
	/*
	 * Sets the callback for received header.  The registered callback
	 * will receive a pointer the header of the response, the length
	 * of the header response as it's second argument (don't assume that
	 * this is null-terminated), and the pointer registered with
	 * gfc_set_headerarg (or NULL if not specified) as the third argument.
	 *
	 * You may assume that the callback will only be called once and will
	 * contain the full header.
	 */
	 //gfr->headerfunc = headerfunc;

}

void gfc_set_headerarg(gfcrequest_t *gfr, void *headerarg){
	/*
	 * Sets the third argument for all calls to the registered header callback.
	 */
	//gfr->headerarg = headerarg;
}

void gfc_set_writefunc(gfcrequest_t *gfr, void (*writefunc)(void*, size_t, void *)){
	/*
	 * Sets the callback for received chunks of the body.  The registered
	 * callback will receive a pointer the chunk, the length of the chunk
	 * as it's second argument (don't assume that this is null-terminated),
	 * and the pointer registered with gfc_set_writearg (or NULL if not
	 * specified) as the third argument.
	 *
	 * The callback may be called multiple times in a single request.  The
	 * gfclient library does not store the entire contents of the requested file
	 * but rather calls this callback each time that it receives a chunk of data
	 * from the server.
	 */
	gfr->writefunc = writefunc;
}

void gfc_set_writearg(gfcrequest_t *gfr, void *writearg){
	/*
	 * Sets the third argument for all calls to the registered header callback.
	 */
	gfr->writearg = writearg;
}

int gfc_perform(gfcrequest_t *gfr){
	/*
	 * Performs the transfer as described in the options.  Returns a value of 0
	 * if the communication is successful, including the case where the server
	 * returns a response with a FILE_NOT_FOUND or ERROR response.  If the
	 * communication is not successful (e.g. the connection is closed before
	 * transfer is complete or an invalid header is returned), then a negative
	 * integer will be returned.
	 */
	char str[80];
	int sockfd = 0; //socket file descriptor
	int n_conn;
	int bytes_read;
	int bytes_written;
	int status;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	const char *hostname = "localhost";
	char request_str[300];
	struct addrinfo hints;
	struct addrinfo *servinfo;

	printf("This is the server: %s\n", gfr->server);
	printf("This is the path: %s\n", gfr->path);
	printf("This is the port: %d\n", gfr->port);
    //
	//memset(str, 0, sizeof str);
	//sprintf(str, "%d", gfr->port);
	//printf("This is the port (Str): %s\n", str);
	puts("stewy");
	//server = gethostbyname("localhost");
	server = gethostbyname(gfr->server);
	//status = getaddrinfo(gfr->server, gfr->port, &hints, &servinfo);
	//status = getaddrinfo("localhost", "8888", &hints, &servinfo);
	char *err_ms = "fucked up getaddrinfo";
	if (status < 0){
		perror(err_ms);
	}
    //server = gethostbyname(hostname);
	/*
	if(server == NULL){
		perror("Error: No such host\n");
		return EXIT_FAILURE;
	}

	//Create socket with defaults
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("Error: Could not create socket\n");
		return EXIT_FAILURE;
	}

	//Update serv_addr struct to have server information
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	//Update port number before connect
	serv_addr.sin_port = htons(gfr->port);
	printf("This is the sent text: %s", request_str);
	//n_conn = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	//if (n_conn < 0){
	//	perror("Could not establish connection\n");
	//	return EXIT_FAILURE;
	//}
	 *

    while (1){

    	bytes_written = send(sockfd,  )
    	bytes_read = recv(sock, (void *)buffer, sizeof(buffer),0);
    	printf("client: bytes read %d\n", bytes_read);
    	if (bytes_read == 0){
    		break;
    	}
    	else if (bytes_read < 0){
    		error("ERROR reading socket");
    	}
    	void *p = buffer;
    	while (bytes_read > 0){
    		bytes_written = write(fhandle_open, p, bytes_read);
    		printf("client: bytes written %d\n", bytes_written);
    		if (bytes_written <= 0){
    			error("ERROR writing to socket");
    		}
    		bytes_read -= bytes_written;
    		p+=bytes_written;
    	}
    }
    */
	//gfr->writefunc((void *)&sockfd, sizeof(&sockfd), gfr->writearg);
	//puts(str);
	puts("finish perform");
	return 0;
}

gfstatus_t gfc_get_status(gfcrequest_t *gfr){
	/*
	 * Returns the status of the response.
	 */

	return gfr->status;
}

/*
 * Returns the status of the response.
 */
char* gfc_strstatus(gfstatus_t status){
	/*
	 * Returns the string associated with the input status
	 */
	if (status == GF_OK)
	{
		return "OK";
	}
	else if (status == GF_FILE_NOT_FOUND)
	{
		return "FILE_NOT_FOUND";
	}
	else
	{
		return "ERROR";
	}
}

size_t gfc_get_filelen(gfcrequest_t *gfr){
	/*
	 * Returns the length of the file as indicated by the response header.
	 * Value is not specified if the response status is not OK.
	 */
	if (gfr != NULL && gfc_get_status(gfr) == GF_OK)
	{
		return sizeof(gfr->path);
	}
	else{
		return -1;
	}
}

size_t gfc_get_bytesreceived(gfcrequest_t *gfr){

	/*
	 * Returns actual number of bytes received before the connection is closed.
	 * This may be distinct from the result of gfc_get_filelen when the response
	 * status is OK but the connection is reset before the transfer is completed.
	 */
	return gfr->bytesreceived;
}

void gfc_cleanup(gfcrequest_t *gfr){
	/*
	 * Frees memory associated with the request.
	 */
	//free(gfr->server);
	//free(gfr->path);
	//free(gfr);
}

void gfc_global_init(){
	/*
	 * Sets up any global data structures needed for the library.
	 * Warning: this function may not be thread-safe.
	 */
	puts("3");
}


void gfc_global_cleanup(){
	/*
	 * Cleans up any global data structures needed for the library.
	 * Warning: this function may not be thread-safe.
	 */
	puts("3");
}
