/*
--------------------------------------------------------------------------------
SPADE - Support for Provenance Auditing in Distributed Environments.
Copyright (C) 2011 SRI International

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <sys/un.h>
#include <assert.h>

#define SERVER_PATH     "/dev/audit"
#define BUFFER_LENGTH   10000
#define FALSE           0

#ifdef DEBUG
#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "spade-audit", __VA_ARGS__)
#else
#define LOGD(...)
#define NDEBUG
#endif



// Defining SUN_LEN. Does not exist in Bionic LIBC
#include <string.h>
#define SUN_LEN(ptr) ((size_t) (((struct sockaddr_un *) 0)->sun_path)	\
	+ strlen ((ptr)->sun_path))

int main(int argc, char *argv[]) {
    int sd = -1, rc, bytesReceived, start, end;
    char buffer[BUFFER_LENGTH];
    struct sockaddr_un serveraddr;  

    /***********************************************************************/
    /* A do/while(FALSE) loop is used to make error cleanup easier.  The   */
    /* close() of the socket descriptor is only done once at the very end  */
    /* of the program.                                                     */
    /***********************************************************************/
    do {

        /***********************************************************************/
        /* The socket() function returns a socket descriptor, which represents */
        /* an endpoint.  The statement also identifies that the UNIX           */
        /* address family with the stream transport (SOCK_STREAM) will be      */
        /* used for this socket.                                               */
        /***********************************************************************/
        sd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sd < 0) {
            //perror("socket() failed");
            break;
        }

        /********************************************************************/
        /* If an argument was passed in, use this as the server, otherwise  */
        /* use the #define that is located at the top of this program.      */
        /********************************************************************/
        memset(&serveraddr, 0, sizeof (serveraddr));
        serveraddr.sun_family = AF_UNIX;
        strcpy(serveraddr.sun_path, SERVER_PATH);

        /********************************************************************/
        /* Use the connect() function to establish a connection to the      */
        /* server.                                                          */
        /********************************************************************/
        rc = connect(sd, (struct sockaddr *) &serveraddr, SUN_LEN(&serveraddr));
        if (rc < 0) {
            //perror("connect() failed");
            break;
        }

        /********************************************************************/
        /* Receive the data from the socket in a blocking call              */
        /********************************************************************/
        memset(&buffer, 0, BUFFER_LENGTH);
	start = 0;
	end = 0;
        while (1) {
	  char* leftover;
            rc = recv(sd, & buffer[start], BUFFER_LENGTH - end - 1, 0);

            if (rc <= 0) {
	        LOGD("*** Recv failed! ***");
                //perror("recv() failed");
                break;
            }
            end += rc;
	    buffer[rc] = '\0';

	    assert( memmem(buffer, end, "\0", 1) == NULL);

	    printf("%s", buffer);    
	    start = end = 0;
	    LOGD(buffer);
        }
    } while (FALSE);

    if (sd != -1) close(sd);
    return 0;

}
