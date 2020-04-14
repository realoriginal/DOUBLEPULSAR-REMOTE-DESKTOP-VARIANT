;
; Aligns the stack for x64 before 
; executing the entrypoint. 
;
BITS 64

extern c_EntryPoint
global asm_EntryPoint

segment .text

asm_EntryPoint:
	push rsi
	mov rsi, rsp
	and rsp, 0FFFFFFFFFFFFFFF0h
	sub rsp, 020h
	call c_EntryPoint
	mov rsp, rsi
	pop rsi
	ret		
