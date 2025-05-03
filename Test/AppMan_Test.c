#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <spawn.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>

bool notify_monitor = false;

static void thread_handler(void *arg)
{
    int fd1;
    int fd2;
    unsigned char buffer[5] = {0x00, 0x01, 0x00, 0x01, '\0'}; // This also facilitates sending pid of the process
    unsigned int prio = 0U;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    fd1 = mq_open("/tmp/npipe", O_WRONLY);
    if (fd1 == -1)
    {
        printf("Error: Queue may not exist, create new\n");
        mkfifo("/tmp/npipe", 0666);
        fd1 = open("/npipe", O_WRONLY);   
    }

    if(fd1 == -1){
        perror("fifo_open failed");
        exit(-1);
    }
    #if 1
    // another way is to use mq_notify()
    while(1)
    {
        if(notify_monitor == true){
            if (write(fd1, buffer, 5U) != -1)
            {
                printf("Monitor notified of new Appn");
                close(fd1);
                notify_monitor = false;
            }
        }
    }
    #endif
}

int main(int argc, char **argv)
{
    if(argc == 2U)
    {
        if(strcmp(argv[1], "-help") == 0U){
            printf("Help!!!\n");
        }
    }
    unsigned char idx;
    pid_t pid;
    pthread_attr_t attr;
    pthread_t *pthread = calloc(1, sizeof(pthread_t));

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(pthread, &attr, thread_handler, NULL);
    while (1)
    {
        /* code */
    }
    
    /* This is to support one entry one exit */   
    end:
        pthread_attr_destroy(&attr);
        pthread_cancel(pthread);
        pthread_join(pthread, NULL);
        free(pthread);
        exit(0);   
    return 0;

    return 0;
}