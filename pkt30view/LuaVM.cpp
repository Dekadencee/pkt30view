#include "StdAfx.h"
#include "LuaVM.h"


CLuaVM::CLuaVM(void)
	: m_LuaState(NULL)
{
}

CLuaVM::~CLuaVM(void)
{
	if (m_LuaState)
		lua_close(m_LuaState);
}

void CLuaVM::InitModule(LPCTSTR filename)
{
	if (m_LuaState)
		lua_close(m_LuaState);

	m_LuaState = lua_open();
	if (!m_LuaState)
		throw("Error opening lua state");

	static const luaL_Reg packet[] =
	{
		{"length", lua_length},
		{"read8", lua_read8},
		{"read16", lua_read16},
		{"read32", lua_read32},
		{"readstring", lua_readstring},
		{"readguid", lua_readguid},
		{"readpackedguid", lua_readpackedguid},
		{"readfloat", lua_readfloat},
		{"decompress", lua_decompress},
		{0, 0}
	};	
	luaL_register(m_LuaState, "packet", packet);

	static const luaL_Reg output[]=
	{
		{"print", lua_print},
		{0, 0}
	};	
	luaL_register(m_LuaState, "output", output);

    if (luaL_loadfile(m_LuaState, filename) || lua_pcall(m_LuaState, 0, 0, 0))
	{
		TCHAR str[512];
		sprintf_s(str, "Error loading & executing file ""%s""", filename);
		throw(str);
	}
}

int CLuaVM::lua_length(lua_State* L)
{
	lua_pushnumber(L, m_packet.Length());
	return 1;
}

int CLuaVM::lua_read8(lua_State* L)
{
	lua_pushnumber(L, m_packet.Read8());
	return 1;
}

int CLuaVM::lua_read16(lua_State* L)
{
	lua_pushnumber(L, m_packet.Read16());
	return 1;
}

int CLuaVM::lua_read32(lua_State* L)
{
	lua_pushnumber(L, m_packet.Read32());
	return 1;
}

int CLuaVM::lua_readfloat(lua_State* L)
{
	lua_pushnumber(L, m_packet.ReadFloat());
	return 1;
}

int CLuaVM::lua_readguid(lua_State* L)
{
	__int64 guid = m_packet.ReadGuid();
	LPBYTE pg = (LPBYTE)&guid;
	char str[100];
	sprintf_s(str, "%02X%02X%02X%02X%02X%02X%02X%02X", pg[0], pg[1], pg[2], pg[3], pg[4], pg[5], pg[6], pg[7]);
	lua_pushstring(L, str);
	return 1;
}

int CLuaVM::lua_readpackedguid(lua_State* L)
{
	__int64 guid = m_packet.ReadPackedGuid();
	LPBYTE pg = (LPBYTE)&guid;
	char str[100];
	sprintf_s(str, "%02X%02X%02X%02X%02X%02X%02X%02X", pg[0], pg[1], pg[2], pg[3], pg[4], pg[5], pg[6], pg[7]);
	lua_pushstring(L, str);
	return 1;
}

int CLuaVM::lua_readstring(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc == 0)
	{
		lua_pushstring(L, m_packet.ReadString());
		return 1;
	}

	if (argc == 1)
	{
		if (lua_isnumber(L, 1))
		{
			lua_pushstring(L, m_packet.ReadString(lua_tointeger(L, 1)));
			return 1;
		}
		else
			throw("readstring(arg): invalid arg type");
	}

	return 0;
}

int CLuaVM::lua_print(lua_State* L)
{
	int argc = lua_gettop(L);
	if (argc)
	{
		if (lua_isstring(L, 1))
		{
			if (m_out)
				m_out->Insert(lua_tostring(L, 1));
		}
	}
	return 1;
}

int CLuaVM::lua_decompress(lua_State* L)
{
	lua_pushnumber(L, m_packet.Decompress());
	return 1;
}

void CLuaVM::CallFunc(LPCTSTR psName, LPBYTE data, int len, void* out)
{
	m_packet.Init(data, len);
	m_out = static_cast<CScrollView*>(out);

	lua_getglobal(m_LuaState, psName);
	if (lua_pcall(m_LuaState, 0, 0, 0) != 0)
	{
		throw(lua_tostring(m_LuaState, -1));
	}
}

CPacket			CLuaVM::m_packet;
CScrollView*	CLuaVM::m_out;
