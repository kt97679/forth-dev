/* SOD32 the Stack Oriented Design computer.
   Copyright 1994 L.C. Benschop, Eindhoven, The Netherlands.
   The program is released under the GNU General Public License version 2.
   There is NO WARRANTY.
*/

#include "sod32.h"

#define sp CELL(0)
#define rp CELL(4)

/* Perform byte swap of 32-bit words in region of memory of virtual machine*/
void swap_mem(UNS32 start,UNS32 len) {
#ifndef BIG_ENDIAN
 UNS32 i,m;
 for(i=0;i<len;i+=4) {
  m=CELL(start+i);
  m=(m>>16)|(m<<16);
  m=((m&0xff00ff00)>>8)|((m&0x00ff00ff)<<8);
  CELL(start+i)=m;
 } 
#endif
}

void virtual_machine(void)
{
 register UNS32 ret,ip,ireg,t;
 interrupt=0;
 rp=MEMSIZE;
 sp=MEMSIZE-1024;
 ip=32;
 for(;;) {
  if(interrupt) {
   ireg=1; 
  doint:
   rp-=4;CELL(rp)=ip;
   sp-=4;CELL(sp)=ireg;
   ip=CELL(interrupt);
   interrupt=0;
  }
  ireg=CELL(ip);
  ip+=4;
  restart: /* Instruction restart after interrupt */
  if(!(ireg&1)) {
            /* call */
            rp-=4;
            CELL(rp)=ip;
            ip=ireg&CELLMASK; 
  } else {
   ret=ireg&0x80000000;
   ireg=(ireg&0x7fffffff)>>1;
   do {
    switch(ireg&31) { 
     case 0: /*nop*/ break;
     case 1: /* nand */ sp += 4; CELL(sp) = ~(CELL(sp) & CELL(sp - 4)); break;
     case 5: /* !    */ sp += 8; CELL(CELL(sp - 8)) = CELL((sp - 4) & CELLMASK); break;
     case 7: /* @ */   CELL(sp)=CELL(CELL(sp)&CELLMASK);break;
     case 18:/* um+  */ t = CELL(sp) + CELL(sp + 4); CELL(sp) = (t < CELL(sp)); CELL(sp + 4) = t; break;
     case 19:/*special*/ t=CELL(sp);sp+=4;
                         if(t==49) { /* iret instruction */
                          ip=CELL(rp);rp+=4;
                          ireg=CELL(sp);sp+=4;
                          goto restart;
                         }
                         save_sp=sp;save_ip=ip;save_rp=rp;
                         do_special(t);
                         sp=save_sp;ip=save_ip;rp=save_rp;
                         if(interrupt) {
                          ireg=(((ireg>>5)<<1)+1)|ret; 
                          goto doint;
                         }
                         break;
     case 31:/*lit*/ sp-=4;CELL(sp)=CELL(ip);ip+=4;break; 
    }
    ireg>>=5;
   } while(ireg);
   if(ret)/*return*/ {
            ip=(CELL(rp))&CELLMASK;
            rp+=4;
           } 
  }      
 }  
}

