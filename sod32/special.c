/* SOD32 the Stack Oriented Design computer.
   Copyright 1994, L.C. Benschop, Eindhoven, The Netherlands.
   The program is released under the GNU General Public License version 2.
   There is NO WARRANTY.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sod32.h"


#define SWAP swap_mem(addr&CELLMASK,len+3);
#define CLIP addr&=MEMMASK;len&=MEMMASK;if(addr+len>MEMSIZE)len=MEMSIZE-addr;
#define FILEID(n) if((n)>=20){ior=-201;goto end;}if(!fileids[n])\
                  {ior=-201;goto end;} fp=fileids[n]; 

char *filemodes[]={"r","rb","w","wb","r+","r+b"};
char filename[256];

int make_name(char *addr,UNS32 len)
{
 int i;
 for(i=0;i<len && i<256;i++) {
  filename[i]=addr[i];
  if(filename[i]=='/' || filename[i]=='\\' || filename[i]==':') return 0;
   /* Prohibit filenames in other directories */ 
 }
 filename[i]='\0';
 return 1;
}


void do_os(void)
{
 UNS32 addr,len,ior=0;
 int res;
 static FILE *fileids[20];
 FILE *fp;
 char *unused;
 UNS32 code=CELL(save_sp);save_sp+=4;
 switch(code) {
  case 0: /*exit*/ systerm();exit(0);break;
  case 1: /*read char */ save_sp-=4;CELL(save_sp)=getch();break; 
  case 2: /*print char*/ putch(CELL(save_sp));save_sp+=4;break;
  case 3: /*check key */ save_sp-=4;CELL(save_sp)=-kbhit();break;
  case 4: /* set-term */ if(CELL(save_sp)) forthterm(); else systerm();
                         save_sp+=4;break;
  case 7: /*open-file*/  save_sp+=4;len=CELL(save_sp);addr=CELL(save_sp+4);
                         CLIP SWAP if(!make_name(mem+addr,len)) {ior=-202;
                         SWAP goto end;} SWAP 
                         {
                          int i;
                          for(i=0;i<20;i++) {
                           if(!fileids[i])break;
                          } 
                          if(i==20){ior=-201;goto end;}
                          if(CELL(save_sp-4)>5){ior=-203;goto end;}
                          fp=fopen(filename,filemodes[CELL(save_sp-4)]);  
                          if(!fp)ior=200;
                          fileids[i]=fp;
                          CELL(save_sp+4)=i;                         
                         }goto end;   
  case 8: /*close-file*/ FILEID(CELL(save_sp)) fclose(fp); 
                         fileids[CELL(save_sp)]=0;
  			 goto end;;
  case 9: /*write-line*/ save_sp+=8;FILEID(CELL(save_sp-8)) len=CELL(save_sp-4);
                         addr=CELL(save_sp);CLIP SWAP clearerr(fp); 
                         fwrite(mem+addr,1,len,fp);SWAP
                         fputc('\n',fp);if(ferror(fp))ior=-200;
                         goto end; 
  case 10: /*read-line*/ FILEID(CELL(save_sp)) len=CELL(save_sp+4)+2;
                         addr=CELL(save_sp+8);CLIP SWAP 
                         clearerr(fp);
                         unused = fgets(mem+addr,len,fp);
                         if(ferror(fp)){ior=-200;SWAP goto end;}
                         res=strlen(mem+addr);
                         if(!res || feof(fp))CELL(save_sp+4)=0; 
                         else {
                           CELL(save_sp+4)=0xffffffff;
                          if(*(mem+addr+res-1)=='\n') res--; 
                         }
                         CELL(save_sp+8)=res;
                         SWAP goto end;
  case 11: /*write-file*/save_sp+=8;FILEID(CELL(save_sp-8)) len=CELL(save_sp-4);
                         addr=CELL(save_sp);CLIP SWAP clearerr(fp); 
                         fwrite(mem+addr,1,len,fp);SWAP if(ferror(fp))ior=-200; 
                         goto end; 
  case 12: /*read-file*/ save_sp+=4;FILEID(CELL(save_sp-4)) len=CELL(save_sp);
                         addr=CELL(save_sp+4);CLIP SWAP  clearerr(fp);
                         res=fread(mem+addr,1,len,fp);SWAP 
                         CELL(save_sp+4)=res;if(ferror(fp)) ior=-200;goto end; 
  case 13: /*delete-file*/ save_sp+=4;len=CELL(save_sp-4);addr=CELL(save_sp);
                         CLIP SWAP if(!make_name(mem+addr,len)) {ior=-202;
                         SWAP goto end;} SWAP remove(filename); goto end;
  case 14: /*reposition-file*/ save_sp+=8;FILEID(CELL(save_sp-8)) 
                          fseek(fp,CELL(save_sp),SEEK_SET);goto end;
  
  case 15: /*file-position*/ save_sp-=8;FILEID(CELL(save_sp+8))
                          CELL(save_sp+8)=ftell(fp);CELL(save_sp+4)=0;
                          goto end;
  case 16: /*system */   save_sp+=4;len=CELL(save_sp-4);addr=CELL(save_sp);
                         CLIP SWAP if(!make_name(mem+addr,len)) {ior=-202;
                          SWAP goto end;} SWAP ior=system(filename); goto end; 
  case 17: /* file-size*/ save_sp-=8;FILEID(CELL(save_sp+8))
                          {long oldpos;
                           oldpos=ftell(fp); 
                           fseek(fp,0L,SEEK_END);
                           CELL(save_sp+8)=ftell(fp);CELL(save_sp+4)=0;
                           fseek(fp,oldpos,SEEK_SET);
                          }
                          goto end;                        
 } return;
 end: CELL(save_sp)=ior;
}

void do_special(UNS32 code) /* execute a special instruction */
{
 switch(code) {
  case 32: /* trap0 */ do_os();break; 
  case 50: /* setalarm */ setalarm(CELL(save_sp));save_sp+=4;break;
 }
}

