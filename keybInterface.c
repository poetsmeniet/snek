#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#define DEVICE "/dev/input/event0"
#include "snek.h"
#include "keybInterface.h"

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
        if(ev.code == 103 && ev.value == 1)
            printf("Up key pressed\n");
        if(ev.code == 106 && ev.value == 1)
            printf("Right key pressed\n");
        if(ev.code == 108 && ev.value == 1)
            printf("Down key pressed\n");
        if(ev.code == 105 && ev.value == 1)
            printf("Left key pressed\n");
    }
    return 0;
}
