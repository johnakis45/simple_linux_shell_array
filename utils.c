/*
# Author: Ioannis Davanos
# Academic ID: 4622
# E-mail: csd4622@csd.uoc.gr
*/

#include "utils.h"

// function that removes the quotes from the input(in the beginning and in the end)
char *removeQuotes(char *input)
{
    if (input[0] == '"')
    {
        input++;
    }
    if (input[strlen(input) - 1] == '"')
    {
        input[strlen(input) - 1] = '\0';
    }

    return input;
}

// function that checks if the command is echo,mkdir or cat
int cmd_checker(char *arg)
{
    if (arg == NULL)
    {
        perror("Error , command was null.\n");
        return 0;
    }
    if (strcmp(arg, "echo") == 0 || strcmp(arg, "mkdir") == 0 || strcmp(arg, "cat") == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// function that counts the number of pipes in the input
int pipe_counter(char *str)
{
    int count = 0;

    while (*str)
    {
        if (*str == '|')
        {
            count++;
        }
        str++;
    }

    return count;
}

// function that adds spaces before and after the special characters(<<,<,>,|)
void addSPaces(char *x)
{
    char *output = (char *)malloc(2 * strlen(x) + 1); // Allocate memory for the output string
    int index = 0;
    for (int i = 0; i < strlen(x); i++)
    {
        char current = x[i];
        output[index++] = current;

        if (current == READ || ((index + 1 < strlen(x)) && (++current == WRITE && current == WRITE)) || current == WRITE || current == PIPE)
        {
            // Check if there is a space before the special character
            if (i > 0 && x[i - 1] != SPACE)
            {
                output[index++] = SPACE; // Insert a space before the special character
            }

            // Check if there is a space after the special character
            if (i < strlen(x) - 1 && x[i + 1] != SPACE)
            {
                output[index++] = SPACE; // Insert a space after the special character
            }
        }
    }
    output[index] = '\0'; // Null-terminate the output string
    // Copy the modified string back to the input
    strcpy(x, output);
    free(output); // Free the dynamically allocated memory
}

// function that creates the linked list
void initialize(command *c, char **token, int pipes)
{
    int i = 0;     // token index
    int j = 0;     // argument counter
    int l =0;      // commant counter
    int flag = 0;  // check if there is a command like echo,mkdir and cat
    FILE *fp;      // file pointer
    c[0].pipes = pipes; // save the number of pipes
    while (token[i] != NULL  && l < MAX_COMMANDS) // while there are tokens and we haven't reached the end of the input
    {
        if (strcmp(token[i], PIPE_STR) == 0)
        {
            j = 0; //args of next command initizalize from 0
            l++; //go to the next command
        }
        else if (strcmp(token[i], READ_STR) == 0) // check if there is a '<' in the input
        {
            if ((token[i + 1]) == NULL) // check if there is an argument after '<'
            {
                perror("Error on redirection.\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                if (c[l].read > 0) // check if there is already an argument that is the input file
                {
                    c[l].params[c[l].read] = token[i+1]; // save the next argument after '<'
                }
                else
                {
                    c[l].read = j;               // save the index of the argument that is the input file
                    c[l].params[j] = token[++i]; // in the position of '<' save the next argument after '<'
                    j++;                             // go to the next token
                }
            }
        }
        else if (strcmp(token[i], WRITE_STR) == 0) // check if there is a '>' in the input
        {
            if ((token[i + 1]) == NULL) // check if there is an argument after '>'
            {
                perror("Error on redirection.\n");
                exit(1);
            }
            else
            {
                if (c[l].write >= 0) // check if there is already an argument that is the output file
                {
                    c[l].params[c[l].write] = token[i + 1]; // save the next argument after '>'
                }
                else
                {
                    c[l].write = j;                // save the index of the argument that is the output file
                    c[l].params[j] = token[i + 1]; // in the position of '>' save the next argument after '>'
                    j++;                               // go to the next token
                }
            }
            fp = fopen(c[l].params[c[l].write], "w+"); // create the file (if we have a lot of redirections we create every file)
            fclose(fp);                                        // close the file
        }
        else if (strcmp(token[i], APPEND_STR) == 0) // check if there is a '>>' in the input
        {
            if ((token[i + 1]) == NULL) // check if there is an argument after '>>'
            {
                perror("Error on redirection.\n");
                exit(1);
            }
            else
            {
                if (c[l].write >= 0) // check if there is already an argument that is the append file
                {
                    c[l].params[c[l].write] = token[i + 1]; // save the next argument after '>>'
                }
                else
                {
                    c[l].write = j;                // save the index of the argument that is the append file
                    c[l].params[j] = token[i + 1]; // in the position of '>>' save the next argument after '>>'
                    j++;                               // go to the next token
                }
            }
            fp = fopen(c[l].params[c[l].write], "a"); // create the file (if we have a lot of redirections we create every file)
            fclose(fp);                                       // close the file
        }
        else
        {
            if (cmd_checker(token[i])) // check if there is a command like echo,mkdir and cat
            {
                flag = 1;
            }
            if (flag && cmd_checker(token[i]) == 0) // if there is a command like echo,mkdir and cat and the next token is not a command like echo,mkdir and cat then remove the quotes
            {
                token[i] = removeQuotes(token[i]);
                flag = 0;
            }

            //c[l].params[j] = malloc(strlen(token[i]) + 1); // allocate memory for the argument
            c[l].params[j] = token[i];                     // save the argument to the node's parameters
            j++;                                               // increase the argument counter
        }
        i++; // go to the next token
    }
}

void initialize_tokens(char *tokens[MAX_TOKENS]) // Initialize the tokens array
{
    int i = 0;
    while (i != MAX_TOKENS)
    {
        tokens[i] = NULL;
        i++;
    }
}

void tokenize(char *command, char **tokens, char *dm) // Tokenize the input command(command is the input command,tokens is the array that we store the tokens,dm is the delimiter)
{

    // Initialize variables for tokenization
    char *token;
    int index;
    const char *delim = dm;
    index = 0;

    // Tokenize the input command
    token = strtok(command, delim);

    while (token != NULL)
    {
        tokens[index] = token;
        index = index + 1;
        token = strtok(NULL, delim); // Pass NULL to continue tokenization
    }
}

void execute_command(command *x) // Execute the command
{
    int pipeline[x[0].pipes][2]; // Create the pipeline array
    pid_t pid_child;
    int check;
    int commands = x[0].pipes + 1;
    FILE *fp;
    int i;                         // pipe counter
    int l = 0;                      //command counter
    for (i = 0; i < x[0].pipes; i++) // Create the pipes
    {
        pipe(pipeline[i]);
    }
    
    i = 0;
    
    while (l < commands) //while there are commands
    {
        pid_child = fork();  // create a child process
        if (pid_child == -1) // check if the child process was created
        {
            perror("fork");
            exit(1);
        }
        else if (pid_child == 0) // success
        {
            if (x[l].write >= 0) // check if there is an argument that is the output file
            {
                if (x[l].ap == 0) // check if the output file is append or not
                {
                    fp = fopen(x[l].params[x[l].write], "w+");
                }
                else
                {
                    fp = fopen(x[l].params[x[l].write], "a+");
                }
                if (fileno(fp) >= 0) // check if the file was created
                {
                    x[l].params[x[l].write] = NULL; // remove the argument that is the output file
                    check = dup2(fileno(fp), 1);    // redirect the output to the file
                    if (check < 1)
                    {
                        perror("Could not redirect output");
                        exit(EXIT_FAILURE);
                    }
                    fclose(fp); // close the file
                }
                else
                {
                    perror("Could not open file");
                    exit(EXIT_FAILURE);
                }
            }
            else if (x[l].write < 0 && i != commands - 1)
            {
                check = dup2(pipeline[i][1], 1); // write to the next pipe
                if (check < 0)                   // check if the pipe was created
                {
                    perror("Could not redirect output");
                    exit(EXIT_FAILURE);
                }
            }

            if (x[l].read >= 0) // check if there is an argument that is the input file
            {
                fp = fopen(x[l].params[x[l].read], "r");
                if (fileno(fp) >= 0) // check if the file was created
                {
                    x[l].params[x[l].read] = NULL; // remove the argument that is the input file
                    check = dup2(fileno(fp), 0);   // redirect the input to the file
                    if (check < 0)
                    {
                        perror("Could not take input for command");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    perror("Error taking input");
                    exit(EXIT_FAILURE);
                }
            }
            else if (x[l].read < 0 && i != 0) // check if there is a pipe
            {
                check = dup2(pipeline[i - 1][0], 0); // read from the previous pipe
                if (check < 0)
                {
                    perror("Error taking input");
                    exit(EXIT_FAILURE);
                }
            }

            int k;
            for (k = 0; k < x[0].pipes; k++) // close all the pipes
            {
                close(pipeline[k][0]);
                close(pipeline[k][1]);
            }

            check = execvp(x[l].params[0], x[l].params); // execute the command
            if (check < 0)
            {
                perror("Error executing command");
                exit(EXIT_FAILURE);
            }
        }
        l++; // go to the next node
        i++; // increase the pipe counter
    }

    // for the child_process = 0
    // close all the pipes
    for (i = 0; i < commands; i++)
    {
        close(pipeline[i][0]);
        close(pipeline[i][1]);
    }

    // for the parent process
    for (i = 0; i < commands; i++)
    {
        wait(&x[0].pipes);
    }
}

void run_command(command *x) // run the command
{
    if (strcmp(x[0].params[0], "quit") == 0 && x[0].pipes == 0) // check if the command is quit then exit
    {
        exit(0);
    }
    else if (strcmp(x[0].params[0], "cd") == 0 && x[0].pipes == 0) // check if the command is cd then change directory
    {
        chdir(x[0].params[1]); // change directory
    }
    else
    {
        execute_command(x); // execute the command
    }
}

void display_prompt()
{
    char dir[MAX_DIRNAME];
    getcwd(dir, sizeof(dir));
    printf("\033[0;33m");
    printf("csd4622-hy345sh@");
    printf("\033[0;32m");
    printf("%s:%s~$", getenv("USER"), dir);
    printf("\033[0m");
}

void get_command(char *command, size_t size) // gets the input from the user (stdin)
{
    fgets(command, size, stdin);
    command[strcspn(command, "\n")] = '\0';
}

void array_copy(char *copy[MAX_TOKENS], char *src[MAX_TOKENS]) // copy the tokens array to the copy array
{
    int i = 0;
    while (src[i] != NULL)
    {
        copy[i] = src[i];
        i++;
    }
}


void initialize_commands(command commands[MAX_COMMANDS]) // initialize the commands array with ;
{
    int i = 0;
    while (i < MAX_COMMANDS)
    {
        int j=0;
        while (j < MAX_PARAM)
        {
            commands[i].params[j] =NULL;
            j++;
        }
        commands[i].ap = -1;
        commands[i].write = -1;
        commands[i].read = -1;
        commands[i].pipes = -1;
        i++;
    }
}

void execute(char *cmd) // execute the command (main function)
{
    command commands[MAX_COMMANDS];    // commands array with ;
    char *tokens[MAX_TOKENS];       // tokens array with ;
    char *copy[MAX_TOKENS];         // copy of the tokens array
    char *space_tokens[MAX_TOKENS]; // tokens array with spaces
    // initialize the arrays
    initialize_commands(commands);
    initialize_tokens(tokens);
    initialize_tokens(space_tokens);
    initialize_tokens(copy);
    addSPaces(cmd); // add spaces before and after the special characters
    tokenize(cmd, tokens, ";"); // tokenize the input with ';'
    array_copy(copy, tokens);   // copy the tokens array to the copy array
    int i = 0;
    while (tokens[i] != NULL && i<MAX_TOKENS) // while there are tokens
    {                       
        initialize_commands(commands);
        initialize_tokens(space_tokens);              // initialize the space_tokens array
        int pipes = pipe_counter(tokens[i]);          // count the number of pipes
        tokenize(copy[i], space_tokens, " ");         // tokenize the input with ' '
        initialize(commands, space_tokens, pipes);    // initialize the commands array
        run_command(commands); // run the command
        i++; // go to the next token
    }
}

