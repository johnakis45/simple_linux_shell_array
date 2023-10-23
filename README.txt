# Author: Ioannis Davanos
# Academic ID: 4622
# E-mail: csd4622@csd.uoc.gr    

This README file describes the implementation of the c linux shell.

To create the needed files run make all
To run the shell run make run
To delete the created files from make all just run make clean

The shell supports:
	- Simple command execution 
	- Hardcoded commands: "cd" and "quit".
	- Pipelined commands execution.
	- Redirection to and from files.
    - Command with arguments execution.
    - Echo,mkdir and car remove quotes from the argument as the real commands do.

The shell does not support:
    - Using arguments tha have spaces in them. (e.g. echo "hello world" will not work properly, as the quotes will not be removed)

Information:
	- Max supported input is 1024(can be changed from MAX_INPUT) and max arguments of each command are 100(can be changed from MAX_PARAMS).
    - The shell sometimes crashes after some commands are executed. Something happenes to the list and kills malloc() (malloc(): invalid size (unsorted)).


How it works:
    - The shell reads the input from the user and stores it in a buffer.
    - The buffer is then parsed and the commands are stored in a list.
    - Each node has a an array of strings that contains the command and its arguments.
    - The next node in the list is created only in case of a pipe.
    - The node write,ap,read and apread are used to store (the index in the params array) file names for redirection.
    - The list is then executed.
    - The list is then freed.
    - The shell then waits for the next input.

    Tested on Ubuntu 22.04.2 LTS and worked perfectly.



PS I also tested the program on the univeristy computers , but it did not work correctly. 
I thought that were a problem with my list.  
I also implemented the assignment with arrays but it had the same issues.