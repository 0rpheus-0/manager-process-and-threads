#include <stdio.h>
#include "process.h"
#include "parser.h"
#include "thread.h"
#include "system.h"
#include "display.h"

int main()
{

    struct system syst = system_init();
    for (int i = 0; i < 200; i++)
    {
        printf("%d\n", syst.procs.pids[i]);
        struct process pr = process_init(syst.procs.pids[i]);
        // if (pr.pid != -1)
        //{
        printf("\n%d ", pr.pid);
        printf("%s ", pr.user);
        printf("%s ", format_time(pr.time));
        printf("%s %f %ld\n", pr.command, pr.cpu_use, pr.ram);
        for (int j = 0; j < pr.tids.pids_count; j++)
            printf("%d ", pr.tids.pids[j]);
        printf("\n\n");
        //} // free(pr);
    }
    // display(syst);
    return 0;
}