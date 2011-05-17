#pragma once
#include "scrollView.h"

class CHexView : public CScrollView
{
	LPBYTE m_data;
	int m_len;
	char m_hex[60];
	char m_alpha[20];
	void printhex(int len)
	{
		char str[10];
		memset(m_hex, 0, 60);
		for (int i = 0; i < len; i++)
		{
			sprintf_s(str, " %02X", m_data[i]);
			strcat_s(m_hex, str);
		}
		if (len < 16)
			for (int i = 0; i < 16 - len; i++)
				strcat_s(m_hex, "   ");
	}
	void printalpha(int len)
	{
		memset(m_alpha, 0, 20);
		for (int i = 0; i < len; i++)
		{
			if (isprint(m_data[i]))
				m_alpha[i] = m_data[i];
			else
				m_alpha[i] = '.';
		}
		if (len < 16)
			for (int i = 0; i < 16 - len; i++)
				strcat_s(m_alpha, " ");
	}
	void DumpToHex()
	{
		Clear();
		int nrows = m_len / 16;
		int nrem = m_len % 16;
		Insert("+------+-------------------------------------------------+-----------------+");
		Insert("|offset|  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F | 0123456789ABCDEF|");
		Insert("+------+-------------------------------------------------+-----------------+");
		if (nrows)
		{
			for (int i = 0; i < nrows; i++)
			{
				printhex(16);
				printalpha(16);
				char str[100];
				sprintf_s(str, "|%06X|%s | %s|", i * 16, m_hex, m_alpha);
				Insert(str);
				m_data += 16;
			}
		}
		if (nrem)
		{
			printhex(nrem);
			printalpha(nrem);
			char str[100];
			sprintf_s(str, "|%06X|%s | %s|", nrows * 16, m_hex, m_alpha);
			Insert(str);
		}
		Insert("+------+-------------------------------------------------+-----------------+");
		//isprint()
		//SetScrollPos(
	}
public:
	void SetData(LPBYTE data, int len)
	{
		m_data = data;
		m_len = len;
		DumpToHex();
	}
};
