/* SOD32 the Stack Oriented Design computer.
   Copyright 1994 L.C. Benschop, Eindhoven, The Netherlands.
   The program is released under the GNU General Public License version 2.
   There is NO WARRANTY.
*/

#include "sod32.h"

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

/* Multiply 32-bit unsigned numbers *a and *b.
   High half of 64-bit result in *a, low half in *b
*/   
static void umul(UNS32 *a,UNS32 *b)
{
 UNS32 ah,al,bh,bl,ph,pm1,pm2,pl;
 ah=*a>>16;al=*a&0xffff;
 bh=*b>>16;bl=*b&0xffff;
 pl=al*bl;
 if((ah|bh)==0) {
  ph=0;
 } else {
  pm1=al*bh;
  pm2=ah*bl;
  ph=ah*bh;
  pl=pl+(pm1<<16);
  ph+=(pl<(pm1<<16));
  pl=pl+(pm2<<16);
  ph+=(pl<(pm2<<16));
  ph=ph+(pm1>>16)+(pm2>>16);
 } 
 *a=ph;*b=pl;
}

/* Divide 64-bit unsigned number (high half *b, low half *c) by
   32-bit unsigend number in *a. Quotient in *b, remainder in *c.
*/
static void udiv(UNS32 *a,UNS32 *b,UNS32 *c)
{
 UNS32 d,qh,ql;
 int i,cy;
 qh=*b;ql=*c;d=*a;
 if(qh==0) {
  *b=ql/d;
  *c=ql%d;
 } else {
  for(i=0;i<32;i++) {
   cy=qh&0x80000000;
   qh<<=1;
   if(ql&0x80000000)qh++;
   ql<<=1;
   if(qh>=d||cy) {
    qh-=d;
    ql++;
    cy=0;
   } 
   *c=qh;
   *b=ql; 
  }  
 }
}

void virtual_machine(void)
{
 register UNS32 sp,rp,ret,ip,ireg,t;
 interrupt=0;
 rp=MEMSIZE;
 sp=MEMSIZE-1024;
 ip=0;
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
    if(ireg&2) /*jumpz*/ {
                t=CELL(sp);sp+=4;  
                if(!t)ip=ireg&CELLMASK;
          } else /*call*/ {
            rp-=4;
            CELL(rp)=ip;
            ip=ireg&CELLMASK; 
          } 
  } else {
   ret=ireg&0x80000000;
   ireg=(ireg&0x7fffffff)>>1;
   do {
    switch(ireg&31) { 
     case 0: /*nop*/ break;
     case 1: /*swap*/ t=CELL(sp);CELL(sp)=CELL(sp+4);
                      CELL(sp+4)=t;break;
     case 2: /*rot*/  t=CELL(sp+8);CELL(sp+8)=CELL(sp+4);
                      CELL(sp+4)=CELL(sp);CELL(sp)=t;break;
     case 3: /*0=*/   CELL(sp)=-!CELL(sp);break;
     case 4: /*negate*/ CELL(sp)=-CELL(sp);break;
     case 5: /* um* */ umul(&CELL(sp),&CELL(sp+4));
                       break;
     case 6: /* C@ */  CELL(sp)=BYTE(CELL(sp));break;                                                   
     case 7: /* @ */   CELL(sp)=CELL(CELL(sp)&CELLMASK);break;
     case 8: /* + */   CELL(sp+4)=CELL(sp)+CELL(sp+4);sp+=4;break;
     case 9: /* and */ CELL(sp+4)=CELL(sp)&CELL(sp+4);sp+=4;break;
     case 10:/* or  */ CELL(sp+4)=CELL(sp)|CELL(sp+4);sp+=4;break;
     case 11:/* xor */ CELL(sp+4)=CELL(sp)^CELL(sp+4);sp+=4;break;
     case 12:/* u<  */ CELL(sp+4)=-(CELL(sp+4)<CELL(sp));sp+=4;break;
     case 13:/* <   */ CELL(sp+4)=-((INT32)CELL(sp+4)<(INT32)CELL(sp));
                       sp+=4;break;
     case 14:/*lshift*/CELL(sp+4)<<=CELL(sp);sp+=4;break;
     case 15:/*rshift*/CELL(sp+4)>>=CELL(sp);sp+=4;break;
     case 16:/*um/mod*/if(CELL(sp)<=CELL(sp+4)) { /*overflow */
                         interrupt=8;
                         ireg=((ireg<<1)+1) | ret ;
                         goto doint;
                       }
                       else
                       {
                        udiv(&CELL(sp),&CELL(sp+4),&CELL(sp+8));
			sp+=4;
                       }break;
     case 17:/* +cy */ { UNS32 sum; if(CELL(sp)) { /* carry in */
                          sum=CELL(sp+8)+CELL(sp+4)+1;
                          sp+=4;
                          CELL(sp)=(sum<=CELL(sp));
                          CELL(sp+4)=sum;
                         } else {
                          sum=CELL(sp+8)+CELL(sp+4);
                          sp+=4;
                          CELL(sp)=(sum<CELL(sp));
                          CELL(sp+4)=sum;
                         }
                       }break; 
     case 18:/*scan1*/ if(CELL(sp)){ /* scan from right */
                         t=0;while(!(CELL(sp+4)&1) && t<32) {
                          CELL(sp+4)>>=1;t++;
                         }
                       } else { 
                         t=0;while(!(CELL(sp+4)&0x80000000) && t<32) {
                          CELL(sp+4)<<=1;t++;
                         } 
                       }sp+=4;CELL(sp)=t;break;
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
     case 20:/* drop */ sp+=4;break;
     case 21:/* >r   */ rp-=4;CELL(rp)=CELL(sp);sp+=4;break;
     case 22:/* c!a  */ t=CELL(sp);BYTE(t)=CELL(sp+4);sp+=4;
                        CELL(sp)=t;break;
     case 23:/* !a   */ t=CELL(sp);CELL(t)=CELL((sp+4)&CELLMASK);sp+=4;
                        CELL(sp)=t;break;
     case 24:/* dup  */ sp-=4;CELL(sp)=CELL(sp+4);break;
     case 25:/* over */ sp-=4;CELL(sp)=CELL(sp+8);break;
     case 26:/* r@   */ sp-=4;CELL(sp)=CELL(rp);break;
     case 27:/* r>   */ sp-=4;CELL(sp)=CELL(rp);rp+=4;break;
     case 28:/*push0 */ sp-=4;CELL(sp)=0;break;
     case 29:/*push1 */ sp-=4;CELL(sp)=1;break;
     case 30:/*push4 */ sp-=4;CELL(sp)=4;break;                                                       
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

