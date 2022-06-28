#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define DELIM "#"

int get_company_id(char* input);
int get_order_type(char* input);
int get_number_of_stocks(char* input);
float get_price(char* input);
int get_user_id(char* input);