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

enum Keys : BYTE {
	Key_A = 0x01,
	Key_B = 0x02,
	Key_X = 0x03,
	Key_Y = 0x04,
	Key_LB = 0x05,
	Key_RB = 0x06,
	Key_Start = 0x0E,
	Key_Back = 0x0F,
	Key_LS = 0x10,
	Key_RS = 0x11,
	Key_LT = 0x12,
	Key_RT = 0x13,
	Key_Up = 0x14,
	Key_Down = 0x15,
	Key_Left = 0x16,
	Key_Right = 0x17,
};

enum EntityType_t {
	ET_GENERAL,
	ET_PLAYER,
	ET_CORPSE,
	ET_ITEM,
	ET_MISSLE,
	ET_INVISIBLE_ENTITY,
	ET_SCRIPTMOVER,
	ET_SOUND_BLEND,
	ET_FX,
	ET_LOOP_FX,
	ET_PRIMARY_LIGHT,
	ET_TURRET,
	ET_HELICOPTER,
	ET_PLANE,
	ET_VEHICLE,
	ET_VEHICLE_COLLMAP,
	ET_VEHICLE_CORPSE,
	ET_VEHICLE_SPAWNER
};

enum Submenu {
	Closed,
	Main,
	Main_Options,
	ESP_Options,
	Aimbot_Options,
	Rebind_Options,
};

enum align {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,

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