#include "client.h"

/************************************************************************
 * MAIN
 ************************************************************************/
int main(void)
{
	// Setup Variables
	int client_socket; // socket for communication
    struct addrinfo hints; // instructions for getaddrinfo()
    struct addrinfo *server_info; // info about daytime server
    char time_string[80]; // "storage" for daytime server response (i.e the echo)
    int bytes_received = 0; // how much data has been received
    int result; // what we actually get from the server call

    printf("Daytime client\n");

    // Set up getaddrinfo hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Resolve server address
    result = getaddrinfo(SERVER_ADDR, PORT, &hints, &server_info);

    if (result != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        exit(EXIT_FAILURE);
    }

    // Create socket
    client_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    if (client_socket == -1)
    {
        perror("Error creating socket");
        freeaddrinfo(server_info);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_socket, server_info->ai_addr, server_info->ai_addrlen) == -1)
    {
        perror("Error connecting to server");
        freeaddrinfo(server_info);
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // We no longer need the address information
    freeaddrinfo(server_info);

    // Receive up to 79 characters
    while (bytes_received < sizeof(time_string) - 1)
    {
        int bytes_read;

        bytes_read = read(client_socket, time_string + bytes_received, sizeof(time_string) - 1 - bytes_received);

        if (bytes_read <= 0)
        {
            break;
        }

        bytes_received += bytes_read;
    }

    // Null terminate the string
    time_string[bytes_received] = '\0';

    // Print the time
    printf("%s", time_string);

    // Close socket
    close(client_socket);

    printf("\nDone!\n");

    return EXIT_SUCCESS;
}

