#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <spawn.h>
#include "AppAliveMonitor.h"
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


static void print_options(void);


int main(int argc, char **argv)
{

    /* This is to support one entry one exit */   
    end:
        exit(0);   
    return 0;
}

static void print_options(void)
{
    printf("******************************************************************************\n");
    printf("*********************************** APP Manager ******************************\n");
    printf("******************************************************************************\n");
    printf("\t -c <configuration file> : configuartion file containing application\n");
    printf("\t -h                      : help \n");
    printf("******************************************************************************\n");
}

