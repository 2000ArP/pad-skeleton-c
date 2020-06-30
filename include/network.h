#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ijvm.h"

int socket_init();                          // Initializes socket descriptor with IPv4, tcp and internet protocol

bool netbind();                             // Listens for a connection on the given port

char *decodeAddress(word_t hostEncoded);    // Decodes an ipv4 address in word form to a string (e.g. from 0x7F000001 to "127.0.0.1")

bool netconnect();                          // Connects to the given address and port

bool netin();                               // Gets character from the active socket

bool netout();                              // Sends character on the active socket

bool netclose();                            // Shuts down the active socket
