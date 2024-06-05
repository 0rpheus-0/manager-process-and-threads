#include <stdio.h>
#include "process.h"
#include "parser.h"
#include "thread.h"
#include "system.h"
#include "display.h"

int main()
{
    jiffies_init();
    display();
    return 0;
}
