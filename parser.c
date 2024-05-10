#define _GNU_SOURCE
#include "parser.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// char *value_from_file(FILE *file, char *key) {
//     const char *sep = " \t\n";
//     char *value = NULL;
//     char *value_res = NULL;
//     while (!feof(file)) {
//         char line[256] = {0};
//         fgets(line, sizeof(line), file);
//         if (strncmp(key, line, strlen(key)) == 0) {
//             strtok(str, sep);
//             value = strtok(NULL, sep);
//             printf("%s %d\n", value, strlen(value));
//             value_res = (char *)malloc(strlen(value));
//             strcpy(value_res, value);
// 	    break;
//         }
//     }
//     return value_res;
// }

char *proc_dir = "/proc/";
char *task_dir = "/task/";
char *cmdline_file = "/cmdline";
char *cpuinfo_file = "/cpuinfo";
char *status_file = "/status";
char *stat_file = "/stat";
char *uptime_file = "/uptime";
char *os_path = "/etc/os-release";
char *passwd_path = "/etc/passwd";
char *version_file = "/version";
char *meminfo_file = "/meminfo";

char *key_value_parser(char *key, char *path)
{
    char *value = NULL;
    char *value_res = NULL;
    char *sep = " \t\n";
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        // printf("Error open %s  : %s\n", path, strerror(errno));
        return NULL;
    }
    while (!feof(file))
    {
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        if (strncmp(key, str, strlen(key)) == 0)
        {
            strtok(str, sep);
            value = strtok(NULL, sep);
            // printf("%s %d\n", value, strlen(value));
            value_res = (char *)malloc(strlen(value));
            strcpy(value_res, value);
            break;
        }
    }
    fclose(file);
    return value_res;
}

char *format_time(long time)
{
    const int HOUR = 3600;
    const int MIN = 60;
    int h = time / HOUR;
    int sec = time % HOUR;
    int min = sec / MIN;
    sec %= MIN;
    char *result = (char *)malloc(9);
    snprintf(result, 9, "%02d:%02d:%02d", h, min, sec);
    return result;
}
//------------------------------------------------------------

char *get_operarting_system()
{
    char *value = NULL;
    char *value_res = NULL;
    FILE *file = fopen(os_path, "r");
    if (file == NULL)
    {
        // printf("Error open %s  : %s\n", os_path, strerror(errno));
        return NULL;
    }
    while (!feof(file))
    {
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        if (strncmp("PRETTY_NAME", str, 11) == 0)
        {
            strtok(str, "=");
            value = strtok(NULL, "\n");
            value_res = (char *)malloc(strlen(value) + 1);
            strcpy(value_res, value);
            break;
        }
    }
    fclose(file);
    return value_res;
}

char *get_kernel()
{
    char *kernel = NULL;
    char path[256];
    sprintf(path, "%s%s", proc_dir, version_file);
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        // printf("Error open %s  : %s\n", path, strerror(errno));
        return NULL;
    }
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    strtok(str, " ");
    strtok(NULL, " ");
    kernel = strtok(NULL, " ");
    char *kernel_res = (char *)malloc(strlen(kernel) + 1);
    strcpy(kernel_res, kernel);
    fclose(file);
    return kernel_res;
}

#define CPU_USE_COUNT 10

long *get_cpu_use()
{
    char *cpu = NULL;
    long *cpu_res = (long *)malloc(CPU_USE_COUNT * sizeof(long));
    char path[256];
    sprintf(path, "%s%s", proc_dir, stat_file);
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        // printf("Error open %s  : %s\n", path, strerror(errno));
        return NULL;
    }
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    strtok(str, " ");
    for (int i = 0; i < CPU_USE_COUNT; i++)
    {
        cpu = strtok(NULL, " ");
        cpu_res[i] = strtol(cpu, NULL, 10);
    }
    fclose(file);
    return cpu_res;
}

float get_memory_use()
{
    char *mem_str = NULL;
    float *mem = (float *)malloc(2 * sizeof(float));
    char path[256];
    sprintf(path, "%s%s", proc_dir, meminfo_file);
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        // printf("Error open %s  : %s\n", path, strerror(errno));
        return 0.0;
    }
    for (int i = 0; i < 2; i++)
    {
        // printf("mem\n");
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        strtok(str, " \t");
        mem_str = strtok(NULL, " \t");
        mem[i] = atof(mem_str);
    }
    fclose(file);
    float mem_total = mem[0];
    float mem_free = mem[1];
    free(mem);
    return (mem_total - mem_free) / mem_total;
}

int get_all_process_number()
{
    //("all_process\n");
    char path[256];
    sprintf(path, "%s%s", proc_dir, stat_file);
    char *proc_str = key_value_parser("processes", path);
    int proc = atoi(proc_str);
    free(proc_str);
    return proc;
}

int get_run_process_number()
{
    // printf("run_process\n");
    char path[256];
    sprintf(path, "%s%s", proc_dir, stat_file);
    char *proc_str = key_value_parser("procs_running", path);
    int proc = atoi(proc_str);
    free(proc_str);
    return proc;
}

long get_up_time()
{
    char path[256];
    sprintf(path, "%s%s", proc_dir, uptime_file);
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        // printf("Error open %s  : %s\n", path, strerror(errno));
        return 0;
    }
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    char *time = strtok(str, ".");
    fclose(file);
    return strtol(time, NULL, 10);
}

//------------------------------------------------------------
long get_jiffies()
{
    long *jiffies = get_cpu_use();
    long jiffies_n = 0;
    for (int i = 0; i < CPU_USE_COUNT; i++)
        jiffies_n += jiffies[i];
    free(jiffies);
    return jiffies_n;
}

long get_idle_jiffies()
{
    long *jiffies = get_cpu_use();
    long jiffies_wait = jiffies[3];
    long jiffies_idle = jiffies[4];
    free(jiffies);
    return jiffies_wait + jiffies_idle;
}

long get_active_jiffies()
{
    return get_jiffies() - get_idle_jiffies();
}

long get_active_jiffies_proc(int pid, char *dir)
{
    char path[256];
    sprintf(path, "%s%d%s", dir, pid, stat_file);
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        // printf("Error open %s  : %s\n", path, strerror(errno));
        return 0;
    }
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    strtok(str, " ");
    for (int i = 0; i < 13; i++)
        strtok(NULL, " ");
    char *utime = strtok(NULL, " ");
    char *stime = strtok(NULL, " ");
    fclose(file);
    return strtol(utime, NULL, 10) + strtol(stime, NULL, 10);
}

float get_use_cpu()
{
    return (float)get_active_jiffies() / (float)get_jiffies();
}

//------------------------------------------------------------

struct pids get_pids(char *dir)
{
    struct pids pid_s =
        {.pids = NULL,
         .pids_count = 0};
    DIR *direct = opendir(dir);
    struct dirent **entries;
    int count = scandir(dir, &entries, NULL, NULL);
    if (count < 0)
        ;
    // printf("Error scan dir %S\n", dir);
    for (int i = 0; i < count; i++)
    {
        int pid = atoi(entries[i]->d_name);
        if (pid != 0)
        {
            pid_s.pids = realloc(pid_s.pids, sizeof(int) * ++pid_s.pids_count);
            pid_s.pids[pid_s.pids_count - 1] = pid;
        }
        free(entries[i]);
    }
    free(entries);
    closedir(direct);
    return pid_s;
}

char *get_uid(int pid, char *dir)
{
    char path[256];
    sprintf(path, "%s%d%s", dir, pid, status_file);
    return key_value_parser("Uid:", path);
}

char *get_user(int pid, char *dir)
{
    char *user = NULL;
    char *user_res = NULL;
    char *uid = get_uid(pid, dir);
    char *uid_temp = NULL;
    FILE *file = fopen(passwd_path, "r");
    if (file == NULL)
    {
        printf("Error open %s  : %s\n", passwd_path, strerror(errno));
        return NULL;
    }
    if (!uid)
    {
        // printf("user null\n");
        return NULL;
    }
    while (!feof(file))
    {
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        user = strtok(str, ":");
        strtok(NULL, ":");
        uid_temp = strtok(NULL, ":");
        if (strcmp(uid_temp, uid) == 0)
        {
            user_res = (char *)malloc(strlen(user));
            strcpy(user_res, user);
            break;
        }
    }
    free(uid);
    fclose(file);
    return user_res;
}

long get_ram(int pid, char *dir)
{
    char path[256];
    sprintf(path, "%s%d%s", dir, pid, status_file);
    char *ram_str = key_value_parser("VmSize:", path);
    if (ram_str == NULL)
        return 0;
    long ram = strtol(ram_str, NULL, 10) / 1000;
    free(ram_str);
    return ram;
}

long get_time(int pid, char *dir)
{
    char path[256];
    sprintf(path, "%s%d%s", dir, pid, stat_file);
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        // printf("Error open %s  : %s\n", path, strerror(errno));
        return 0;
    }
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    strtok(str, " ");
    for (int i = 0; i < 12; i++)
        strtok(NULL, " ");
    char *utime_str = strtok(NULL, " ");
    long utime = strtol(utime_str, NULL, 10);
    char *stime_str = strtok(NULL, " ");
    long stime = strtol(stime_str, NULL, 10);
    fclose(file);
    return (utime + stime) / 100;
}

char *get_command(int pid, char *dir)
{
    char path[256];
    sprintf(path, "%s%d%s", dir, pid, cmdline_file);
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        // printf("Error open %s  : %s\n", path, strerror(errno));
        return NULL;
    }
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    char *command = strtok(str, " \n");
    fclose(file);
    if (command == NULL)
        return NULL;
    char *command_res = (char *)malloc(strlen(command) + 1);
    strcpy(command_res, command);
    return command_res;
}
