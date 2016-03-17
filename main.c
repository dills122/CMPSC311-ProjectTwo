#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MSGLEN  64
#define MESSES  3
#define ARR 12

int main(int argc, char *argv[]){
    int     fd[2];
    pid_t   pid;
    int     result;

    int proceID;

    int num[ARR] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    //Needs to be used in order for the intervals to be determined for the splitting of the file/array
    int jump = ARR/MESSES;
    //used to indicate to a specific process what its start and end is in the shared array
    int start, end;

    //Creating a pipe
    result = pipe (fd);
    if (result < 0) {
        //failure in creating a pipe
        perror("pipe error\n");
        exit (1);
    }

    //Creating a child process
    for(int i = 0; i < MESSES; i++){

    /* sets the proceID equal to the i in the loop
        this helps to distinguish the individual processes
    */
    proceID = i;

    //checks if it is the first child process
    if (proceID == 0) {
        start = 0;
        end = jump;

    }
    // sets all of the next processes start and end points
    else {
        start = end;
        end += jump;
    }

	if ((pid = fork()) < 0) {
         //failure in creating a child
         perror ("fork error\n");
         exit(2);
        }
	if(pid == 0)
	   break;
    }

    if (pid == 0) {
        // ACTUALLY CHILD PROCESS
         char message[MSGLEN];

         for (int i = 0; i < MESSES; i++) {

            if (proceID == i) {

                //where all of the children to the mini word counts

                printf("The process ID: %d\n", proceID);
                printf("The start is %d and end is %d\n", start, end);

                for (int k = start; k < end; k++) {
                    printf("The number is %d in process %d\n", num[k], proceID);
                }

            }

         }

            //Clearing the message
            memset (message, 0, sizeof(message));

            //fgets (message, 1024, stdin);
            close(fd[0]);

            //Writing message to the pipe
            write(fd[1], message, strlen(message));

            close(fd[1]);
            close(fd[0]);

            exit (0);
    }
    else {
        //Parent Process

        char message[MSGLEN];
        char *ptr;
        long wc;
        close(fd[1]);

         while (1) {
                    //Clearing the message buffer
            memset (message, 0, sizeof(message));

                    //Reading message from the pipe

            if(read(fd[0], message, sizeof(message)) == 0)
                exit(0);
                printf("Message entered %s\n",message);
                /*
                Message entered needs to be in the format of number first space then string for it to work
                */

                wc = 0;
                wc = strtol(message, &ptr, 10);
                printf("The number(unsigned long integer) is %ld\n", wc);
                printf("String part is %s", ptr);
            }
	    close(fd[0]);
	    wait(NULL);
        exit(0);
     }
     return 0;
}
