#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define DEVICE "/dev/input/event0"
#include "snek.h"
#include "keybInterface.h"

extern int startKeybInterface(snek *snek){
    pthread_attr_t attr;
    pthread_t thread1;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);

    int rc;
    rc=pthread_create( &thread1, &attr, captureKeyCodes, snek);
    if (rc != 0)
        return 0;

    return 1;
}

int captureKeyCodes(snek *snek){
    int fd;
    fd = open(DEVICE, O_RDONLY);
    if(fd == -1){
        printf("Error opening device '%s', exiting..\n", DEVICE);
        return 1;
    }
    struct input_event ev;

    while (1){
        read(fd, &ev, sizeof(struct input_event));

        //Capture keypresses, onDown
        if(ev.code == 103 && ev.value == 1){
            snek->seg->d = 0; //Up
            snek->seg->tok = '^';
        }
        if(ev.code == 106 && ev.value == 1){
            snek->seg->d = 1; //Right
            snek->seg->tok = '>';
        }
        if(ev.code == 108 && ev.value == 1){
            snek->seg->d = 2; //Down
            snek->seg->tok = 'v';
        }
        if(ev.code == 105 && ev.value == 1){
            snek->seg->d = 3;//Left
            snek->seg->tok = '<';
        }
    }
    return 0;
}
