#pragma once

#include "parser.h"

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
    char num[7];
    sprintf(num, "%d", pid);
    strcpy(path, proc_dir);
    strcat(path, num);
    strcat(path, task_dir);
    struct thread thr = {
        .pid = tid,
        .user = get_user(tid, path),
        .command = get_command(tid, path),
        .cpu_use = get_cpu_use_thr(tid, path),
        .ram = get_ram(tid, path),
        .time = get_time(tid, path)};
    return thr;
}
