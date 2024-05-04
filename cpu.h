#pragma once

#include "parser.h"

struct cpu
{
    float use;
};

float get_use_cpu()
{
    return (float)get_active_jiffies() / (float)get_jiffies();
}

struct cpu cpu_init()
{
    struct cpu cpu = {.use = get_use_cpu()};
}