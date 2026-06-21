// Source: https://elinux.org/Rpi_Datasheet_751_GPIO_Registers
//
//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//  Revised: 15-Feb-2013


// Access from ARM Running Linux
#include "pi_mmio.h"

#define BCM2711_PERI_BASE        0xFE000000
#define GPIO_BASE                (BCM2711_PERI_BASE + 0x200000) /* GPIO controller */

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
static void *gpio_map = NULL;

// I/O access
static volatile uint32_t *gpio = NULL;
//volatile unsigned *gpio;

//using new registers. might delete the old ones above
#define GPIO_FSEL0 0
#define GPIO_SET0 7
#define GPIO_CLR0 10
#define GPIO_LEV0 13
#define GPIO_PUP_PDN_CNTRL_REG0 57

//
// Set up a memory regions to access GPIO
//
int pi_mmio_init() {
    /* open /dev/mem */
    mem_fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (mem_fd < 0) {
        return -1;
    }

    /* mmap GPIO */
    gpio_map = mmap(
            NULL,             //Any adddress in our space will do
            BLOCK_SIZE,       //Map length
            PROT_READ | PROT_WRITE,// Enable reading & writting to mapped memory
            MAP_SHARED,       //Shared with other processes
            mem_fd,           //File to map
            GPIO_BASE         //Offset to GPIO peripheral
            );

    close(mem_fd); //No need to keep mem_fd open after mmap

    if (gpio_map == MAP_FAILED) {
        gpio_map = NULL;
        return -1;
    }

    // Always use volatile pointer!
    gpio = (volatile uint32_t *)gpio_map;

    return 0;
} 

void pi_mmio_close(void) {
    if(gpio_map != NULL){
        munmap(gpio_map, BLOCK_SIZE);
        gpio_map = NULL;
        gpio = NULL;
    }
}

void pi_gpio_set_pin(int pin, int value){
    if(gpio == NULL){
        return;
    }

    if(value){
        gpio[GPIO_SET0] = 1u << pin;
    }
    else{
        gpio[GPIO_CLR0] = 1u << pin;
    }
}

void pi_gpio_set_input(int pin){
    if(gpio == NULL){
        return;
    }

    int reg = GPIO_FSEL0 + (pin / 10);
    int shift = (pin % 10) * 3;

    gpio[reg] &= ~(7u << shift);
}

void pi_gpio_set_pull_up(int pin){
    if(gpio == NULL){
        return;
    }
    
    int reg = GPIO_PUP_PDN_CNTRL_REG0 + (pin / 16);
    int shift = (pin % 16) * 2;

    gpio[reg] &= ~(3u << shift);
    gpio[reg] |= (1u << shift);
}

int pi_gpio_read(int pin){
    if(gpio == NULL){
        return -1;
    }

    return (gpio[GPIO_LEV0] & (1u << pin)) != 0;
}

/**
 *
 * For Pull-Up/Pull-Down use this:
 *  // enable pull-up on GPIO24&25
   GPIO_PULL = 2;
   short_wait();
   // clock on GPIO 24 & 25 (bit 24 & 25 set)
   GPIO_PULLCLK0 = 0x03000000;
   short_wait();
   GPIO_PULL = 0;
   GPIO_PULLCLK0 = 0;

 */

