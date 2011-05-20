#include "StdAfx.h"
#include "OutView.h"


COutView::COutView(void)
{
}


COutView::~COutView(void)
{
}

void COutView::Print(LPCTSTR str)
{
	CString text;
	GetWindowText(text);
	text += str;
	SetWindowText(text);
}

void COutView::PrintLn(LPCTSTR str)
{
	CString text;
	GetWindowText(text);
	text = text + str + "\r\n";
	SetWindowText(text);
}

void COutView::Clear()
{
	SetWindowText(NULL);
}
