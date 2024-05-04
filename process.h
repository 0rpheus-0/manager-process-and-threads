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

struct process process_init(int pid)
{
    struct process proc = {
        .pid = pid,
        .user = get_user(pid),
        .command = get_command(pid),
        .cpu_use = 0, // это пока
        .ram = get_ram(pid),
        .time = get_time(pid),
        .state = get_state(pid)};
    return proc;
}