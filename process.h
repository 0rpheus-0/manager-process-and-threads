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
};

float get_cpu_use_proc(int pid)
{
    return get_active_jiffies_proc(pid) / get_active_jiffies();
}

struct process process_init(int pid)
{
    struct process proc = {
        .pid = pid,
        .user = get_user(pid),
        .command = get_command(pid),
        .cpu_use = get_cpu_use_proc(pid),
        .ram = get_ram(pid),
        .time = get_time(pid),
        .state = get_state(pid)};
    return proc;
}
