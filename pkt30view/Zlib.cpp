#include "StdAfx.h"
#include "Zlib.h"

CZlib::CZlib(void)
	: m_out_buffer(NULL)
{
	ZeroMemory(&m_zstream, sizeof(m_zstream));
}

CZlib::~CZlib(void)
{
	if (m_out_buffer)
	{
		delete[] m_out_buffer;
	}
}

bool CZlib::decompress(BYTE* in_buffer, int in_size, int out_size)
{
	m_out_buffer = new BYTE[out_size];
	if (!m_out_buffer)
	{
		return false;
	}
	m_zstream.next_in = in_buffer;
	m_zstream.avail_in = in_size;
	m_zstream.next_out = m_out_buffer;
	m_zstream.avail_out = out_size;
	if (inflateInit(&m_zstream) == Z_OK)
	{
		if (inflate(&m_zstream, Z_FINISH) == Z_STREAM_END)
		{
			return true;
		}
		inflateEnd(&m_zstream);
	}
	return false;
}

BYTE* CZlib::get_data(void)
{
	return m_out_buffer;
}
