\ SOFTWARE SIMULATOR FOR SOD-32, THE STACK ORIENTED DESIGN PROCESSOR 
\ Copyright 1994 L.C. Benschop Eindhoven, The Netherlands.
\ The program is released under the GNU General Public License version 2.
\ There is NO WARRANTY.
\ 

\ PART 1: THE VIRTUAL MEMORY SPACE.

\ We need to define the virtual memory space and the words to access it.
\ THese are very similar to the words that access the target space in
\ a cross compiler.

128 1024 * CONSTANT MEM_SIZE
MEM_SIZE 1- CONSTANT MEMMASK \ This mask confines an address to memory.
MEMMASK 3 - CONSTANT CELLMASK \ This confines an address to aligned address.

CREATE VMEM MEM_SIZE CHARS ALLOT \ This space contains the target image.
       VMEM MEM_SIZE 0 FILL      \ Initialize it te zero.

\ Fetch and store characters in the virtual space.
: VC@ ( t-addr --- c) MEMMASK AND CHARS VMEM + C@ ;
: VC! ( c t-addr ---) MEMMASK AND CHARS VMEM + C! ;

\ Fetch and store cells in the virtual space.
\ SOD32 is big endian 32 bit so store explicitly big-endian.
: V@  ( t-addr --- x)
       CELLMASK AND
       CHARS VMEM + DUP C@ 24 LSHIFT OVER 1 CHARS + C@ 16 LSHIFT +
       OVER 2 CHARS + C@ 8 LSHIFT + SWAP 3 CHARS + C@ + ;
: V!  ( x t-addr ---)
       CELLMASK AND
       CHARS VMEM + OVER 24 RSHIFT OVER C! OVER 16 RSHIFT OVER 1 CHARS + C!
       OVER 8 RSHIFT OVER 2 CHARS + C! 3 CHARS + C! ;


\ First we will load a Forth image into the vir.
: LOAD-IMAGE ( "name" --- )
  32 WORD COUNT R/O BIN OPEN-FILE ABORT" Can't open file" >R
  VMEM  MEM_SIZE R@ READ-FILE ABORT" Can't write file" CR . 
 ." byte image read" CR
  R> CLOSE-FILE ABORT" Can't close file" ;

\ PART 2: Virtual machine registers and stack access.

\ : \D ;
: \D POSTPONE \ ; IMMEDIATE

VARIABLE IR \ Instruction register.
VARIABLE IP \ Instruction pointer.
VARIABLE SP \ Stack pointer.
VARIABLE RP \ Return stack pointer.

: POP ( --- x) \ Pop x from data stack.
  SP @ V@ 4 SP +! 
\D ." Pop: " DUP . 
;

: PUSH ( x ---) \ Push x onto data stack.
\D ." Push: " DUP .
  -4 SP +! SP @ V! ;

: RPOP ( --- x) \ Pop x from return stack.
  RP @ V@ 4 RP +! 
\D ." RPop: " DUP . 
;

: RPUSH ( x ---) \ Push x onto reutrn stack.
\D ." RPush: " DUP . 
  -4 RP +! RP @ V! ;

\ PART 3: The special instructions.

\ For the moment we cut some corners with O.S. access, define only
\ functions 0, 1 and 2.

CREATE ACCEPT-BUFFER 82 CHARS ALLOT
VARIABLE #BUF VARIABLE >BUF
\ Emulate the Cooked input of C.
: GET-KEY 
  >BUF @ #BUF @ > IF
    ACCEPT-BUFFER 80 ACCEPT #BUF ! 0 >BUF !
  THEN 
  >BUF @ #BUF @ = 
  IF
   10 \ Newline
  ELSE
   ACCEPT-BUFFER >BUF @ + C@
  THEN
  1 >BUF +!
;

: DO_OS ( code ---)
   DUP 0= IF CR ." Exiting simulator" CR QUIT 
   ELSE DUP 1 = IF GET-KEY PUSH 
   ELSE DUP 2 = IF POP EMIT 
   ELSE 
\D CR DUP ." Unimplemented O.S. function: " . CR 
   THEN THEN THEN DROP ;

: DO_SPECIAL ( --)
  POP DUP 0= IF SP @ PUSH 
  ELSE DUP 1 = IF POP SP ! 
  ELSE DUP 2 = IF RP @ PUSH 
  ELSE DUP 3 = IF POP RP ! 
  ELSE DUP 32 = IF POP DO_OS 
  ELSE DUP CR ." Unimplemented special instruction: " . CR 
  THEN THEN THEN THEN THEN DROP
;

\ PART 4: The normal instructions.

: DO_NOOP ;
: DO_SWAP POP POP SWAP PUSH PUSH ;
: DO_ROT  POP POP POP ROT ROT PUSH PUSH PUSH ;
: DO_0=   POP 0= PUSH ;
: DO_NEGATE POP NEGATE PUSH ;
: DO_UM*  POP POP UM* SWAP PUSH PUSH ;
: DO_C@   POP VC@ PUSH ;
: DO_@    POP V@ PUSH ;
: DO_+    POP POP + PUSH ;
: DO_AND  POP POP AND PUSH ;
: DO_OR   POP POP OR PUSH ;
: DO_XOR  POP POP XOR PUSH ;
: DO_U<   POP POP SWAP U< PUSH ;
: DO_<    POP POP SWAP < PUSH ;
: DO_LSHIFT POP POP SWAP LSHIFT PUSH ;
: DO_RSHIFT POP POP SWAP RSHIFT PUSH ;
: DO_UM/MOD POP POP POP SWAP ROT UM/MOD SWAP PUSH PUSH ;
: DO_+CY   POP POP POP ROT 1 AND 0 SWAP 0 D+ ROT 0 D+ SWAP PUSH PUSH ;
: DO_SCAN1 CR ." SCAN1 unimplemented " CR ;
: DO_DROP  POP DROP ;
: DO_>R    POP RPUSH ;
: DO_C!A   POP POP SWAP DUP >R VC! R> PUSH ;   
: DO_!A    POP POP SWAP DUP >R V! R> PUSH ;
: DO_DUP   POP DUP PUSH PUSH ;
: DO_OVER  POP POP DUP >R PUSH PUSH R> PUSH ;
: DO_R@    RPOP DUP RPUSH PUSH ;
: DO_R>    RPOP PUSH ;
: DO_0     0 PUSH ;
: DO_1     1 PUSH ;
: DO_4     4 PUSH ;
: DO_LIT   IP @ V@ 4 IP +! 
\D       ." Lit " DUP . 
           PUSH ;

CREATE OPCODES 
 ' DO_NOOP , ' DO_SWAP , ' DO_ROT , ' DO_0= , 
 ' DO_NEGATE , ' DO_UM* , ' DO_C@ , ' DO_@ ,
 ' DO_+ ,  ' DO_AND , ' DO_OR , ' DO_XOR ,
 ' DO_U< , ' DO_< ,  ' DO_LSHIFT , ' DO_RSHIFT ,
 ' DO_UM/MOD , ' DO_+CY , ' DO_SCAN1 , ' DO_SPECIAL ,
 ' DO_DROP , ' DO_>R , ' DO_C!A , ' DO_!A ,
 ' DO_DUP , ' DO_OVER , ' DO_R@ , ' DO_R> ,
 ' DO_0 , ' DO_1 , ' DO_4 , ' DO_LIT ,

\ PART 4: The virtual machine itself.

: DO_INSTR
\D CR ." Fetch instruction at:" IP @ .
  IP @ V@ IR ! 4 IP +! \ Instruction fetch. 
  IR @ 3 AND 0= 
  IF \ Call
\D  CR ." Call: " IR @ . 
   IP @ RPUSH  IR @ IP ! 
  ELSE
    IR @ 3 AND 2 = 
    IF \ Conditional jump
\D   CR ." Cond jump " 
     POP 0= IF IR @ CELLMASK AND IP ! THEN
    ELSE
     IR @ 1 RSHIFT IR !
     6 0 DO
\D    CR ." Opcode: " IR @ 31 AND .
      IR @ 31 AND CELLS OPCODES + @ EXECUTE
      IR @ 5 RSHIFT IR !
     LOOP
    IR @ 1 AND IF \ Return
\D   CR ." Ret "
     RPOP IP !
    THEN
   THEN
  THEN
;

\ PART 5: Running it all.

: SOD32
  LOAD-IMAGE
  0 IP !
  MEM_SIZE RP !
  MEM_SIZE 1024 - SP ! 
  BEGIN
   DO_INSTR
  0 UNTIL
;
    

      