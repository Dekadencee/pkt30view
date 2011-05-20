// pkt30viewView.h : interface of the CPkt30viewView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "wtl\atlsplit.h"
#define _WTL_NO_CSTRING
#include "wtl\atlmisc.h"
#include "Pkt30viewDoc.h"
#include "OutView.h"
#include "hexview.h"
#include <time.h>

class CPkt30viewView : public CFrameWindowImpl<CPkt30viewView>
{
private:
	CFont m_font;
	CPkt30viewDoc* m_doc;
public:
	CPkt30viewView()
		:m_doc(NULL)
	{
	}
//	DECLARE_WND_CLASS(NULL, 0)

	CSplitterWindow m_vSplit;
	CHorSplitterWindow m_hzSplit;

	CPaneContainer m_topPane, m_leftPane, m_rightPane;
	CListViewCtrl m_lv;
	COutView m_out;
	CHexView m_hex;

	BEGIN_MSG_MAP(CPkt30viewView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		NOTIFY_HANDLER(IDC_LISTVIEW, LVN_GETDISPINFO, OnGetDispInfo)
		NOTIFY_HANDLER(IDC_LISTVIEW, LVN_ITEMCHANGED, OnItemChanged)
		CHAIN_MSG_MAP(CFrameWindowImpl<CPkt30viewView>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	void SetDoc(CPkt30viewDoc* doc)
	{
		m_doc = doc;
	}
	LPCTSTR FileInfoStr()
	{
		PKT_HEADER h = m_doc->GetHeader();
		CString result;
		result.Format("WoW build: %d, Language: '%c%c%c%c', Sniffer ID: %d", h.build, h.language[0], h.language[1], h.language[2], h.language[3], h.snifferID);
		switch (h.snifferID)
		{
		case 0:
			result += " (Wad)";
			break;
		case 1:
			result += " (Nomad)";
			break;
		case 2:
			result += " (WoWCore)";
			break;
		case 3:
			result += " (Mangos 'TOM_RUS')";
			break;
		case 4:
			result += " (User456)";
			break;
		case 5:
			result += " (Delfin)";
			break;
		case 6:
			result += " (Burlex)";
			break;
		case 7:
			result += " (WCell)";
			break;
		case 8:
			result += " (Kobold)";
			break;
		case 9:
			result += " (abdula123)";
			break;
		case 10:
			result += " (Konctantin/LordJZ)";
			break;
		case 11:
			result += " (Joha)";
			break;
		default:
			result += " (unknown)";
		}
		return result;
	}

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
	{
		m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Courier New"));
		CRect rect;
		GetParent().GetClientRect(&rect);
		m_hzSplit.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_hzSplit.m_cxyMin = 100;
		m_hzSplit.SetSplitterPos(int(rect.bottom * 0.7));
		m_hzSplit.m_bFullDrag = false;
		m_hWndClient = m_hzSplit;
		
		m_vSplit.Create(m_hzSplit, rect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_vSplit.m_cxyMin = 100;
		m_vSplit.SetSplitterPos(int(rect.right * 0.5));
		m_vSplit.m_bFullDrag = false;
		m_hzSplit.SetSplitterPane(SPLIT_PANE_BOTTOM, m_vSplit);

		m_topPane.Create(m_hzSplit);
		m_topPane.SetTitle(FileInfoStr());
		m_topPane.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_hzSplit.SetSplitterPane(SPLIT_PANE_TOP, m_topPane);

		m_lv.Create(m_topPane, rcDefault, 0, WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDATA | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE, IDC_LISTVIEW);
		m_lv.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_EX_DOUBLEBUFFER);
		m_lv.SetFont(m_font);
		m_lv.GetHeader().SetFont(AtlGetDefaultGuiFont());
		m_lv.InsertColumn(0, _T("¹"), LVCFMT_LEFT, 60);
		m_lv.InsertColumn(1, _T("Time"), LVCFMT_LEFT, 100);
		m_lv.InsertColumn(2, _T("Tick count"), LVCFMT_LEFT, 100);
		m_lv.InsertColumn(3, _T("Direction"), LVCFMT_LEFT, 100);
		m_lv.InsertColumn(4, _T("Opcode"), LVCFMT_LEFT, 400);
		m_lv.InsertColumn(5, _T("Length"), LVCFMT_LEFT, 100);
		m_topPane.SetClient(m_lv);

		m_leftPane.Create(m_vSplit);
		m_leftPane.SetTitle(_T("Hex dump"));
		m_leftPane.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_vSplit.SetSplitterPane(SPLIT_PANE_LEFT, m_leftPane);

		m_hex.Create(m_leftPane, rcDefault, NULL, ES_AUTOHSCROLL | ES_LEFT | ES_READONLY | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
		m_hex.SetFont(m_font);
		m_leftPane.SetClient(m_hex);

		m_rightPane.Create(m_vSplit);
		m_rightPane.SetTitle(_T("Parsed packet"));
		m_rightPane.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_vSplit.SetSplitterPane(SPLIT_PANE_RIGHT, m_rightPane);

		m_out.Create(m_rightPane, rcDefault, NULL, ES_AUTOHSCROLL | ES_LEFT | ES_READONLY | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
		m_out.SetFont(m_font);
		m_rightPane.SetClient(m_out);

		if (m_doc)
			m_lv.SetItemCount(m_doc->GetItemsCount());
		return 0;
	}
	
	LRESULT OnGetDispInfo(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		time_t time;
		char column[100];
		memset(column, 0, 100);

		NMLVDISPINFO *pDispInfo = (NMLVDISPINFO *)pnmh;

		LPPKT_CHUNK chunk = m_doc->GetPacketChunk(pDispInfo->item.iItem);
		if (pDispInfo->item.mask & LVIF_TEXT)
		{
			switch (pDispInfo->item.iSubItem)
			{
			case 0:
				sprintf_s(column, "%d", pDispInfo->item.iItem + 1);
				break;
			case 1:
				time = chunk->unixTime;
				strftime(column, 100, "%X", localtime(&time));
				break;
			case 2:
				sprintf_s(column, "%d", chunk->tickCount - m_doc->GetStartTick());
				break;
			case 3:
				strncpy_s(column, (LPCTSTR)chunk->direction.b, 4);
				break;
			case 4:
				sprintf_s(column, "%s", m_doc->GetOpcodeName(pDispInfo->item.iItem));
				break;
			case 5:
				sprintf_s(column, "%d", chunk->dataLength - sizeof(int));
				break;
			default:
				sprintf_s(column, "UNKNOWN COLUMN");
			}

			strcpy_s(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, column);
		}

		return 0;
	}
	
	LRESULT OnItemChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		NMLISTVIEW *nmlv = (NMLISTVIEW *)pnmh;

		if  ((nmlv->uChanged == LVIF_STATE)  && (nmlv->uOldState == 0) && (nmlv->uNewState == (LVIS_FOCUSED | LVIS_SELECTED)))
		{
			m_hex.SetRedraw(FALSE);
			m_out.SetRedraw(FALSE);
			m_hex.SetData(m_doc->GetPacketData(nmlv->iItem), m_doc->GetPacketChunk(nmlv->iItem)->dataLength - 4);
			m_out.Clear();
			try
			{
				if (m_doc)
					m_doc->OnItemChange(nmlv->iItem);
			}
			catch (LPCTSTR e)
			{
				m_out.PrintLn(e);
			}
			m_hex.SetRedraw(TRUE);
			m_out.SetRedraw(TRUE);
		}
		return 0;
	}
};
