/* SOD32 the Stack Oriented Design computer.
   Copyright 1994, L.C. Benschop, Eindhoven, The Netherlands.
   The program is released under the GNU General Public License version 2.
   There is NO WARRANTY.
*/

#include <stdio.h>
#include <stdlib.h>
#include "sod32.h"

UNS8 mem[MEMSIZE+3];

UNS32 save_sp,save_ip,save_rp,interrupt;

void load_image(char *name)
{
 UNS32 len;
 FILE *infile;
 if((infile=fopen(name,"rb"))==NULL) {
  fprintf(stderr,"Cannot open image file.\n");
  exit(2);
 }
 len=fread(mem,1,MEMSIZE,infile);
 swap_mem(0,len); 
}
 


int main(int argc,char **argv)
{
 if(argc<2) {
  fprintf(stderr,"Usage: sod32 <filename>\n");
  exit(1);
 }
 load_image(argv[1]);
 initterm();
 virtual_machine();
 return 0;
}
