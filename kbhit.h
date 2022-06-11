/*
* getch.c -- getch(), a blocking single character input from stdin
* kbhit(), a keyboard lookahead monitor
*/

/*
* getch() -- a blocking single character input from stdin
*
* This function has been tested on several SysV machines, SunOS4.1,
* BSDI UNIX, and Linux.
*
* BSD or POSIX systems may require a struct termios, older systems
* may require a struct termio, and some (Linux) may be able to use
* either.
*
*
* --- AUTORES ---
* Óscar Baños Campos
* Eduardo Fraile González
* Grupos: L8
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>


#ifndef _KBHIT_H
#define _KBHIT_H
 
static struct termios new_io;
static struct termios old_io;

int getch(void);
int kbhit(void);


#endif
