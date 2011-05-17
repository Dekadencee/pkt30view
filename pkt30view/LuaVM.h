#pragma once
#include "lua\lua.hpp"
#include "packet.h"
#include "scrollView.h"

class CLuaVM
{
public:
	CLuaVM(void);
	~CLuaVM(void);
private:
	lua_State* m_LuaState;
	static CPacket m_packet;
	static CScrollView* m_out;
private:
	static int lua_length(lua_State* L);
	static int lua_read8(lua_State* L);
	static int lua_read16(lua_State* L);
	static int lua_read32(lua_State* L);
	static int lua_readfloat(lua_State* L);
	static int lua_print(lua_State* L);
	static int lua_readstring(lua_State* L);
	static int lua_readguid(lua_State* L);
	static int lua_readpackedguid(lua_State* L);
	static int lua_decompress(lua_State* L);
public:
	void InitModule(LPCTSTR filename);
	void CallFunc(LPCTSTR psName, LPBYTE data, int len, void* out);
};

