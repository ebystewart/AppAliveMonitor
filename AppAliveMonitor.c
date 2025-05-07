#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <spawn.h>
#include "AppAliveMonitor.h"
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <sys/stat.h>

#define NUM_APPS 4U // This also has to come from the App Manager

uint16_t *pid_ptr;
bool termination_request = true;

/* Note:
  IPC between AliveMoitor and other apps is message queue based
  IPC between AliveMonitor and App Manager is named FIFO based 
*/
static void print_options(void);

static void thread_handler(void *arg)
{
    mqd_t mqd;
    /* for some reason, 10K buffer is required to get the recption working in ubuntu */
    unsigned char buffer[10000];
    uint32_t prio;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    mqd = mq_open("/mqueue", O_RDWR); //O_CREAT  required for first time
    
    if(mqd == -1){
        perror("mq_open failed");
        exit(-1);
    }
    #if 0
    char message[10000];
    if(mq_receive(mqd, message, sizeof(message), &prio) != -1){
        printf("checkpoint #1 of app %d reached -> %s\n", prio, message);
    }
    else{
        perror("receive failed");
    }
    #endif
    #if 1
    // another way is to use mq_notify()
    while(1)
    {
        if(mq_receive(mqd, buffer, sizeof(buffer), &prio) != -1)
        {
            printf("checkpoint %d of app %d reached\n", buffer[0], buffer[2]);
        }
        else{
            perror("receive failed");
            sleep(10);
        }
    }
    #endif
    mq_close(mqd);
}

static void thread_handler2(void *arg)
{
    int fd1;
    int fd2;
    unsigned short int idx;
    /* for some reason, 10K buffer is required to get the recption working in ubuntu */
    unsigned char rxBuffer[10];
    unsigned char txBuffer[10];
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    fd2 = open("/home/eby/npipe", O_RDONLY); //O_CREAT  required for first time
    
    if(fd2 == -1){
        perror("fifo_open failed");
        exit(-1);
    }
    #if 1
    // another way is to use mq_notify()
    //while(1)
    {
        if(read(fd2, rxBuffer, sizeof(rxBuffer)) != -1)
        {
            printf("New Process spawned with pid : %d\n", rxBuffer[0]);
            idx = (rxBuffer[0] << 8) | rxBuffer[1];
            pid_ptr[idx] = (rxBuffer[2] << 8) | rxBuffer[3];
            close(fd2);
        }
        else{
            perror("receive failed");
            sleep(10);
        }
        if(termination_request == true){
            fd1 = open("/home/eby/npipe", O_WRONLY);
            txBuffer[0] = 0x55U;
            txBuffer[1] = '\0';
            if (write(fd1, txBuffer, 2U) != -1)
            {
                printf("Request App Manager to terminate a child\n");
                close(fd1);
                termination_request = false;
            }
        }
    }
    #endif
}

int main(int argc, char **argv)
{
    uint8_t idx;
    pid_t pid;
    pthread_attr_t attr;
    pthread_attr_t attr2;
    unsigned int numApps = 1U;
    pthread_t *pthread = calloc(1, sizeof(pthread_t));
    pthread_t *pthread2 = calloc(1, sizeof(pthread_t));
    if((argc == 2U) && (strcmp(argv[1],"-h") == 0U))
    {
        print_options();
        goto end;
    }
    /* This should be the first process spawned out by AppManager */
    /* Get the Number of processes spawned out by AppManager */
    /* Get the process Id of each process from App Manager */

    /* store the PIDs in an array */
    pid_ptr = calloc(1, (sizeof(uint16_t) * NUM_APPS));
    for(idx = 0U; idx < NUM_APPS; idx++)
    {
        pid = pid_ptr[idx];
    }

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(pthread, &attr, thread_handler, (void *)pid_ptr); //pid_ptr is initialized only in thread 2

    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE);
    pthread_create(pthread2, &attr2, thread_handler2, (void *)pid_ptr); //pid_ptr is initialized only inside thread 2
    while(1)
    {

    }

    /* This is to support one entry one exit */   
    end:
        pthread_attr_destroy(&attr);
        pthread_attr_destroy(&attr2);
        pthread_cancel(pthread);
        pthread_cancel(pthread2);
        pthread_join(pthread, NULL);
        pthread_join(pthread2, NULL);
        free(pthread);
        free(pid_ptr);
        free(pthread2);
        pthread = NULL;
        pthread2 = NULL;
        pid_ptr = NULL;
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

