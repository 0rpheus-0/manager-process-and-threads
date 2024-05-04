#pragma once

#include "parser.h"

struct process
{
    int pid;
    char *user;
    float cpu_use;
    long ram;
    long time;
    char *command;
    char *state;
    struct pids tids;
};

float get_cpu_use_proc(int pid)
{
    return (float)get_active_jiffies_proc(pid, proc_dir) / (float)get_active_jiffies();
}

struct process process_init(int pid)
{
    char path[256];
    char num[7];
    sprintf(num, "%d", pid);
    strcpy(path, proc_dir);
    strcat(path, num);
    strcat(path, task_dir);
    struct process proc = {
        .pid = pid,
        .user = get_user(pid, proc_dir),
        .command = get_command(pid, proc_dir),
        .cpu_use = get_cpu_use_proc(pid),
        .ram = get_ram(pid, proc_dir),
        .time = get_time(pid, proc_dir),
        .state = get_state(pid, proc_dir),
        .tids = get_pids(path)};

    return proc;
}
