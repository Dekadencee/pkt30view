#pragma once
class CPacket
{
public:
	CPacket(void);
	~CPacket(void);
private:
	LPBYTE m_begin;
	LPBYTE m_ptr;
	int m_len;
	LPTSTR m_buf;
public:
	void Init(LPBYTE data, int len);
	int Length();
	int Read8();
	int Read16();
	int Read32();
	float ReadFloat();
	__int64 ReadGuid();
	__int64 ReadPackedGuid();
	int Decompress();
	LPCTSTR ReadString(int len = 0);
};

