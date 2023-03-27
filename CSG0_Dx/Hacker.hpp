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

// 需要更新
#define OFFSET1 hazedumper::signatures::dwEntityList   // 人物列表偏移
#define OFFSET2 hazedumper::signatures::dwViewMatrix // 矩阵偏移
#define OFFSET3 hazedumper::signatures::dwLocalPlayer    // 本人数据偏移
#define OFFSET_FORCEJUMP hazedumper::signatures::dwForceJump  // 强制跳跃
#define OFFSET_FORCEATTACK hazedumper::signatures::dwForceAttack  // 强制攻击
#define OFFSET_FLAG 0x104 // 本人标志
#define OFFSET_AIMPLAYER_ID hazedumper::netvars::m_iCrosshairId//瞄准的敌人id

#define OFFSET_FORCELEFT hazedumper::signatures::dwForceLeft
#define OFFSET_FORCERIGHT hazedumper::signatures::dwForceRight
#define OFFSET_FORCEBACKWARD hazedumper::signatures::dwForceBackward
#define OFFSET_FORCEFORWARD hazedumper::signatures::dwForceForward

// 对象下偏移
#define OFFSET_COOR 0x138 // 坐标偏移
#define OFFSET_HP 0x100 // 血量偏移
#define OFFSET_TIME 0xf4 // 阵营偏移
#define OFFSET_ISOBJ 0x274 // 是否是玩家对象
#define OFFSET_ATTACKANGLE 0x3048   // 射击角度偏移
#define OFFSET_ObserverMode 0x3388 // 当前观察状态 // 正常0 其他 2 4  ... 


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

bool GetAroundObject(vector<DWORD>& vec);                // 获取周围对象列表
DWORD GetSelfAdr();                             // 获取本人地址
bool GetObjAttackAngle(DWORD objAdr , float* fy , float* fx); // 获取对象射击角度                      // 获取驱动文件
bool GetMex(float*  p);                      // 获取矩阵
bool GetSelfData(Point3D*  p1, DWORD*  p2, DWORD*  p3); // 获取本人信息
bool GetObjData(DWORD objAdr, DWORD* outObjAdr, Point3D* pXyz, DWORD* pTeam, DWORD* pHp, BYTE* pIsDrawObj,DWORD* pIsRefresh,bool*
    isView);

DWORD GetObjFlags(DWORD objAdr,DWORD* pFlags); // 去对象状态
bool GetObjObserverMode(DWORD objAdr,DWORD* pMode); // 获取对象观察状态
bool GetObjXyz(DWORD ObjAdr , Point3D* pXyz);  // 获取对象坐标
bool GetObjTeam(DWORD ObjAdr , DWORD* pTeam); // 获取对象阵营
bool GetObjHp(DWORD ObjAdr , DWORD* pHp); // 获取对象血量
bool GetObjIsDrawObj(DWORD ObjAdr, BYTE* pIsDrawObj); // 是否是绘制对象
bool GetObjIsRefresh(DWORD ObjAdr, DWORD* pIsRefresh); // 对象是否刷新
bool GetObjIsView(DWORD ObjAdr,bool* isView); // 对象是否可视
void GetBones(LPVOID objAdr, float* mex, int index, Point2D* pScreen, Point3D* pObjXyz);
void GetBones(LPVOID objAdr, float* mex, ImU32 color, bool isCt);
void SetNoRecoil(bool mode);


bool GetCurrentJumpFlag(DWORD* pFlag); // 获取跳跃状态
bool SetCurrentJumpFlag(DWORD* flag);// 写跳跃状态
bool SelfIsCrouchFlag();
bool GetCurrentAttackFlag(DWORD* pFlag); // 获取攻击状态
void AutoAimLockMmeory(Point3D&, Point3D&, int);

float calcHp(float drawH, DWORD hp);
double CalcPoint3DDistance(Point3D& my, Point3D& obj);
double calc2DDistance(Point2D p1, Point2D p2);
void MouseAimbot(Point2D Output, int GameCentX, int GameCentY, int 游戏宽度, int 游戏高度, int AimSpeed);

bool 世界坐标转屏幕坐标(Point3D& IN 游戏坐标, Point2D& OUT 屏幕坐标, DWORD IN 分辨率宽, DWORD IN 分辨率高, float* IN 矩阵);

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
extern DWORD g_dwClientAdr; // client.dll 地址
extern DWORD g_dwEngineAdr; // engine.dll 地址

typedef struct _ON_OFF {
    bool g_AutoJump; // 自动连跳
    bool g_AttackStop; // 急停射击

    bool g_Esp; // 绘制敌人
    bool g_EspFriend; // 绘制友军
    bool g_Espinfo; // 绘制info
    bool g_EspBons; // 绘制骨骼
    bool g_EspName; // 绘制名字
    bool g_EspLine; // 绘制射线
    bool g_AutoAim; // 自动瞄准
    bool g_AutoAimFriend; // 自瞄队友
    int g_AutoAimSpeed; // 自瞄速度
    int g_AutoAimR; // 自瞄半径
    int g_AutoAimKey; // 自瞄按键
    int g_AutoAimBody; // 自瞄位置
    DWORD g_AimObjAdr; // 自瞄锁定敌人ID
    bool g_AimLock;// 死锁
    bool g_AimLock_Memory;//fuck memory lock
    bool g_AutoRecoilForce; // 自动压枪
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

extern float   g_AutoRecoilForceVal; // 自动压枪参数
extern float g_32Col_Obj[4]; // 敌人颜色
extern float g_32Col_friendObj[4]; // 敌人颜色
extern float g_32Col_Hp[4]; // 血量颜色
extern float g_32Col_textBack[4]; // 背景颜色
extern float g_32Col_text[4]; // 文本颜色
extern float g_32Col_bone[4]; // 骨骼颜色
extern float g_32Col_line[4]; // 射线颜色
extern float g_32Col_cir[4]; // 范围颜色