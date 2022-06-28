Order* read_sales()
{
	
    FILE *sales_file;
    int line_count = get_number_of_lines("sales.txt");
    size_t line_buffer_length = 0;
    ssize_t line_len;
    char* line_buffer = NULL;
    char* tok;

    if ((sales_file = fopen("sales.txt", "r")) == NULL){   
        fprintf(stdout, "Error! Could not open the file!\n");
        exit(-1);
    }

    Order* order_list = calloc(sizeof(Order) * line_count, 1);
    
    line_len = getline(&line_buffer, &line_buffer_length, sales_file);

    int i = 0;
    while (line_len >= 0)
    {
        line_count = get_number_of_lines("sales.txt");
        
        tok = strtok(line_buffer, DELIM);
        if(tok != NULL)
        {
            order_list[i].company_id = atoi(tok);
            tok = strtok(NULL, DELIM);
        }
        if (tok != NULL)
        {
            order_list[i].order_type = atoi(tok);
            tok = strtok(NULL, DELIM);
        }
         if (tok != NULL)
        {
            order_list[i].number_of_stocks = atoi(tok);
            tok = strtok(NULL, DELIM);
        }
         if (tok != NULL)
        {
            order_list[i].price = atof(tok); 
            tok = strtok(NULL, DELIM);
        }
        if (tok != NULL)
        {
            order_list[i].user_id = atoi(tok); 
            tok = strtok(NULL, DELIM);
        }
        i++;

        free(line_buffer);
        line_buffer = NULL;
        line_len = getline(&line_buffer, &line_buffer_length, sales_file);
    }

    free(line_buffer);
    line_buffer = NULL;
    
    fflush(sales_file);
    fclose(sales_file);
    return order_list;
}

Order* read_purchases()
{

    FILE *purchase_file;
    int line_count = get_number_of_lines("purchases.txt");
    size_t line_buffer_length = 0;
    ssize_t line_len;
    char* line_buffer = NULL;
    char *tok;

    if ((purchase_file = fopen("purchases.txt", "r")) == NULL){   
        fprintf(stdout, "Error! Could not open the file!\n");
        exit(-1);
    }

    Order* order_list = calloc(sizeof(Order) * line_count, 1);
    
    line_len = getline(&line_buffer, &line_buffer_length, purchase_file);

    int i = 0;
    while (line_len >= 0)
    {
        
        tok = strtok(line_buffer, DELIM);
        if(tok != NULL)
        {
            order_list[i].company_id = atoi(tok);
            tok = strtok(NULL, DELIM);
        }
        if (tok != NULL)
        {
            order_list[i].order_type = atoi(tok);
            tok = strtok(NULL, DELIM);
        }
         if (tok != NULL)
        {
            order_list[i].number_of_stocks = atoi(tok);
            tok = strtok(NULL, DELIM);
        }
         if (tok != NULL)
        {
            order_list[i].price = atof(tok); 
            tok = strtok(NULL, DELIM);
        }
        if (tok != NULL)
        {
            order_list[i].user_id = atoi(tok); 
            tok = strtok(NULL, DELIM);
        }
        i++;

        free(line_buffer);
        line_buffer = NULL;
        line_len = getline(&line_buffer, &line_buffer_length, purchase_file);
    }

    free(line_buffer);
    line_buffer = NULL;
    
    fflush(purchase_file);
    fclose(purchase_file);
    return order_list;
}

Order* read_confirmed_purchases()
{
    FILE *purchase_file;
    int line_count = get_number_of_lines("confirmed_purchases.txt");
    size_t line_buffer_length = 0;
    ssize_t line_len;
    char* line_buffer = NULL;
    char *tok;

    if ((purchase_file = fopen("confirmed_purchases.txt", "r")) == NULL){   
        fprintf(stdout, "Error! Could not open the file!\n");
        exit(-1);
    }

    Order* order_list = calloc(sizeof(Order) * line_count, 1);
    
    line_len = getline(&line_buffer, &line_buffer_length, purchase_file);

    int i = 0;
    while (line_len >= 0)
    {
        
        tok = strtok(line_buffer, DELIM);
        if(tok != NULL)
        {
            order_list[i].company_id = atoi(tok);
            tok = strtok(NULL, DELIM);
        }
        if (tok != NULL)
        {
            order_list[i].order_type = atoi(tok);
            tok = strtok(NULL, DELIM);
        }
         if (tok != NULL)
        {
            order_list[i].number_of_stocks = atoi(tok);
            tok = strtok(NULL, DELIM);
        }
         if (tok != NULL)
        {
            order_list[i].price = atof(tok); 
            tok = strtok(NULL, DELIM);
        }
        if (tok != NULL)
        {
            order_list[i].user_id = atoi(tok); 
            tok = strtok(NULL, DELIM);
        }

        i++;

        free(line_buffer);
        line_buffer = NULL;
        line_len = getline(&line_buffer, &line_buffer_length, purchase_file);
    }

    free(line_buffer);
    line_buffer = NULL;
    
    fflush(purchase_file);
    fclose(purchase_file);
    return order_list;
}


void delete_purchases_line(int line)
{
    int index = 0;
    FILE* confirmed_file;
    int array_size = get_number_of_lines("confirmed_purchases.txt");
    Order* confirm_purchases_array = read_confirmed_purchases();
    Order* new_confirmed_array = calloc(array_size - 1, sizeof(Order));

    for(int i = 0; i < array_size; i++)
    {
        if(i != line)
        {
            new_confirmed_array[index] = confirm_purchases_array[i];
            index++;
        }
    }

    if((confirmed_file = fopen("confirmed_purchases.txt", "r")) == NULL)
    {
        fprintf(stderr, "[!] Error opening 'confirmed_purchases.txt'!\n");
        exit(-1);
    }

    fclose(confirmed_file);

    for(int j = 0; j < array_size-1;j++)
    {
        confirmed_file = fopen("confirmed_purchases.txt", "w");
        fprintf(confirmed_file, "%d#", new_confirmed_array[j].company_id);
        fprintf(confirmed_file, "%d#", new_confirmed_array[j].order_type);
        fprintf(confirmed_file, "%d#", new_confirmed_array[j].number_of_stocks);
        fprintf(confirmed_file, "%.2f#", new_confirmed_array[j].price);
        fprintf(confirmed_file, "%d\n", new_confirmed_array[j].user_id);

        fflush(confirmed_file);
        fclose(confirmed_file);

    }

}

int get_number_of_lines(char* filename)
{
    char* line_buffer = NULL;
    size_t line_buffer_length = 0;
    int line_count = 0;
    ssize_t line_len;

    FILE *fl;
    
    if((fl = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "[!] File could not be opened!");
        exit(1);
    }

    line_len = getline(&line_buffer, &line_buffer_length, fl);

    while(line_len >= 0)
    {
        line_count++;
        line_len = getline(&line_buffer, &line_buffer_length, fl);

    }

    free(line_buffer);
    line_buffer = NULL;
    fflush(fl);
    fclose(fl);
        
    return line_count;
}

void delete_sales(int line)
{
    int index = 0;
    FILE* sales_file;
    int array_size = get_number_of_lines("sales.txt");
    Order* sales_array = read_sales(line);
    Order* new_sales_array = calloc(array_size-1, sizeof(Order));

    for(int i =0; i < array_size; i++)
    {
        if(i != line)
        {
            new_sales_array[index] = sales_array[i];
            index++;
        }
    }

    if((sales_file = fopen("sales.txt", "r")) == NULL)
    {
        fprintf(stderr, "[!] Error opening 'sales.txt'!\n");
        exit(1);
    }

    fclose(sales_file);

    for(int j = 0; j < array_size-1;j++)
    {
        sales_file = fopen("sales.txt", "w");
        fprintf(sales_file, "%d#", new_sales_array[j].company_id);
        fprintf(sales_file, "%d#", new_sales_array[j].order_type);
        fprintf(sales_file, "%d#", new_sales_array[j].number_of_stocks);
        fprintf(sales_file, "%.2f#", new_sales_array[j].price);
        fprintf(sales_file, "%d\n", new_sales_array[j].user_id);

        fflush(sales_file);
        fclose(sales_file);

    }
}

void delete_purchase_order(int line)
{
    int index = 0;
    FILE* purchases_file;
    int array_size = get_number_of_lines("purchases.txt");
    Order* purchases_array = read_purchases(line);
    Order* new_purchases_array = calloc(array_size-1, sizeof(Order));

    for(int i =0; i < array_size; i++)
    {
        if(i != line)
        {
            new_purchases_array[index] = purchases_array[i];
            index++;
        }
    }

    if((purchases_file = fopen("purchases.txt", "r")) == NULL)
    {
        fprintf(stderr, "[!] Error opening 'purchases.txt'!\n");
        exit(1);
    }

    fclose(purchases_file);

    for(int j = 0; j < array_size-1;j++)
    {
        purchases_file = fopen("purchases.txt", "w");
        fprintf(purchases_file, "%d#", new_purchases_array[j].company_id);
        fprintf(purchases_file, "%d#", new_purchases_array[j].order_type);
        fprintf(purchases_file, "%d#", new_purchases_array[j].number_of_stocks);
        fprintf(purchases_file, "%.2f#", new_purchases_array[j].price);
        fprintf(purchases_file, "%d\n", new_purchases_array[j].user_id);

        fflush(purchases_file);
        fclose(purchases_file);

    }
}