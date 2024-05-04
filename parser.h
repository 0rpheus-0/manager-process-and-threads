#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

char *proc_dir = "/proc/";
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
    int search = 1;
    FILE *file = fopen(path, "r");
    // printf("aboba kv %s\n", path); // убрать
    if (file == NULL)
    {
        printf("Error open %s", path);
    }
    while (!feof(file) && search)
    {
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        // printf("before if\n");
        if (strncmp(key, str, strlen(key)) == 0)
        {
            // printf("in if\n");
            strtok(str, " \t\n");
            value = strtok(NULL, " \t\n");
            // printf("%s\n", value);
            value_res = (char *)malloc(strlen(value));
            strcpy(value_res, value);
            //    printf("str %s\n", str);
            search = 0;
        }
    }
    fclose(file);
    // printf("f %s \n", value);
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

char *get_operaring_system()
{
    char *value = NULL;
    char *value_res = NULL;
    int search = 1;
    FILE *file = fopen(os_path, "r");
    while (!feof(file) && search)
    {
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        if (strncmp("PRETTY_NAME", str, 11) == 0)
        {
            strtok(str, "=");
            value = strtok(NULL, "\n");
            value_res = (char *)malloc(strlen(value));
            strcpy(value_res, value);
            search = 0;
        }
    }
    fclose(file);
    return value_res;
}

char *get_kernel()
{
    char *kernel = NULL;
    char *kernel_res = NULL;
    char path[256];
    strcpy(path, proc_dir);
    strcat(path, version_file);
    FILE *file = fopen(path, "r");
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    strtok(str, " ");
    strtok(NULL, " ");
    kernel = strtok(NULL, " ");
    kernel_res = (char *)malloc(strlen(kernel));
    strcpy(kernel_res, kernel);
    fclose(file);
    return kernel_res;
}

#define CPU_USE_COUNT 10

char **get_cpu_use()
{
    char **cpu = (char **)malloc(CPU_USE_COUNT * sizeof(char *));
    char **cpu_res = (char **)malloc(CPU_USE_COUNT * sizeof(char *));
    char path[256];
    strcpy(path, proc_dir);
    strcat(path, stat_file);
    FILE *file = fopen(path, "r");
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    strtok(str, " ");
    for (int i = 0; i < CPU_USE_COUNT; i++)
    {
        cpu[i] = strtok(NULL, " ");
        cpu_res[i] = (char *)malloc(strlen(cpu));
        strcpy(cpu_res[i], cpu[i]);
    }
    fclose(file);
    return cpu_res;
}

float get_memory_use()
{
    char **mem_strs = (char **)malloc(2 * sizeof(char *));
    char **mem_strs_res = (char **)malloc(2 * sizeof(char *));
    char path[256];
    strcpy(path, proc_dir);
    strcat(path, meminfo_file);
    FILE *file = fopen(path, "r");
    for (int i = 0; i < 2; i++)
    {
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        strtok(str, " \t");
        mem_strs[i] = strtok(NULL, " \t");
        printf("%s\n", mem_strs[i]);
        mem_strs_res[i] = (char *)malloc(strlen(mem_strs));
        strcpy(mem_strs_res[i], mem_strs[i]);
    }
    fclose(file);
    float mem_total = atof(mem_strs_res[0]);
    float mem_free = atof(mem_strs_res[1]);
    return (mem_total - mem_free) / mem_total;
}

//------------------------------------------------------------
struct pids
{
    int *pids;
    int pids_count;
};

struct pids get_pids()
{
    struct pids pid_s =
        {.pids = NULL,
         .pids_count = 0};
    DIR *dir = opendir(proc_dir);
    struct dirent **entries;
    int count = scandir(proc_dir, &entries, NULL, NULL);
    for (int i = 0; i < count; i++)
    {
        struct dirent *entry = entries[i];
        free(entries[i]);
        int pid = atoi(entry->d_name);
        if (pid != 0)
        {
            pid_s.pids = realloc(pid_s.pids, sizeof(int) * ++pid_s.pids_count);
            pid_s.pids[pid_s.pids_count - 1] = pid;
        }
    }
    return pid_s;
}

char *get_uid(int pid)
{
    char path[256];
    char num[7];
    sprintf(num, "%d", pid);
    strcpy(path, proc_dir);
    strcat(path, num);
    strcat(path, status_file);
    // printf("%s\n", path); // убрать
    return key_value_parser("Uid:", path);
}

char *get_user(int pid)
{
    char *user = NULL;
    char *user_res = NULL;
    // char *uid = get_uid(pid); //ивану вопрос
    char *uid_temp = NULL;
    int serch = 1;
    FILE *file = fopen(passwd_path, "r");
    while (!feof(file) && serch)
    {
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        user = strtok(str, ":");
        strtok(NULL, ":");
        uid_temp = strtok(NULL, ":");
        char *uid = get_uid(pid);
        // printf("%s\t%s\t%s\n", user, uid_temp, uid);
        if (strcmp(uid_temp, uid) == 0)
        {
            user_res = (char *)malloc(strlen(user));
            strcpy(user_res, user);
            serch = 0;
        }
    }
    fclose(file);
    return user_res;
}

long get_ram(int pid)
{
    char path[256];
    char num[7];
    sprintf(num, "%d", pid);
    strcpy(path, proc_dir);
    strcat(path, num);
    strcat(path, status_file);
    // printf("%s\n", path); // убрать
    char *ram_str = key_value_parser("VmSize:", path);
    long ram = strtol(ram_str, NULL, 10) / 1000;
    return ram;
}

long get_time(int pid)
{
    // long ticks = 0;
    char path[256];
    char num[7];
    sprintf(num, "%d", pid);
    strcpy(path, proc_dir);
    strcat(path, num);
    strcat(path, stat_file);
    FILE *file = fopen(path, "r");
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    // printf("%s", str);
    strtok(str, " ");
    for (int i = 0; i < 21; i++)
        strtok(NULL, " ");
    char *ticks_str = strtok(NULL, " ");
    fclose(file);
    long ticks = strtol(ticks_str, NULL, 10);
    return ticks / sysconf(_SC_CLK_TCK);
}

char *get_command(int pid)
{
    // char *command = NULL;
    char path[256];
    char num[7];
    sprintf(num, "%d", pid);
    strcpy(path, proc_dir);
    strcat(path, num);
    strcat(path, cmdline_file);
    FILE *file = fopen(path, "r");
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    char *command = strtok(str, " \n");
    fclose(file);
    char *command_res = (char *)malloc(strlen(command));
    strcpy(command_res, command);
    // printf("%s\n", str);
    // printf("cmd %s\n", command);
    return command_res;
}

char *get_state(int pid)
{
    char path[256];
    char num[7];
    sprintf(num, "%d", pid);
    strcpy(path, proc_dir);
    strcat(path, num);
    strcat(path, status_file);
    // printf("%s\n", path); // убрать
    return key_value_parser("State:", path);
}