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
char *meminfo_file = "/meminfo";
char *version_file = "/version";
char *os_path = "/etc/os-release";
char *passwd_path = "/etc/passwd";

char *key_value_parser(char *key, char *path)
{
    char *value = NULL;
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
            printf("%s\n", value);
            //    printf("str %s\n", str);
            search = 0;
        }
    }
    fclose(file);
    // printf("f %s \n", value);
    return value;
}

char *format_time(long time)
{
    const int HOUR = 3600;
    const int MIN = 60;
    int h = time / HOUR;
    int sec = time % HOUR;
    int min = sec / MIN;
    sec %= MIN;
    char *result = (char *)malloc(9 * sizeof(char));
    snprintf(result, 9, "%02d:%02d:%02d", h, min, sec);
    return result;
}

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
    // char *uid = get_uid(pid); //ивану вопрос
    char *uid_temp = NULL;
    int serch = 1;
    FILE *file = fopen(passwd_path, "r");
    while (!feof(file) && serch)
    {
        char *uid = get_uid(pid);
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        user = strtok(str, ":");
        strtok(NULL, ":");
        uid_temp = strtok(NULL, ":");
        if (strcmp(uid_temp, uid) == 0)
            serch = 0;
    }
    fclose(file);
    return user;
}

char *get_ram(int pid)
{
    char path[256];
    char num[7];
    sprintf(num, "%d", pid);
    strcpy(path, proc_dir);
    strcat(path, num);
    strcat(path, status_file);
    // printf("%s\n", path); // убрать
    return key_value_parser("VmSize:", path);
}

long get_up_time(int pid)
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
    printf("%s", str);
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
    // printf("%s\n", str);
    return command;
}