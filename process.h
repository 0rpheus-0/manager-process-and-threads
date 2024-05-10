#pragma once

#include "parser.h"
#include <stdlib.h>

struct process
{
    int pid;
    char *user;
    float cpu_use;
    long ram;
    long time;
    char *command;
    struct pids tids;
};

char *proc_dir1 = "/proc/";
char *task_dir1 = "/task/";

float get_cpu_use_proc(int pid)
{
    return (float)get_active_jiffies_proc(pid, proc_dir1) / (float)get_active_jiffies();
}

struct process process_init(int pid)
{
    char path[256];
    sprintf(path, "%s%d%s", proc_dir1, pid, task_dir1);
    struct process proc = {
        .pid = pid,
        .user = get_user(pid, proc_dir1),
        .command = get_command(pid, proc_dir1),
        .cpu_use = get_cpu_use_proc(pid),
        .ram = get_ram(pid, proc_dir1),
        .time = get_time(pid, proc_dir1),
        .tids = get_pids(path)};

    return proc;
}
void process_free(struct process proc)
{
    if (proc.user != NULL)
        free(proc.user);
    if (proc.command != NULL)
        free(proc.command);
    if (proc.tids.pids != NULL)
        free(proc.tids.pids);
}
