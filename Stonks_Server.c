#include "Stonks_Server.h"
#include "Order_Getters.h"
#include "File_Library.h"
#include "Generators_Validators.h"
#include "Order_Getters.c"
#include "File_Library.c"
#include "Generators_Validators.c"

//Note: In order for all the commands to properly work, the files need to be filled with information.

int priority = 0;

int main(int argc, char** argv)
{
    int sockfd, clientfd;
    socklen_t len;
    pthread_t tid;

    struct sockaddr_in client;
    struct sockaddr_in server;

    char input[1024];

    if(argc != 2)
    {
        fprintf(stderr, "[!] Usage: <./Stonks_Server.out> <port_number>");
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
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*) &server, sizeof(server)) != 0)
    {
        fprintf(stderr, "[!] Failure binding socket.\n");
        exit(EXIT_FAILURE);
    }

    if(listen(sockfd, 40) != 0)
    {
        fprintf(stderr, "[!] Error listening!\n");
        exit(EXIT_FAILURE);
    }
    
    fprintf(stdout, "[+] Listening for connections...\n");

    while(1)
    {
        clientfd = accept(sockfd, (struct sockaddr*)&client, &len);

        if(!clientfd)
        {
            fprintf(stderr, "[+] Error accepting the client!\n");
            exit(EXIT_FAILURE);
        }
        while(clientfd <= 0)
        {
            clientfd = accept(sockfd, (struct sockaddr*)&client, &len);
        }
        bzero(input, 1024);
        strcpy(input, "Welcome to Stonks!\n");
        strcat(input, "Usage: <usage>\n");
        strcat(input, "Example: <example>\n");
        send(clientfd, input, 1024, 0);
        pthread_create(&tid, NULL, manage_user_commands, (void*)(intptr_t)clientfd);
    }
}

void* manage_user_commands(void* clientfd)
{
    char input[1024];
    int create_thread_flag = 1;
    intptr_t client_d = (intptr_t) clientfd;

    while(1)
    {
        pthread_t sid;
        
        // Recv
        bzero(input, 1024);
        recv(client_d, input, 1024, 0);

        int company_id = get_company_id(input);
        int order_type = get_order_type(input);
        int number_of_stocks = get_number_of_stocks(input);
        float price = get_price(input);
        int user_id = get_user_id(input);
        int command_id = generate_command_id();
        int command_id_converted = htonl(command_id);

        fprintf(stdout, "Command Id: %d\n", command_id);
        send(client_d, &command_id_converted, 1024, 0);

        if(order_type == 1 || order_type == 2)
        {
            
            FILE *sales_file;
            Order user_order;
           
            if ((sales_file = fopen("sales.txt", "r")) == NULL){  
                fprintf(stdout, "[!] Error! Could not open the file!\n");
                exit(-1);
            }
           
            user_order.company_id = company_id;
            user_order.order_type = order_type;
            user_order.number_of_stocks = number_of_stocks;
            user_order.price = price;
            user_order.user_id = user_id;
            user_order.command_id = command_id;

            if(check_ownership(user_order))
            {

                sales_file = fopen("sales.txt", "a");
                fprintf(sales_file, "%d#", company_id);
                fprintf(sales_file, "%d#", order_type);
                fprintf(sales_file, "%d#", number_of_stocks);
                fprintf(sales_file, "%.2f#", price);
                fprintf(sales_file, "%d#", user_id);
                fprintf(sales_file, "%d\n", command_id);
            }
            else
            {
                fprintf(stdout,"The stock isn't owned!\n");
            }
            
            fflush(sales_file);
            fclose(sales_file);

        }
        else if(order_type == 3 || order_type == 4)
        {
            FILE *purchase_file;

            if ((purchase_file = fopen("purchases.txt", "r")) == NULL){   
                fprintf(stdout, "Error! Could not open the file!\n");
                exit(-1);
            }


            purchase_file = fopen("purchases.txt", "a");
            fprintf(purchase_file, "%d#", company_id);
            fprintf(purchase_file, "%d#", order_type);
            fprintf(purchase_file, "%d#", number_of_stocks);
            fprintf(purchase_file, "%.2f#", price);
            fprintf(purchase_file, "%d#", user_id);
            fprintf(purchase_file, "%d\n", command_id);

            fflush(purchase_file);
            fclose(purchase_file);
        }
        else if(order_type == 5)
        {
            fprintf(stdout, "Removing sale order < %d >...\n", company_id);
            cancel_sale_order(company_id);
        }
        else if(order_type == 6)
        {
            fprintf(stdout, "Removing purchase order < %d >...\n", company_id);
            cancel_purchase_order(company_id);
        }
        else
        {
            fprintf(stdout, "Invalid Order!\n");
        }
        
        if(create_thread_flag != 0)
        {
            pthread_create(&sid, NULL, purchase_on_price, (void*)(intptr_t)user_id);
            pthread_create(&sid, NULL, purchase_on_percentage, (void*)(intptr_t)user_id);
            pthread_create(&sid, NULL, sell_on_price, (void*)(intptr_t)user_id);
            pthread_create(&sid, NULL, sell_on_percentage, (void*)(intptr_t)user_id);
            create_thread_flag = 0;
        }
        

    } 
}

int get_number_of_user_purchases(int user_id)
{
    int number_of_user_purchases = 0;
    int purchases_line_count = get_number_of_lines("purchases.txt");
    Order* all_purchases_list = calloc(sizeof(Order), purchases_line_count);
    all_purchases_list = read_purchases();

    for(int i=0; i < purchases_line_count; i++)
    {
        if(all_purchases_list[i].user_id == user_id)
        {
            number_of_user_purchases++;
        }
    }

    return number_of_user_purchases;
}

int get_number_of_user_sales(int user_id)
{
    int number_of_user_sales = 0;
    int sales_line_count = get_number_of_lines("sales.txt");
    Order* all_sales_list = calloc(sizeof(Order), sales_line_count);
    all_sales_list = read_sales();

    for(int i = 0; i < sales_line_count; i++)
    {
        if(all_sales_list[i].user_id == user_id)
        {
            number_of_user_sales++;
        }
    }

    return number_of_user_sales;
}

Order* get_user_purchase_orders(int user_id)
{
    int j = 0;
    Order* user_purchases_list;
    int number_of_user_purchases = get_number_of_user_purchases(user_id);
    int purchases_line_count = get_number_of_lines("purchases.txt");
    Order* all_purchases_list = calloc(sizeof(Order), purchases_line_count);
    all_purchases_list = read_purchases();
    user_purchases_list = calloc(sizeof(Order), number_of_user_purchases);
    
    for(int i = 0; i < purchases_line_count; i++)
    {
        if(all_purchases_list[i].user_id == user_id)
        {
            user_purchases_list[j] = all_purchases_list[i];
            j++;
        }
    }

    return user_purchases_list;
    
}

Order* get_user_sale_orders(int user_id)
{
    int j = 0;
    Order* user_sales_list;
    int number_of_user_sales = get_number_of_user_sales(user_id);
    int sales_line_count = get_number_of_lines("sales.txt");
    Order* all_sales_list = calloc(sizeof(Order), sales_line_count);
    all_sales_list = read_sales();
    user_sales_list = calloc(sizeof(Order), number_of_user_sales);
    
    for(int i = 0; i < sales_line_count; i++)
    {
        if(all_sales_list[i].user_id == user_id)
        {
            user_sales_list[j] = all_sales_list[i];
            j++;
        }
    }

    return user_sales_list;
}

void* purchase_on_price(void* user_id)
{
    pthread_mutex_t lock;
    intptr_t user_id_casted = (intptr_t) user_id;
    int* pr;
    pr = &priority;

    while(1)
    {
	if(priority == 0)
	{
        int number_of_sales = get_number_of_lines("sales.txt");
        int number_of_purchases = get_number_of_user_purchases(user_id_casted);
        Order* available_sales = calloc(sizeof(Order), number_of_sales);
        Order* user_purchases_list = calloc(sizeof(Order), number_of_purchases);
        user_purchases_list = get_user_purchase_orders(user_id_casted);
        available_sales = read_sales();
        

        for(int i = 0; i < number_of_purchases; i++)
        {
            for(int j = 0; j < number_of_sales; j++)
            {
                if(user_purchases_list[i].price == available_sales[j].price && 
                user_purchases_list[i].company_id == available_sales[j].company_id && 
                user_purchases_list[i].order_type == 3)
                {
                    pthread_mutex_lock(&lock);
                    confirm_purchase(user_id_casted, i);
                    pthread_mutex_unlock(&lock);
                }
            }
        }
  *pr = 1; 
    }
   } 

}

void* purchase_on_percentage(void* user_id)
{
    pthread_mutex_t lock;
    intptr_t user_id_casted = (intptr_t) user_id;
	int* pr;
    pr = &priority;
    while(1)
    {
	if(priority == 1)
	{
        int number_of_sales = get_number_of_lines("sales.txt");
        int number_of_purchases = get_number_of_user_purchases(user_id_casted);
        Order* available_sales = calloc(sizeof(Order), number_of_sales);
        Order* user_purchases_list = calloc(sizeof(Order), number_of_purchases);
        user_purchases_list = get_user_purchase_orders(user_id_casted);
        available_sales = read_sales();
        
        for(int i = 0; i < number_of_purchases; i++)
        {
            for(int j = 0; j < number_of_sales; j++)
            {
                int percentage = user_purchases_list[i].price - (user_purchases_list[i].price * 0.05);

                if(available_sales[j].price <= percentage && 
                user_purchases_list[i].company_id == available_sales[j].company_id && 
                user_purchases_list[i].order_type == 4)
                {
                    pthread_mutex_lock(&lock);
                    confirm_purchase(user_id_casted, i);
                    pthread_mutex_unlock(&lock);
                }
            }
        }
*pr = 2;
}
    }
}

void confirm_purchase(int user_id, int p_index)
{
    int sales_num = get_number_of_lines("sales.txt");
    int number_of_purchases = get_number_of_user_purchases(user_id);
    Order* user_purchases_list = calloc(sizeof(Order), number_of_purchases);
    Order* sales_list = calloc(sizeof(Order), sales_num);
    user_purchases_list = get_user_purchase_orders(user_id);
    sales_list = read_sales();
        
    FILE* confirmed_p_file;
            
    if ((confirmed_p_file = fopen("confirmed_purchases.txt", "r")) == NULL)
    {  
        fprintf(stdout, "Error! Could not open the file!\n");
        exit(-1);
    }

    confirmed_p_file = fopen("confirmed_purchases.txt", "a");
    fprintf(confirmed_p_file, "%d#", user_purchases_list[p_index].company_id);
    fprintf(confirmed_p_file, "%d#", user_purchases_list[p_index].order_type);
    fprintf(confirmed_p_file, "%d#", user_purchases_list[p_index].number_of_stocks);
    fprintf(confirmed_p_file, "%.2f#", user_purchases_list[p_index].price);
    fprintf(confirmed_p_file, "%d#", user_purchases_list[p_index].user_id);
    fprintf(confirmed_p_file, "%d\n", user_purchases_list[p_index].command_id);

    for(int i = 0; i < sales_num; i++)
    {
        if(user_purchases_list[p_index].company_id == sales_list[i].company_id &&
        user_purchases_list[p_index].number_of_stocks == sales_list[i].number_of_stocks)
        {
            delete_sales(i);
            delete_purchase_order(p_index);
        }
        else if(user_purchases_list[p_index].company_id == sales_list[i].company_id &&
        user_purchases_list[p_index].number_of_stocks != sales_list[i].number_of_stocks)
        {
            delete_sales(i);
            delete_purchase_order(p_index);
            //stocks
            FILE* sales_file = fopen("sales.txt", "a");
            fprintf(sales_file, "%d#", sales_list[i].company_id);
            fprintf(sales_file, "%d#", sales_list[i].order_type);
            fprintf(sales_file, "%d#", sales_list[i].number_of_stocks - user_purchases_list[p_index].number_of_stocks);
            fprintf(sales_file, "%.2f#", sales_list[i].price);
            fprintf(sales_file, "%d#", sales_list[i].user_id);
            fprintf(sales_file, "%d\n", sales_list[i].command_id);
        }
    }

    fflush(confirmed_p_file);
    fclose(confirmed_p_file);
}

void* sell_on_price(void* user_id)
{
    intptr_t user_id_casted = (intptr_t) user_id;
    int* pr;
    pr = &priority;
    while(1)
    {
	if(priority == 2)
	{
        int confirmed_purchases_number = get_number_of_lines("confirmed_purchases.txt");
        int sales_number = get_number_of_user_sales(user_id_casted);
        Order* original_orders = calloc(sizeof(Order), confirmed_purchases_number);
        Order* user_sales_list = calloc(sizeof(Order), sales_number);
        original_orders = read_confirmed_purchases();
        user_sales_list = get_user_sale_orders(user_id_casted);
	
        for(int i = 0; i < sales_number; i++)
        {
            for(int j = 0; j < confirmed_purchases_number; j++)
            {
                
                if(user_sales_list[i].price == original_orders[j].price &&
                user_sales_list[i].company_id == original_orders[j].company_id &&
                user_sales_list[i].order_type == 1)
                {
                    for(int k = 0; k < confirmed_purchases_number; k++)
                    {
                        if(original_orders[k].user_id == user_id_casted)
                        {
                            delete_purchases_line(k);
                        }
                    }
                }
            }
        }
*pr= 3;
}

    }   
}

void* sell_on_percentage(void* user_id)
{
    intptr_t user_id_casted = (intptr_t) user_id;
    int* pr;
    pr = &priority;
    while(1)
    {
	if(priority == 3)
	{
        int confirmed_purchases_number = get_number_of_lines("confirmed_purchases.txt");
        int sales_number = get_number_of_user_sales(user_id_casted);
        Order* original_orders = calloc(sizeof(Order), confirmed_purchases_number);
        Order* user_sales_list = calloc(sizeof(Order), sales_number);
        original_orders = read_confirmed_purchases();
        user_sales_list = get_user_sale_orders(user_id_casted);


        for(int i = 0; i < sales_number; i++)
        {
            for(int j = 0; j < confirmed_purchases_number; j++)
            {

                int percentage = user_sales_list[i].price - (user_sales_list[i].price * 0.05);
                if(original_orders[j].price <= percentage &&
                user_sales_list[i].company_id == original_orders[j].company_id &&
                user_sales_list[i].order_type == 2)
                {
                    for(int k = 0; k < confirmed_purchases_number; k++)
                    {
                        
                        if(original_orders[k].user_id == user_id_casted)
                        {
                            delete_purchases_line(k);
                        }
                    }
                }
            }
        }
	*pr = 0;
}
    }
}