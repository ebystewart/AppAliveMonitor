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

bool notify_monitor = true;

static void thread_handler(void *arg)
{
    int fd1;
    int fd2;
    unsigned char txBuffer[5] = {0x00, 0x01, 0x00, 0x01, '\0'}; // This also facilitates sending pid of the process
    unsigned char rxBuffer[10];
    unsigned int prio = 0U;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    fd1 = open("/home/eby/npipe", O_WRONLY);
    if (fd1 == -1)
    {
        printf("Error: Queue may not exist, create new\n");
        /* We use a named fifo because, we create the fifo much later than spanwing teh child process */
        if(mkfifo("/home/eby/npipe", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH) == -1){
            perror("fifo creation failed");
            exit(-1);
        }
        fd1 = open("/home/eby/npipe", O_WRONLY);   
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
            if (write(fd1, txBuffer, 5U) != -1)
            {

                printf("Monitor notified of new Appn");
                close(fd1);
                notify_monitor = false;
            }
        }
        fd2 = open("/home/eby/npipe", O_RDONLY);  
        if(read(fd2, rxBuffer, sizeof(rxBuffer)) != -1)
        {
            printf("Process termination request received for PID : %d\n", rxBuffer[0]);
            close(fd2);
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