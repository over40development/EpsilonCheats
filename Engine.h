#pragma once
#include "stdafx.h"
#include "kernel.h"
#include <stdio.h>
#include <vector>

// Variables
BOOL bInitialized = FALSE;
BOOL bInitializedMessage = FALSE;
XINPUT_STATE InputState;

void PatchInJump(DWORD* Address, DWORD Destination) {
	if (Destination & 0x8000)
		Address[0] = 0x3D600000 + (((Destination >> 16) & 0xFFFF) + 1);
	else
		Address[0] = 0x3D600000 + ((Destination >> 16) & 0xFFFF);

	Address[1] = 0x396B0000 + (Destination & 0xFFFF);
	Address[2] = 0x7D6903A6;
	Address[3] = 0x4E800420;
}
DWORD ResolveFunction(PCHAR ModuleName, DWORD Ordinal)
{
	HANDLE ModuleHandle; DWORD Address;
	XexGetModuleHandle(ModuleName, &ModuleHandle);
	XexGetProcedureAddress(ModuleHandle, Ordinal, &Address);
	return Address;
}
VOID DetourFunction(PDWORD FunctionAddress, DWORD DestinationFunction)
{
	if(DestinationFunction & 0x8000) {
		FunctionAddress[0] = 0x3D600000 + (((DestinationFunction >> 16) & 0xFFFF) + 1);
	}
	else {
		FunctionAddress[0] = 0x3D600000 + ((DestinationFunction >> 16) & 0xFFFF);
	}
	FunctionAddress[1] = 0x396B0000 + (DestinationFunction & 0xFFFF);
	FunctionAddress[2] = 0x7D6903A6;
	FunctionAddress[3] = 0x4E800420;
}
void __declspec(naked) GLPR(void)
{
	__asm
	{
		std     r14, -0x98(sp)
		std     r15, -0x90(sp)
		std     r16, -0x88(sp)
		std     r17, -0x80(sp)
		std     r18, -0x78(sp)
		std     r19, -0x70(sp)
		std     r20, -0x68(sp)
		std     r21, -0x60(sp)
		std     r22, -0x58(sp)
		std     r23, -0x50(sp)
		std     r24, -0x48(sp)
		std     r25, -0x40(sp)
		std     r26, -0x38(sp)
		std     r27, -0x30(sp)
		std     r28, -0x28(sp)
		std     r29, -0x20(sp)
		std     r30, -0x18(sp)
		std     r31, -0x10(sp)
		stw     r12, -0x8(sp)
		blr
	}
}
DWORD RelinkGPLR(DWORD SFSOffset, PDWORD SaveStubAddress, PDWORD OriginalAddress)
{
	DWORD Instruction = 0, Replacing;
	PDWORD Saver = (PDWORD)GLPR;
	if (SFSOffset & 0x2000000)
		SFSOffset = SFSOffset | 0xFC000000;
	Replacing = OriginalAddress[SFSOffset / 4];
	for (int i = 0; i < 20; i++) {
		if (Replacing == Saver[i]) {
			int NewOffset = (int)&Saver[i] - (int)SaveStubAddress;
			Instruction = 0x48000001 | (NewOffset & 0x3FFFFFC);
		}
	}
	return Instruction;
}
VOID HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination)
{
	if ((SaveStub != NULL) && (Address != NULL))
	{
		DWORD AddressRelocation = (DWORD)(&Address[4]);

		SaveStub[0] = ((AddressRelocation & 0x8000) ? (0x3D600000 + (((AddressRelocation >> 16) & 0xFFFF) + 1)) : (0x3D600000 + ((AddressRelocation >> 16) & 0xFFFF)));
		SaveStub[1] = 0x396B0000 + (AddressRelocation & 0xFFFF);
		SaveStub[2] = 0x7D6903A6;

		for (int i = 0; i < 4; i++)
			if ((Address[i] & 0x48000003) == 0x48000001)
				SaveStub[i + 3] = RelinkGPLR((Address[i] & ~0x48000003), &SaveStub[i + 3], &Address[i]);
			else
				SaveStub[i + 3] = Address[i];

		SaveStub[7] = 0x4E800420;
		__dcbst(0, SaveStub);
		__emit(0x7C0004AC);
		__emit(0x4C00012C);
		memcpy((void*)SaveStub, SaveStub, 8 * 4);
		PatchInJump(Address, Destination);
	}
}

VOID (__cdecl *XNotifyQueueUI)(DWORD dwType, DWORD dwUserIndex, DWORD dwPriority, LPCWSTR pwszStringParam, ULONGLONG qwParam) = (VOID (__cdecl *)(DWORD, DWORD, DWORD, LPCWSTR, ULONGLONG))ResolveFunction("xam.xex", 0x290);
VOID XNotifyThread(PWCHAR NotifyText)
{ XNotifyQueueUI(14, 0, 2, NotifyText, NULL); }
VOID XNotify(PWCHAR NotifyText)
{
	if(KeGetCurrentProcessType() != USER_PROC)
	{ CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)XNotifyThread, (LPVOID)NotifyText, 0 ,NULL); }
	else { XNotifyThread(NotifyText); }
}
DWORD (__cdecl *XamGetCurrentTitleId)() = (DWORD (__cdecl *)())ResolveFunction("xam.xex", 0x1CF);