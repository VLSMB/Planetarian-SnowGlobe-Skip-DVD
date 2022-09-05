// 参考教程：https://www.bilibili.com/video/BV1ba411Q7Tc?spm_id_from=333.999.0.0

#include <windows.h>
extern "C" void __declspec(dllexport) VLSMB()
{

}
DWORD orgAddr = 0;
DWORD retAddr = 0;

DWORD GetProcAddr(LPCWSTR dllName, LPCSTR procName)
{
	HMODULE hDLL = LoadLibraryW(dllName);
	if (hDLL == NULL)
		return NULL;
	FARPROC addr = GetProcAddress(hDLL, procName);
	if (addr == NULL)
	{
		FreeLibrary(hDLL);
		return NULL;
	}
	FreeLibrary(hDLL);
	return (DWORD)addr;
}

void WriteHookCode(DWORD tarAddr, DWORD orgAddr)
{
	DWORD oldProtect = 0;
	// 将Hook点地址可读可写
	VirtualProtect((LPVOID)orgAddr, 0x5, PAGE_EXECUTE_READWRITE, &oldProtect);
	// 计算RVA
	DWORD rvaAddr = tarAddr - orgAddr - 0x5;
	// 跳转指令
	BYTE code[5] = { 0xE9,0x90,0x90,0x90,0x90 };
	memcpy_s(&code[1], 4, &rvaAddr, 4);
	// 将指令写入Hook点
	memcpy_s((void*)orgAddr, 5, code, 5);
}

DWORD ImageBase = 0, curBase = 0, HookBase = 0;
DWORD RVA = 0x165F4E, tmpRVA = 0x263FBD;
BYTE buffer[5] = { 0x90, 0x90 ,0x90 ,0x90 ,0x90 };
BYTE orgASM[5] = { 0x8B ,0xFF ,0x55 ,0x8B ,0xEC };
DWORD temp = 0;

void __declspec(naked) MyHookCode()
{
	__asm
	{
		push eax
		mov eax, [esp + 4]
		mov[curBase], eax
		pop eax
		pushad
		pushfd
	}
	ImageBase = curBase - tmpRVA;
	HookBase = ImageBase + RVA;
	VirtualProtect((LPVOID)HookBase, 5, PAGE_EXECUTE_READWRITE, &temp);
	memcpy_s((void*)HookBase, 5, buffer, 5);
	memcpy_s((void*)orgAddr, 5, orgASM, 5);
	__asm
	{
		popfd
		popad
		mov edi, edi
		push ebp
		mov ebp, esp
		jmp retAddr
	}
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		orgAddr = GetProcAddr(L"kernel32.dll", "InterlockedDecrement");
		retAddr = orgAddr + 0x5;
		WriteHookCode((DWORD)MyHookCode, orgAddr);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
