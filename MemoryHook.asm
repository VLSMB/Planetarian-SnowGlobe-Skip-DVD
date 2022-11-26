	.386
	.model flat,stdcall
	option casemap:none
include windows.inc
include user32.inc
includelib user32.lib
include kernel32.inc
includelib kernel32.lib
	.data?
s_i	db 68 dup (0)
pi 	db 16 dup (0)
image_base db 4 dup (0)
Regs db 716 dup (0)
old db 4 dup (0)
	.const
pro db "SiglusEngine.exe",0
buf1 db 90H,90H,90H,90H,90H
buf2 db 0EBH,4
	.code
start:
	lea eax,Regs
	mov dword ptr [eax],CONTEXT_ALL
	lea eax,s_i
	mov dword ptr [eax],68
	lea eax,pi
	push eax
	lea eax,s_i
	push eax
	push 0
	push 0
	push 4
	push 0
	push 0
	push 0
	push 0
	lea eax,pro
	push eax
	call CreateProcessA
	lea eax,Regs
	push eax
	mov eax,dword ptr [pi+4]
	push eax
	call GetThreadContext
	push 0
	push 4
	lea eax,image_base
	push eax
	mov eax,dword ptr [Regs+164]
	add eax,8
	push eax
	mov eax,dword ptr [pi]
	push eax
	call ReadProcessMemory
	lea eax,old
	push eax
	push PAGE_EXECUTE_READWRITE
	push 5
	mov eax,dword ptr [image_base]
	add eax,165F4EH
	push eax
	mov eax,dword ptr [pi]
	push eax
	call VirtualProtectEx
	push 0
	push 5
	lea eax,buf1
	push eax
	mov eax,dword ptr [image_base]
	add eax,165F4EH
	push eax
	mov eax,dword ptr [pi]
	push eax
	call WriteProcessMemory
	mov eax,dword ptr [pi+4]
	push eax
	call ResumeThread
	push 0
	call ExitProcess
end start