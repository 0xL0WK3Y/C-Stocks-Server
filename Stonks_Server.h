#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

typedef struct Order{
    int company_id;
    int order_type;
    int number_of_stocks;
    float price;
    int user_id;
    int command_id;
} Order;

void* manage_user_commands(void* clientfd);
void* purchase_on_price(void* user_id);
void confirm_purchase(int user_id, int p_index);
void* purchase_on_percentage(void* user_id);
void* sell_on_price(void* user_id);
void* sell_on_percentage(void* user_id);

Order* get_user_purchase_orders(int user_id);
int get_number_of_user_purchases(int user_id);
int get_number_of_user_sales(int user_id);
int check_ownership(Order user_order);
char* strip_input(char* input);