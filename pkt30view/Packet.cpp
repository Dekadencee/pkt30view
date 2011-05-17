#include "StdAfx.h"
#include "Packet.h"
#include "Zlib.h"

CPacket::CPacket(void)
	: m_begin(NULL)
	, m_ptr(NULL)
	, m_len(0)
	, m_buf(NULL)
{
}


CPacket::~CPacket(void)
{
	if (m_buf)
		delete m_buf;

	if (m_begin)
		delete m_begin;
}


void CPacket::Init(LPBYTE data, int len)
{
	if (m_begin)
		delete m_begin;
	m_begin = new BYTE[len];
	memcpy(m_begin, data, len);
	m_ptr = m_begin;
	m_len = len;
}

int CPacket::Length()
{
	return m_len;
}

int CPacket::Read8()
{
	BYTE result = 0;

	if ((m_len - (m_ptr - m_begin)) > sizeof(result))
	{
		memcpy(&result, m_ptr, sizeof(result));
		m_ptr += sizeof(result);
	}
	return result;
}

int CPacket::Read16()
{
	WORD result = 0;

	if ((m_len - (m_ptr - m_begin)) > sizeof(result))
	{
		memcpy(&result, m_ptr, sizeof(result));
		m_ptr += sizeof(result);
	}
	return result;
}

int CPacket::Read32()
{
	DWORD result = 0;

	if ((m_len - (m_ptr - m_begin)) > sizeof(result))
	{
		memcpy(&result, m_ptr, sizeof(result));
		m_ptr += sizeof(result);
	}
	return result;
}

float CPacket::ReadFloat()
{
	float result = 0;

	if ((m_len - (m_ptr - m_begin)) > sizeof(result))
	{
		memcpy(&result, m_ptr, sizeof(result));
		m_ptr += sizeof(result);
	}
	return result;
}

__int64 CPacket::ReadGuid()
{
	__int64 result = 0;

	if ((m_len - (m_ptr - m_begin)) > sizeof(result))
	{
		memcpy(&result, m_ptr, sizeof(result));
		m_ptr += sizeof(result);
	}
	return result;
}

__int64 CPacket::ReadPackedGuid()
{
	__int64 result = 0;
	BYTE *presult = (BYTE*)&result;
	BYTE mask	= Read8();
	BYTE bit	= 1;
	
	for (int i = 0; i < 8; i++)
	{
		if ((mask & bit))
			presult[i] = Read8();
		else
			presult[i] = 0;
		bit = bit << 1;
	}
	return result;
}

char* CPacket::ReadString(int len)
{
	int bufsize;
	if (m_buf)
		delete m_buf;
	
	if (len)
		bufsize = len + 1;
	else
		bufsize = m_len - (m_ptr - m_begin) + 1;

	m_buf = new TCHAR[bufsize];
	memset(m_buf, 0, bufsize);

	TCHAR c;
	int i = 0;
	do
	{
		c = *m_ptr;
		m_buf[i] = c;
		i++;
		m_ptr++;
	} while ((c) && (i < bufsize) && (m_ptr < m_begin + m_len));
	
	char* r = str_convert(m_buf, CP_UTF8, CP_ACP);
	return r;
}

int CPacket::Decompress()
{
	CZlib zlib;
	m_ptr = m_begin;
	DWORD unpackedlen = Read32();
	if (unpackedlen)
	{
		zlib.decompress(m_begin + 4, m_len - 4, unpackedlen);
		delete[] m_begin;
		m_begin = new BYTE[unpackedlen];
		memcpy(m_begin, zlib.get_data(), unpackedlen);
		m_ptr = m_begin;
		m_len = unpackedlen;
		return 1;
	}
	return 0;
}

char* CPacket::str_convert(const char *buf, UINT codepage_from, UINT codepage_to)
{
	wchar_t* pResw = 0;
	char* pRes = 0;
	int res_len = 0;
	res_len = MultiByteToWideChar(codepage_from, 0, (LPCSTR)buf, -1, 0, 0);
	if (!res_len)
		return NULL;
	pResw = new wchar_t[res_len];
	if (!MultiByteToWideChar(codepage_from, 0, (LPCSTR)buf, -1, pResw, res_len))
	{
		delete[] pResw;
		return NULL;
	}
	res_len = WideCharToMultiByte(codepage_to, 0, pResw, -1, 0, 0, 0, 0);
	if (!res_len)
		return 0;
	pRes = new char[res_len];
	if (!pRes)
		return NULL;
	if (!WideCharToMultiByte(codepage_to, 0, pResw, -1, pRes, res_len, 0, 0))
	{
		delete[] pRes;
		return NULL;
	}
	delete[] pResw;
	return pRes;
}