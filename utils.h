#include "stdafx.h";

enum _XBOX_GAMES : DWORD {
	COD_BO2 = 0x415608C3,
	DASHBOARD = 0xFFFE07D1,
	COD_GHOSTS = 0x415608FC,
	COD_MW2 = 0x41560817,
	COD_MW3 = 0x415608CB,
	GTAV = 0x545408A7,
	COD_AW = 0x41560914,
	COD_BO1 = 0x41560855,
};

wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}
char* FtoS(float input)
{
	char returnvalue[64];
	int wholenumber = (int)input;
	input -= wholenumber;
	input *= 100;
	sprintf(returnvalue, "%d.%d", wholenumber, (int)input);
	return returnvalue;
}
char *ItoS(int num)
{
	char buf[30];
	sprintf(buf, "%i : 0x%X", num, num);
	return buf;
}
void wait(int seconds)
{
	clock_t endwait;
	endwait = clock() + seconds * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}

char vaBuffer[0x1000];
char* va(const char* fmt, ...) {
	memset(vaBuffer, 0, 0x1000);
	va_list ap;
	va_start(ap, fmt);
	vsprintf(vaBuffer, fmt, ap);
	va_end(ap);
	return vaBuffer;
}

template<typename ValueType, typename AddressType>
void WriteToAddress(AddressType address, ValueType value) {
	memcpy((void*)address, &value, sizeof(value));
}