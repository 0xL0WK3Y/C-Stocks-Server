#include "Stonks_Client.h"


int main(int argc, char* argv[])
{
    int sockfd;

    struct sockaddr_in server;

    char input_buffer[1024];
    char output_buffer[1024];

    char* input;
    char* encoded_input;

    if(argc != 2)
    {
        fprintf(stderr, "[!] Usage: <./Stonks_Client.out> <port_number>");
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        fprintf(stderr, "[!] Failure creating the socket.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "[+] Socket creation was successful!\n");

    bzero(&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(atoi(argv[1]));

    if(connect(sockfd, (struct sockaddr*)&server, sizeof(server)) != 0)
    {
        fprintf(stderr, "[!] Failure connecting to the server!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "[+] Connected succesfully!\n");

    bzero(input_buffer, 1024);
    recv(sockfd, input_buffer, 1024, 0);
    fprintf(stdout, "%s", input_buffer);

    while(1)
    {
        fprintf(stdout, "STONKS> ");
        input = get_input();
        encoded_input = encode_input(&input);
        bzero(output_buffer, 1024);
        strcpy(output_buffer, encoded_input);
        fprintf(stdout,"[+] Order submitted: %s\n",encoded_input);
        send(sockfd, output_buffer, 1024, 0);

        if(!strcmp(output_buffer, "exit"))
        {
            fprintf(stdout, "[+] Exiting...");
            break;
        }

        int id = 0;
        recv(sockfd, &id, 1024, 0);
        fprintf(stdout,"The ID for the order submitted is: %d\n", ntohl(id));
    }

    close(sockfd);
}

char* get_input()
{
    int buffer_size = 1024;
    int position = 0;
    char* buffer = malloc(sizeof(char) * buffer_size);
    int chr;

    if(!buffer)
    {
        fprintf(stderr, "[!] Error allocating memory!");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        chr = getchar();

        if(chr == EOF || chr == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = chr;
        }

        position++;

        if(position >= buffer_size)
        {
            buffer_size += 1024;
            buffer = realloc(buffer, buffer_size);
            if(!buffer)
            {
                fprintf(stderr, "[!] Error allocating memory!\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    return buffer;
}

char* encode_input(char** input_buffer)
{
    char* token;
    char* encoded_input = calloc(sizeof(char), strlen(*input_buffer));
    char* temp_input = calloc(sizeof(char), strlen(*input_buffer));

    if(!strcmp(*input_buffer, "exit"))
    {
        strcpy(encoded_input, "exit");
        return encoded_input;
    }

    strcpy(temp_input, *input_buffer);

    // Company id
    token = strtok(temp_input, " ");
    strcpy(encoded_input, token);
    strcat(encoded_input,DELIM);

    // Command type
    token = strtok(NULL, " ");
    strcat(encoded_input, token);
    strcat(encoded_input, DELIM);
    
    // Number of stocks
    token = strtok(NULL, " ");
    strcat(encoded_input, token);
    strcat(encoded_input, DELIM);

    // Price
    token = strtok(NULL, " ");
    strcat(encoded_input, token);
    strcat(encoded_input, DELIM);

    // User Id
    token = strtok(NULL, " ");
    strcat(encoded_input, token);

    return encoded_input;
}