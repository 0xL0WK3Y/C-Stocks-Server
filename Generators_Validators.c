int generate_command_id()
{
    int command_id;
    srand(time(0));
    command_id = rand();

    while(!is_id_unique(command_id))
    {
        srand(time(0));
        command_id = rand();
    }

    return command_id;
}

int is_id_unique(int command_id)
{
    int confirmed_orders_num = get_number_of_lines("confirmed_purchases.txt");
    Order* confirmed_orders = calloc(sizeof(Order), confirmed_orders_num);
    confirmed_orders = read_confirmed_purchases();

    for(int i = 0; i < confirmed_orders_num; i++)
    {
        if(confirmed_orders[i].command_id == command_id)
        {
            return 0;
        }
    }

    return 1;
}

int check_ownership(Order user_order)
{
    int confirmed_purchases_number = get_number_of_lines("confirmed_purchases.txt");
    Order* confirmed_orders = calloc(sizeof(Order), confirmed_purchases_number);
    confirmed_orders = read_confirmed_purchases();

    for(int i = 0; i < confirmed_purchases_number; i++)
    {
        if(confirmed_orders[i].user_id == user_order.user_id &&
        confirmed_orders[i].company_id == user_order.company_id &&
        confirmed_orders[i].number_of_stocks >= user_order.number_of_stocks)
        {
            return 1;
        }
    }

    return 0;
}