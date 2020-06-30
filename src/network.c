// 6.4 Network functionality, only for a single connection
// Based on whether the ijvm uses netbind or netconnect, an activeSocket gets set to either be the socket accepted from the bound connection
// or the local socket, initialized in sock_init
// The netin and netout then always read/write to that active socket

#include "network.h"
#include "stack.h"
#include "string.h"

int activeSock = -1;

int socket_init()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        fprintf(stderr, "Socket creation fail\n");
        return -1;
    }
    return sock;
}

bool netbind()
{
    word_t port = pop();

    int localSock = socket_init();

    struct sockaddr_in bindAddress;
    bindAddress.sin_family = AF_INET;               // Address set to ipv4
    bindAddress.sin_addr.s_addr = INADDR_ANY;       // Address set to localhost interfaces
    bindAddress.sin_port = htons(port);             // Set port to the specified one
    int addressLen;

    if(bind(localSock, (struct sockaddr*) &bindAddress, sizeof(bindAddress)) < 0)
	{
		fprintf(stderr, "Bind failed\n");
        return push(0);
	}

    if(listen(localSock, 2) < 0)
    {
        fprintf(stderr, "Listen failed\n");
        return push(0);
    }

    addressLen = sizeof(bindAddress);
    activeSock = accept(localSock, (struct sockaddr*) &bindAddress, (socklen_t*) &addressLen);  // Sets activeSock to the one obtained from the accepted connection
    if(activeSock < 0)
    {
        fprintf(stderr, "Accept failed\n");
        return push(0);
    }

    return push(1);
}

char* decodeAddress(word_t hostEncoded)
{
    unsigned char bytes[4];
    char *host;
    host = malloc(15 * sizeof(char));
    host[0] = 0;                                            // This turns the encoded word to a byte array
    bytes[0] = (hostEncoded >> 24) & 0xFF;
    bytes[1] = (hostEncoded >> 16) & 0xFF;
    bytes[2] = (hostEncoded >> 8) & 0xFF;
    bytes[3] = hostEncoded & 0xFF;
    for(int i = 0; i < 3; i++)                              // This concatenates the 4 bytes into one string
    {
        int addrSegment = bytes[i];
        sprintf(host + strlen(host), "%d", addrSegment);
        strcat(host, ".");
    }
    int addrSegment = bytes[3];
    sprintf(host + strlen(host), "%d", addrSegment);

    return host;
}

bool netconnect()
{
    word_t port = pop();
    word_t hostEncoded = pop();
    char *host = decodeAddress(hostEncoded);

    int serverSock = socket_init();

    struct sockaddr_in connAddress;
    connAddress.sin_family = AF_INET;                           // Setting the connection address to ipv4
    connAddress.sin_port = htons(port);                         // Setting the connection address to the designated port
    if(inet_pton(AF_INET, host, &connAddress.sin_addr) <= 0)    // Setting the connection address to the designated address
    {
        fprintf(stderr, "Invalid address\n");
        return push(0);
    }

    if(connect(serverSock, (struct sockaddr*) &connAddress, sizeof(connAddress)) < 0)
    {
        fprintf(stderr, "Connect failed\n");
        return push(0);
    }
    free(host);                             // Frees memory allocated in decodeAddress()

    activeSock = serverSock;                // Sets activeSock to the local socket
    return push(1);
}

bool netin()                                // Reads a character from active connection, puts it on stack
{
    pop();                                  // Pops netref, unused in this implementation
    char buf;
    if(read(activeSock, &buf, 1) < 0)
    {
        fprintf(stderr, "Read failed\n");
        return false;
    }
    fprintf(stderr, "IN: %c\n", buf);
    return push((word_t) buf);
}

bool netout()                               // Writes a charcater from stack
{
    pop();                                  // Pops netref, unused in this implementation
    char toOutput = pop();
    if(write(activeSock, &toOutput, 1) < 0)
    {
        fprintf(stderr, "Write failed\n");
        return false;
    }
    fprintf(stderr, "OUT: %c\n", toOutput);
    return true;
}

bool netclose()                             // Closes the activeSock set in netbind/netconnect
{
    shutdown(activeSock, 2);
    return true;
}
