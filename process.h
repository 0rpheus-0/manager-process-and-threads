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
    struct pids tids;
};

float get_cpu_use_proc(int pid)
{
    printf("cpu\n");
    return (float)get_active_jiffies_proc(pid, proc_dir) / (float)get_active_jiffies();
}

struct process process_init(int pid)
{
    char path[256];
    sprintf(path, "%s%d%s", proc_dir, pid, task_dir);
    printf("init %s\n", path);
    struct process proc = {
        .pid = pid,
        .user = get_user(pid, proc_dir),
        .command = get_command(pid, proc_dir)};
    if (proc.command == NULL)
    {
        printf("init NULL\n");
        proc.pid = -1;
        return proc;
    }
    printf("init CMD %s\n", proc.command);
    proc.cpu_use = get_cpu_use_proc(pid);
    printf("init Cpu %f\n", proc.cpu_use);
    proc.ram = get_ram(pid, proc_dir);
    printf("init RAM %ld\n", proc.ram);
    proc.time = get_time(pid, proc_dir);
    printf("init TIMe %ld\n", proc.time);
    proc.tids = get_pids(path);
    // proc = (struct process){
    //     .pid = pid,
    //     .user = get_user(pid, proc_dir),
    //     .command = get_command(pid, proc_dir),
    //     .cpu_use = get_cpu_use_proc(pid),
    //     .ram = get_ram(pid, proc_dir),
    //     .time = get_time(pid, proc_dir),
    //     .tids = get_pids(path)};

    // for (int j = 0; j < proc.tids.pids_count; j++)
    //     printf("%d ", proc.tids.pids[i]);
    // printf("\n");
    return proc;
}
// void process_free(struct process *proc)
// {
//     free(proc)
//     free(proc.command);
// }
