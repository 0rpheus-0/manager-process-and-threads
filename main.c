#include <stdio.h>
#include "process.h"
#include "parser.h"

int main()
{
    // struct pids pids = get_pids();
    // for (int i = 0; i < pids.pids_count; i++)
    //     printf("%d\n", pids.pids[i]);

    // char *a = key_value_parser("Pid:", "/proc/7050/status");
    // printf("%s\n", a);

    // printf("gu %s\n", get_uid(7050));

    // printf("%s\n", get_user(1));

    // printf("gr %s\n", get_ram(4483));

    // printf("gut %ld\n", get_up_time(1));

    // printf("ft %s\n", format_time(get_up_time(1)));

    // printf("gc %s\n", get_command(4483));

    // printf("aboba\n");
    // struct process proc = process_init(1);
    // printf("pid : %d\nuser : %s\ncmd : %s\ncpu : %f\nram : %ld\ntime : %ld\nstate %s\n",
    //        proc.pid, proc.user, proc.command, proc.cpu_use, proc.ram, proc.up_time, proc.state);

    printf("OS : %s\n", get_operaring_system());
    printf("kernel : %s\n", get_kernel());
    // char **b = get_cpu_use();
    // for (int i = 0; i < CPU_USE_COUNT; i++)
    //     printf("%s ", b[i]);

    printf("mem : %f", get_memory_use());

    return 0;
}