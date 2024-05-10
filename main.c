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

    // struct system syst;
    //  for (int q = 0; q < 1000; q++)
    //  {
    //      printf("MAIN %d\n", q);
    //  int fp = 100;
    //  syst = system_init();
    //  for (int i = fp, l = 0; i < syst.procs.pids_count && l < 10; i++)
    //  {
    //      // printf("%d Im %d\n", syst.procs.pids[i], i);
    //      struct process pr = process_init(syst.procs.pids[i]);
    //      if (pr.command)
    //      {
    //          char *t = format_time(pr.time);
    //          printf("\n%d\t%s\t%s\t", pr.pid, pr.user, t);
    //          printf("%s\t%f\t%ld\t", pr.command, pr.cpu_use, pr.ram);
    //          free(t);
    //          l++;
    //          // for (int j = 0; j < pr.tids.pids_count; j++)
    //          //     printf("%d ", pr.tids.pids[j]);
    //          // printf("\n");
    //          for (int k = 0; k < pr.tids.pids_count; k++)
    //          {
    //              struct thread th = thread_init(pr.tids.pids[k], pr.pid);
    //              char *t = format_time(pr.time);
    //              printf("\n%d\t%s\t%s\t", th.pid, th.user, t);
    //              printf("%s\t%f\t%ld\t", th.command, th.cpu_use, th.ram);
    //              free(t);
    //              thread_free(th);
    //              l++;
    //          }
    //      }
    //      process_free(pr);
    //  }
    //  system_free(syst);
    //  }
    display();

    // int height = 30;
    // int first = 10;
    // struct system sys;
    // sys = system_init();
    // for (int i = first, l = 0; i < sys.procs.pids_count && l < 10; i++)
    // {
    //     struct process pr = process_init(sys.procs.pids[i]);
    //     while (!pr.command)
    //     {
    //         pr = process_init(sys.procs.pids[++i]);
    //     }

    //     // if (!pr.command)
    //     //     continue;
    //     char *t = format_time(pr.time);
    //     printf("%d\t%s\t%s\t", pr.pid, pr.user, t);
    //     printf("%s\t%f\t%ld\n", pr.command, pr.cpu_use, pr.ram);
    //     free(t);
    //     l++;
    //     //  for (int k = 0; k < pr.tids.pids_count; k++)
    //     //  {
    //     //      struct thread th = thread_init(pr.tids.pids[k], pr.pid);
    //     //      char *t = format_time(pr.time);
    //     //      printf("\n%d\t%s\t%s\t", th.pid, th.user, t);
    //     //      printf("%s\t%f\t%ld\t", th.command, th.cpu_use, th.ram);
    //     //      free(t);
    //     //      thread_free(th);
    //     //  }
    //     // process_free(pr);
    // }
    // system_free(sys);
    return 0;
}
