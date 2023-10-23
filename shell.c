/*
# Author: Ioannis Davanos
# Academic ID: 4622
# E-mail: csd4622@csd.uoc.gr
*/

#include "utils.h"

int main()
{
    char command[MAX_INPUT];
    while (1)
    {

        display_prompt();
        get_command(command, sizeof(command));
        execute(command);
    }
    return 0;
}
