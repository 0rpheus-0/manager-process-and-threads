#pragma once

struct pids
{
    int *pids;
    int pids_count;
};

char *key_value_parser(char *key, char *path);
char *format_time(long time);
char *get_operarting_system();
char *get_kernel();
long *get_cpu_use();
float get_memory_use();
int get_all_process_number();
int get_run_process_number();
long get_up_time();
long get_jiffies();
long get_idle_jiffies();
long get_active_jiffies();
long get_active_jiffies_proc(int pid, char *dir);
float get_use_cpu();
struct pids get_pids(char *dir);
char *get_uid(int pid, char *dir);
char *get_user(int pid, char *dir);
long get_ram(int pid, char *dir);
long get_time(int pid, char *dir);
char *get_command(int pid, char *dir);