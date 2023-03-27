#pragma once
#include "def.hpp"


struct PlayerInfo
{
	std::uint64_t version;
	union
	{
		std::uint64_t xuid;//玩家SteamID
		struct
		{
			std::uint32_t xuidLow;
			std::uint32_t xuidHigh;
		};
	};
	char name[128];//玩家名称
	int userId;//玩家ID
	char guid[33];
	std::uint32_t friendsId;
	char friendsName[128];
	bool fakeplayer;//是否是电脑玩家
	bool hltv;
	int customfiles[4];
	unsigned char filesdownloaded;
	int entityIndex;
};

//矩阵辅助类
class matrix3x4
{
private:
	float mat[3][4];
public:
	constexpr auto operator[](int i) const noexcept { return mat[i]; }
};

struct Vector
{
	constexpr float operator[](int i)
	{
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		}
		return 0.0f;
	}

	constexpr operator bool() const noexcept
	{
		return x || y || z;
	}

	constexpr Vector& operator=(float array[3]) noexcept
	{
		x = array[0];
		y = array[1];
		z = array[2];
		return *this;
	}

	constexpr Vector& operator+=(const Vector& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	constexpr Vector& operator-=(const Vector& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	constexpr auto operator-(const Vector& v) const noexcept
	{
		return Vector{ x - v.x, y - v.y, z - v.z };
	}

	constexpr auto operator+(const Vector& v) const noexcept
	{
		return Vector{ x + v.x, y + v.y, z + v.z };
	}

	constexpr Vector& operator/=(float div) noexcept
	{
		x /= div;
		y /= div;
		z /= div;
		return *this;
	}

	constexpr auto operator*(float mul) const noexcept
	{
		return Vector{ x * mul, y * mul, z * mul };
	}

	constexpr void normalize() noexcept
	{
		x = std::isfinite(x) ? std::remainder(x, 360.0f) : 0.0f;
		y = std::isfinite(y) ? std::remainder(y, 360.0f) : 0.0f;
		z = 0.0f;
	}

	auto length() const noexcept
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	auto length2D() const noexcept
	{
		return std::sqrt(x * x + y * y);
	}

	constexpr auto squareLength() const noexcept
	{
		return x * x + y * y + z * z;
	}

	constexpr auto dotProduct(const Vector& v) const noexcept
	{
		return x * v.x + y * v.y + z * v.z;
	}

	constexpr auto transform(const matrix3x4& mat) const noexcept
	{
		return Vector{ dotProduct({ mat[0][0], mat[0][1], mat[0][2] }) + mat[0][3],
					   dotProduct({ mat[1][0], mat[1][1], mat[1][2] }) + mat[1][3],
					   dotProduct({ mat[2][0], mat[2][1], mat[2][2] }) + mat[2][3] };
	}

	float x, y, z;
};

//相对地址转化为绝对地址
template <typename T>
static constexpr auto relativeToAbsolute(int* address) noexcept
{
	return reinterpret_cast<T>(reinterpret_cast<char*>(address + 1) + *address);
}

template<typename T, typename ...Args>
constexpr auto callVirtualMethod(void* classBase, int index, Args... args) noexcept
{
	return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(classBase))[index])(classBase, args...);
}

class Engine
{
public:
	//根据索引获取玩家信息
	constexpr auto getPlayerInfo(int entityIndex, const PlayerInfo& playerInfo) noexcept
	{
		return callVirtualMethod<bool, int, const PlayerInfo&>(this, 8, entityIndex, playerInfo);
	}

	//根据玩家ID获取玩家索引
	constexpr auto getPlayerForUserID(int userId) noexcept
	{
		return callVirtualMethod<int, int>(this, 9, userId);
	}

	//获取本地玩家索引
	constexpr auto getLocalPlayer() noexcept
	{
		return callVirtualMethod<int>(this, 12);
	}

	//获取视图角度
	constexpr auto getViewAngles(Vector& angles) noexcept
	{
		callVirtualMethod<void, Vector&>(this, 18, angles);
	}

	//设置视图角度
	constexpr auto setViewAngles(const Vector& angles) noexcept
	{
		callVirtualMethod<void, const Vector&>(this, 19, angles);
	}

	//获取当前玩家的数量
	constexpr auto getMaxClients() noexcept
	{
		return callVirtualMethod<int>(this, 20);
	}

	//判断是否在对局中
	constexpr auto isInGame() noexcept
	{
		return callVirtualMethod<bool>(this, 26);
	}

	//判断游戏是否连接
	constexpr auto isConnected() noexcept
	{
		return callVirtualMethod<bool>(this, 27);
	}

	using Matrix = float[4][4];

	//获取自身矩阵
	constexpr auto worldToScreenMatrix() noexcept
	{
		return callVirtualMethod<const Matrix&>(this, 37);
	}

	constexpr auto getBSPTreeQuery() noexcept
	{
		return callVirtualMethod<void*>(this, 43);
	}

	//获取等级名称
	constexpr auto getLevelName() noexcept
	{
		return callVirtualMethod<const char*>(this, 53);
	}

	constexpr auto clientCmdUnrestricted(const char* cmd) noexcept
	{
		callVirtualMethod<void, const char*, bool>(this, 114, cmd, false);
	}
};

bool FindInterface();

extern Engine* engine;