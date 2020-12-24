/* SOD32 the Stack Oriented Design computer.
   Copyright 1994, L.C. Benschop, Eindhoven, The Netherlands.
   The program is released under the GNU General Public License version 2.
   There is NO WARRANTY.
*/

/* POSIX version */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/time.h>
#include "sod32.h"

static struct termios oldterm;
static struct termios newterm;
static int tflags;
static int realterm;
int keybuf,raw;
static int childpid;

#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK
#endif

void inthandler(int s)
{
 signal(SIGINT,inthandler);
 interrupt=12; 
}

void quithandler(int s)
{
 systerm();
 printf("Quit!\n");exit(1);
}

#ifdef SIGTSTP
void stophandler(int s)
{
 if(raw){systerm();raw=1;}
 raise(SIGTSTP);
}

void conthandler(int s)
{
 signal(SIGTSTP,stophandler);
 signal(SIGCONT,conthandler);
 if(raw)forthterm(); 
}
#endif

void alarmhandler(int s)
{
 interrupt=16;
}

void initterm(void)
{
 realterm=isatty(0);
 if(realterm) {
  tcgetattr(0,&oldterm);
  newterm=oldterm;
  newterm.c_iflag = newterm.c_iflag & ~INLCR & ~ICRNL;
  newterm.c_lflag = newterm.c_lflag & ~ECHO & ~ICANON;
  newterm.c_cc[VMIN]=1;  
  newterm.c_cc[VTIME]=0;
  tflags=fcntl(0,F_GETFL,0);
 } 
 signal(SIGQUIT,quithandler);
 signal(SIGINT,inthandler);
 #ifdef SIGTSTP
  signal(SIGTSTP,stophandler);
  signal(SIGCONT,conthandler);
 #endif 
}


void forthterm(void)
{
 if(realterm) {
  tcsetattr(0,TCSAFLUSH,&newterm);
  fcntl(0,F_SETFL,tflags|O_NDELAY);
  keybuf=EOF;
  raw=1;
 } 
}

void systerm(void) 
{
 if(realterm) {
  tcsetattr(0,TCSAFLUSH,&oldterm);
  fcntl(0,F_SETFL,tflags);
  raw=0;
 } 
}

int getch(void)
{
 int c;
 if(realterm && raw) {
  if(keybuf==EOF) {
   fcntl(0,F_SETFL,tflags);
   while((keybuf=getchar())==EOF&&errno==EINTR)
     ;   
   fcntl(0,F_SETFL,tflags|O_NDELAY);
  } 
  c=keybuf;keybuf=EOF;
  return c; 
 } else return getchar();
}

int kbhit(void)
{
 if(realterm && raw) {
  if(keybuf!=EOF) return 1;
  keybuf=getchar();
  return (keybuf != EOF );
 } else return 1;
}

void putch(int c)
{
int res;
char k=c;
if(raw)while(write(1,&k,1)<0);   
else putchar(c);
}

struct itimerval tt;

void setalarm(unsigned int usecs)
{
 signal(SIGALRM,alarmhandler);
 tt.it_interval.tv_sec=0;
 tt.it_interval.tv_usec=0;
 tt.it_value.tv_sec=usecs/1000000;
 tt.it_value.tv_usec=usecs%1000000; 
 setitimer(ITIMER_REAL,&tt,0);
}
