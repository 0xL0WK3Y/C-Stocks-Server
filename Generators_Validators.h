#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generate_command_id();
int is_id_unique(int command_id);
int check_ownership(Order user_order);