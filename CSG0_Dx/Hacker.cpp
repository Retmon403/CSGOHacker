#include "Hacker.hpp"
#include "Gui.hpp"
#include "csgo.hpp"
#include "hook.hpp"
#include "sdk.hpp"


HANDLE DeviceHandle;
DWORD g_dwClientAdr; 
DWORD g_dwEngineAdr;
RECT g_winRecv;
DWORD g_SelfCtT;
ON_OFF on_off = { 0 };

bool g_thread_isrun = true;
float   g_AutoRecoilForceVal = 1.0f; // �Զ�ѹǹ����
extern bool g_isdebug_window;
float g_32Col_Obj[4] = { 1.f,0.f,0.f,1.f }; // ������ɫ
float g_32Col_friendObj[4] = { 0.f,1.f,0.f,1.f }; // ������ɫ
float g_32Col_Hp[4] = { 1.f,0.5f,0.f,1.f }; // Ѫ����ɫ
float g_32Col_textBack[4] = { 0.1f,0.1f,0.1f,1.f }; // ������ɫ
float g_32Col_text[4] = { 1.0f,1.0f,1.0f,1.f }; // �ı���ɫ
float g_32Col_bone[4] = { 1.0f,0.0f,0.0f,1.f }; // ������ɫ
float g_32Col_line[4] = { 1.0f,0.0f,0.0f,1.f }; // ������ɫ
float g_32Col_cir[4] = { 1.0f,1.0f,1.0f,1.f }; // ��Χ��ɫ

void LoadConfig() {
    on_off.g_AutoJump = true; // �Զ�����
    on_off.g_Esp = true; // ���Ƶ���
    on_off.g_EspFriend = false; // �����Ѿ�
    on_off.g_Espinfo = true; // ����info
    on_off.g_EspBons = false; // ���ƹ���
    on_off.g_EspName = true; // ��������
    on_off.g_EspLine = false; // ��������
    on_off.g_AutoAim = true; // �Զ���׼
    on_off.g_AutoAimFriend = false; // �������
    on_off.g_AutoAimSpeed = 2; // �����ٶ�
    on_off.g_AutoAimR = 180; // ����뾶
    on_off.g_AutoAimKey = VK_MENU; // ���鰴��
    on_off.g_AutoAimBody = 5; // ����λ��
    on_off.g_AimObjAdr = -1; // ������������ID
    on_off.g_AimLock = true;// ����
    on_off.g_AimLock_Memory = false;//fuck memory lock
    on_off.g_AutoRecoilForce = false; // �Զ�ѹǹ
    on_off.g_AttackStop = false; // ��ͣ���
    on_off.g_no_recoil = false; //�޺�����
    on_off.g_d3dhook = false;
    on_off.g_auto_fire = true;
    on_off.g_auto_fire_delay_back = 180;
    on_off.g_auto_fire_delay_former = 30;
    on_off.g_radar = false;
    on_off.g_objlight = false;
    on_off.g_light_t = false;
    on_off.g_debug_mode = false;
}

typedef struct _DataStruct
{
    ULONG	ProcessPid;
    ULONG	TargetAddress;
    ULONG	Length;
    CHAR	Buffer[64];
} DataStruct, * PDataStruct;
typedef struct _DataStruct_Ex
{
    ULONG	ProcessPid;
    PVOID	name;
    ULONG length;
} DataStruct_Ex, * PDataStruct_Ex;

void get_aimbot(float* self, float* obj, float* ret) {
    float x = self[0] - obj[0];
    float y = self[1] - obj[1];
    float z = self[2] - obj[2];
    ret[0] = (float)atan(z / sqrt(x * x + y * y)) / PI * 180.0f;
    ret[1] = (float)atan(y / x);
    if (x >= 0.0f && y >= 0.0f) {
        ret[1] = ret[1] / PI * 180.0f - 180.0f;
    }
    else if (x < 0.0f && y >= 0.0f) {
        ret[1] = ret[1] / PI * 180.0f;
    }
    else if (x < 0.0f && y < 0.0f) {
        ret[1] = ret[1] / PI * 180.0f;
    }
    else if (x >= 0.0f && y < 0.0f) {
        ret[1] = ret[1] / PI * 180.0f + 180.0f;
    }
}

bool GetAroundObject(vector<DWORD>& vec){
    DWORD listEntry = g_dwClientAdr + OFFSET1;
    for (int i = 0; i < 128; i++)
    {
        DWORD curPoint = 0;
        ReadGameMemory((LPVOID)(listEntry + i * 0x10), &curPoint, sizeof(DWORD));
        vec.push_back(curPoint);
    }

    /*
    PlayerInfo self = { 0 };
    if (!engine->getPlayerInfo(engine->getLocalPlayer(), self)) {
        //return;
    }

    for (int i = 1; i <= engine->getMaxClients(); i++) {
        PlayerInfo player_info;
        if (!engine->getPlayerInfo(i, player_info)) continue;

        //����Ǽ���һ������Լ�����SteamID������
        
        if (player_info.fakeplayer  || self.userId == player_info.userId) {
            continue;
        }
        //printf("%d,%d,%s\n", player_info.xuid, player_info.fakeplayer, player_info.name);
    }*/
    return true;
}

void AutoAimLockMmeory(Point3D& self, Point3D& fuck,int speed) {
    ULONG base = 0;
    Point2D xy;
    Point2D self_xy;
    fuck.z -= 62;
    if (SelfIsCrouchFlag())//�ж��Ƿ���� ��΢����������
        fuck.z += 16;
    get_aimbot((float*)&self, (float*)&fuck, (float*)&xy);
    ReadGameMemory((LPVOID)(g_dwEngineAdr + PFFSET_Self_Mouse_XY_1), &base, sizeof(base));
    if (speed == 1) {
        WriteGameMemory((LPVOID)(base + PFFSET_Self_Mouse_XY_2), &xy.x, sizeof(float));
        WriteGameMemory((LPVOID)(base + PFFSET_Self_Mouse_XY_2 + 0x4), &xy.y, sizeof(float));
    }
    else {
        ReadGameMemory((LPVOID)(g_dwEngineAdr + PFFSET_Self_Mouse_XY_1), &base, sizeof(base));
        ReadGameMemory((LPVOID)(base + PFFSET_Self_Mouse_XY_2), &self_xy, sizeof(Point2D));
        float temp = (xy.x - self_xy.x) / 30 * (30 - speed);
        temp += self_xy.x;
        WriteGameMemory((LPVOID)(base + PFFSET_Self_Mouse_XY_2), &temp, sizeof(float));
        temp = (xy.y - self_xy.y) / 31 * (31 - speed);
        temp += self_xy.y;
        WriteGameMemory((LPVOID)(base + PFFSET_Self_Mouse_XY_2 + 0x4), &temp, sizeof(float));
    }
}

DWORD GetSelfAdr(){
    DWORD selfAdr = NULL; // ���˵�ַ
    ReadGameMemory((LPVOID)(g_dwClientAdr + OFFSET3), &selfAdr, sizeof(selfAdr));
    return selfAdr;
}

bool GetObjAttackAngle(DWORD objAdr, float* fy, float* fx)
{
    bool bRet = false;
    bRet = ReadGameMemory((LPVOID)(objAdr + OFFSET_ATTACKANGLE), fy, sizeof(float));
    if (!bRet) return false;
    bRet = ReadGameMemory((LPVOID)(objAdr + OFFSET_ATTACKANGLE + 4), fx, sizeof(float));
    if (!bRet) return false;
    return true;
}

bool GetMex(float* p){
    return ReadGameMemory((LPVOID)(g_dwClientAdr + OFFSET2), p, 64);
}

bool GetSelfData(Point3D* p1, DWORD* p2, DWORD* p3){
    DWORD selfAdr = NULL; // ���˵�ַ
    bool bRet = ReadGameMemory((LPVOID)(g_dwClientAdr + OFFSET3), &selfAdr, sizeof(selfAdr));
    if (!bRet || selfAdr == NULL)
    {
        return false;
    }
    if (p1 != NULL)
    {
        if (!GetObjXyz(selfAdr, p1))
        {
            return false;
        }
    }
    if (p2 != NULL)
    {
        if (!GetObjTeam(selfAdr, p2))
        {
            return false;
        }
    }
    if (p3 != NULL)
    {
        if (!GetObjHp(selfAdr, p3))
        {
            return false;
        }
    }
    return true;
}

bool GetObjData(DWORD objAdr, DWORD* outObjAdr, Point3D* pXyz, DWORD* pTeam, DWORD* pHp, BYTE* pIsDrawObj, DWORD* pIsRefresh, bool* isView){
    if (pXyz != NULL)
    {
        if (!(GetObjXyz(objAdr, pXyz))) return false;
    }
    if (pTeam != NULL)
    {
        if (!GetObjTeam(objAdr, pTeam)) return false;
    }
    if (pHp != NULL)
    {
        if (!GetObjHp(objAdr, pHp)) return false;
    }
    if (pIsDrawObj != NULL)
    {
        if (!GetObjIsDrawObj(objAdr, pIsDrawObj)) return false;
    }
    if (pIsRefresh != NULL)
    {
        if (!GetObjIsRefresh(objAdr, pIsRefresh)) return false;
    }
    if (isView != NULL)
    {
        if (!GetObjIsView(objAdr, isView)) return false;
    }
    if (outObjAdr != NULL)
    {
        *outObjAdr = objAdr;
    }
    return true;
}

DWORD GetObjFlags(DWORD objAdr, DWORD* pFlags){
    return ReadGameMemory(LPVOID(objAdr + OFFSET_FLAG), pFlags, sizeof(DWORD));
}

bool GetObjObserverMode(DWORD objAdr, DWORD* pMode){
    return ReadGameMemory(LPVOID(objAdr + OFFSET_ObserverMode), pMode, sizeof(DWORD));
}

bool GetObjXyz(DWORD ObjAdr, Point3D* pXyz){
    return ReadGameMemory((LPVOID)(ObjAdr + OFFSET_COOR), pXyz, sizeof(Point3D));
}

bool GetObjTeam(DWORD ObjAdr, DWORD* pTeam){
    bool bRet = false;
    bRet = ReadGameMemory((LPVOID)(ObjAdr + OFFSET_TIME), pTeam, sizeof(DWORD));
    return bRet;
}

bool GetObjIsC4(DWORD ObjAdr) {
    DWORD c4 = 0;
    ReadGameMemory((LPVOID)(ObjAdr + 0x118C0), &c4, sizeof(c4));
    return c4 ? true : false;
}

bool GetObjHp(DWORD ObjAdr, DWORD* pHp){
    return ReadGameMemory((LPVOID)(ObjAdr + OFFSET_HP), pHp, sizeof(DWORD));
}

bool GetObjIsDrawObj(DWORD ObjAdr, BYTE* pIsDrawObj){
    return ReadGameMemory((LPVOID)(ObjAdr + OFFSET_ISOBJ), pIsDrawObj, sizeof(BYTE));
}

bool GetObjIsRefresh(DWORD ObjAdr, DWORD* pIsRefresh){
    return ReadGameMemory((LPVOID)(ObjAdr + 0xED), pIsRefresh, sizeof(DWORD));
}

bool GetObjIsView(DWORD ObjAdr, bool* isView){
    bool bRet = false;
    DWORD tmpVal = 0;
    bRet = ReadGameMemory((LPVOID)(ObjAdr + 0x93D), &tmpVal, sizeof(tmpVal));
    *isView = (tmpVal == 1);
    return bRet;
}

void SetNoRecoil(bool mode) {
    /*client.dll+213C20 - 55                    - push ebp
    client.dll+213C21 - 8B EC                 - mov ebp,esp
    client.dll+213C23 - 8B 55 08              - mov edx,[ebp+08]
    client.dll+213C26 - 8B 4D 10              - mov ecx,[ebp+10]
    client.dll+213C29 - 8B 42 08              - mov eax,[edx+08]
    client.dll+213C2C - 89 01                 - mov [ecx],eax
    client.dll+213C2E - 8B 42 0C              - mov eax,[edx+0C]
    client.dll+213C31 - 89 41 04              - mov [ecx+04],eax
    client.dll+213C34 - 8B 42 10              - mov eax,[edx+10]
    client.dll+213C37 - 89 41 08              - mov [ecx+08],eax*/
    CHAR buf = !mode ? (CHAR)0x55 : (CHAR)0xc3;
    WriteGameMemory((LPVOID)(g_dwClientAdr + 0x213C20), &buf, sizeof(buf));
}

void GetBones(LPVOID objAdr, float* mex, int index, Point2D* pScreen, Point3D* pObjXyz)
{
    map<int, int> bonesId;
    bonesId[0] = 0;
    bonesId[1] = 1;
    bonesId[3] = 3;
    bonesId[4] = 4;
    bonesId[5] = 5;
    bonesId[8] = 8;
    bonesId[9] = 9;
    bonesId[0xA] = 0xA;
    bonesId[0xB] = 0xB;
    bonesId[0xC] = 0xC;
    bonesId[0x24] = 0x24;
    bonesId[0x25] = 0x25;
    bonesId[0x26] = 0x26;
    bonesId[0x27] = 0x27;
    bonesId[0x28] = 0x28;
    bonesId[0x2A] = 0x2A;
    bonesId[0x3F] = 0x3F;
    bonesId[0x40] = 0x40;
    bonesId[0x41] = 0x41;
    bonesId[0x42] = 0x42;
    bonesId[0x44] = 0x44;
    bonesId[0x45] = 0x45;
    bonesId[0x46] = 0x46;
    bonesId[0x47] = 0x47;
    bonesId[0x48] = 0x48;
    bonesId[0x49] = 0x49;
    bonesId[0x4B] = 0x4B;
    bonesId[0x4C] = 0x4C;
    bonesId[0x4D] = 0x4D;
    bonesId[0x4F] = 0x4F;

    for (auto itr : bonesId)
    {
        Point2D BonesScreen;
        DWORD headPointAddr = NULL;
        BOOL bRet = 0;
        bRet = ReadGameMemory((LPVOID)((DWORD)objAdr + 0x26A8), &headPointAddr, sizeof(headPointAddr));
        // ��ȡ��Ҫ��Ӧ�Ĺ���
        Point3D DrawBonesPoint = { NULL };
        ReadGameMemory((LPVOID)(headPointAddr + 0x9C + itr.second * 0x30), &DrawBonesPoint.x, sizeof(DrawBonesPoint.x));
        ReadGameMemory((LPVOID)(headPointAddr + 0x9C + 0x10 + itr.second * 0x30), &DrawBonesPoint.y, sizeof(DrawBonesPoint.y));
        ReadGameMemory((LPVOID)(headPointAddr + 0x9C + 0x20 + itr.second * 0x30), &DrawBonesPoint.z, sizeof(DrawBonesPoint.z));
        ��������ת��Ļ����(DrawBonesPoint, BonesScreen, g_winRecv.right - g_winRecv.left, g_winRecv.bottom - g_winRecv.top, mex);
        if (itr.second == index)
        {
            if (pObjXyz != NULL)
            {
                *pObjXyz = DrawBonesPoint;
            }
            if (pScreen != NULL)
            {
                *pScreen = BonesScreen;
            }
            break;
        }
    }
    return;
}

void GetBones(LPVOID objAdr, float* mex, ImU32 color, bool isCt){
    map<int, Point2D> retVec;
    map<int, int> bonesId;
    bonesId[0] = 0;
    bonesId[1] = 1;
    bonesId[3] = 3;
    bonesId[4] = 4;
    bonesId[5] = 5;
    bonesId[8] = 8;
    bonesId[9] = 9;
    bonesId[0xA] = 0xA;
    bonesId[0xB] = 0xB;
    bonesId[0xC] = 0xC;
    bonesId[0x24] = 0x24;
    bonesId[0x25] = 0x25;
    bonesId[0x26] = 0x26;
    bonesId[0x27] = 0x27;
    bonesId[0x28] = 0x28;
    bonesId[0x2A] = 0x2A;
    bonesId[0x3F] = 0x3F;
    bonesId[0x40] = 0x40;
    bonesId[0x41] = 0x41;
    bonesId[0x42] = 0x42;
    bonesId[0x44] = 0x44;
    bonesId[0x45] = 0x45;
    bonesId[0x46] = 0x46;
    bonesId[0x47] = 0x47;
    bonesId[0x48] = 0x48;
    bonesId[0x49] = 0x49;
    bonesId[0x4B] = 0x4B;
    bonesId[0x4C] = 0x4C;
    bonesId[0x4D] = 0x4D;
    bonesId[0x4F] = 0x4F;

    for (auto itr : bonesId)
    {
        Point2D BonesScreen;
        DWORD headPointAddr = NULL;
        BOOL bRet = 0;
        bRet = ReadGameMemory((LPVOID)((DWORD)objAdr + 0x26A8), &headPointAddr, sizeof(headPointAddr));

        // ��ȡ��Ҫ��Ӧ�Ĺ���
        Point3D DrawBonesPoint = { NULL };
        ReadGameMemory((LPVOID)(headPointAddr + 0x9C + itr.second * 0x30), &DrawBonesPoint.x, sizeof(DrawBonesPoint.x));
        ReadGameMemory((LPVOID)(headPointAddr + 0x9C + 0x10 + itr.second * 0x30), &DrawBonesPoint.y, sizeof(DrawBonesPoint.y));
        ReadGameMemory((LPVOID)(headPointAddr + 0x9C + 0x20 + itr.second * 0x30), &DrawBonesPoint.z, sizeof(DrawBonesPoint.z));
        ��������ת��Ļ����(DrawBonesPoint, BonesScreen, g_winRecv.right - g_winRecv.left, g_winRecv.bottom - g_winRecv.top, mex);
        retVec[itr.second] = BonesScreen;
    }
    vector<vector<int>> allIndex_T = {
        {0x5,0x4,0x8,0x9,0xb,0xc} ,
        {0x5,0x4,0x24,0x25,0x26,0x28} ,
        //{0x5,0x4,0x3,0x0,0x3f,0x40,0x41,0x42} ,
        //{0x5,0x4,0x3,0x0,0x46,0x47,0x48,0x49}
    };
    vector<vector<int>> allIndex_CT = {
        {0x5,0x4,0x8,0x9,0xA,0xC} ,
        {0x5,0x4,0x26,0x27,0x28,0x2A} ,
        //{0x4,0x1,0x48,0x44,0x45,0x46} ,
        //{0x1,0x4f,0x4b,0x4c,0x4d}
    };
    vector<vector<int>> allIndex;
    if (isCt)
    {
        allIndex = allIndex_CT;
    }
    else
    {
        allIndex = allIndex_T;
    }
    for (auto allItr : allIndex)
    {
        int lastPoint = -1;
        for (auto itr : allItr)
        {
            if (lastPoint == -1)
            {
                lastPoint = itr;
            }
            else
            {
                if (retVec.at(lastPoint).x == 0 || retVec.at(lastPoint).y == 0 ||
                    retVec.at(itr).x == 0 || retVec.at(itr).y == 0)
                {

                }
                else
                {
                    ImGui::GetForegroundDrawList()->AddLine(
                        ImVec2(retVec.at(lastPoint).x, retVec.at(lastPoint).y),
                        ImVec2(retVec.at(itr).x, retVec.at(itr).y),
                        color, 2.0f);
                }
                lastPoint = itr;
            }
        }
    }
    return;
}

bool GetCurrentJumpFlag(DWORD* pFlag){
    return ReadGameMemory((LPVOID)(g_dwClientAdr + OFFSET_FORCEJUMP), pFlag, sizeof(DWORD));
}

bool SetCurrentJumpFlag(DWORD* flag){
    return WriteGameMemory((LPVOID)(g_dwClientAdr + OFFSET_FORCEJUMP), flag, sizeof(DWORD));
}

bool GetCurrentAttackFlag(DWORD* pFlag){
    return ReadGameMemory((LPVOID)(g_dwClientAdr + OFFSET_FORCEATTACK), pFlag, sizeof(DWORD));
}

bool ��������ת��Ļ����(Point3D& IN ��Ϸ����, Point2D& OUT ��Ļ����, DWORD IN �ֱ��ʿ�, DWORD IN �ֱ��ʸ�, float* IN ����){
    Point4D �ü�����;
    �ü�����.x = ��Ϸ����.x * ����[0] + ��Ϸ����.y * ����[1] + ��Ϸ����.z * ����[2] + ����[3];
    �ü�����.y = ��Ϸ����.x * ����[4] + ��Ϸ����.y * ����[5] + ��Ϸ����.z * ����[6] + ����[7];
    �ü�����.z = ��Ϸ����.x * ����[8] + ��Ϸ����.y * ����[9] + ��Ϸ����.z * ����[10] + ����[11];
    �ü�����.w = ��Ϸ����.x * ����[12] + ��Ϸ����.y * ����[13] + ��Ϸ����.z * ����[14] + ����[15];
    if (�ü�����.w < 0.1f){
        return false;
    }
    Point3D NDC;
    NDC.x = �ü�����.x / �ü�����.w;
    NDC.y = �ü�����.y / �ü�����.w;
    NDC.z = �ü�����.z / �ü�����.w;
    ��Ļ����.x = (�ֱ��ʿ� / 2 * NDC.x) + (NDC.x + �ֱ��ʿ� / 2);
    ��Ļ����.y = -(�ֱ��ʸ� / 2 * NDC.y) + (NDC.y + �ֱ��ʸ� / 2);
    return true;
}

float calcHp(float drawH, DWORD hp)
{
    float f = ((float)hp) / 100.0f;
    return f * drawH;
}

double CalcPoint3DDistance(Point3D& my, Point3D& obj)
{
    return sqrt((obj.x - my.x) * (obj.x - my.x) + (obj.y - my.y) * (obj.y - my.y) + (obj.z - my.z) * (obj.z - my.z));
}

double calc2DDistance(Point2D p1, Point2D p2)
{
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

void MouseAimbot(Point2D Output, int GameCentX, int GameCentY, int ��Ϸ���, int ��Ϸ�߶�, int AimSpeed)
{
    float TargetX = 0, TargetY = 0, Frevise;
    Frevise = (float)0.3;
    if (Output.x != 0)
    {
        if (Output.x > GameCentX)
        {
            TargetX = -(GameCentX - Output.x);
            TargetX = TargetX / AimSpeed;
            if (TargetX + GameCentX + 1 > ��Ϸ���)
            {
                TargetX = 0;
            }
            if (TargetX<1 && TargetX>Frevise)
            {
                TargetX = 1;
            }
        }
        if (Output.x < GameCentX)
        {
            TargetX = Output.x - GameCentX;
            TargetX = TargetX / AimSpeed;
            if (TargetX + GameCentX - 1 < 0)
            {
                TargetX = 0;
            }
            if (TargetX > -1 && TargetX < Frevise)
            {
                TargetX = -1;
            }
        }
    }
    if (Output.y != 0)
    {
        if (Output.y > GameCentY)
        {
            TargetY = -(GameCentY - Output.y);
            TargetY = TargetY / AimSpeed;
            if (TargetY + GameCentY + 1 > ��Ϸ�߶�)
            {
                TargetY = 0;
            }
            if (TargetY<1 && TargetY>Frevise)
            {
                TargetY = 1;
            }
        }
        if (Output.y < GameCentY)
        {
            TargetY = Output.y - GameCentY;
            TargetY = TargetY / AimSpeed;
            if (TargetY + GameCentY - 1 < 0)
            {
                TargetY = 0;
            }
            if (TargetY > -1 && TargetY < Frevise)
            {
                TargetY = -1;
            }
        }
    }
    int dx = 0, dy = 0;
    char szBuf[22] = { NULL };
    sprintf(szBuf, "%.f", TargetX);
    dx = atoi(szBuf);

    sprintf(szBuf, "%.f", TargetY);
    dy = atoi(szBuf);

    //printf("ˮƽ: %d  |  ��ֱ��%d\r\n", dx, dy);
    mouse_event(1, dx, dy, 3, 3);
    //MouseMove_Relative(dx, dy, TRUE);
}

bool SelfIsCrouchFlag() {

    LONG flags = 0;
    ReadGameMemory((LPVOID)(g_dwClientAdr + OFFSET_FORCEJUMP + 0x24), &flags, sizeof(flags));
    return flags == 4 ? false : true;//4վ 5��
}

void StopStop()
{
    DWORD buf = 0;
    WriteGameMemory((LPVOID)(g_dwClientAdr + OFFSET_FORCELEFT), &buf, 4);
    WriteGameMemory((LPVOID)(g_dwClientAdr + OFFSET_FORCERIGHT), &buf, 4);
    WriteGameMemory((LPVOID)(g_dwClientAdr + OFFSET_FORCEBACKWARD), &buf, 4);
    WriteGameMemory((LPVOID)(g_dwClientAdr + OFFSET_FORCEFORWARD), &buf, 4);
}

DWORD WINAPI ThreadLoops(LPVOID lp)
{
    while (true) {
        if (on_off.g_AttackStop) {
            DWORD buf = 0;
            ReadGameMemory((LPVOID)(g_dwClientAdr + OFFSET_FORCEATTACK), &buf, sizeof(DWORD));
            if (buf == 5) {
                StopStop();
            }
        }
        if (on_off.g_AutoJump) {
            DWORD selfFlags = 0;
            if (GetObjFlags(GetSelfAdr(), &selfFlags)) {
                if (GetAsyncKeyState(VK_SPACE) != 0 && (selfFlags & 1 << 0)) {
                    ULONG jmp = 6;
                    SetCurrentJumpFlag(&jmp); // ǿ��д��
                    Sleep(3);
                }
            }
        }
        Sleep(1);
    }
    return 0;
}

ImU32 floatToImU32(float* col){
    return ImGui::ColorConvertFloat4ToU32({ col[0],col[1],col[2],col[3] });
}

bool ReadGameMemory(LPVOID ptr, LPVOID buf, DWORD len) {
    return ReadProcessMemory(GetCurrentProcess(), ptr, buf, len, 0);
}

bool WriteGameMemory(LPVOID ptr, LPVOID buf, DWORD len) {
    return WriteProcessMemory(GetCurrentProcess(), ptr, buf, len, 0);;
}

bool ListProcessModules() {
    g_dwClientAdr = (DWORD)GetModuleHandleA("client.dll");
    g_dwEngineAdr= (DWORD)GetModuleHandleA("engine.dll");
    return g_dwClientAdr && g_dwEngineAdr ? true : false;
}

void SetRadarEsp(DWORD obj,bool mode) {
    bool temp = mode;
    WriteGameMemory((LPVOID)(obj + hazedumper::netvars::m_bSpotted), &temp, sizeof(temp));
}

void SetPlayLight(DWORD obj, PGLOWCOLOR color) {
    DWORD glowObjectManager = 0;
    ReadGameMemory((LPVOID)(g_dwClientAdr + hazedumper::signatures::dwGlowObjectManager), &glowObjectManager, sizeof(glowObjectManager));
    DWORD index = 0;
    ReadGameMemory((LPVOID)(obj + hazedumper::netvars::m_iGlowIndex), &index, sizeof(index));
    bool temp = true;
    WriteGameMemory((LPVOID)(glowObjectManager + index * 0x38 + 0x8), color, sizeof(GLOWCOLOR));
    WriteGameMemory((LPVOID)(glowObjectManager + index * 0x38 + 0x27), &temp, sizeof(temp));
    WriteGameMemory((LPVOID)(glowObjectManager + index * 0x38 + 0x28), &temp, sizeof(temp));
}

DWORD WINAPI ThreadLoops_Funny(LPVOID lp) {
    while (true)
    {
        vector<DWORD> aroundVec;
        DWORD selfCtT = 0;
        Point3D selfobj = { 0 };
        if (!GetSelfData(&selfobj, &selfCtT, NULL)) {
            continue;
        }
        if (!GetAroundObject(aroundVec)) {
            continue;
        }
        for (auto itr = aroundVec.begin(); itr != aroundVec.end(); itr++) {
            DWORD isCT = 0, hp = 0;
            Point3D obj3D = { 0 };
            BYTE isSelf = -1;
            if (!GetObjData(*itr, NULL, &obj3D, &isCT, &hp, &isSelf, NULL, NULL)) {
                continue;
            }
            if (isCT == 0 || (hp <= 0 || hp > 100) || selfobj == obj3D) {
                continue;
            }
            //���Ҫ����
            if (on_off.g_objlight) {
                //��������Ѿ�
                GLOWCOLOR color;
                if (isCT == selfCtT && on_off.g_light_t) {
                    color.Set(0.f, 1.f, 0.f, 1.f);//��ɫ
                    SetPlayLight(*itr, &color);
                }
                if (isCT != selfCtT) {
                    color.Set(1.f, 0.f, 0.f, 1.f);
                    SetPlayLight(*itr, &color);
                }
            }
            //����״�͸��
            if (on_off.g_radar) {
                SetRadarEsp(*itr, true);
            }
        }
        Sleep(5);
    }
    return 0;
}

DWORD WINAPI ThreadLoops_AutoFire(LPVOID lp) {
    while (true)
    {
        if (on_off.g_auto_fire) {
            DWORD selfCtT = 0;
            if (!GetSelfData(NULL, &selfCtT, NULL)) {
                continue;
            }
            DWORD objid = 0;
            //��ȡ����id
            ReadGameMemory((LPVOID)(OFFSET_AIMPLAYER_ID + GetSelfAdr()), &objid, sizeof(objid));
            if (objid && objid < 64) {
                DWORD objAdr = 0, hp = 0, ct = 0;
                //��ȡ�����ַ
                ReadGameMemory((LPVOID)(g_dwClientAdr + OFFSET1 + (objid - 1) * 0x10), &objAdr, sizeof(objAdr));
                if (GetObjTeam(objAdr, &ct)) {
                    if (ct != selfCtT && GetObjHp(objAdr, &hp)) {
                        //������������ǵ���
                        if (hp > 0) {
                            Sleep(on_off.g_auto_fire_delay_former);
                            SendMessageA(g_hwnd, 513, 1, 0);
                            SendMessageA(g_hwnd, 514, 0, 0);
                            Sleep(on_off.g_auto_fire_delay_back);
                        }
                    }
                }
            }
        }
        Sleep(5);
    }
    return 0;
}

void ESP() {

    Point3D fuck = { 0 };
    Point2D nearObj;
    DWORD nearloopObjAdr = -1;
    double nearObjDistance = 100000.f;

    //�þ���
    float mex[4][4] = { 0 };
    if (!GetMex(mex[0])) {
        return;
    }
    // ��ȡ���ڴ�С
    GetClientRect(g_hwnd, &g_winRecv);

    Point3D self = { 0 };
    DWORD selfCtT = 0;
    DWORD selfHp = 0;
    //��ȡ�Լ�����Ӫ��hp
    if (!GetSelfData(&self, &selfCtT, &selfHp)) {
        return;
    }
    if (selfHp > 0) {
        g_SelfCtT = selfCtT;
    }
    else {
        selfCtT = g_SelfCtT;
    }
    //��ȡ���ж���ʼ����
    vector<DWORD> aroundVec;
    if (!GetAroundObject(aroundVec)) {
        return;
    }
    for (auto itr = aroundVec.begin(); itr != aroundVec.end(); itr++) {
        Point3D obj3D = { 0 };
        DWORD objAdr = 0, isCT = 0, HP = 0, isRefresh = 0;
        BYTE isSelf = -1;
        BOOL bRet = 0;
        bool isView = false;
        //��ȡ������ϸ��Ϣ
        if (!GetObjData(*itr, &objAdr, &obj3D, &isCT, &HP, &isSelf, &isRefresh, &isView)) {
            continue;
        }
        if (isCT == 0) {
            continue;
        }
        if (isCT != 0 && isRefresh != 1)
        {
            if (HP <= 0 || HP > 100)
            {
                if (selfHp >= 1) {
                    //�ж϶�����������Ƿ��غ� �ó��Ƿ��ڹ�ս��
                    if ((abs(self.x - obj3D.x) <= 40.f) && (abs(self.y - obj3D.y) <= 40.f)) {
                        ImGui::GetForegroundDrawList()->AddText(ImVec2((FLOAT)((g_winRecv.right - g_winRecv.left) / 2 - 70.f), (FLOAT)((g_winRecv.bottom - g_winRecv.top) / 2 + 20.f)), Ig_COL32(255, 0, 0, 255), u8"���˹�ս����ע���ݼ�");
                    }
                }
                continue;
            }
            else if (HP > 0 && self == obj3D) {
                continue; // ����
            }
            DWORD headPointAddr = 0;
            Point3D headPoint = { 0 };
            //��ȡ���� headPoint==ͷ������
            GetBones((LPVOID)objAdr, mex[0], 5, NULL, &headPoint);
            headPoint.z += 10.f; // 3D����ƫ��
            // �������
            Point2D screen;
            Point2D headScreen; // ͷ����Ļλ��
            if (!��������ת��Ļ����(obj3D, screen, g_winRecv.right - g_winRecv.left, g_winRecv.bottom - g_winRecv.top, mex[0])) {
                continue;
            }
            // �����ͷ��Ļ����
            ��������ת��Ļ����(headPoint, headScreen, g_winRecv.right - g_winRecv.left, g_winRecv.bottom - g_winRecv.top, mex[0]);
            float DrawH = screen.y - headScreen.y; // ģ����� / ���Ƹ߶�
            float DrawW = DrawH / 2; // ģ�Ϳ�� / ���ƿ��
            float fRet = calcHp(DrawW, HP); // ����Ѫ�����Ƹ߶�
            double  dbDistance = CalcPoint3DDistance(self, obj3D); // ����3D����

            //���Ҫ������ϸ��Ϣ
            if (on_off.g_Espinfo && ((on_off.g_EspFriend && selfCtT == isCT) || selfCtT != isCT)) {
                char szBuf[0x100] = { 0 };
                ::sprintf(szBuf, u8"���룺%.1f", dbDistance / 10); // д���ַ���
                ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(screen.x - DrawW / 2, screen.y + 5.f),
                    ImVec2(screen.x - DrawW / 2 + strlen(szBuf) * 7, screen.y + 5.f + 14.f), floatToImU32(g_32Col_textBack));

                //����Լ��Ǿ� ����������Ƿ� �Ҵ���c4
                if (selfCtT == 3 && selfCtT != isCT && GetObjIsC4(objAdr)) {
                    strcat(szBuf, u8" -> C4ը��");
                }
                ImGui::GetForegroundDrawList()->AddText(ImVec2(screen.x - DrawW / 2, screen.y + 5.f), floatToImU32(g_32Col_text), szBuf);
            }
            //�������ģʽ
            if (on_off.g_debug_mode) {
                char buf[0x100] = { 0 };
                ::sprintf(buf, "object:%p\n x:%.2f y:%.2f z:%.2f", objAdr, obj3D.x, obj3D.y, obj3D.z);
                ImGui::GetForegroundDrawList()->AddText(ImVec2{ screen.x - DrawW / 2, screen.y + 22.f }, floatToImU32(g_32Col_text), buf);
                ZeroMemory(buf, 0x100);
                ::sprintf(buf, "Self\n object:%p\n x:%.2f y:%.2f z:%.2f",GetSelfAdr(), self.x, self.y, self.z);
                ImGui::GetForegroundDrawList()->AddText(ImVec2{ 20.f,20.f }, floatToImU32(g_32Col_text), buf);
            }

            //���Ҫ���ƹ��� ��ȡ�������ݲ�����
            if (on_off.g_EspBons && ((on_off.g_EspFriend && selfCtT == isCT) || selfCtT != isCT)) {
                GetBones((LPVOID)objAdr, mex[0], floatToImU32(g_32Col_bone), isCT != 2);
            }
            // ���Ҫ���ƶ��� �� ������Ӫ����һ�� �� ���Ʒ����ܿ���
            if (on_off.g_EspFriend && selfCtT == isCT && on_off.g_Esp) {
                DrawBox2(screen.x - (DrawW / 2), screen.y - DrawH, DrawW, DrawH, floatToImU32(g_32Col_friendObj), 2.f);
                DrawBox(screen.x - (DrawW / 2) - 1, screen.y - DrawH - 8, fRet, 1, floatToImU32(g_32Col_Hp));
            }
            //�����Ӫ���Ҳ�һ�� ���ǵ���
            else if (selfCtT != isCT && on_off.g_Esp) {
                DrawBox2(screen.x - (DrawW / 2), screen.y - DrawH, DrawW, DrawH, floatToImU32(g_32Col_Obj), 2.f);
                DrawBox(screen.x - (DrawW / 2) - 1, screen.y - DrawH - 8, fRet, 1, floatToImU32(g_32Col_Hp));
            }

            Point3D autoaig_body = { 0 };
            Point2D headScreenAim; // ��׼λ��
            //��ȡͷ������Ļ�����3d����
            GetBones((LPVOID)objAdr, mex[0], on_off.g_AutoAimBody, &headScreenAim, &autoaig_body); // ��ȡ��׼λ�õ���Ļ����
            //�������׼�Ǿ���
            double dbDis = calc2DDistance(headScreenAim, { (float)(g_winRecv.right - g_winRecv.left) / 2.f,
                (float)(g_winRecv.bottom - g_winRecv.top) / 2.f });
            //���Ҫ������� �� ���������Ӫ����һ��
            if (on_off.g_AutoAimFriend && selfCtT == isCT) {

                if (dbDis < nearObjDistance) {
                    fuck = autoaig_body;
                    nearObj = headScreenAim; // ��¼�����
                    nearloopObjAdr = *itr; // ��ȡ���������ID
                    nearObjDistance = dbDis;
                }
            }
            //��������ǵ���
            else if (selfCtT != isCT) {

                if (dbDis < nearObjDistance)
                {
                    fuck = autoaig_body;
                    nearObj = headScreenAim; // ��¼�����
                    nearloopObjAdr = *itr; // ��ȡ���������ID
                    nearObjDistance = dbDis;
                }
            }
        }
    }
    DWORD winW = (g_winRecv.right - g_winRecv.left);
    DWORD winH = (g_winRecv.bottom - g_winRecv.top);
    bool bIsView = false;
    DWORD objct = 0;
    //������������� g_AimObjAdr==���˶����ַ
    if (on_off.g_AimObjAdr != -1)
    {
        Point2D AimBodyScreen;
        DWORD ObjAdr = -1;
        DWORD Hp = -1;
        if (!GetObjData(on_off.g_AimObjAdr, &ObjAdr, NULL, &objct, &Hp, NULL, NULL, &bIsView)) {
            //���û�л�ȡ��������Ϣ ��Ϊδѡ��״̬
            on_off.g_AimObjAdr = -1;
        }
        //��ȡ��׼λ�õ���Ļ����
        GetBones((LPVOID)ObjAdr, mex[0], on_off.g_AutoAimBody, &AimBodyScreen, NULL);
        double dbDis = calc2DDistance(AimBodyScreen, { (float)(g_winRecv.right - g_winRecv.left) / 2.f,
           (float)(g_winRecv.bottom - g_winRecv.top) / 2.f });
        if (Hp <= 0 || dbDis > on_off.g_AutoAimR || !bIsView)
        {
            //g_AimObjAdr = -1;/*��Ϊδѡ��״̬*/
        }
        else
        {
            nearObj = AimBodyScreen;/*������׼����Ļ����*/
        }
    }
    if (on_off.g_AutoAim)
    {
        if (on_off.g_AimObjAdr == -1) // ��û�в�����׼�ĵ���
        {
            on_off.g_AimObjAdr = nearloopObjAdr; // ������ѭ������ĵ���ID��ȡ
        }
        else
        {
            //�������鷶Χ
            if (on_off.g_EspLine) {
                ImGui::GetForegroundDrawList()->AddCircle(ImVec2(winW / 2, winH / 2), on_off.g_AutoAimR,
                    floatToImU32(g_32Col_cir), 0, 2.f);
            }
            DWORD dwAttackFlag = 0;
            GetCurrentAttackFlag(&dwAttackFlag);
            //�������������뾶��
            if (nearObjDistance < on_off.g_AutoAimR)
            {
                //��������������
                if (GetAsyncKeyState(on_off.g_AutoAimKey) != 0) {
                    int nAimX = 0, nAimY = 0;
                    nAimX = winW / 2;
                    nAimY = winH / 2;
                    //���Ҫ�Զ�ѹǹ
                    if (on_off.g_AutoRecoilForce)
                    {
                        // ��ȡ���׼��
                        DWORD dwSelfAdr = GetSelfAdr();
                        float fOffsetY = 0.f;
                        float fOffsetX = 0.f;
                        GetObjAttackAngle(dwSelfAdr, &fOffsetY, &fOffsetX);
                        fOffsetY = fOffsetY * g_AutoRecoilForceVal;
                        nearObj.x += fOffsetX;
                        nearObj.y -= fOffsetY;
                    }
                    if (on_off.g_AimLock)// ����
                    {
                        MouseAimbot(nearObj, nAimX, nAimY, winW, winH, on_off.g_AutoAimSpeed);
                    }
                    if (on_off.g_AimLock_Memory) {
                        //�ڴ汩��������������Ů��~
                        AutoAimLockMmeory(self, fuck, on_off.g_AutoAimSpeed);
                    }
                }
            }
        }
    }
}