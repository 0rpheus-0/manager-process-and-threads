#pragma once

#include "process.h"
#include "parser.h"
#include "thread.h"
#include <stdlib.h>

struct system
{
    char *os;
    char *kernel;
    float cpu;
    float mem;
    long uptime;
    int all_proc;
    int run_proc;
    struct pids procs;
};

struct system system_init()
{
    struct system sys =
        {
            .os = get_operarting_system(),
            .kernel = get_kernel(),
            .cpu = get_use_cpu(),
            .mem = get_memory_use(),
            .uptime = get_up_time(),
            .all_proc = get_all_process_number(),
            .run_proc = get_run_process_number(),
            .procs = get_pids(proc_dir1)};

    return sys;
}
void system_free(struct system sys)
{
    free(sys.os);
    free(sys.kernel);
    free(sys.procs.pids);
}