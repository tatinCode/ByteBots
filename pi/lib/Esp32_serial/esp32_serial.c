#include "esp32_serial.h"

#include "pi_config.h"

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int esp32_serial_open(const char *device){
    int fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if(fd < 0){
        return -1;
    }

    if(configure_serial(fd) < 0){
        close(fd);
        return -1;
    }

    return fd;
}

void esp32_serial_close(int fd){
    if(fd >= 0){
        close(fd);
    }
}

int esp32_serial_send(int fd, const char *command){
    if(fd < 0 || command == NULL){
        return -1;
    }

    size_t len = strlen(command);
    size_t written_tot = 0;

    while(written_tot < len){
        ssize_t written = write(fd, command + written_tot, len - written_tot);

        if(written < 0){
            if(errno == EAGAIN){
                continue;
            }

            return -1;
        }

        written_tot += (size_t)written;
    }

    return 0;
}

static int configure_serial(int fd){
    struct termios tty;

    if(tcgetattr(fd, &tty) < 0){
        return -1;
    }

    cfmakeraw(&tty);

    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 5;

    if(tcsetattr(fd, TCSANOW, &tty) != 0){
        return -1;
    }

    return 0;
}
