#include "stdafx.h"
#include "Engine.h"
#include "utils.h"

#pragma region Classes & Structs
class Material {
public:
	const char* materialName;
};
typedef struct {
	float x;
	float y;
	float z;
}Vector3;
typedef struct {
	float x;
	float y;
}Vector2;
struct RefDef_t {
	int x;
	int y;
	int Width;						//0x8
	int Height;						//0xC
	Vector2 Fov;					//0x10
	Vector3 Origin;					//0x18
	Vector3 ViewAxis[3];				//0x24
	char _0x0048[0x24];				//0x48
	float ZoomProgress;				//0x6C
	char _0x0070[0x4ABC];			//0x70
	Vector3 ViewAngles;				//0x4B28
	char _0x4B34[0x30];				//0x4B34
	Vector2 WeaponViewAngle;		//0x4B68
}; 
struct clientInfo_t {
	int Valid;						//0x0
	char _0x0004[0x8];				//0x4
	char Name[0x20];				//0xC
	int Team;						//0x2C,
	char _0x0030[0x4];
	int Rank;
	int Prestige;
	char _0x003C[0x24];
	char Model[0x40];
	char HeadModel[0x40];
	char _0x00E0[0x3D0];
	int Attacking;
	char _0x04B4[0x4];
	int Zooming;
	char _0x04BC[0x60];
	int Weapon;
	char _0x0520[0x74];
};
struct cgArray_t {
	int commandtime;                //0x0
	char padding00[0x8];            //0x4
	int pm_flags;					//0xC
	char padding01[0xC];            //0x10
	Vector3 Origin;                  //0x1C
	char padding02[0x128];          //0x28
	int clientNum;					//0x150
	char padding03[0x4];            //0x154
	Vector3 viewAngles;				//0x158
	char padding04[0x38];			//0x164
	int Health;                     //0x19C   
	char padding08[0x4];			//0x1A0
	int MaxHealth;					//0x1A4
	char padding05[0x1D4];          //0x1A8
	float aimSpreadScale;           //0x37C
	char padding06[0x6B72C];        //0x380
	RefDef_t refdef_s;              //0x6BAAC
	char padding07[0x8D96C];		//0x7061C
	clientInfo_t clientInfo[18];	//0xFDF88 // + 0X48, 8263EE60
};
struct serverInfo_t {
	char _0x0000[0x24];				//0x0
	char Gametype[0x20];			//0x24 
	char HostName[0x20];			//0x44 
	char _0x0064[0xE4];				//0x64
	int MaxClients;					//0x148 
	int PrivateClients;				//0x14C 
	char _0x0150[0x8];				//0x150
	char Mapname[0x34];				//0x158 
};
struct Centity_t {
	char _0x0000[0x2];				//0x0
	char Alive;						//0x2 
	char _0x0003[0x11];				//0x3
	Vector3 Origin;					//0x14 
	Vector3 Angles;					//0x20 
	char _0x002C[0x30];				//0x2C
	int Flags;						//0x5C 
	char _0x0060[0x64];				//0x60
	int Handle;						//0xC4
	EntityType_t Type;				//0xC8
	char _0x00CC[0x88];				//0xCC
	int ClientNumber;				//0x154 
	char _0x0158[0x34];				//0x158
	int WeaponIndex;				//0x18C 
	char _0x0190[0x5C];				//0x190
	//(0x826436B8 + (i * 0x1EC)) + 0x18C
};
struct usercmd_s {
	int time; //0-4
	unsigned int button; //4-8
	Vector3 viewAngles; //8-14
	int weapon; //14 - 18
	int offHand; //18-1c
	char forwardmove; //1c
	char rightmove; //1d
	unsigned short airburstMarkDistance; //1E - 20
	unsigned short meleeChargeEnt; //20 - 22
	char meleeChargeDist; //22
	char selectedLoc[2]; //23 - 25
	char selectedLocAngle; //26
	unsigned short sightedClientsMask; //26 - 2A
	unsigned short spawnTraceEntIndex; //2A - 2C
};
struct clientActive_t {
	bool usingAds;
	char padding00[0xDF];			//0x1
	float BaseAngles[2];			//0xE0
	char paddig00[0x34C8];			//0xE8
	float ViewAngles[2];			//0x35B0
	char padding01[0x4];			//0x35B8
	usercmd_s Usercmd[128];			//0x35BC
	int curCmd;						//0x4BBC

	usercmd_s* getCmd(int cmdNum) {
		return &Usercmd[cmdNum & 0x7F];
	}
};
struct WeaponDef {
	char* weaponName; //0x0
	char _0x4[0x80]; //0x4
	Material* killIcon; //0x84
};

struct RGBA {
	int r;
	int g;
	int b;
	int a;
};
class CRadarHud
{
public:
	float x;
	float y;
	float w;
	float h;
	char _0x0010[12];
};

RGBA rgba(int r, int g, int b, int a) {
	RGBA clrTmp = { r,g,b,a };
	return clrTmp;
}
RGBA MenuColor = { 17, 99, 222, 255 };

CRadarHud radarhud; 
cgArray_t* cg_t;
serverInfo_t* cgs_t;
Centity_t* centity;
clientActive_t* active;

struct UIItems {
	int currentOption;
	int maxOptions;
	int optionCount;
	int submenu;
	int submenuLevel;
	int lastSubmenu[20];
	int lastOption[20];
	bool optionPress;
	bool leftPress;
	bool rightPress;
}UIItems;
#pragma endregion

struct Toggles {
	bool ClosestPlayerIsReadyForUberHax;
	int SelectedPlayer;

	bool ESP_Required;
	bool ESP_Name;
	bool ESP_RedBoxes;
	bool ESP_WeaponIcon;
	bool ESP_Bone;
	bool ESP_2DBox;
	bool ESP_3DBox;
	bool ESP_Snaplines;
	int ESP_Snaplines_Type;//0 = center, 1 = top, 2 = bottom

	bool Wallhack;
	bool NoRecoil;
	bool UnlimitedUAV;
	bool Laser;
	bool NoSpread;

	bool DeleteOriginalRadar;
	bool SetNewRadarZoom;

	bool Aimbot_Required;
	bool Aimbot_Stealth;
	bool Aimbot_AutoAim;
	bool Aimbot_AutoZoom;
	bool Aimbot_AutoShoot;
	int Aimbot_Bone;
	bool Aimbot_CrouchBot;

	bool Shoot;
	bool Zoom;
	bool Crouch;
}Toggles;

namespace OptionItems {
	char* SnaplinePositions[] = { "Center", "Top", "Bottom" };
	char* Bones[] = { "j_knee_ri", "j_knee_le", "j_elbow_ri", "j_elbow_le", "j_ankle_ri", "j_ankle_le", "j_wrist_ri", "j_wrist_le", "pelvis", "j_neck", "j_head", "j_mainroot" };
	char* RebindKeys[] = { "A", "B", "X", "Y", "LB", "RB", "Start", "Back", "LS", "RS", "LT", "RT", "Up", "Down", "Left", "Right" };
	int RebindKeyValues[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
}
namespace Math {
#define PI (3.1415926535897931)
	double atan(double x, int n){
		double a = 0.0;
		double sum = 0.0;

		if (x == 1.0) return PI / 4.0;
		if (x == -1.0) return -PI / 4.0;

		if (n > 0){
			if ((x < -1.0) || (x > 1.0)){
				if (x > 1.0)
					sum = PI / 2.0;
				else
					sum = -PI / 2.0;
				a = -1.0 / x;
				for (int j = 1; j <= n; j++){
					sum += a;
					a *= -1.0*(2.0*j - 1) / ((2.0*j + 1)*x*x);
				}
			} else {
				sum = 0.0;
				a = x;
				for (int j = 1; j <= n; j++){
					sum += a;
					a *= -1.0*(2.0*j - 1)*x*x / (2.0*j + 1);
				}
			}
		}

		return sum;
	}
	double atan2(double y, double x){
		double u = atan(y / x, 24);
		if (x < 0.0){
			if (u > 0.0)
				u -= PI;
			else
				u += PI;
		}
		return u;
	}
}
namespace Internal {
	void *R_RegisterMaterial(const char* font, int imageTrac) {
		return ((void*(*)(const char*, int))0x82413E80)(font, imageTrac);
	}
	void *R_RegisterFont(const char* font, int imageTrac) {
		return ((void*(*)(const char*, int))0x824079C8)(font, imageTrac);
	}
	void *R_AddCmdDrawStretchPic(float x, float y, float width, float height, float xScale, float yScale, float xay, float yay, const float * color, void * material) {
		return ((void*(*)(float, float, float, float, float, float, float, float, const float*, void*))0x8241F038)(x, y, width, height, xScale, yScale, xay, yay, color, material);
	}
	void R_AddCmdDrawText(const char *text, int maxChars, void *font, float x, float y, float xScale, float yScale, float rotation, const float *color, int style) {
		((void(*)(const char *, int, void*, float, float, float, float, float, const float*, int))0x8241F918)(text, maxChars, font, x, y, xScale, yScale, rotation, color, style);
	}
	int R_TextWidth(const char* text, const char* fontName) {
		return ((int(*)(const char*, int, void*))0x82407A38)(text, 0x7FFFFFFF, R_RegisterFont(fontName, 0));
	}
	void DrawEngineRadar(int a1, int a2, int a3, CRadarHud* radar, void* shader, const float * color) {
		((void(*)(int, int, int, CRadarHud*, void*, const float*))0x8210AC98)(a1, a2, a3, radar, shader, color);//+ 99920
	}
	void DrawEngineRadarPackages(int a1, int a2, int a3, CRadarHud* radar, void* shader, const float * color) {
		((void(*)(int, int, int, CRadarHud*, void*, const float*))0x8210C328)(a1, a2, a3, radar, shader, color);
	}
	void DrawEngineRadarSelfIcon(int a1, int a2, int a3, CRadarHud* radar, const float * color) {
		((void(*)(int, int, int, CRadarHud*, const float*))0x8210B540)(a1, a2, a3, radar, color);
	}
	void DrawEngineRadarFriendlyTeam(int a1, int a2, int a3, CRadarHud* radar, const float * color) {
		((void(*)(int, int, int, CRadarHud*, const float*))0x8210DB68)(a1, a2, a3, radar, color);
	}
	void DrawEngineRadarEnemyTeam(int a1, int a2, int a3, CRadarHud* radar, const float * color) {
		((void(*)(int, int, int, CRadarHud*, const float*))0x8210E3E0)(a1, a2, a3, radar, color);
	}
	void DrawEngineRadarPlane(int a1, int a2, int a3, CRadarHud* radar, const float * color) {
		((void(*)(int, int, int, CRadarHud*, const float*))0x8210B818)(a1, a2, a3, radar, color);
	}
	void DrawEngineRadarHelicopter(int a1, int a2, int a3, CRadarHud* radar, const float * color) {
		((void(*)(int, int, int, CRadarHud*, const float*))0x823AD5B0)(a1, a2, a3, radar, color);
	}
	void DrawEngineRadarPredatorMissile(int a1, int a2, int a3, CRadarHud* radar, const float * color) {
		((void(*)(int, int, int, CRadarHud*, const float*))0x8210FE10)(a1, a2, a3, radar, color);
	}
	void DrawEngineRadarTurret(int a1, int a2, int a3, CRadarHud* radar, const float * color) {
		((void(*)(int, int, int, CRadarHud*, const float*))0x8210E9A8)(a1, a2, a3, radar, color);
	}
	bool Dvar_GetBool(const char* Dvar) {
		return ((bool(*)(const char*))0x8232E200)(Dvar);
	}
	const char* Dvar_GetString(const char* Dvar) {
		return ((const char*(*)(const char*))0x8232E3C0)(Dvar);
	}
	void Cbuf_AddText(int lc, char* cmd) {
		((void(*)(int, char*))0x82287EE0)(lc, cmd);
	}
	void *CG_DrawRotatedPicPhysical(int mask, float x, float y, float l2, float width, float angle, const float * color, void* material) {
		return ((void*(*)(int, float, float, float, float, float, const float *, void*))0x82117668)(mask, x, y, l2, width, angle, color, material);
	}
	bool CG_DObjGetWorldTagPos(Centity_t* a, int b, unsigned int c, Vector3* d) {
		return ((bool(*)(Centity_t*, int, unsigned int, Vector3*))0x8211BCB8)(a, b, c, d);
	}
	int Com_GetClientDObj(int handle, int localClientNum) {
		return ((int(*)(int, int))0x822A2848)(handle, localClientNum);
	}
	int CL_GetCurrentCmdNumber(int unk) {
		return ((int(*)(int))0x82164170)(unk);
	}
	int SL_GetString(const char *tag) {//RegisterTag ?
		return ((int(*)(const char*))0x822B5BF8)(tag);
	}
	bool CG_CanSeePlayer(int localClientNum, Centity_t* cent, int contentMask) {
		return ((bool(*)(int, Centity_t*, int))0x8215D848)(localClientNum, cent, contentMask);
	}
	void ThrowGameException(int type, char*text, int r5) {
		//r3 is usually 1, but i found it to be from 0 - 8
		((void(*)(int, char*, int))0x822A1770)(type, text, r5);
	}
	void SV_GameSendServerCommand(int client, int type, char* cmd) {
		((void(*)(int, int, char*))0x82272830)(client, type, cmd);
	}
}
namespace Gui {
	void DrawShader(float x, float y, float width, float height, RGBA color, const char * material) {
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::R_AddCmdDrawStretchPic(x, y, width, height, 1, 1, 1, 1, flColor, Internal::R_RegisterMaterial(material, 0));
	}
	void DrawText(const char *xtext, float xx, float xy, const char *xfont, float xfontSize, RGBA color) {
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::R_AddCmdDrawText(xtext, 0x7FFFFFFF, Internal::R_RegisterFont(xfont, 0), xx, xy, xfontSize, xfontSize, 0, flColor, 0);
	}
	int getXValue(int X, const char* text, float scale, align align, const char* font) {
		if (align == ALIGN_LEFT)
			return X;
		if (align == ALIGN_CENTER)
			return X - (Internal::R_TextWidth(text, font) / 2 * scale);
		if (align == ALIGN_RIGHT)
			return X - (Internal::R_TextWidth(text, font) * scale);
	}
	void TopRightInfo(char*text, int id) {
		DrawShader((1270) - (Internal::R_TextWidth(text, "fonts/normalfont")*0.60 + 7), 25 + (id * 20 - 20), Internal::R_TextWidth(text, "fonts/normalfont")*0.60 + 7, 22, rgba(0, 0, 0, 100), "white");//background header

		DrawShader((1270) - (Internal::R_TextWidth(text, "fonts/normalfont")*0.60 + 7), 25 + (id * 20 - 20), 2, 22, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//left side bar
		DrawShader((1270), 25 + (id * 20 - 20), 2, 22, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//right side bar
		DrawShader((1270) - (Internal::R_TextWidth(text, "fonts/normalfont")*0.60 + 7), 25 + (id * 20 - 20), (Internal::R_TextWidth(text, "fonts/normalfont")*0.60 + 7), 2, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//top header
		DrawShader((1270) - (Internal::R_TextWidth(text, "fonts/normalfont")*0.60 + 7), 25 + (id * 20), (Internal::R_TextWidth(text, "fonts/normalfont")*0.60 + 7), 2, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//top header

		Gui::DrawText(text, Gui::getXValue(1270, text, 0.60, ALIGN_RIGHT, "fonts/normalfont"), 26.5 + (id * 20), "fonts/normalfont", 0.60, rgba(255, 255, 255, 255));
	}
	void DrawGUI() {
		DrawShader(547, 57, 267, 2, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//background header
		if (UIItems.optionCount > UIItems.maxOptions){
			DrawShader(547, 58, 267, (UIItems.maxOptions * 23 + 14), rgba(0, 0, 0, 100), "white");//background
			DrawShader(547, 58, 2, (UIItems.maxOptions * 23 + 14), rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//left side bar
			DrawShader(812, 58, 2, (UIItems.maxOptions * 23 + 14), rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//right side bar
			DrawShader(547, (UIItems.maxOptions * 23 + 14), 267, 2, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//bottom bar
		} else {
			DrawShader(547, 58, 267, (UIItems.optionCount * 23 + 14), rgba(0, 0, 0, 100), "white");//background
			DrawShader(547, 58, 2, (UIItems.optionCount * 23 + 14), rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//left side bar
			DrawShader(812, 58, 2, (UIItems.optionCount * 23 + 14), rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//right side bar
			DrawShader(547, ((UIItems.optionCount + 2) * 23 + 25), 267, 2, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//bottom bar
		}

	}
}
namespace Functions {
	void GrabStructs() {
		cg_t = *(cgArray_t**)0x8263EE60;// + 0xFDF88
		cgs_t = *(serverInfo_t**)0x8263EE44;
		centity = *(Centity_t**)0x826436B8;
		active = *(clientActive_t**)0x82713DC4;
	}
	WeaponDef* BG_GetWeaponDef(int weaponId) {
		int WeapDefId = ((int(*)(...))0x820D5A10)(weaponId, 0);
		return *(WeaponDef**)(0x825E0318 + ((WeapDefId * 4) & 0x3FC));
	}
	Material* GetWeaponShader(int weaponId) {
		WeaponDef *WeapDef = BG_GetWeaponDef(weaponId);
		return WeapDef->killIcon;
	}
	float GetDistanceBetweenVector2Coords(Vector2 one, Vector2 two) {
		float x[2], y[2];
		double P1, P2, total, result;

		P1 = (x[1] - x[0]);
		P2 = (y[1] - y[0]);
		P1 *= P1;
		P2 *= P2;
		result = (P1 + P2);
		total = sqrt(result);
		return total;
	}
	void DrawRadar(int x, int y, int w, int h, RGBA color) {
		radarhud.x = x;
		radarhud.y = y;
		radarhud.w = w;
		radarhud.h = h;
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::DrawEngineRadar(0, 0, 1879174336, &radarhud, "white", flColor);
	}
	void DrawEngineRadarPackages(int x, int y, int w, int h, RGBA color) {
		radarhud.x = x;
		radarhud.y = y;
		radarhud.w = w;
		radarhud.h = h;
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::DrawEngineRadarPackages(0, 0, 0, &radarhud, "white", flColor);
	}
	void DrawRadarSelfIcon(int x, int y, int w, int h, RGBA color) {
		radarhud.x = x;
		radarhud.y = y;
		radarhud.w = w;
		radarhud.h = h;
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::DrawEngineRadarSelfIcon(0, 0, 0, &radarhud, flColor);
	}
	void DrawEngineRadarFriendlyTeam(int x, int y, int w, int h, RGBA color) {
		radarhud.x = x;
		radarhud.y = y;
		radarhud.w = w;
		radarhud.h = h;
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::DrawEngineRadarFriendlyTeam(0, 0, 0, &radarhud, flColor);
	}
	void DrawEngineRadarEnemyTeam(int x, int y, int w, int h, RGBA color) {
		radarhud.x = x;
		radarhud.y = y;
		radarhud.w = w;
		radarhud.h = h;
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::DrawEngineRadarEnemyTeam(0, 0, 0, &radarhud, flColor);
	}
	void DrawEngineRadarPlane(int x, int y, int w, int h, RGBA color) {
		radarhud.x = x;
		radarhud.y = y;
		radarhud.w = w;
		radarhud.h = h;
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::DrawEngineRadarPlane(0, 0, 0, &radarhud, flColor);
	}
	void DrawEngineRadarHelicopter(int x, int y, int w, int h, RGBA color) {
		radarhud.x = x;
		radarhud.y = y;
		radarhud.w = w;
		radarhud.h = h;
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::DrawEngineRadarHelicopter(0, 0, 0, &radarhud, flColor);
	}
	void DrawEngineRadarPredatorMissile(int x, int y, int w, int h, RGBA color) {
		radarhud.x = x;
		radarhud.y = y;
		radarhud.w = w;
		radarhud.h = h;
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::DrawEngineRadarPredatorMissile(0, 0, 0, &radarhud, flColor);
	}
	void DrawEngineRadarTurret(int x, int y, int w, int h, RGBA color) {
		radarhud.x = x;
		radarhud.y = y;
		radarhud.w = w;
		radarhud.h = h;
		float flColor[4] = { (float)color.r / 255.0f,(float)color.g / 255.0f,(float)color.b / 255.0f,(float)color.a / 255.0f };
		Internal::DrawEngineRadarTurret(0, 0, 0, &radarhud, flColor);
	}
	void DrawLine(float X1, float Y1, float X2, float Y2, RGBA color, void * Shader, float Width){
		float X, Y, Angle, L1, L2, H1;
		H1 = Y2 - Y1;
		L1 = X2 - X1;
		L2 = sqrt(L1 * L1 + H1 * H1);
		X = X1 + ((L1 - L2) / 2);
		Y = Y1 + (H1 / 2);
		Angle = (float)atan(H1 / L1) * (180 / 3.14159265358979323846);
		float flColor[4] = { (float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, (float)color.a / 255.0f };
		Internal::CG_DrawRotatedPicPhysical(0x827322C0, X, Y, L2, Width, Angle, flColor, Shader);
	}
	bool AimTarget_GetTagPos(Centity_t *ent, Vector3* pos, int tag) {
		return Internal::CG_DObjGetWorldTagPos(ent, Internal::Com_GetClientDObj(ent->Handle, 0), tag, pos);
	}
	Vector3 GetTagPos(int ClientNumber, const char* Tag) {
		Vector3 OutPut;
		if (AimTarget_GetTagPos(&centity[ClientNumber], &OutPut, Internal::SL_GetString(Tag)))
			return OutPut;
	}
	bool AliveCheck(int clientNum) {
		return centity[clientNum].Alive && strcmp(cg_t->clientInfo[clientNum].HeadModel, "");
	}
	bool IsFriendly(int localClient, int targetClient) {
		return cg_t->clientInfo[localClient].Team == cg_t->clientInfo[targetClient].Team && strcmp(Internal::Dvar_GetString("ui_gametype"), "dm");
	}
	float GetDistance(Vector3 c1, Vector3 c2) {
		float dx = c2.x - c1.x;
		float dy = c2.y - c1.y;
		float dz = c2.z - c1.z;

		return sqrt((float)((dx * dx) + (dy * dy) + (dz * dz)));
	}
	float DotProduct(Vector3 hax, Vector3 Vector) {
		return (hax.x * Vector.x) + (hax.y * Vector.y) + (hax.z * Vector.z);
	}
	Vector3 FloatToVector3(float in0, float in1, float in2){
		Vector3 out;
		out.x = in0;
		out.y = in1;
		out.z = in2;
		return out;
	}
	Vector2 FloatToVector2(float in0, float in1) {
		Vector2 out;
		out.x = in0;
		out.y = in1;
		return out;
	}
	Vector3 SubstractVector(Vector3 Vec1, Vector3 Vec2) {
		Vector3 Output;
		Output.x = Vec1.x - Vec2.x;
		Output.y = Vec1.y - Vec2.y;
		Output.z = Vec1.z - Vec2.z;
		return Output;
	}
	void WorldPosToCompassPos(Vector3 world, Vector2* compass) {
		float Radar[4] = { 112.5, 112.5, 225, 225 };
		float pixPerInch = Radar[3] / 1.5f; //<--- compassMaxRange value
		float ViewAngle = cg_t->viewAngles.y / 180 * PI;

		Vector2 Difference;
		Difference.x = pixPerInch * (cg_t->refdef_s.Origin.x - world.x);
		Difference.y = pixPerInch * (cg_t->refdef_s.Origin.y - world.y);

		Vector2 Screen;
		Screen.x = Radar[0] + (Difference.y * cosf(ViewAngle) - Difference.x * sinf(ViewAngle));
		Screen.y = Radar[1] + (Difference.x * cosf(ViewAngle) + Difference.y * sinf(ViewAngle));

		if (Screen.x < Radar[0] - (Radar[2] / 2)) Screen.x = Radar[0] - (Radar[2] / 2);
		if (Screen.x > Radar[0] + (Radar[2] / 2)) Screen.x = Radar[0] + (Radar[2] / 2);
		if (Screen.y < Radar[1] - (Radar[3] / 2)) Screen.y = Radar[1] - (Radar[3] / 2);
		if (Screen.y > Radar[1] + (Radar[3] / 2)) Screen.y = Radar[1] + (Radar[3] / 2);

		compass->x = Screen.x;
		compass->y = Screen.y;
	}
	void VectorToAngle(Vector3 *dir, Vector3 *save) {
		float forward;
		float yaw, pitch;
		if (dir->x == 0 && dir->y == 0){
			yaw = 0;
			if (dir->z > 0) pitch = 90.00;
			else pitch = 270.00;
		} else {
			if (dir->x != -1) yaw = (float)(Math::atan2((double)dir->y, (double)dir->x) * 180.00 / PI);
			else if (dir->y > 0) yaw = 90.00;
			else yaw = 270;
			if (yaw < 0) yaw += 360.00;
			forward = (float)sqrt((double)(dir->x * dir->x + dir->y * dir->y));
			pitch = (float)(Math::atan2((double)dir->z, (double)forward) * 180.00 / PI);
			if (pitch < 0) pitch += 360.00;
		}
		save->x = -pitch;
		save->y = yaw;
		save->z = 0;
	}
	bool WorldToScreen(Vector3 World, float *ScreenX, float *ScreenY) {
		Vector3 Position = SubstractVector(World, cg_t->refdef_s.Origin);
		Vector3 Transform;

		Transform.x = DotProduct(Position, cg_t->refdef_s.ViewAxis[1]);
		Transform.y = DotProduct(Position, cg_t->refdef_s.ViewAxis[2]);
		Transform.z = DotProduct(Position, cg_t->refdef_s.ViewAxis[0]);

		if (Transform.z < 0.1f)
			return false;

		Vector2 Center = { (float)cg_t->refdef_s.Width * 0.5f, (float)cg_t->refdef_s.Height * 0.5f };

		*ScreenX = Center.x * (1 - (Transform.x / cg_t->refdef_s.Fov.x / Transform.z));
		*ScreenY = Center.y * (1 - (Transform.y / cg_t->refdef_s.Fov.y / Transform.z));

		return true;
	}
	int Key_IsDown(int ClientNum, int ButtonEnum) {
		return ((int(*)(int, int))0x8216ED28)(ClientNum, ButtonEnum);
	}
	void BoneOnPlayer(int client, char*startBone, char*endBone, RGBA color) {
		float SX, SY, EX, EY;
		Vector3 Start = GetTagPos(client, startBone);
		Vector3 Finish = GetTagPos(client, endBone);
		if(WorldToScreen(Start, &SX, &SY) && WorldToScreen(Finish, &EX, &EY))
			DrawLine(SX, SY, EX, EY, color, Internal::R_RegisterMaterial("white", 0), 1.0);
	}
	void WeaponOnScreenOnPlayer(int clientNum) {
		float Blue[4] = { (float)0, (float)0, (float)255, (float)255 / 255.0f };
		float Yellow[4] = { (float)255 / 255.0f,(float)255 / 255.0f,(float)0, (float)255 / 255.0f };

		Vector2 screen;
		Vector3 world = centity[clientNum].Origin;
		if (WorldToScreen(world, &screen.x, &screen.y)) {
			if (centity[clientNum].WeaponIndex) {
				Material* WeaponShader = GetWeaponShader(centity[clientNum].WeaponIndex);
				if (Internal::CG_CanSeePlayer(0, &centity[clientNum], 0x803003))
					Internal::CG_DrawRotatedPicPhysical(0x827322C0, screen.x - 20, screen.y - 15, 40, 25, 0, Blue, WeaponShader);
				else
					Internal::CG_DrawRotatedPicPhysical(0x827322C0, screen.x - 20, screen.y - 15, 40, 25, 0, Yellow, WeaponShader);
			}
		}
	}
	void ESP() {
		for (int i = 0; i < 18; ++i) {
			if (i != cg_t->clientNum) {
				if (AliveCheck(i) && !IsFriendly(cg_t->clientNum, i)) {
					if (Toggles.ESP_Bone) {
						BoneOnPlayer(i, "j_knee_le", "pelvis", rgba(255, 255, 255, 255));
						BoneOnPlayer(i, "j_elbow_ri", "j_wrist_ri", rgba(255, 255, 255, 255));
						BoneOnPlayer(i, "j_neck", "j_elbow_ri", rgba(255, 255, 255, 255));
						BoneOnPlayer(i, "j_ankle_ri", "j_knee_ri", rgba(255, 255, 255, 255));
						BoneOnPlayer(i, "j_elbow_le", "j_wrist_le", rgba(255, 255, 255, 255));
						BoneOnPlayer(i, "j_ankle_le", "j_knee_le", rgba(255, 255, 255, 255));
						BoneOnPlayer(i, "j_knee_ri", "pelvis", rgba(255, 255, 255, 255));
						BoneOnPlayer(i, "pelvis", "j_neck", rgba(255, 255, 255, 255));
						BoneOnPlayer(i, "j_neck", "j_elbow_le", rgba(255, 255, 255, 255));
						BoneOnPlayer(i, "j_neck", "j_head", rgba(255, 255, 255, 255));
					}
					float X, Y;
					if (WorldToScreen(GetTagPos(i, "j_mainroot"), &X, &Y)) {
						if (Toggles.ESP_2DBox) {
							if (Internal::CG_CanSeePlayer(0, &centity[i], 0x803003))
								Gui::DrawShader(X + -12, Y + -18, 27, 38, rgba(0, 0, 255, 100), "white");
							else
								Gui::DrawShader(X + -12, Y + -18, 27, 38, rgba(255, 255, 0, 100), "white");

							Gui::DrawShader(X + 14, Y + -18, 1, 38, rgba(0, 0, 0, 150), "white");
							Gui::DrawShader(X + -12, Y + -18, 1, 38, rgba(0, 0, 0, 150), "white");
							Gui::DrawShader(X + -12, Y + -18, 27, 1, rgba(0, 0, 0, 150), "white");
							Gui::DrawShader(X + -12, Y + 19, 27, 1, rgba(0, 0, 0, 150), "white");
						}
						if (Toggles.ESP_Name) {
							float NameX, NameY;
							if (WorldToScreen(GetTagPos(i, "j_head"), &NameX, &NameY)) {
								Gui::DrawShader(Gui::getXValue(NameX, cg_t->clientInfo[i].Name, 0.30, ALIGN_CENTER, "fonts/normalfont"), NameY - 15, (Internal::R_TextWidth(cg_t->clientInfo[i].Name, "fonts/normalfont") * 0.30 + 2), 10, rgba(0, 0, 0, 150), "white");
								Gui::DrawText(cg_t->clientInfo[i].Name, Gui::getXValue(NameX, cg_t->clientInfo[i].Name, 0.30, ALIGN_CENTER, "fonts/normalfont"), NameY - 5, "fonts/normalfont", 0.30, rgba(255, 255, 255, 255));
							}
						}
						if (Toggles.ESP_WeaponIcon) {
							WeaponOnScreenOnPlayer(i);
						}
						if (Toggles.ESP_Snaplines) {
							float ToTimes = 0.5f;
							switch (Toggles.ESP_Snaplines_Type) {
							case 0:
								ToTimes = 0.5f;
								break;
							case 1:
								ToTimes = 0.0f;
								break;
							case 2:
								ToTimes = 1.0;
								break;
							}
							if(Internal::CG_CanSeePlayer(0, &centity[i], 0x803003))
								DrawLine(cg_t->refdef_s.Width * 0.5f, cg_t->refdef_s.Height * ToTimes, X, Y, rgba(0, 0, 255, 255), Internal::R_RegisterMaterial("white", 0), 1.0);
							else
								DrawLine(cg_t->refdef_s.Width * 0.5f, cg_t->refdef_s.Height * ToTimes, X, Y, rgba(255, 255, 0, 255), Internal::R_RegisterMaterial("white", 0), 1.0);
						}
					}
				}
			}
		}
	}
	int ClosestPlayer;
	int GetNearestVisiblePlayer() {
		float ClosestDistance = FLT_MAX;
		ClosestPlayer = -1;

		for (int i = 0; i < 18; i++) {
			if (i != cg_t->clientNum) {
				if (Functions::AliveCheck(i) && !Functions::IsFriendly(cg_t->clientNum, i)) {
					float Distance = GetDistance(centity[cg_t->clientNum].Origin, centity[i].Origin);
					if (Distance < ClosestDistance){
						if ((Internal::CG_CanSeePlayer(0, &centity[i], 0x803003) && Toggles.Aimbot_Stealth)) {
							ClosestDistance = Distance;
							ClosestPlayer = i;
							Toggles.ClosestPlayerIsReadyForUberHax = true;
						} else if (!Toggles.Aimbot_Stealth) {
							ClosestDistance = Distance;
							ClosestPlayer = i;
							Toggles.ClosestPlayerIsReadyForUberHax = true;
						}
					}
				}
			}
		}
		return ClosestPlayer;
	}
}
namespace Functionality {
	float WhiteColor[4] = { (float)255 / 255.0f,(float)255 / 255.0f,(float)255 / 255.0f,(float)255 / 255.0f };

	int getOption() {
		if (UIItems.optionPress)
			return UIItems.currentOption;
		else
			return 0;
	}
	int DelayCounter;
	void MonitorButtons() {
		if (GetTickCount() - DelayCounter > 200) {
			if (UIItems.submenu == Closed) {
				if (Functions::Key_IsDown(0, Key_Left)) {
					printf("Key_Left Pressed\n");
					DelayCounter = GetTickCount();
					UIItems.lastSubmenu[UIItems.submenuLevel] = UIItems.submenu;
					UIItems.lastOption[UIItems.submenuLevel] = UIItems.currentOption;
					UIItems.currentOption = 1;
					UIItems.submenu = Main;
					UIItems.submenuLevel++;
					UIItems.submenuLevel = 0;
					UIItems.currentOption = 1;
				}
			} else {
				if (Functions::Key_IsDown(0, Key_B)) {
					printf("Key_B Pressed\n");
					DelayCounter = GetTickCount();
					if (UIItems.submenu == Main) {
						UIItems.submenu = Closed;
					} else {
						UIItems.submenu = UIItems.lastSubmenu[UIItems.submenuLevel - 1];
						UIItems.currentOption = UIItems.lastOption[UIItems.submenuLevel - 1];
						UIItems.submenuLevel--;
					}
				} else if (Functions::Key_IsDown(0, Key_A)) {
					printf("Key_A Pressed\n");
					UIItems.optionPress = true;
					DelayCounter = GetTickCount();
				} else if (Functions::Key_IsDown(0, Key_Up)) {
					printf("Key_Up Pressed\n");
					UIItems.currentOption--;
					UIItems.currentOption = UIItems.currentOption < 1 ? UIItems.optionCount : UIItems.currentOption;
					DelayCounter = GetTickCount();
				} else if (Functions::Key_IsDown(0, Key_Down)) {
					printf("Key_Down Pressed\n");
					UIItems.currentOption++;
					UIItems.currentOption = UIItems.currentOption > UIItems.optionCount ? 1 : UIItems.currentOption;
					DelayCounter = GetTickCount();
				} else if (Functions::Key_IsDown(0, Key_Left)) {
					printf("Key_Left Pressed\n");
					UIItems.leftPress = true;
					DelayCounter = GetTickCount();
				} else if (Functions::Key_IsDown(0, Key_Right)) {
					printf("Key_Right Pressed\n");
					UIItems.rightPress = true;
					DelayCounter = GetTickCount();
				}
			}
		}
	}
	class Item {
	private:
		char *optionName;
	public:
		Item(char*name)
			: optionName(name)
		{ }
		void AddOption(char*name) {
			UIItems.optionCount++;
			if (UIItems.currentOption <= UIItems.maxOptions && UIItems.optionCount <= UIItems.maxOptions)
				Gui::DrawText(name, 555, (UIItems.optionCount * 23 + 64), "fonts/normalfont", 0.65, UIItems.currentOption == UIItems.optionCount ? rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a) : rgba(255, 255, 255, 255));
			else if ((UIItems.optionCount > (UIItems.currentOption - UIItems.maxOptions)) && UIItems.optionCount <= UIItems.currentOption)
				Gui::DrawText(name, 555, ((UIItems.optionCount - (UIItems.currentOption - UIItems.maxOptions)) * 23 + 64), "fonts/normalfont", 0.65, UIItems.currentOption == UIItems.optionCount ? rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a) : rgba(255, 255, 255, 255));
		}
		void DrawTitle(const char *xtext, float xx, float xy, const char *xfont, float xfontSize) {
			Gui::DrawShader(547, 35, Internal::R_TextWidth(xtext, "fonts/normalfont")*xfontSize + 7, 22, rgba(0, 0, 0, 100), "white");//background header
			Gui::DrawShader(547, 35, 2, 22, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//left side bar
			Gui::DrawShader((547) + (Internal::R_TextWidth(xtext, "fonts/normalfont")*xfontSize + 7), 35, 2, 22, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//right side bar
			Gui::DrawShader(547, 35, (Internal::R_TextWidth(xtext, "fonts/normalfont")*xfontSize + 7), 2, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");//top header

			float flColor[4] = { (float)255 / 255.0f,(float)255 / 255.0f,(float)255 / 255.0f,(float)255 / 255.0f };
			Internal::R_AddCmdDrawText(xtext, 0x7FFFFFFF, Internal::R_RegisterFont(xfont, 0), xx, xy, xfontSize, xfontSize, 0, flColor, 0);
		}
		void Title() {
			this->DrawTitle(this->optionName, 552, 57.5, "fonts/normalfont", 0.65);
		}
		void Option() {
			this->AddOption(this->optionName);
		}
		void SubmenuOption(int newSubmenu) {
			this->AddOption(this->optionName);
			if (UIItems.currentOption == UIItems.optionCount) {
				if (UIItems.optionPress) {
					UIItems.lastSubmenu[UIItems.submenuLevel] = UIItems.submenu;
					UIItems.lastOption[UIItems.submenuLevel] = UIItems.currentOption;
					UIItems.currentOption = 1;
					UIItems.submenu = newSubmenu;
					UIItems.submenuLevel++;
				}
			}
		}
		void Toggle(bool*b00l) {
			this->AddOption(this->optionName);
			if (*b00l) {
				Gui::DrawText("ON", Gui::getXValue(805, "ON", 0.65, ALIGN_RIGHT, "fonts/normalfont"), (UIItems.optionCount * 23 + 64), "fonts/normalfont", 0.65, rgba(130, 214, 157, 255));
			} else {
				Gui::DrawText("OFF", Gui::getXValue(805, "OFF", 0.65, ALIGN_RIGHT, "fonts/normalfont"), (UIItems.optionCount * 23 + 64), "fonts/normalfont", 0.65, rgba(200, 55, 80, 255));
			}
			if (UIItems.currentOption == UIItems.optionCount) {
				if (UIItems.optionPress)
					*b00l = !*b00l;
			}
		}
		void CharOption(char **From, int*var, int min, int max){
			this->AddOption(this->optionName);

			char SelectedOption[30];
			sprintf(SelectedOption, "%s", (From[*var]));
			if (UIItems.currentOption == UIItems.optionCount) {
				if (UIItems.currentOption <= UIItems.maxOptions && UIItems.optionCount <= UIItems.maxOptions)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), (UIItems.optionCount * 23 + 64), "fonts/normalfont", 0.65, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a));
				else if ((UIItems.optionCount > (UIItems.currentOption - UIItems.maxOptions)) && UIItems.optionCount <= UIItems.currentOption)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), ((UIItems.optionCount - (UIItems.currentOption - UIItems.maxOptions)) * 23 + 64), "fonts/normalfont", 0.65, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a));

				if (UIItems.rightPress){
					if (*var >= max)
						*var = min;
					else
						*var = *var + 1;
				} else if (UIItems.leftPress) {
					if (*var <= min)
						*var = max;
					else
						*var = *var - 1;
				}
			} else {
				if (UIItems.currentOption <= UIItems.maxOptions && UIItems.optionCount <= UIItems.maxOptions)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), (UIItems.optionCount * 23 + 64), "fonts/normalfont", 0.65, rgba(255, 255, 255, 255));
				else if ((UIItems.optionCount > (UIItems.currentOption - UIItems.maxOptions)) && UIItems.optionCount <= UIItems.currentOption)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), ((UIItems.optionCount - (UIItems.currentOption - UIItems.maxOptions)) * 23 + 64), "fonts/normalfont", 0.65, rgba(255, 255, 255, 255));
			}
		}
		void IntOption(int *var, int min, int max){
			this->AddOption(this->optionName);

			char SelectedOption[30]; 
			sprintf(SelectedOption, "%i", *var);
			if (UIItems.currentOption == UIItems.optionCount){
				if (UIItems.currentOption <= UIItems.maxOptions && UIItems.optionCount <= UIItems.maxOptions)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), (UIItems.optionCount * 23 + 64), "fonts/normalfont", 0.65, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a));
				else if ((UIItems.optionCount > (UIItems.currentOption - UIItems.maxOptions)) && UIItems.optionCount <= UIItems.currentOption)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), ((UIItems.optionCount - (UIItems.currentOption - UIItems.maxOptions)) * 23 + 64), "fonts/normalfont", 0.65, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a));
				if (UIItems.leftPress){
					if (*var <= min)
						*var = max;
					else
						*var = *var - 1;
					printf("%i\n", *var);
				} else if (UIItems.rightPress) {
					if (*var >= max)
						*var = min;
					else
						*var = *var + 1;
					printf("%i\n", *var);
				}
			} else {
				if (UIItems.currentOption <= UIItems.maxOptions && UIItems.optionCount <= UIItems.maxOptions)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), (UIItems.optionCount * 23 + 64), "fonts/normalfont", 0.65, rgba(255, 255, 255, 255));
				else if ((UIItems.optionCount > (UIItems.currentOption - UIItems.maxOptions)) && UIItems.optionCount <= UIItems.currentOption)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), ((UIItems.optionCount - (UIItems.currentOption - UIItems.maxOptions)) * 23 + 64), "fonts/normalfont", 0.65, rgba(255, 255, 255, 255));
			}
		}
		void FloatOption(float *var, float min, float max, float increment) {
			this->AddOption(this->optionName);

			char SelectedOption[30];
			sprintf(SelectedOption, "%s", FtoS(*var));
			if (UIItems.currentOption == UIItems.optionCount) {
				if (UIItems.currentOption <= UIItems.maxOptions && UIItems.optionCount <= UIItems.maxOptions)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), (UIItems.optionCount * 23 + 64), "fonts/normalfont", 0.65, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a));
				else if ((UIItems.optionCount > (UIItems.currentOption - UIItems.maxOptions)) && UIItems.optionCount <= UIItems.currentOption)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), ((UIItems.optionCount - (UIItems.currentOption - UIItems.maxOptions)) * 23 + 64), "fonts/normalfont", 0.65, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a));
				if (UIItems.leftPress) {
					if (*var <= min)
						*var = max;
					else
						*var = *var - increment;
				}
				else if (UIItems.rightPress) {
					if (*var >= max)
						*var = min;
					else
						*var = *var + increment;
				}
			}
			else {
				if (UIItems.currentOption <= UIItems.maxOptions && UIItems.optionCount <= UIItems.maxOptions)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), (UIItems.optionCount * 23 + 64), "fonts/normalfont", 0.65, rgba(255, 255, 255, 255));
				else if ((UIItems.optionCount > (UIItems.currentOption - UIItems.maxOptions)) && UIItems.optionCount <= UIItems.currentOption)
					Gui::DrawText(SelectedOption, Gui::getXValue(805, SelectedOption, 0.65, ALIGN_RIGHT, "fonts/normalfont"), ((UIItems.optionCount - (UIItems.currentOption - UIItems.maxOptions)) * 23 + 64), "fonts/normalfont", 0.65, rgba(255, 255, 255, 255));
			}
		}
	};
}

void LoopedStatements() {
	if (Toggles.DeleteOriginalRadar) {
		if (*(int*)0x82132CF0 != 0x60000000) {
			WriteToAddress(0x82132CF0, 0x60000000);
			WriteToAddress(0x82132DD0, 0x60000000);
			WriteToAddress(0x82132DB4, 0x60000000);
			WriteToAddress(0x82132E10, 0x60000000);
			WriteToAddress(0x82132D28, 0x60000000);
			WriteToAddress(0x82132DEC, 0x60000000);
			WriteToAddress(0x82132D98, 0x60000000);
			WriteToAddress(0x82132E2C, 0x60000000);
			WriteToAddress(0x822FEF94, 0x60000000);
			WriteToAddress(0x82133340, 0x60000000);
		}

		Gui::DrawShader(10, 15, 225, 225, rgba(0, 0, 0, 100), "white");
		Functions::DrawRadar(-100, 10, 150, 150, rgba(255, 255, 255, 255));
		Functions::DrawRadarSelfIcon(-100, 10, 150, 150, rgba(0, 255, 255, 255));
		Functions::DrawEngineRadarFriendlyTeam(-100, 10, 150, 150, rgba(130, 214, 157, 255));
		Functions::DrawEngineRadarHelicopter(-100, 10, 150, 150, rgba(255, 255, 255, 255));
		Functions::DrawEngineRadarTurret(-100, 10, 150, 150, rgba(255, 255, 255, 255));
		Functions::DrawEngineRadarEnemyTeam(-100, 10, 150, 150, rgba(200, 55, 80, 255));
		Functions::DrawEngineRadarPackages(-100, 10, 150, 150, rgba(255, 255, 255, 255));
		Gui::DrawShader(10, 180 - 165, 225, 2, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");
		Gui::DrawShader(10, 405 - 165, 225, 2, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");
		Gui::DrawShader(10, 180 - 165, 2, 225, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");
		Gui::DrawShader(235, 180 - 165, 2, 227, rgba(MenuColor.r, MenuColor.g, MenuColor.b, MenuColor.a), "white");

		if (Toggles.SetNewRadarZoom) {
			Internal::Cbuf_AddText(0, va("set compassMaxRange %f", *(float*)0x837DAEE0 * 1.5f));
			Toggles.SetNewRadarZoom = false;
		}
	}
	
	/*for (int i = 0; i < 2048; i++) {
		float X, Y;
		switch (centity[i].Type) {
		case ET_MISSLE:
			if (Functions::WorldToScreen(centity[i].Origin, &X, &Y) && centity[i].Alive == 4) {
				Internal::CG_DrawRotatedPicPhysical(0x827322C0, X, Y, 35, 20, 0, Functionality::WhiteColor, Functions::GetWeaponShader(centity[i].WeaponIndex));
			}
			break;
		}
	}*/
}
void ResetVars() {
	UIItems.optionPress = false;
	UIItems.leftPress = false;
	UIItems.rightPress = false;
}

void Hook()
{
	if (Internal::Dvar_GetBool("cl_ingame")) {
		Functions::GrabStructs();
		LoopedStatements();
		Functionality::MonitorButtons();
		if (UIItems.submenu > 0)Gui::DrawGUI();
		UIItems.optionCount = 0;
		switch (UIItems.submenu) {
		case Main:
			Functionality::Item("Epsilon Cheats v0.01b").Title();
			Functionality::Item("Main Options").SubmenuOption(Main_Options);
			Functionality::Item("ESP Options").SubmenuOption(ESP_Options);
			Functionality::Item("Aimbot Options").SubmenuOption(Aimbot_Options);
			break;
		case Main_Options:
			Functionality::Item("Main Options").Title();
			Functionality::Item("Wallhack").Toggle(&Toggles.Wallhack);
			Functionality::Item("No Recoil").Toggle(&Toggles.NoRecoil);
			Functionality::Item("Unlimited UAV").Toggle(&Toggles.UnlimitedUAV);
			Functionality::Item("Laser").Toggle(&Toggles.Laser);
			Functionality::Item("No Spread").Toggle(&Toggles.NoSpread);
			switch (Functionality::getOption()) {
			case 1:
				if (Toggles.Wallhack)
					WriteToAddress<int>(0x82001B7C, 0x42200000);
				else
					WriteToAddress<int>(0x82001B7C, 0x40800000);
				break;
			case 2:
				if (Toggles.NoRecoil)
					WriteToAddress<byte>(0x821614BB, 0x00);
				else
					WriteToAddress<byte>(0x821614BB, 0x07);
				break;
			case 3:
				if (Toggles.UnlimitedUAV)
					WriteToAddress(0x8210E58C, 0x3B800001);
				else
					WriteToAddress(0x8210E58C, 0x557C87FE);
				break;
			case 4:
				if (Toggles.Laser)
					WriteToAddress(0x82124BD0, 0x38600001);
				else
					WriteToAddress(0x82124BD0, 0x38600000);
				break;
			case 5:
				if (Toggles.NoSpread) {
					WriteToAddress(0x82101B08, 0x4e800020); 
					WriteToAddress(0x82101B6C, 0x419A0040);
				} else {
					WriteToAddress(0x82101B08, 0x7D8802A6);
					WriteToAddress(0x82101B6C, 0x409A0040);
				}
				break;
			}
			break;
		case ESP_Options:
			Functionality::Item("ESP Options").Title();
			Functionality::Item("ESP Required").Toggle(&Toggles.ESP_Required);
			Functionality::Item("Snaplines").Toggle(&Toggles.ESP_Snaplines);
			Functionality::Item("Snapline Position").CharOption(OptionItems::SnaplinePositions, &Toggles.ESP_Snaplines_Type, 0, 2);
			Functionality::Item("Bone").Toggle(&Toggles.ESP_Bone);
			Functionality::Item("Name").Toggle(&Toggles.ESP_Name);
			Functionality::Item("Weapon Icon").Toggle(&Toggles.ESP_WeaponIcon);
			Functionality::Item("2D Box").Toggle(&Toggles.ESP_2DBox);
			break;
		case Aimbot_Options:
			Functionality::Item("Aimbot Options").Title();
			Functionality::Item("Aimbot Required").Toggle(&Toggles.Aimbot_Required);
			Functionality::Item("Stealth").Toggle(&Toggles.Aimbot_Stealth);
			Functionality::Item("Bone").CharOption(OptionItems::Bones, &Toggles.Aimbot_Bone, 0, 11);
			Functionality::Item("Auto Aim").Toggle(&Toggles.Aimbot_AutoAim);
			Functionality::Item("Auto Zoom").Toggle(&Toggles.Aimbot_AutoZoom);
			Functionality::Item("Auto Shoot").Toggle(&Toggles.Aimbot_AutoShoot);
			Functionality::Item("Crouch Bot (When Visible)").Toggle(&Toggles.Aimbot_CrouchBot);
			break;
		}
		ResetVars();
	}
	else {
		Toggles.SetNewRadarZoom = true;
		Gui::TopRightInfo("Epsilon Engine v0.01b Loaded!", 1);
	}
}
void AimbotHook() {
	if (Toggles.ESP_Required) {
		Functions::ESP();
	}
	if (Toggles.Aimbot_Required) {
		if (Functions::Key_IsDown(0, Key_LT) || Toggles.Aimbot_AutoAim) {
			int NearestClient = Functions::GetNearestVisiblePlayer();
			if (NearestClient != -1) {
				Vector3 NearestClientOrigin = Functions::GetTagPos(NearestClient, OptionItems::Bones[Toggles.Aimbot_Bone]);
				Vector3 MyOrigin = cg_t->refdef_s.Origin;
				Vector3 Subtracted = Functions::SubstractVector(NearestClientOrigin, MyOrigin);

				Vector3 ToAngle;
				Functions::VectorToAngle(&Subtracted, &ToAngle);

				active->ViewAngles[0] = ToAngle.x - active->BaseAngles[0];
				active->ViewAngles[1] = ToAngle.y - active->BaseAngles[1];

				if (Toggles.Aimbot_AutoShoot) {
					Toggles.Shoot = true;
				}
				if (Toggles.Aimbot_AutoZoom) {
					Toggles.Zoom = true;
				}
				if (Toggles.Aimbot_CrouchBot) {
					Toggles.Crouch = true;
				}
			}
		}
	}
}

__declspec(naked) DWORD AddCmdDrawStretchPicStub(...) {
	__asm
	{
		li r3, 1
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
DWORD AddCmdDrawStretchPicHook(float x, float y, float width, float height, float xScale, float yScale, float xay, float yay, const float * color, Material * material) {
	DWORD ret;
	if (!strcmp(material->materialName, "minimap_scanlines") || !strcmp(material->materialName, "scanlines_stencil")) {
		ret = AddCmdDrawStretchPicStub(x, y, 0, 0, xScale, yScale, xay, yay, color, material);
	} else {
		ret = AddCmdDrawStretchPicStub(x, y, width, height, xScale, yScale, xay, yay, color, material);
	}
	return ret;
}

DWORD XamInputStub(...) {
	__asm {
		li r3, 1
		nop
		li r4, 1
		nop
		nop
		nop
		nop
		blr
	}
}
DWORD XamInputHook(DWORD r3, int r4, PXINPUT_STATE r5) {
	DWORD result = XamInputStub(r3, r4, r5);

	static bool wasAutoAimOn = false;
	if (Toggles.Zoom && !Toggles.Shoot) {
		if (Toggles.Aimbot_AutoAim) {
			wasAutoAimOn = true;
			Toggles.Aimbot_AutoAim = false;
		}
		r5->Gamepad.bLeftTrigger = 255;
		Toggles.Zoom = false;
		if (wasAutoAimOn) {
			Toggles.Aimbot_AutoAim = true;
			wasAutoAimOn = false;
		}
	}
	if (Toggles.Shoot) {
		if (Toggles.Zoom) {
			if (Toggles.Aimbot_AutoAim) {
				wasAutoAimOn = true;
				Toggles.Aimbot_AutoAim = false;
			}
			r5->Gamepad.bLeftTrigger = 255;
			wait(0.1);
			if (r5->Gamepad.bLeftTrigger == 255) {
				r5->Gamepad.bRightTrigger = 255; 
				wait(0.1);
				Toggles.Shoot = false;
				Toggles.Zoom = false;
				if (wasAutoAimOn) {
					Toggles.Aimbot_AutoAim = true;
					wasAutoAimOn = false;
				}
			}
		} else {
			r5->Gamepad.bRightTrigger = 255;
			Toggles.Shoot = false;
		}
	}

	if (Toggles.Crouch) {
		wait(0.1);
		r5->Gamepad.wButtons = 0x2000;
		wait(0.1);
		Toggles.Crouch = false;
	}

	return result;
}

DWORD CL_WritePacketStub(...){
	__asm {
		li r3, 1
	}
}
DWORD CL_WritePacketHook(int r3) {
	DWORD result = CL_WritePacketStub(r3);

	return result;
}

void Engine()
{
	Sleep(10000);
	for(;;) {
		if (XamGetCurrentTitleId() == DASHBOARD){
			if (bInitializedMessage == FALSE) {
				XNotify(L"Epsilon Cheats v0.01b Loaded");
				bInitializedMessage = TRUE;
			}
			if (bInitialized == TRUE){
				bInitialized = FALSE;
			}
		} else if (XamGetCurrentTitleId() == COD_MW3){
			if (bInitialized == FALSE) {

				UIItems.maxOptions = 16;
				Toggles.DeleteOriginalRadar = true;

				XNotify(L"Epsilon Cheats Hooked");
				DetourFunction((PDWORD)0x82309108, (int)Hook);
				DetourFunction((PDWORD)0x82444448, (int)AimbotHook);
				HookFunctionStart((DWORD*)0x8241F038, (DWORD*)AddCmdDrawStretchPicStub, (DWORD)AddCmdDrawStretchPicHook);
				HookFunctionStart((DWORD*)0x8257FE8C, (DWORD*)XamInputStub, (DWORD)XamInputHook);
				//HookFunctionStart((DWORD*)0x8216DAF0, (DWORD*)CL_WritePacketStub, (DWORD)CL_WritePacketHook);
				bInitialized = TRUE;
			}
		} else {
			bInitialized = FALSE;
		}
		Sleep(100);
	}
}

BOOL WINAPI DllMain(HANDLE hInstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0, &hThreadID, (PVOID)XapiThreadStartup , (LPTHREAD_START_ROUTINE)Engine, NULL, 0x2);
		XSetThreadProcessor(hThread, 4);
		ResumeThread(hThread);
		CloseHandle(hThread);
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}