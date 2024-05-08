#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

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

char *value_from_file(FILE *file, char *key) {
    const char *sep = " \t\n";
    char *value = NULL;
    char *value_res = NULL;
    while (!feof(file)) {
        char line[256] = {0};
        fgets(line, sizeof(line), file);
        if (strncmp(key, line, strlen(key)) == 0) {
            strtok(str, sep);
            value = strtok(NULL, sep);
            printf("%s %d\n", value, strlen(value));
            value_res = (char *)malloc(strlen(value));
            strcpy(value_res, value);
	    break;
        }
    }
    return value_res;
}

char *key_value_parser(char *key, char *path)
{
    printf("kv\n");
    char *value = NULL;
    char *value_res = NULL;
    const char *sep = " \t\n";
    FILE *file = fopen(path, "r");
    if (file == NULL)
        printf("Error open %s", path);
    while (!feof(file))
    {
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        if (strncmp(key, str, strlen(key)) == 0)
        {
            strtok(str, sep);
            value = strtok(NULL, sep);
            printf("%s %d\n", value, strlen(value));
            value_res = (char *)malloc(strlen(value));
            strcpy(value_res, value);
	    break;
        }
    }
    printf("kv %s\n", value_res);
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
    char path[256];
    sprintf(path, "%s%s", proc_dir, version_file);
    FILE *file = fopen(path, "r");
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    strtok(str, " ");
    strtok(NULL, " ");
    kernel = strtok(NULL, " ");
    char *kernel_res = (char *)malloc(strlen(kernel));
    strcpy(kernel_res, kernel);
    fclose(file);
    return kernel_res;
}

#define CPU_USE_COUNT 10

long *get_cpu_use()
{
    // printf("gcu 0 \n");
    //  char **cpu = (char **)malloc(CPU_USE_COUNT * sizeof(char *));
    //  //char **cpu_res = (char **)malloc(CPU_USE_COUNT * sizeof(char *));
    char *cpu = NULL;
    long *cpu_res = (long *)malloc(CPU_USE_COUNT * sizeof(long));
    char path[256];
    sprintf(path, "%s%s", proc_dir, stat_file);
    FILE *file = fopen(path, "r");
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    strtok(str, " ");
    for (int i = 0; i < CPU_USE_COUNT; i++)
    {
        // printf("gcu lo \n");
        cpu = strtok(NULL, " ");
        cpu_res[i] = strtol(cpu, NULL, 10);
    }
    fclose(file);
    // printf("gcu ");
    // for (int i = 0; i < CPU_USE_COUNT; i++)
    // {
    //     printf("%ld ", cpu_res[i]);
    // }
    // printf("\n");
    return cpu_res;
}

float get_memory_use()
{
    printf("mem\n");
    char **mem_strs = (char **)malloc(2 * sizeof(char *));
    // char **mem_strs_res = (char **)malloc(2 * sizeof(char *));
    char path[256];
    sprintf(path, "%s%s", proc_dir, meminfo_file);
    FILE *file = fopen(path, "r");
    for (int i = 0; i < 2; i++)
    {
        // printf("mem\n");
        char str[256] = {0};
        fgets(str, sizeof(str), file);
        strtok(str, " \t");
        mem_strs[i] = strtok(NULL, " \t");
        // printf("%s\n", mem_strs[i]);
    }
    fclose(file);
    float mem_total = atof(mem_strs[0]);
    float mem_free = atof(mem_strs[1]);
    // for (int i = 0; i < 2; i++)
    //     free(mem_strs_res[i]);
    // free(mem_strs);
    // free(mem_strs_res);
    return (mem_total - mem_free) / mem_total;
}

int get_all_process_number()
{
    char path[256];
    sprintf(path, "%s%s", proc_dir, stat_file);
    return atoi(key_value_parser("processes", path));
}

int get_run_process_number()
{
    char path[256];
    sprintf(path, "%s%s", proc_dir, stat_file);
    return atoi(key_value_parser("procs_running", path));
}

long get_up_time()
{
    char path[256];
    sprintf(path, "%s%s", proc_dir, uptime_file);
    FILE *file = fopen(path, "r");
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    char *time = strtok(str, ".");
    fclose(file);
    return strtol(time, NULL, 10);
}

//------------------------------------------------------------
long get_jiffies()
{
    // printf("gj\n");
    long *jiffies = get_cpu_use();
    long jiffies_n = 0;
    for (int i = 0; i < CPU_USE_COUNT; i++)
        jiffies_n += jiffies[i];
    free(jiffies);
    return jiffies_n;
}

long get_idle_jiffies()
{
    // printf("gij\n");
    long *jiffies = get_cpu_use();
    return jiffies[3] + jiffies[4];
}

long get_active_jiffies()
{
    // printf("gaj\n");
    return get_jiffies() - get_idle_jiffies();
}

long get_active_jiffies_proc(int pid, char *dir)
{
    // printf("gajp\n");
    char path[256];
    sprintf(path, "%s%d%s", dir, pid, stat_file);
    FILE *file = fopen(path, "r");
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
struct pids
{
    int *pids;
    int pids_count;
};

struct pids get_pids(char *dir)
{
    struct pids pid_s =
        {.pids = NULL,
         .pids_count = 0};
    printf("pids %s cou %d\n", dir, pid_s.pids_count);
    struct dirent **entries;
    int count = scandir(dir, &entries, NULL, NULL);
    if (count < 0)
        printf("Error scan dir\n");
    // printf("pids cou %d\n", count);
    for (int i = 0; i < count; i++)
    {
        printf("N %s ", entries[i]->d_name);
        int pid = atoi(entries[i]->d_name);
        if (pid != 0)
        {
            // printf("PID : %d\n", pid);
            pid_s.pids = realloc(pid_s.pids, sizeof(int) * ++pid_s.pids_count);
            pid_s.pids[pid_s.pids_count - 1] = pid;
            // printf("Coun %d\n", pid_s.pids_count);
        }
        free(entries[i]);
    }
    // printf("\n in f ");
    // for (int i = 0; i < pid_s.pids_count; i++)
    //     printf("%d ", pid_s.pids[i]);
    // printf("\n");
    free(entries);

    // DIR *directory = opendir(dir);
    // struct dirent *entry;
    // while ((entry = readdir(directory)) != NULL)
    // {
    //     int pid = atoi(entry->d_name);
    //     if (pid != 0)
    //     {
    //         // printf("PID : %d\n", pid);
    //         pid_s.pids = realloc(pid_s.pids, sizeof(int) * ++pid_s.pids_count);
    //         pid_s.pids[pid_s.pids_count - 1] = pid;
    //     }
    // }
    // free(entry);
    // closedir(directory);
    return pid_s;
}

char *get_uid(int pid, char *dir)
{
    char path[256];
    sprintf(path, "%s%d%s", dir, pid, status_file);
    // printf("%s\n", path); // убрать
    return key_value_parser("Uid:", path);
}

char *get_user(int pid, char *dir)
{
    // printf("user\n");
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
        char *uid = get_uid(pid, dir);
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

long get_ram(int pid, char *dir)
{
    printf("ram\n");
    char path[256];
    sprintf(path, "%s%d%s", dir, pid, status_file);
    printf("ram %s\n", path); // убрать
    char *ram_str = key_value_parser("VmSize:", path);
    printf("ram %s\n", ram_str);
    if (ram_str == NULL)
        return 0;
    long ram = strtol(ram_str, NULL, 10) / 1000;
    // free(ram_str);
    printf("ram e\n");
    return ram;
}

long get_time(int pid, char *dir)
{
    // printf("time\n");
    //   long ticks = 0;
    char path[256];
    sprintf(path, "%s%d%s", dir, pid, stat_file);
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
    // free(ticks_str);
    return ticks / sysconf(_SC_CLK_TCK);
}

char *get_command(int pid, char *dir)
{
    // printf("cmd\n");
    //  char *command = NULL;
    char path[256];
    sprintf(path, "%s%d%s", dir, pid, cmdline_file);
    FILE *file = fopen(path, "r");
    char str[256] = {0};
    fgets(str, sizeof(str), file);
    char *command = strtok(str, " \n");
    fclose(file);
    // printf("cmd 2\n");
    // printf("cmd %s\n", command);
    if (command == NULL)
        return NULL;
    char *command_res = (char *)malloc(strlen(command));
    strcpy(command_res, command);
    return command_res;
}
