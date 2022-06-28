#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define DELIM "#"

Order* read_sales();
Order* read_purchases();
Order* read_confirmed_purchases();
void delete_purchases_line(int line);
int get_number_of_lines(char* filename);
void delete_sales(int index);