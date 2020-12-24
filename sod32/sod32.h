/* SOD32 the Stack Oriented Design computer.
   Copyright 1994 L.C. Benschop, Eindhoven, The Netherlands.
   The program is released under the GNU General Public License version 2.
   There is NO WARRANTY.
*/

#define UNS8  unsigned char
#define INT32 int
#define UNS32 unsigned int

#define MEMSIZE 1*1024*1024 /* must be a power of two */
#define MEMMASK MEMSIZE-1 /* mask for addresses to force them into range */

extern UNS8 mem[];

extern UNS32 save_sp,save_ip,save_rp,interrupt;

#define CELL(reg) (*(UNS32*)(mem+((reg)&MEMMASK)))
#define CELLMASK (~3)

#ifdef BIG_ENDIAN
 #define BYTEMASK 0
#else
 #define BYTEMASK 3
#endif

#define BYTE(reg) mem[((reg)&MEMMASK)^BYTEMASK]

void swap_mem(UNS32,UNS32);
void do_os(void);
void do_special(UNS32);
void virtual_machine(void);
void initterm(void);
void forthterm(void);
void systerm(void);
void putch(int);
int getch(void);
int kbhit(void);
void setalarm(unsigned int);
