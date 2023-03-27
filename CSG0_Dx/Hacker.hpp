#pragma once
#include "def.hpp"

using namespace std;


#define IOCTL_IO_ReadMemory		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_GetProcessModules32		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x901, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_WriteMemory		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_IO_WriteMemory_MDL		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x903, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_AllocMemory		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x904, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_FreeMemory		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x905, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_InjectDLL		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x906, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_WriteMemory_test   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x907, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_ReadMemory_test   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x908, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_GetProcessModules64		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x909, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_DeleteFile		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x910, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_KeyBoardClick		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x911, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_MouseMove		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x912, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_MouseClick		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x913, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define DLLNAME "rwR3.dll"
#define DRIVER_LOAD "SH_DriverLoad"
#define DRIVER_UNLOAD "SH_UnDriverLoad"
#define DRIVER_GETMODULE "SH_GetModule"
#define DRIVER_READMEMORY "SH_ReadMemory"
#define DRIVER_WRITEMEMORY "SH_WriteMemory"
#define DRIVER_PROJECTPROCESS "SH_ProtectProcess"

#define PI 3.1415926f

// ��Ҫ����
#define OFFSET1 hazedumper::signatures::dwEntityList   // �����б�ƫ��
#define OFFSET2 hazedumper::signatures::dwViewMatrix // ����ƫ��
#define OFFSET3 hazedumper::signatures::dwLocalPlayer    // ��������ƫ��
#define OFFSET_FORCEJUMP hazedumper::signatures::dwForceJump  // ǿ����Ծ
#define OFFSET_FORCEATTACK hazedumper::signatures::dwForceAttack  // ǿ�ƹ���
#define OFFSET_FLAG 0x104 // ���˱�־
#define OFFSET_AIMPLAYER_ID hazedumper::netvars::m_iCrosshairId//��׼�ĵ���id

#define OFFSET_FORCELEFT hazedumper::signatures::dwForceLeft
#define OFFSET_FORCERIGHT hazedumper::signatures::dwForceRight
#define OFFSET_FORCEBACKWARD hazedumper::signatures::dwForceBackward
#define OFFSET_FORCEFORWARD hazedumper::signatures::dwForceForward

// ������ƫ��
#define OFFSET_COOR 0x138 // ����ƫ��
#define OFFSET_HP 0x100 // Ѫ��ƫ��
#define OFFSET_TIME 0xf4 // ��Ӫƫ��
#define OFFSET_ISOBJ 0x274 // �Ƿ�����Ҷ���
#define OFFSET_ATTACKANGLE 0x3048   // ����Ƕ�ƫ��
#define OFFSET_ObserverMode 0x3388 // ��ǰ�۲�״̬ // ����0 ���� 2 4  ... 


#define RADIO_W 1000
#define RADIO_H 1000

#define PFFSET_Self_Mouse_XY_1 0x59F19C 
#define PFFSET_Self_Mouse_XY_2 0x4d90
typedef struct GlowColor
{
    GlowColor() :r(0.f), g(0.f), b(0.f), a(0.f) {}
    GlowColor(float fr, float fg, float fb, float fa) : r(fr), g(fg), b(fb), a(fa) {}
    void Set(float fr, float fg, float fb, float fa) {
        r = fr;
        g = fg;
        b = fb;
        a = fa;
    }
    float r;
    float g;
    float b;
    float a;
}GLOWCOLOR, * PGLOWCOLOR;
typedef struct _MOUSE_INPUT_DATA {
    USHORT UnitId;
    USHORT Flags;
    union {
        ULONG Buttons;
        struct {
            USHORT ButtonFlags;
            USHORT ButtonData;
        };
    };
    ULONG  RawButtons;
    LONG   LastX;
    LONG   LastY;
    ULONG  ExtraInformation;
} MOUSE_INPUT_DATA, * PMOUSE_INPUT_DATA;

struct Point2D
{
    Point2D() { x = y = 0.0f; }
    Point2D(float _x, float _y) { x = _x; y = _y; }
    float x;
    float y;
};

struct Point3D
{
    float x;
    float y;
    float z;

    bool operator== (Point3D& obj)
    {
        if (abs(obj.x - this->x) <= 5.f &&
            abs(obj.y - this->y) <= 5.f &&
            abs(obj.z - this->z) <= 5.f)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

struct Point4D
{
    float x;
    float y;
    float z;
    float w;
};

struct ObjListData
{
    DWORD objAdr;
    DWORD res;
    LIST_ENTRY list;
};


void ESP();
void StopStop();

bool GetAroundObject(vector<DWORD>& vec);                // ��ȡ��Χ�����б�
DWORD GetSelfAdr();                             // ��ȡ���˵�ַ
bool GetObjAttackAngle(DWORD objAdr , float* fy , float* fx); // ��ȡ��������Ƕ�                      // ��ȡ�����ļ�
bool GetMex(float*  p);                      // ��ȡ����
bool GetSelfData(Point3D*  p1, DWORD*  p2, DWORD*  p3); // ��ȡ������Ϣ
bool GetObjData(DWORD objAdr, DWORD* outObjAdr, Point3D* pXyz, DWORD* pTeam, DWORD* pHp, BYTE* pIsDrawObj,DWORD* pIsRefresh,bool*
    isView);

DWORD GetObjFlags(DWORD objAdr,DWORD* pFlags); // ȥ����״̬
bool GetObjObserverMode(DWORD objAdr,DWORD* pMode); // ��ȡ����۲�״̬
bool GetObjXyz(DWORD ObjAdr , Point3D* pXyz);  // ��ȡ��������
bool GetObjTeam(DWORD ObjAdr , DWORD* pTeam); // ��ȡ������Ӫ
bool GetObjHp(DWORD ObjAdr , DWORD* pHp); // ��ȡ����Ѫ��
bool GetObjIsDrawObj(DWORD ObjAdr, BYTE* pIsDrawObj); // �Ƿ��ǻ��ƶ���
bool GetObjIsRefresh(DWORD ObjAdr, DWORD* pIsRefresh); // �����Ƿ�ˢ��
bool GetObjIsView(DWORD ObjAdr,bool* isView); // �����Ƿ����
void GetBones(LPVOID objAdr, float* mex, int index, Point2D* pScreen, Point3D* pObjXyz);
void GetBones(LPVOID objAdr, float* mex, ImU32 color, bool isCt);
void SetNoRecoil(bool mode);


bool GetCurrentJumpFlag(DWORD* pFlag); // ��ȡ��Ծ״̬
bool SetCurrentJumpFlag(DWORD* flag);// д��Ծ״̬
bool SelfIsCrouchFlag();
bool GetCurrentAttackFlag(DWORD* pFlag); // ��ȡ����״̬
void AutoAimLockMmeory(Point3D&, Point3D&, int);

float calcHp(float drawH, DWORD hp);
double CalcPoint3DDistance(Point3D& my, Point3D& obj);
double calc2DDistance(Point2D p1, Point2D p2);
void MouseAimbot(Point2D Output, int GameCentX, int GameCentY, int ��Ϸ���, int ��Ϸ�߶�, int AimSpeed);

bool ��������ת��Ļ����(Point3D& IN ��Ϸ����, Point2D& OUT ��Ļ����, DWORD IN �ֱ��ʿ�, DWORD IN �ֱ��ʸ�, float* IN ����);

bool ReadGameMemory(LPVOID ptr,LPVOID buf, DWORD len);
bool WriteGameMemory(LPVOID ptr, LPVOID buf, DWORD len);
bool ListProcessModules();
void LoadConfig();

DWORD WINAPI ThreadLoops(LPVOID lp);
DWORD WINAPI ThreadLoops_Funny(LPVOID lp);
DWORD WINAPI ThreadLoops_AutoFire(LPVOID lp);
ImU32 floatToImU32(float* col);

extern DWORD           g_SelfCtT;
extern HANDLE          g_hAndle;
extern HMODULE         g_hModule;
extern DWORD g_dwClientAdr; // client.dll ��ַ
extern DWORD g_dwEngineAdr; // engine.dll ��ַ

typedef struct _ON_OFF {
    bool g_AutoJump; // �Զ�����
    bool g_AttackStop; // ��ͣ���

    bool g_Esp; // ���Ƶ���
    bool g_EspFriend; // �����Ѿ�
    bool g_Espinfo; // ����info
    bool g_EspBons; // ���ƹ���
    bool g_EspName; // ��������
    bool g_EspLine; // ��������
    bool g_AutoAim; // �Զ���׼
    bool g_AutoAimFriend; // �������
    int g_AutoAimSpeed; // �����ٶ�
    int g_AutoAimR; // ����뾶
    int g_AutoAimKey; // ���鰴��
    int g_AutoAimBody; // ����λ��
    DWORD g_AimObjAdr; // ������������ID
    bool g_AimLock;// ����
    bool g_AimLock_Memory;//fuck memory lock
    bool g_AutoRecoilForce; // �Զ�ѹǹ
    bool g_no_recoil;
    bool g_d3dhook;
    bool g_auto_fire;
    int g_auto_fire_delay_back;
    int g_auto_fire_delay_former;
    bool g_radar;
    bool g_objlight;
    bool g_light_t;

    bool g_debug_mode;
}ON_OFF, * PON_OFF;

extern ON_OFF on_off;

extern bool g_thread_isrun;

extern float   g_AutoRecoilForceVal; // �Զ�ѹǹ����
extern float g_32Col_Obj[4]; // ������ɫ
extern float g_32Col_friendObj[4]; // ������ɫ
extern float g_32Col_Hp[4]; // Ѫ����ɫ
extern float g_32Col_textBack[4]; // ������ɫ
extern float g_32Col_text[4]; // �ı���ɫ
extern float g_32Col_bone[4]; // ������ɫ
extern float g_32Col_line[4]; // ������ɫ
extern float g_32Col_cir[4]; // ��Χ��ɫ