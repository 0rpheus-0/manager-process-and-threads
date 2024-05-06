#pragma once

#include "system.h"
#include <curses.h>

int width, height;

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

void display_system(struct system sys, WINDOW *window)
{
    // wbkgd(window, COLOR_PAIR(1));
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
    mvwprintw(window, 3, inform, progress_bar(sys.cpu));
    mvwprintw(window, 4, inform, progress_bar(sys.mem));
    inform += 12;
    mvwprintw(window, 5, inform, "%d", sys.all_proc);
    mvwprintw(window, 6, inform, "%d", sys.run_proc);
    mvwprintw(window, 7, inform, format_time(sys.uptime));
    wattroff(window, COLOR_PAIR(2));
    wrefresh(window);
}
void display_hat(WINDOW *window)
{
    wattron(window, COLOR_PAIR(3));
    int row = 0;
    int width_col = 10;
    int const pid_col = 2;
    int const user_col = pid_col + width_col;
    int const stat_col = user_col + width_col;
    int const cpu_col = stat_col + width_col;
    int const ram_col = cpu_col + width_col;
    int const time_col = ram_col + width_col;
    int const command_col = time_col + width_col;
    mvwprintw(window, row, pid_col, "PID");
    mvwprintw(window, row, user_col, "USER");
    mvwprintw(window, row, stat_col, "STATE");
    mvwprintw(window, row, cpu_col, "CPU[%%]");
    mvwprintw(window, row, ram_col, "RAM[MB]");
    mvwprintw(window, row, time_col, "TIME+");
    mvwprintw(window, row, command_col, "COMMAND");
    wattroff(window, COLOR_PAIR(3));
    wrefresh(window);
}

void display_process(struct system sys, WINDOW *window)
{
}

void display(struct system sys)
{
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    getmaxyx(stdscr, height, width);

    WINDOW *system_window = newwin(9, width - 1, 0, 0);
    WINDOW *hat_window = newwin(1, width - 1, system_window->_maxy + 1, 0);

    while (1)
    {
        sys = system_init();
        // box(system_window, 0, 0);
        display_system(sys, system_window);
        display_hat(hat_window);
        refresh();
        sleep(1);
    }
    endwin();
}