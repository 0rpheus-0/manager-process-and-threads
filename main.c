#include <stdio.h>
#include "process.h"
#include "parser.h"

int main()
{
    // struct pids pids = get_pids();
    // for (int i = 0; i < pids.pids_count; i++)
    //     printf("%d\n", pids.pids[i]);

    // char *a = key_value_parser("Pid:", "/proc/4483/status");
    // printf("%s\n", a);

    // printf("gu %s\n", get_uid(4483));
    //  printf("%s\n", i111));

    // printf("%s\n", get_user(4483));

    printf("gr %s\n", get_ram(4483));

    // printf("gut %ld\n", get_up_time(1));

    // printf("ft %s\n", format_time(get_up_time(1)));

    // printf("gc %s\n", get_command(4483));
    return 0;
}