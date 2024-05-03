#pragma once

#include "parser.h"

struct process
{
    int pid;
    char *user;
    char *command;
    float cpu_use;
    char *ram;
    long up_time;
};

// float get_cpu_use(int pit) {}

char *get_ram_mb(int pid)
{
    int kbytes = atoi(get_ram(pid));
}