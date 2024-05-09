#pragma once

#include "parser.h"
#include <string.h>

struct thread
{
    int pid;
    char *user;
    float cpu_use;
    long ram;
    long time;
    char *command;
};

float get_cpu_use_thr(int tid, char *dir)
{
    return (float)get_active_jiffies_proc(tid, dir) / (float)get_active_jiffies();
}

struct thread thread_init(int tid, int pid)
{
    char path[256];
    sprintf(path, "%s%d%s", proc_dir1, pid, task_dir1);
    struct thread thr = {
        .pid = tid,
        .user = get_user(tid, path),
        .command = get_command(tid, path),
        .cpu_use = get_cpu_use_thr(tid, path),
        .ram = get_ram(tid, path),
        .time = get_time(tid, path)};
    return thr;
}
void thread_free(struct thread th)
{
    if (th.user != NULL)
        free(th.user);
    if (th.command != NULL)
        free(th.command);
}