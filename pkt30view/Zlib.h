#pragma once
#include "zlib\zlib.h"
#include <windows.h>

class CZlib
{
public:
	CZlib(void);
	~CZlib(void);

private:
	z_stream m_zstream;
	BYTE *m_out_buffer;
public:
	bool decompress(BYTE* in_buffer, int in_size, int out_size);
	BYTE* get_data(void);
};

