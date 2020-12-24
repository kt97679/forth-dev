/* This is the 386 version of the SOD 32 interpreter.
   Copyright 1994 L.C. Benschop, Eindhoven, The Netherlands.
   The program is released under the GNU General Public License version 2.
   There is NO WARRANTY.
*/

#define MEMSIZE (1024*1024)
#define MEMMASK (MEMSIZE-1)
#define CELLMASK (MEMSIZE-4)

.text
	.align 4
.globl _swap_mem
_swap_mem:
	pushl %ebp
	movl %esp,%ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%ebx
	movl 12(%ebp),%esi
	cmpl $0,%esi
	jbe L3
	.align 2,0x90
L5:
	andl $CELLMASK,%ebx
	movw _mem(%ebx),%cx
        movw _mem+2(%ebx),%ax
        xchgb %ah,%al
        xchgb %ch,%cl
        movw %ax,_mem(%ebx)
	movw %cx,_mem+2(%ebx)
        addl $4,%ebx
	subl $4,%esi
	ja L5
L3:
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret


#define NEXTINSTR \
        shrl $5,%ebx;\
        je L18;\
        movl %ebx,%eax;\
        andl $31,%eax;\
	jmp *L67(,%eax,4);\

#define PREPARE \
	movl %ebx,%ecx;\
	andl $0x7fffffff,%ebx;\
	shrl $1,%ebx;\
	.align 2,0x90;\
	
	.align 4
.globl _virtual_machine
_virtual_machine:
        /* This function never returns, so why push anything? */
        /* ebp=return stack pointer.
           esi=program counter. 
           edi=stack pointer.
           ecx=return bit
           ebx=instruction register.
        */
	movl $MEMSIZE,%ebp
	movl $MEMSIZE-1024,%edi
	movl $0,%esi
	movl $0,_interrupt
	.align 2,0x90
L7:     /* Fetch new instruction */
        testb $255,_interrupt
        jne INTER
	andl $CELLMASK,%esi
	movl _mem(%esi),%ebx
	addl $4,%esi
RESTART:
	movl %ebx,%eax
	andl $63,%eax
	jmp *L1ST(,%eax,4)
	.align 4,0x90
LJUMP:	
        movl %edi,%eax
	addl $4,%edi
	andl $CELLMASK,%edi 
	cmpl $0,_mem(%eax)
	jne L7
        movl %ebx,%esi
	jmp L7
	.align 4,0x90
LCALL:  /* call instruction */
	addl $-4,%ebp
	andl $CELLMASK,%ebp
	movl %esi,_mem(%ebp) /* push old program counter */
L71:    /* perform the jump */
	movl %ebx,%esi
	jmp L7
	.align 4,0x90
INTER:	movl $1,%ebx      /* handle interrupt */
DOINT:  andl $CELLMASK,%esi
 	addl $-4,%ebp
	andl $CELLMASK,%ebp
	movl %esi,_mem(%ebp)
	addl $-4,%edi
	andl $CELLMASK,%edi
	movl %ebx,_mem(%edi)
        movl _interrupt,%esi
	movl _mem(%esi),%esi
        movl $0,_interrupt
	jmp L7  
	.align 4,0x90
L67:
	.long LXX
	.long L20
	.long L21
	.long L22
	.long L25
	.long L26
	.long L27
	.long L28
	.long L29
	.long L30
	.long L31
	.long L32
	.long L33
	.long L36
	.long L39
	.long L40
	.long L41
	.long L44
	.long L45
	.long L54
	.long L75
	.long L56
	.long L57
	.long L58
	.long L59
	.long L60
	.long L61
	.long L62
	.long L63
	.long L64
	.long L65
	.long L66
L1ST:
	.long LCALL
	.long INS0
	.long LJUMP
	.long INS1
	.long LCALL
	.long INS2
	.long LJUMP
	.long INS3
	.long LCALL
	.long INS4
	.long LJUMP
	.long INS5
	.long LCALL
	.long INS6
	.long LJUMP
	.long INS7
	.long LCALL
	.long INS8
	.long LJUMP
	.long INS9
	.long LCALL
	.long INS10
	.long LJUMP
	.long INS11
	.long LCALL
	.long INS12
	.long LJUMP
	.long INS13
	.long LCALL
	.long INS14
	.long LJUMP
	.long INS15
	.long LCALL
	.long INS16
	.long LJUMP
	.long INS17
	.long LCALL
	.long INS18
	.long LJUMP
	.long INS19
	.long LCALL
	.long INS20
	.long LJUMP
	.long INS21
	.long LCALL
	.long INS22
	.long LJUMP
	.long INS23
	.long LCALL
	.long INS24
	.long LJUMP
	.long INS25
	.long LCALL
	.long INS26
	.long LJUMP
	.long INS27
	.long LCALL
	.long INS28
	.long LJUMP
	.long INS29
	.long LCALL
	.long INS30
	.long LJUMP
	.long INS31
	.align 4,0x90
INS0:	PREPARE
LXX:	NEXTINSTR
	.align 4,0x90
INS1:	PREPARE
L20:    /* swap */
	movl _mem(%edi),%edx
        movl _mem+4(%edi),%eax
        movl %edx,_mem+4(%edi)
        movl %eax,_mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS2:	PREPARE
L21:    /* rot */
        pushl %ecx
	movl _mem(%edi),%edx
        movl _mem+4(%edi),%ecx
        movl _mem+8(%edi),%eax
        movl %ecx,_mem+8(%edi)
        movl %edx,_mem+4(%edi)
        movl %eax,_mem(%edi)
        popl %ecx
	NEXTINSTR
	.align 4,0x90
INS3:	PREPARE
L22:    /* 0= */
	cmpl $0,_mem(%edi)
	sete %dl
	andl $255,%edx
	negl %edx
	movl %edx,_mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS4:	PREPARE
L25:    /* negate */
	negl _mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS5:	PREPARE
L26:    /* UM* */
        movl _mem(%edi),%eax
        movl _mem+4(%edi),%edx
        mull %edx
        movl %eax,_mem+4(%edi)
        movl %edx,_mem(%edi)
        NEXTINSTR
	.align 4,0x90
INS6:	PREPARE
L27:    /* C@ */
	movl _mem(%edi),%eax
	andl $MEMMASK,%eax
	xorb $3,%al
	movzbl _mem(%eax),%eax
	movl %eax,_mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS7:	PREPARE
L28:    /* @ */
	movl _mem(%edi),%edx
	andl $CELLMASK,%edx
	movl _mem(%edx),%edx
	movl %edx,_mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS8:	PREPARE 
L29:    /* + */
	movl _mem(%edi),%edx
        addl %edx,_mem+4(%edi)
        addl $4,%edi
        andl $CELLMASK,%edi
	NEXTINSTR
	.align 4,0x90
INS9:	PREPARE
L30:    /* and */
	movl _mem(%edi),%edx
        andl %edx,_mem+4(%edi)
        addl $4,%edi
        andl $CELLMASK,%edi
	NEXTINSTR
	.align 4,0x90
INS10:	PREPARE
L31:    /* or */
	movl _mem(%edi),%edx
        orl %edx,_mem+4(%edi)
        addl $4,%edi
        andl $CELLMASK,%edi
	NEXTINSTR
	.align 4,0x90
INS11:	PREPARE
L32:    /* xor */
	movl _mem(%edi),%edx
        xorl %edx,_mem+4(%edi)
        addl $4,%edi
        andl $CELLMASK,%edi
	NEXTINSTR
	.align 4,0x90
INS12:	PREPARE
L33:    /* U< */
        movl _mem(%edi),%edx
        cmpl _mem+4(%edi),%edx
	seta %al
        andl $255,%eax
        negl %eax
        movl %eax,_mem+4(%edi)
        addl $4,%edi
        andl $CELLMASK,%edi
        NEXTINSTR  
	.align 4,0x90
INS13:	PREPARE
L36:    /* < */
        movl _mem(%edi),%edx
        cmpl _mem+4(%edi),%edx
	setg %al
        andl $255,%eax
        negl %eax
        movl %eax,_mem+4(%edi)
        addl $4,%edi
        andl $CELLMASK,%edi
        NEXTINSTR 
	.align 4,0x90
INS14:	PREPARE
L39:    /* lshift */
	movl %ecx,%edx
        movl _mem(%edi),%ecx
	sall %cl,_mem+4(%edi)
        movl %edx,%ecx
        addl $4,%edi
        andl $CELLMASK,%edi
	NEXTINSTR
	.align 4,0x90
INS15:	PREPARE
L40:    /* rshift */
	movl %ecx,%edx
        movl _mem(%edi),%ecx
	shrl %cl,_mem+4(%edi)
        movl %edx,%ecx
        addl $4,%edi
        andl $CELLMASK,%edi
	NEXTINSTR
	.align 4,0x90
INS16:	PREPARE
L41:     /* UM/MOD */
	pushl %ebx
        movl _mem(%edi),%ebx
        movl _mem+4(%edi),%edx
        movl _mem+8(%edi),%eax
        cmpl %ebx,%edx  
        jae L42
        divl %ebx
        jmp L43 
L42:    andl $0x80000000,%ecx
	shll $1,%ebx
        incl %ebx
	or %ecx,%ebx
	movl $8,_interrupt
        jmp DOINT
	.align 4,0x90
L43:    movl %eax,_mem+4(%edi)
        movl %edx,_mem+8(%edi)
        popl %ebx
        addl $4,%edi
        andl $CELLMASK,%edi
        NEXTINSTR
	.align 4,0x90
INS17:	PREPARE
L44:    /* +CY */
        xorl %edx,%edx
        movb $255,%al
        adcb _mem(%edi),%al
        movl _mem+4(%edi),%eax
        adcl %eax,_mem+8(%edi)
        adcl $0,%edx
        movl %edx,_mem+4(%edi)        
	addl $4,%edi
        andl $CELLMASK,%edi
        NEXTINSTR
	.align 4,0x90
INS18:	PREPARE
L45:    /* SCAN1 */
        movl _mem(%edi),%edx    
        movl _mem+4(%edi),%eax
        andl %eax,%eax
        je L47
        andl %edx,%edx
        movl $-1,%edx
        je L46
L49:    shrl $1,%eax         
        incl %edx
        jnb L49
        movl %edx,_mem+4(%edi)
        jmp L48
L46:    sall $1,%eax
        incl %edx
        jnb L46
        movl %edx,_mem+4(%edi)
        jmp L48
L47:    movl $32,_mem+4(%edi)
L48:    addl $4,%edi
        andl $CELLMASK,%edi
        NEXTINSTR
	.align 4,0x90
INS19:	PREPARE
L54:    /* SPECIAL */
	movl _mem(%edi),%eax
	addl $4,%edi
        andl $CELLMASK,%edi
	cmpl $49,%eax
	jne L54A
        mov _mem(%edi),%ebx
	addl $4,%edi
	andl $CELLMASK,%edi
	mov _mem(%ebp),%esi
	addl $4,%ebp
	andl $CELLMASK,%ebp
	jmp RESTART
	.align 4,0x90  
L54A:
	movl %edi,_save_sp
	movl %esi,_save_ip
	movl %ebp,_save_rp
	pushl %ebx
        pushl %ecx
        pushl %eax 
	call _do_special
        addl $4,%esp
        popl %ecx
        popl %ebx
	movl _save_sp,%edi
	movl _save_ip,%esi
	movl _save_rp,%ebp
        testb $255,_interrupt
	je L54B
	andl $0x80000000,%ecx
        shrl $5,%ebx
	shll $1,%ebx
        incl %ebx
	or %ecx,%ebx
        jmp DOINT
	.align 4,090
L54B:
	NEXTINSTR
	.align 4,0x90
INS20:	PREPARE
L75:	/* DROP */
	addl $4,%edi
	andl $CELLMASK,%edi
	NEXTINSTR        
	.align 4,0x90
INS21:	PREPARE
L56:    /* >R */
        addl $-4,%ebp 
        andl $CELLMASK,%ebp
        movl _mem(%edi),%eax
        movl %eax,_mem(%ebp)
        addl $4,%edi
        andl $CELLMASK,%edi
	NEXTINSTR
	.align 4,0x90
INS22:	PREPARE
L57:    /* C!A */
        movl _mem(%edi),%edx
	movl _mem+4(%edi),%eax
        movl %edx,_mem+4(%edi)
        andl $MEMMASK,%edx
        xorb $3,%dl
        movb %al,_mem(%edx)      
        addl $4,%edi
        andl $CELLMASK,%edi
	NEXTINSTR
	.align 4,0x90
INS23:	PREPARE
L58:    /* !A */
        movl _mem(%edi),%edx
	movl _mem+4(%edi),%eax
        movl %edx,_mem+4(%edi)
        andl $CELLMASK,%edx
        movl %eax,_mem(%edx)      
        addl $4,%edi
        andl $CELLMASK,%edi
        NEXTINSTR
        .align 4,0x90
INS24:	PREPARE
L59:    /* DUP */
	addl $-4,%edi
        andl $CELLMASK,%edi
        movl _mem+4(%edi),%eax
        movl %eax,_mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS25:	PREPARE
L60:    /* OVER */
	addl $-4,%edi
        andl $CELLMASK,%edi
        movl _mem+8(%edi),%eax
        movl %eax,_mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS26:	PREPARE
L61:    /* R@ */
	addl $-4,%edi
        andl $CELLMASK,%edi
        movl _mem(%ebp),%eax
        movl %eax,_mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS27: 	PREPARE
L62:    /* R> */
	addl $-4,%edi
        andl $CELLMASK,%edi
        movl _mem(%ebp),%eax
        movl %eax,_mem(%edi)
        addl $4,%ebp
        andl $CELLMASK,%ebp
	NEXTINSTR
	.align 4,0x90
INS28:	PREPARE
L63:     /* 0 */
	addl $-4,%edi
        andl $CELLMASK,%edi
        movl $0,_mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS29:	PREPARE
L64:    /* 1 */
	addl $-4,%edi
        andl $CELLMASK,%edi
        movl $1,_mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS30:	PREPARE
L65:    /* 4 */
	addl $-4,%edi
        andl $CELLMASK,%edi
        movl $4,_mem(%edi)
	NEXTINSTR
	.align 4,0x90
INS31:	PREPARE
L66:    /* lit */
	addl $-4,%edi
        andl $CELLMASK,%edi
        andl $CELLMASK,%esi
        movl _mem(%esi),%eax
        addl $4,%esi
        movl %eax,_mem(%edi)
	NEXTINSTR
        .align 2,0x90
L18:
	testl $0x80000000,%ecx
	je L7 /* return instruction */
	movl _mem(%ebp),%esi
	addl $4,%ebp
        andl $CELLMASK,%ebp
	jmp L7
	.align 4,0x90
