#include <linux/joystick.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define DEVICE "/dev/input/by-id/usb-SFC30_SFC30_Joystick-joystick"
//#define DEVICE "/dev/hidraw4"
#include "snek.h"
#include "joyInterface.h"

//Simplified interface for sfc30 gamepad, yes needs work

extern int startJoyInterface(void *arg){
    pthread_attr_t attr;
    pthread_t thread1;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);

    int rc;
    rc=pthread_create( &thread1, &attr, captureJoyCodes, arg);
    if (rc != 0)
        return 0;

    return 1;
}

//For 8bitdo via BT (hidraw device)
//Yes I clearly don't know yet what is going on
//learning is on the todo list though :D
void *captureJoyCodesHidraw(void *arg){
    snek *sn;
    sn = (snek *)arg;
    int fd;
    fd = open(DEVICE, O_RDONLY);
    if(fd == -1){
        printf("Error opening device '%s', exiting..\n", DEVICE);
    }else{
        printf("Opened device '%s' for reading..\n", DEVICE);
    }

    struct joystick{
        __s32 value; /* event timestamp in milliseconds */
        __u32 value1;    /* value */
        __u32 value2;    /* value */
        __u64 value3;    /* value */
    };
    struct joystick js;

    while (1){
        read(fd, &js, sizeof(struct input_event));

        printf("%d\t%d\t%d", js.value, js.value1, js.value2);
        printf("\n");
        //Capture buttons, onDown
        if(js.value == 8326915\
                && sn->seg->d != 2\
                && sn->seg->next->d != 2){
            sn->seg->d = 0; //Up
            sn->seg->tok = '^';
        }
        if(js.value == 2147421955\
                && sn->seg->d != 3\
                && sn->seg->next->d != 3){
            sn->seg->d = 1; //Right
            sn->seg->tok = '>';
        }
        if(js.value == -8450301\
                && sn->seg->d != 0\
                && sn->seg->next->d != 0){
            sn->seg->d = 2; //Down
            sn->seg->tok = 'v';
        }
        if(js.value == 2130710275\
                && sn->seg->d != 1\
                && sn->seg->next->d != 1){
            sn->seg->d = 3;//Left
            sn->seg->tok = '<';
        }
    }
}

//For 8bitdo via USB
void *captureJoyCodes(void *arg){
    snek *sn;
    sn = (snek *)arg;
    int fd;
    fd = open(DEVICE, O_RDONLY);
    if(fd == -1){
        printf("Error opening device '%s', exiting..\n", DEVICE);
    }else{
        printf("Opened device '%s' for reading..\n", DEVICE);
    }

    struct joystick{
        __u32 time; /* event timestamp in milliseconds */
        __u32 value;    /* value */
        __u64 value2;    /* value */
        __u64 value3;    /* value */
    };
    struct joystick js;

    while (1){
        read(fd, &js, sizeof(struct input_event));

        //printf("%d\t%d\t%d", js.time, js.value, js.value2);
        //printf("\n");
        //Capture buttons, onDown
        if(js.value == 16941057
                && sn->seg->d != 2\
                && sn->seg->next->d != 2){
            sn->seg->d = 0; //Up
            sn->seg->tok = '^';
        }
        if(js.value == 163839\
                && sn->seg->d != 3\
                && sn->seg->next->d != 3){
            sn->seg->d = 1; //Right
            sn->seg->tok = '>';
        }
        if(js.value == 16941055\
                && sn->seg->d != 0\
                && sn->seg->next->d != 0){
            sn->seg->d = 2; //Down
            sn->seg->tok = 'v';
        }
        if(js.value == 163841\
                && sn->seg->d != 1\
                && sn->seg->next->d != 1){
            sn->seg->d = 3;//Left
            sn->seg->tok = '<';
        }
    }
}
