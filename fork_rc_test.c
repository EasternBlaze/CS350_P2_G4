#include "types.h"
#include "user.h"

/* ATTENTION STUDENTS: to ensure correct compilation of the base code, 
   stub functions for the system call user space wrapper functions are provided. 
   REMEMBER to disable the stub functions (by commenting the following macro) to 
   allow your implementation to work properly. */
/*
define STUB_FUNCS
#ifdef STUB_FUNCS
void fork_winner(int winner) {}
void set_sched(int scheduler) {}
#endif
*/
/* IMPORTANT INSTRUCTION: the test code below should not be changed. 
   Failure to follow this instruction will lead to zero point for this part */

#define TOTAL_TEST_TRIALS 50
#define SCHEDULER_DEFAULT  0
#define SCHEDULER_STRIDE 1

int
main(int argc, char *argv[])
{
    int i = 0;
    int w = 0;
    int ret = 0;

    if (argc < 2)
    {
        printf(1, "Usage: %s 0|1 \n"
                  "\t0: Parent is scheduled to run most often\n"
                  "\t1: Child is scheduled to run most often\n", argv[0]);
        exit();
    }

    if (argv[1][0] == '0')
    {
        w = 0;
        printf(1, "\nSetting parent as the fork winner ...\n");
    }
    else
    {
        w = 1;
        printf(1, "\nSetting child as the fork winner ...\n");
    }

    fork_winner(w); // set according to user input
    set_sched(SCHEDULER_DEFAULT); 
    
    for (i = 0; i < TOTAL_TEST_TRIALS; i++)
    {
        printf(1, "\nTrial %d: ", i);
        ret = fork();
        if (ret < 0)
        {
            printf(1, "fork() failed (%d)\n", ret);
            exit();
        }
        else if (ret == 0) // child
        {
            printf(1, " child! ");
            exit();
        }

        // parent
        printf(1, " parent! ");
        if (ret != wait())
        {
            printf(1, "wait() failed!\n");
        }
    }

    printf(1, "\n");

    fork_winner(0); // set back to default

    exit();
}

