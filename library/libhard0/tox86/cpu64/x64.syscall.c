#include "libhard.h"
#define _ver_ hex32('v','e','r',0)
#define _exit_ hex32('e','x','i','t')
#define _take_ hex32('t','a','k','e')
#define _give_ hex32('g','i','v','e')


struct saved_cpureg{
	//[0,3f]
	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;
	//[40,77]
	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;
	u64 rbp;
	//[78,9f]
	u64 ip;
	u64 cs;
	u64 flag;
	u64 sp;
	u64 ss;
	//[a0,??]
}__attribute__((packed));


void syscall_ver()
{
	say("ver: date=%s,time=%s\n", __DATE__, __TIME__);
}
void syscall_exit()
{
	//thread or process: want exit
}
void syscall_read()
{
}
void syscall_write()
{
}




void syscall_handler(struct saved_cpureg* cpureg)
{
    say("@syscall: rax=%llx,rbx=%llx,rcx=%llx,rdx=%llx\n", cpureg->rax, cpureg->rbx, cpureg->rcx, cpureg->rdx);
	switch(cpureg->rax){
	case _ver_:syscall_ver();break;
	case _exit_:syscall_exit();break;
	case _take_:syscall_read();break;
	case _give_:syscall_write();break;
	}
}
void syscall_caller(u64 a, u64 b, u64 c, u64 d)
{
	asm("movq %0, %%rax\n"
		"movq %1, %%rbx\n"
		"movq %2, %%rcx\n"
		"movq %3, %%rdx\n"
		"int $0x80\n"
		:
		:"r"(a),"r"(b),"r"(c),"r"(d)
	);
}




/*
isr80_ring3to0:
	push rax

	mov       rax, 0x10		;kernel cs @ 10
	mov [rsp+0x10], rax
	mov       rax, 0x18		;kernel ss @ 18
	mov [rsp+0x28], rax

	pop rax
	iretq


global enterring3
enterring3:
	pop             rax		;user ip
	mov [rsp-0x28], rax
	mov       rax, 0x23		;user cs
	mov [rsp-0x20], rax
	mov      rax, 0x202		;rflag
	mov [rsp-0x18], rax
	mov        rax, rsp		;user sp
	mov [rsp-0x10], rax
	mov       rax, 0x2b		;user ss
	mov [rsp-0x08], rax

	sub rsp, 0x28
	iretq
*/


