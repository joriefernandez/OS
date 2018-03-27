#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>     // support for the exec routines.
#include <string.h>
#include <sys/wait.h>

#define MAXSTR 255
#define MAXARGS 5       // Max amount of arguments including command itself
#define MAXINPUTS 4     // max amount of user inputs

int main(int argc, char *argv[]) {

    char mash1[MAXSTR];
    char mash2[MAXSTR];
    char mash3[MAXSTR];
    char file[MAXSTR];

    printf("mash-1>");
    fgets(mash1, MAXSTR, stdin);
    
    printf("mash-2>");
    fgets(mash2, MAXSTR, stdin);
    
    printf("mash-3>");
    fgets(mash3, MAXSTR, stdin);

    printf("file>");
    fgets(file, MAXSTR, stdin);
    
    // printf("%lu\n", strcspn(mash1, "\n"));
    // fgets() function reads the whole line including whitespaces and new line character "\n"
    // to remove the new line charater we are using strcspn() function to find the new line
    // character and replace it with NULL terminated character
    mash1[strcspn(mash1, "\n")] = 0;    // now mash1 does not contain new line character "\n"
    mash2[strcspn(mash2, "\n")] = 0;
    mash3[strcspn(mash3, "\n")] = 0;
    file[strcspn(file, "\n")] = 0;
    
    // printf("%s, %s, %s, %s\n", mash1, mash2, mash3, file);

    char **inputs = malloc(sizeof(char *) * MAXARGS);   // string array of inputs {mash1, mash2, mash3, file}
    *(inputs + 0) = mash1;
    *(inputs + 1) = mash2;
    *(inputs + 2) = mash3;
    *(inputs + 3) = file;

    int pids[3];
    
    int i;
    // MAXINPUTS - 1: do not include file
    for (i=0; i < (MAXINPUTS - 1); i++) {
        int rc = fork();

        if (rc < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        // child process
        else if (rc == 0) {
            char **args = malloc(sizeof(char *) * MAXARGS);
            int count = 0;                                      // count number of individual words choped from input
            
            char *word = strtok(inputs[i], " ");                // choped individual word from input
            while (word != NULL) {
                char *s = malloc(sizeof(char) * strlen(word));  // allocate memory space for choped word
                strcpy(s, word);                                //make a copy of choped word
                *(args + count++) = s;                          // put choped word to arguments array of strings
                word = strtok(NULL, " ");
            }
            *(args + count++) = file;
            execvp(args[0], args);
            // printf("STATUS CODE=%d\n", status);
        }
        // parent process
        else {
            int wc = wait(NULL);
            pids[i] = wc;
            // printf("Done waiting on %d\n", wc);
        }        
    }

    printf("Done waiting on children: %d %d %d\n", pids[0], pids[1], pids[2]);

    return 0;
}