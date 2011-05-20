#pragma once
#include "wtl\atlctrls.h"
#include <atlstr.h>

class COutView :public CEdit
{
public:
	void Print(LPCTSTR str);
	void PrintLn(LPCTSTR str);
	void Clear();
	COutView(void);
	~COutView(void);
};

