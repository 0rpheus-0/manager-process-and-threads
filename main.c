#include <stdio.h>
#include "process.h"
#include "parser.h"
#include "thread.h"
#include "system.h"
#include "display.h"

int main()
{

    // char *a = get_command(2, "/proc/");
    // printf("%s\n", a);
    // free(a);
    // char *a = get_kernel();
    // printf("%s\n", a);
    // long *a = get_cpu_use();
    // for (int i = 0; i < 10; i++)
    //     printf("%ld ", a[i]);
    // free(a);

    // long a = get_time(1, "/proc/");
    // printf("%ld\n", a);

    struct system syst = system_init();
    // for (int k = 0; k < 100; k++)
    // {
    //     for (int i = 0; i < syst.procs.pids_count; i++)
    //     {
    //         printf("%d Im %d\n", syst.procs.pids[i], i);
    //         struct process pr = process_init(syst.procs.pids[i]);
    //         // if (pr.pid != -1)
    //         // {
    //         char *t = format_time(pr.time);
    //         printf("\n%d\t%s\t%s\t", pr.pid, pr.user, t);
    //         printf("%s\t%f\t%ld\t", pr.command, pr.cpu_use, pr.ram);
    //         free(t);
    //         for (int j = 0; j < pr.tids.pids_count; j++)
    //             printf("%d ", pr.tids.pids[j]);
    //         printf("\n\n");
    //         //}
    //         process_free(pr);
    //     }
    // }
    display(syst);
    system_free(syst);

    return 0;
}