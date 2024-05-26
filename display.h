#pragma once
#include "system.h"
#include <curses.h>
#include <pthread.h>
#include <unistd.h>

struct system sys;
int width, height;
WINDOW *system_window;
WINDOW *hat_window;
WINDOW *process_window;
int thread_flag = 0;
int first_proc = 0;
pthread_mutex_t mutex_proc;
pthread_mutex_t mutex_sys;
char *spase = "                                                                                                                                                      ";

char *progress_bar(float percent)
{
    int size = 50;
    char *result = (char *)malloc(sizeof(char) * (size + 15));
    float bars = percent * size;
    result[0] = '0';
    result[1] = '%';
    result[2] = ' ';
    result[3] = '[';
    for (int i = 0; i < size; i++)
    {
        result[i + 4] = i <= bars ? '|' : ' ';
    }
    result[size + 3] = ']';
    result[size + 4] = ' ';
    sprintf((result + size + 5), "%f", percent * 100);
    sprintf((result + size + 9), "/100%%");
    return result;
}

void display_system(WINDOW *window)
{
    int field = 2;
    int inform = 11;
    mvwprintw(window, 1, field, "OS :");
    mvwprintw(window, 2, field, "Kernel :");
    mvwprintw(window, 3, field, "CPU: ");
    mvwprintw(window, 4, field, "Memory: ");
    mvwprintw(window, 5, field, "All Processes : ");
    mvwprintw(window, 6, field, "Running Processes : ");
    mvwprintw(window, 7, field, "Up Time: ");
    wattron(window, COLOR_PAIR(2));
    mvwprintw(window, 1, inform, "%s", sys.os);
    mvwprintw(window, 2, inform, "%s", sys.kernel);
    mvwprintw(window, 3, inform, "%s", progress_bar(sys.cpu));
    mvwprintw(window, 4, inform, "%s", progress_bar(sys.mem));
    inform += 12;
    mvwprintw(window, 5, inform, "%d", sys.all_proc);
    mvwprintw(window, 6, inform, "%d", sys.run_proc);
    mvwprintw(window, 7, inform, "%s", format_time(sys.uptime));
    wattroff(window, COLOR_PAIR(2));
    wrefresh(window);
}
void display_hat(WINDOW *window)
{
    werase(window);
    wattron(window, COLOR_PAIR(3));
    int row = 0;
    int width_col = 10;
    int const pid_col = 2;
    int const user_col = pid_col + width_col;
    int const cpu_col = user_col + width_col + 7;
    int const ram_col = cpu_col + width_col;
    int const time_col = ram_col + width_col;
    int const command_col = time_col + width_col;
    mvwprintw(window, row, pid_col, "PID");
    mvwprintw(window, row, user_col, "USER");
    mvwprintw(window, row, cpu_col, "CPU[%%]");
    mvwprintw(window, row, ram_col, "RAM[MB]");
    mvwprintw(window, row, time_col, "TIME+");
    mvwprintw(window, row, command_col, "COMMAND");
    wattroff(window, COLOR_PAIR(3));
    wrefresh(window);
}

void display_process(WINDOW *window)
{
    int width_col = 10;
    int const pid_col = 2;
    int const user_col = pid_col + width_col;
    int const cpu_col = user_col + width_col + 7;
    int const ram_col = cpu_col + width_col;
    int const time_col = ram_col + width_col;
    int const command_col = time_col + width_col;
    pthread_mutex_lock(&mutex_proc);
    for (int i = first_proc, l = 0; i < sys.procs.pids_count - 1 && l < height - 10; i++)
    {
        struct process pr = process_init(sys.procs.pids[i]);
        if (pr.command)
        {
            char *t = format_time(pr.time);
            mvwprintw(window, l, 0, "%s", spase);
            mvwprintw(window, l, pid_col, "%d", pr.pid);
            mvwprintw(window, l, user_col, "%s", pr.user);
            mvwprintw(window, l, cpu_col, "%f", pr.cpu_use);
            mvwprintw(window, l, ram_col, "%ld", pr.ram);
            mvwprintw(window, l, time_col, "%s", t);
            mvwprintw(window, l++, command_col, "%s", pr.command);
            free(t);

            if (thread_flag)
                for (int k = 0; k < pr.tids.pids_count; k++)
                {
                    struct thread th = thread_init(pr.tids.pids[k], pr.pid);
                    char *t = format_time(pr.time);
                    mvwprintw(window, l, 0, "%s", spase);
                    wattron(window, COLOR_PAIR(4));
                    mvwprintw(window, l, pid_col, "%d", th.pid);
                    wattroff(window, COLOR_PAIR(4));
                    mvwprintw(window, l, user_col, "%s", th.user);
                    mvwprintw(window, l, cpu_col, "%f", th.cpu_use);
                    mvwprintw(window, l, ram_col, "%ld", th.ram);
                    mvwprintw(window, l, time_col, "%s", t);
                    wattron(window, COLOR_PAIR(4));
                    mvwprintw(window, l++, command_col + 1, "`~> %s", th.command);
                    wattroff(window, COLOR_PAIR(4));
                    free(t);
                    thread_free(th);
                }
        }
        process_free(pr);
    }
    pthread_mutex_unlock(&mutex_proc);
    wrefresh(window);
}
void update_window()
{
    getmaxyx(stdscr, height, width);

    system_window = newwin(9, width - 1, 0, 0);
    hat_window = newwin(1, width - 1, system_window->_maxy + 1, 0);
    process_window = newwin(height - 10, width - 1, system_window->_maxy + 2, 0);
    scrollok(process_window, TRUE);

    while (1)
    {
        pthread_mutex_lock(&mutex_sys);
        sys = system_init();
        display_system(system_window);
        display_hat(hat_window);
        box(system_window, 0, 0);
        display_process(process_window);
        system_free(sys);
        pthread_mutex_unlock(&mutex_sys);
        usleep(250000);
    }
}

void *button(void *arg)
{
    int ch;
    while ((ch = getch()) != 'q')
    {
        if (ch == 'h')
        {
            if (thread_flag == 0)
                thread_flag = 1;
            else
                thread_flag = 0;
        }
        else if (ch == KEY_DOWN)
        {
            struct process proc;
            pthread_mutex_lock(&mutex_sys);
            pthread_mutex_lock(&mutex_proc);
            while (1)
            {
                first_proc++;
                if (first_proc >= sys.procs.pids_count - 1)
                    first_proc = sys.procs.pids_count - 1;
                proc = process_init(sys.procs.pids[first_proc]);
                if (proc.command)
                {
                    process_free(proc);
                    break;
                }
                process_free(proc);
            }
            pthread_mutex_unlock(&mutex_proc);
            pthread_mutex_unlock(&mutex_sys);
        }
        else if (ch == KEY_UP)
        {
            struct process proc;
            pthread_mutex_lock(&mutex_proc);
            while (1)
            {
                first_proc--;
                if (first_proc <= 0)
                    first_proc = 0;
                proc = process_init(sys.procs.pids[first_proc]);
                if (proc.command)
                {
                    process_free(proc);
                    break;
                }
                process_free(proc);
            }
            pthread_mutex_unlock(&mutex_proc);
        }
    }
    pthread_mutex_destroy(&mutex_proc);
    endwin();
    exit(0);
}

void display()
{
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    keypad(stdscr, TRUE);
    pthread_t but;
    pthread_mutex_init(&mutex_proc, NULL);
    pthread_mutex_init(&mutex_sys, NULL);
    pthread_create(&but, NULL, button, NULL);
    update_window();
    pthread_cancel(but);
    pthread_join(but, NULL);
    pthread_mutex_destroy(&mutex_proc);
    pthread_mutex_destroy(&mutex_sys);
    endwin();
}