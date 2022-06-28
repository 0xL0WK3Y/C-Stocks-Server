int get_company_id(char* input)
{
    char* token;
    char* buffer = calloc(sizeof(char), strlen(input));

    strcpy(buffer, input);
    token = strtok(buffer, DELIM); //1#2#2#4

    return atoi(token);
}

int get_order_type(char* input)
{
    char* token;
    char* buffer = calloc(sizeof(char), strlen(input));

    strcpy(buffer, input);
    token = strtok(buffer, DELIM);
    token = strtok(NULL, DELIM);
    
    return atoi(token);
}

int get_number_of_stocks(char* input)
{
    char* token;
    char* buffer = calloc(sizeof(char), strlen(input));

    strcpy(buffer, input);
    token = strtok(buffer, DELIM);
    token = strtok(NULL, DELIM);
    token = strtok(NULL, DELIM);
    
    return atoi(token);   
}

float get_price(char* input)
{
    char* token;
    char* buffer = calloc(sizeof(char), strlen(input));
    float price;

    strcpy(buffer, input);
    token = strtok(buffer, DELIM);
    token = strtok(NULL, DELIM);
    token = strtok(NULL, DELIM);
    token = strtok(NULL, DELIM);
    price = atof(token);
    return price;
}

int get_user_id(char* input)
{
    char* token;
    char* buffer = calloc(sizeof(char), strlen(input));

    strcpy(buffer, input);
    token = strtok(buffer, DELIM);
    token = strtok(NULL, DELIM);
    token = strtok(NULL, DELIM);
    token = strtok(NULL, DELIM);
    token = strtok(NULL, DELIM);
    
    return atoi(token);   
}
