// SkinCheck.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EnvCtrls.h"
#include <NKGdiMgr/NKGdiMgr.h>
#include <NKGlobalDataMgr/NKGlobalDataMgr.h>

#define WM_CXSHADE_RADIO WM_USER+0x100

//#define GetRValue(rgb)      ((BYTE)((rgb) & 0xFF))
//#define GetGValue(rgb)      ((BYTE)(((rgb) >> 8) & 0xFF))
//#define GetBValue(rgb)      ((BYTE)(((rgb) >> 16) & 0xFF))

extern CNKGdiManager* GetGDIMng();
//extern CNKColorMap* GetColorMap();

#define WMU_POPUP_MENU		(WM_USER + 1201)

//-------------------------------------------------------------------------
// CEnvCheck

IMPLEMENT_DYNAMIC(CEnvCheck, CButton)

CEnvCheck::CEnvCheck()
{
	m_stInfo.bCheck = FALSE;
	m_stInfo.nMarkWidth = 0;
	m_stInfo.nMarkHeight = 0;
	m_stInfo.nFontSize = 12;
	m_stInfo.bFontBold = FALSE;
	m_stInfo.crText = RGB(8,3,69);
	m_stInfo.nAlignFormat = DT_CENTER|DT_VCENTER;
	m_stInfo.crBack = RGB(236,233,213);

	m_pFont = nullptr;
}

CEnvCheck::~CEnvCheck()
{
}


BEGIN_MESSAGE_MAP(CEnvCheck, CButton)
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WMU_INIT_ENV_CTRL, &CEnvCheck::OnInitEnvCtrl)
END_MESSAGE_MAP()


LRESULT CEnvCheck::OnInitEnvCtrl(WPARAM wParam, LPARAM lParam)
{
	// 사용자 정의 초기화 코드
	TRACE(_T("WMU_INIT_ENV_CTRL 메시지 수신!\n"));

	// 예시: wParam/lParam을 이용한 설정
	//int envID = static_cast<int>(wParam);
	//CString config = reinterpret_cast<LPCTSTR>(lParam);

	m_stInfo.nNormal		= -1;
	m_stInfo.nNormaldis		= -1;
	m_stInfo.nSelected		= -1;
	m_stInfo.nSelecteddis	= -1;

	// 초기화 로직 수행
	GetGDIMng()->GetBitmapMng()->GetImage(m_stInfo.nNormal, _T("main/common/mnu_check_normal.svg"));
	GetGDIMng()->GetBitmapMng()->GetImage(m_stInfo.nNormaldis, _T("main/common/mnu_check_normal.svg"));
	GetGDIMng()->GetBitmapMng()->GetImage(m_stInfo.nSelected, _T("main/common/mnu_check_select.svg"));
	GetGDIMng()->GetBitmapMng()->GetImage(m_stInfo.nSelecteddis, _T("main/common/env_check_sel_d.svg"));

	SetMarkSize(12, 12);

	m_stInfo.crBack = RGB(40, 42, 51);// RGB(29, 31, 37);
	m_stInfo.crText = RGB(238, 238, 238);
	//SetFontAttr(_T("DB Heavent FSS"), 10, FontStyleRegular);
	m_pFont = GetGDIMng()->GetFont(78);
	m_stInfo.nAlignFormat = DT_LEFT | DT_VCENTER;

	return 0;
}

//BOOL CEnvCheck::SetMarkImage(UINT nImageID)
//{
//	if( m_stInfo.bmpMark.GetSafeHandle() )
//		m_stInfo.bmpMark.DeleteObject();
//
//	if( !m_stInfo.bmpMark.LoadBitmap(nImageID) )
//		return FALSE;
//
//	BITMAP bm;
//	m_stInfo.bmpMark.GetBitmap(&bm);
//
//	m_stInfo.nMarkWidth = bm.bmWidth / 2;
//	m_stInfo.nMarkHeight = bm.bmHeight;
//
//	Invalidate(TRUE);
//	return TRUE;
//}

void CEnvCheck::DrawImageMark(CPaintDC* pdc, CRect rScreen, BOOL bCheck)
{
//	if( !m_stInfo.bmpMark.GetSafeHandle() )
//		return;
#if 1
	Graphics g(pdc->GetSafeHdc());
	Rect rcSrc(rScreen.left, rScreen.top, rScreen.Width(), rScreen.Height());

	CDC tmpdc;
	tmpdc.CreateCompatibleDC(pdc);

	int cx = m_stInfo.nMarkWidth;
	int cy = m_stInfo.nMarkHeight;
	int margin = (rScreen.Height() / 2) - (cy / 2);

	CBitmap tmpBmp;
	tmpBmp.CreateCompatibleBitmap(pdc, cx, cy);
	CBitmap* pOldBmp = tmpdc.SelectObject(&tmpBmp);

	DWORD dwBGColorRGBA = GetGDIMng()->GetBitmapMng()->ConvertRGBToRGBA(m_stInfo.crBack);

	if (bCheck) {
		GetGDIMng()->GetBitmapMng()->DrawIdBmpEx(&tmpdc, CRect(0, 0, cx, cy), m_stInfo.nSelected, dwBGColorRGBA);
		Bitmap bitmap(tmpBmp, NULL);
		g.DrawImage(&bitmap, rcSrc.X, rcSrc.Y + margin);
}
	else {
		GetGDIMng()->GetBitmapMng()->DrawIdBmpEx(&tmpdc, CRect(0, 0, cx, cy), m_stInfo.nNormal, dwBGColorRGBA);
		Bitmap bitmap(tmpBmp, NULL);
		g.DrawImage(&bitmap, rcSrc.X, rcSrc.Y + margin);
	}
	tmpdc.SelectObject(pOldBmp);
#else
	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	CDC dcMem;
	dcMem.CreateCompatibleDC(pdc);
	dcMem.SelectObject(pBitmap);

	int nStateWidth=0;

	nStateWidth = bm.bmWidth / 2;

	if( m_stInfo.nAlignFormat & DT_BOTTOM )
		rScreen.top = rScreen.bottom - bm.bmHeight;
	else if( m_stInfo.nAlignFormat & DT_VCENTER )
		rScreen.top = rScreen.CenterPoint().y - (bm.bmHeight / 2);

	if( bCheck )
		pdc->BitBlt(rScreen.left, rScreen.top, nStateWidth, bm.bmHeight, &dcMem, nStateWidth, 0, SRCCOPY);
	else
		pdc->BitBlt(rScreen.left, rScreen.top, nStateWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);
#endif
}

void CEnvCheck::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CRect rScreen;
	GetClientRect(&rScreen);

	pDC->FillRect(&rScreen, new CBrush(m_stInfo.crBack));

	//DrawImageMark((CPaintDC*)pDC, &m_stInfo.bmpMark, rScreen, m_stInfo.bCheck);
	DrawImageMark((CPaintDC*)pDC, rScreen, m_stInfo.bCheck);

	CString strCaption;
	GetWindowText(strCaption);

	rScreen.left += (m_stInfo.nMarkWidth+8);
	DrawText((CPaintDC*)pDC, strCaption, m_stInfo.nFontSize, 
		m_stInfo.crText, rScreen, 
		m_stInfo.nAlignFormat, m_stInfo.bFontBold);
}

void CEnvCheck::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW | BS_AUTOCHECKBOX);

	CButton::PreSubclassWindow();
}

void CEnvCheck::SetCheck(BOOL bCheck)
{
	m_stInfo.bCheck = bCheck;
	Invalidate(FALSE);
}

BOOL CEnvCheck::GetCheck(void)
{
	return m_stInfo.bCheck;
}

void CEnvCheck::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetCheck(!m_stInfo.bCheck);

	// 클릭 메시지를 부모에게 전달
	if (GetParent())
	{
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
	}

	CButton::OnLButtonUp(nFlags, point);
}

void CEnvCheck::DrawText(CPaintDC *pdc, CString strText, int nFontSize, COLORREF crFontColor, CRect rArea, UINT nFormat, BOOL bFontBold)
{
	CFont *pftOld, FontInfo;
	int nFontHeavy=0;

	if (m_pFont == nullptr) {
		nFontHeavy = (bFontBold) ? FW_BOLD : FW_NORMAL;
		FontInfo.CreateFont(nFontSize, 0, 0, 0, nFontHeavy, 0, 0, 0,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_stInfo.strFontName);

		pftOld = pdc->SelectObject(&FontInfo);
	}
	else {
		pftOld = pdc->SelectObject(m_pFont);
	}

	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextColor(crFontColor);

	pdc->DrawText(strText, &rArea, nFormat|DT_SINGLELINE);

	pdc->SelectObject(pftOld);
	FontInfo.DeleteObject();
}

void CEnvCheck::SetWindowText(CString strText)
{
	CWnd::SetWindowText(strText);

	Invalidate(FALSE);
}

void CEnvCheck::SetAlignFormat(UINT nAlignFormat)
{
	m_stInfo.nAlignFormat = nAlignFormat;

	Invalidate(FALSE);
}

void CEnvCheck::SetFontAttr(LPCTSTR lpszFontName, int nSize, int nFontStyle)
{
	m_stInfo.strFontName = lpszFontName;
	m_stInfo.nFontSize = nSize;
	m_stInfo.nFontStyle = nFontStyle;
}

void CEnvCheck::SetFont(CFont* pFont)
{
	m_pFont = pFont;
}


//---------------------------------------------------------------------------------------
// CEnvRadio

IMPLEMENT_DYNAMIC(CEnvRadio, CButton)

CEnvRadio::CEnvRadio()
{
	m_pFont = nullptr;

	m_stInfo.bCheck = FALSE;
	m_stInfo.nMarkWidth = 0;
	m_stInfo.nMarkHeight = 0;
	m_stInfo.nFontSize = 12;
	m_stInfo.bFontBold = FALSE;
	m_stInfo.crText = RGB(8, 3, 69);
	m_stInfo.nAlignFormat = DT_CENTER | DT_VCENTER;
	m_stInfo.crBack = RGB(236, 233, 213);
}

CEnvRadio::~CEnvRadio()
{
}


BEGIN_MESSAGE_MAP(CEnvRadio, CButton)
	ON_MESSAGE(WM_CXSHADE_RADIO, OnRadioInfo)
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WMU_INIT_ENV_CTRL, &CEnvRadio::OnInitEnvCtrl)
END_MESSAGE_MAP()

// CEnvRadio 메시지 처리기입니다.

//BOOL CEnvRadio::SetMarkImage(UINT nImageID)
//{
//	if (m_stInfo.bmpMark.GetSafeHandle())
//		m_stInfo.bmpMark.DeleteObject();
//
//	if (!m_stInfo.bmpMark.LoadBitmap(nImageID))
//		return FALSE;
//
//	BITMAP bm;
//	m_stInfo.bmpMark.GetBitmap(&bm);
//
//	m_stInfo.nMarkWidth = bm.bmWidth / 2;
//	m_stInfo.nMarkHeight = bm.bmHeight;
//
//	Invalidate(TRUE);
//	return TRUE;
//}

void CEnvRadio::SetImages(int nNormal, int nNormaldis, int nSelected, int nSelecteddis)
{
	m_stInfo.nNormal = nNormal;
	m_stInfo.nNormaldis = nNormaldis;
	m_stInfo.nSelected = nSelected;
	m_stInfo.nSelecteddis = nSelecteddis;
}

//void CEnvRadio::DrawImageMark(CPaintDC* pdc, CBitmap* pBitmap, CRect rScreen, BOOL bCheck)
void CEnvRadio::DrawImageMark(CPaintDC* pdc, CRect rScreen, BOOL bCheck)
{
//	if (!m_stInfo.bmpMark.GetSafeHandle())
//		return;
#if 1
	Graphics g(pdc->GetSafeHdc());
	Rect rcSrc(rScreen.left, rScreen.top, rScreen.Width(), rScreen.Height());
	
	CDC tmpdc;
	tmpdc.CreateCompatibleDC(pdc);

	int cx = m_stInfo.nMarkWidth;
	int cy = m_stInfo.nMarkHeight;
	int margin = (rScreen.Height() / 2) - (cy / 2);
	
	CBitmap tmpBmp;
	tmpBmp.CreateCompatibleBitmap(pdc, cx, cy);
	CBitmap* pOldBmp = tmpdc.SelectObject(&tmpBmp);
	
	DWORD dwBGColorRGBA = GetGDIMng()->GetBitmapMng()->ConvertRGBToRGBA(m_stInfo.crBack);
	
	if (bCheck) {
		GetGDIMng()->GetBitmapMng()->DrawIdBmpEx(&tmpdc, CRect(0, 0, cx, cy), m_stInfo.nSelected, dwBGColorRGBA);
		Bitmap bitmap(tmpBmp, NULL);
		g.DrawImage(&bitmap, rcSrc.X, rcSrc.Y+margin);
	}
	else {
		GetGDIMng()->GetBitmapMng()->DrawIdBmpEx(&tmpdc, CRect(0, 0, cx, cy), m_stInfo.nNormal, dwBGColorRGBA);
		Bitmap bitmap(tmpBmp, NULL);
		g.DrawImage(&bitmap, rcSrc.X, rcSrc.Y+margin);
	}
	tmpdc.SelectObject(pOldBmp);
#else
	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	CDC dcMem;
	dcMem.CreateCompatibleDC(pdc);
	dcMem.SelectObject(pBitmap);

	int nStateWidth = 0;

	nStateWidth = bm.bmWidth / 2;

	if (m_stInfo.nAlignFormat & DT_BOTTOM)
		rScreen.top = rScreen.bottom - bm.bmHeight;
	else if (m_stInfo.nAlignFormat & DT_VCENTER)
		rScreen.top = rScreen.CenterPoint().y - (bm.bmHeight / 2);

	if (bCheck)
		pdc->BitBlt(rScreen.left, rScreen.top, nStateWidth, bm.bmHeight, &dcMem, nStateWidth, 0, SRCCOPY);
	else
		pdc->BitBlt(rScreen.left, rScreen.top, nStateWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);
#endif
}


void CEnvRadio::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CRect rScreen;
	GetClientRect(&rScreen);

	pDC->FillRect(&rScreen, new CBrush(m_stInfo.crBack));

	//DrawImageMark((CPaintDC*)pDC, &m_stInfo.bmpMark, rScreen, m_stInfo.bCheck);
	DrawImageMark((CPaintDC*)pDC, rScreen, m_stInfo.bCheck);

	CString strCaption;
	GetWindowText(strCaption);

	rScreen.left += (m_stInfo.nMarkWidth + 8);
	DrawText((CPaintDC*)pDC, strCaption, m_stInfo.nFontSize,
		m_stInfo.crText, rScreen,
		m_stInfo.nAlignFormat, m_stInfo.bFontBold);
}

void CEnvRadio::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW | BS_AUTORADIOBUTTON);

	CButton::PreSubclassWindow();
}

void CEnvRadio::SetCheck(BOOL bCheck)
{
	//CButton::SetCheck(bCheck);
	m_stInfo.bCheck = bCheck;
	Invalidate(FALSE);

	OnBMSetCheck(bCheck);
}

BOOL CEnvRadio::GetCheck()
{
	return m_stInfo.bCheck;
}

void CEnvRadio::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetCheck(TRUE);

	// 클릭 메시지를 부모에게 전달
	if (GetParent())
	{
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
	}

	CButton::OnLButtonUp(nFlags, point);
}

void CEnvRadio::OnBMSetCheck(BOOL bCheck)
{
	if (bCheck)
	{
		HWND hthis, hwnd2, hpwnd;
		hpwnd = GetParent()->GetSafeHwnd();
		hwnd2 = hthis = GetSafeHwnd();
		if (hthis && hpwnd)
		{
			while (1)
			{
				hwnd2 = ::GetNextDlgGroupItem(hpwnd, hwnd2, 0);
				if ((hwnd2 == hthis) || (hwnd2 == NULL)) break;
				if (hwnd2 != this->GetSafeHwnd())
					::PostMessage(hwnd2, WM_CXSHADE_RADIO, 0, 0);
			}
		}
	}
}

LRESULT CEnvRadio::OnRadioInfo(WPARAM wparam, LPARAM)
{
	if (m_stInfo.bCheck)
		SetCheck(FALSE);

	return 0;
}

void CEnvRadio::DrawText(CPaintDC* pdc, CString strText, int nFontSize, COLORREF crFontColor, CRect rArea, UINT nFormat, BOOL bFontBold)
{
	CFont* pftOld, FontInfo;
	int nFontHeavy = 0;

	if (m_pFont == nullptr) {
		nFontHeavy = (bFontBold) ? FW_BOLD : FW_NORMAL;
		FontInfo.CreateFont(nFontSize, 0, 0, 0, nFontHeavy, 0, 0, 0,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_stInfo.strFontName);

		pftOld = pdc->SelectObject(&FontInfo);
	}
	else {
		pftOld = pdc->SelectObject(m_pFont);
	}
	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextColor(crFontColor);

	pdc->DrawText(strText, &rArea, nFormat | DT_SINGLELINE);

	pdc->SelectObject(pftOld);
	FontInfo.DeleteObject();
}

void CEnvRadio::SetWindowText(CString strText)
{
	CWnd::SetWindowText(strText);

	Invalidate(FALSE);
}

void CEnvRadio::SetAlignFormat(UINT nAlignFormat)
{
	m_stInfo.nAlignFormat = nAlignFormat;

	Invalidate(FALSE);
}

void CEnvRadio::SetFontAttr(LPCTSTR lpszFontName, int nSize, int nFontStyle)
{
	m_stInfo.strFontName = lpszFontName;
	m_stInfo.nFontSize = nSize;
	m_stInfo.nFontStyle = nFontStyle;
}

void CEnvRadio::SetFont(CFont* pFont) 
{ 
	m_pFont = pFont; 
}

LRESULT CEnvRadio::OnInitEnvCtrl(WPARAM wParam, LPARAM lParam)
{
	// 사용자 정의 초기화 코드
	TRACE(_T("WMU_INIT_ENV_CTRL 메시지 수신!\n"));

	// 예시: wParam/lParam을 이용한 설정
	int envID = static_cast<int>(wParam);
	CString config = reinterpret_cast<LPCTSTR>(lParam);

	m_stInfo.nNormal		= -1;
	m_stInfo.nNormaldis		= -1;
	m_stInfo.nSelected		= -1;
	m_stInfo.nSelecteddis	= -1;

	// 초기화 로직 수행
	GetGDIMng()->GetBitmapMng()->GetImage(m_stInfo.nNormal, _T("main/common/mnu_radio_normal.svg"));
	GetGDIMng()->GetBitmapMng()->GetImage(m_stInfo.nNormaldis, _T("main/common/mnu_radio_normal.svg"));
	GetGDIMng()->GetBitmapMng()->GetImage(m_stInfo.nSelected, _T("main/common/mnu_radio_select.svg"));
	GetGDIMng()->GetBitmapMng()->GetImage(m_stInfo.nSelecteddis, _T("main/common/mnu_radio_select.svg"));

	SetMarkSize(12, 12);

	m_stInfo.crBack = RGB(29, 31, 37);
	m_stInfo.crText = RGB(238, 238, 238);
	//SetFontAttr(_T("DB Heavent FSS"), 10, FontStyleRegular);
	m_pFont = GetGDIMng()->GetFont(78);
	m_stInfo.nAlignFormat = DT_LEFT | DT_VCENTER;
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

// CSkinListBox

IMPLEMENT_DYNAMIC(CEnvListBox, CListBox)

CEnvListBox::CEnvListBox()
{
	m_stInfo.nAlignFormat = DT_CENTER;
	m_stInfo.nFontSize = 12;
	m_stInfo.bFontBold = FALSE;
	m_stInfo.crText = RGB(8, 3, 69);
	m_stInfo.crItemBack = RGB(255, 255, 255);
	m_stInfo.crSelText = RGB(255, 255, 128);
	m_stInfo.crSelItemBack = RGB(128, 192, 255);
	m_stInfo.crItemDivideLine = RGB(181, 181, 181);
	m_stInfo.crBack = RGB(255, 255, 255);

	m_nTopIndex = 0;
	m_nVisibleItems = 0;
	m_nItemHeight = 24;
	m_bScrollBarVisible = FALSE;
	m_bScrollBarDragging = FALSE;
	m_nScrollBarWidth = 16;
	m_bInitialized = FALSE;

	m_nThumbPos = 0;
}

CEnvListBox::~CEnvListBox()
{
}


BEGIN_MESSAGE_MAP(CEnvListBox, CListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()



// CSkinListBox 메시지 처리기입니다.

void CEnvListBox::DrawText(CPaintDC* pdc, CString strText, int nFontSize, COLORREF crFontColor, CRect rArea, UINT nFormat, BOOL bFontBold)
{
	CFont* pftOld;
	pftOld = pdc->SelectObject(m_pFont);

	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextColor(crFontColor);

	pdc->DrawText(strText, &rArea, nFormat | DT_SINGLELINE);

	pdc->SelectObject(pftOld);
	//FontInfo.DeleteObject();
}

void CEnvListBox::UpdateScrollInfo()
{
	if (!m_bInitialized || !::IsWindow(m_hWnd)) return;

	GetClientRect(&m_rcClient);

	int nTotalItems = GetCount();
	if (nTotalItems > 0 && m_nItemHeight > 0)
	{
		m_nVisibleItems = m_rcClient.Height() / m_nItemHeight;
		m_bScrollBarVisible = (nTotalItems > m_nVisibleItems);

		// TopIndex 범위 체크
		int nMaxTopIndex = max(0, nTotalItems - m_nVisibleItems);
		m_nTopIndex = min(m_nTopIndex, nMaxTopIndex);
	}
	else
	{
		m_nVisibleItems = 0;
		m_bScrollBarVisible = FALSE;
		m_nTopIndex = 0;
	}

	if (m_bScrollBarVisible)
	{
		m_rcScrollBar = m_rcClient;
		m_rcScrollBar.left = m_rcScrollBar.right - m_nScrollBarWidth;
	}
}

void CEnvListBox::OnPaint()
{
	CPaintDC dc(this);
	UpdateScrollInfo();

	// === GDI 더블 버퍼링 시작 ===
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(&dc, m_rcClient.Width(), m_rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

	// 전체 배경
	CBrush bgBrush(m_stInfo.crBack);
	memDC.FillRect(&m_rcClient, &bgBrush);

	// 아이템 영역
	CRect itemRect = m_rcClient;
	if (m_bScrollBarVisible)
		itemRect.right -= m_nScrollBarWidth;

	int nTotalItems = GetCount();
	int nCurrentSel = GetCurSel();

	for (int i = 0; i < m_nVisibleItems && (m_nTopIndex + i) < nTotalItems; i++)
	{
		int nItemIndex = m_nTopIndex + i;
		CRect rcItem(
			itemRect.left,
			itemRect.top + i * m_nItemHeight,
			itemRect.right,
			itemRect.top + (i + 1) * m_nItemHeight);

		BOOL bSelected = (nCurrentSel == nItemIndex);

		COLORREF bgColor = bSelected ? m_stInfo.crSelItemBack : m_stInfo.crItemBack;
		CBrush itemBrush(bgColor);
		memDC.FillRect(&rcItem, &itemBrush);

		CRect rcBox = rcItem;
		if (m_bCheckImage) 
		{
			rcBox.OffsetRect(8, 6);
			rcBox.right = rcBox.left + 12;
			rcBox.bottom = rcBox.top + 12;

			if (bSelected) {
				DWORD dwBGColorRGBA = GetGDIMng()->GetBitmapMng()->ConvertRGBToRGBA(m_stInfo.crSelItemBack);
				GetGDIMng()->GetBitmapMng()->DrawIdBmpEx(&memDC, rcBox, m_nChecked, dwBGColorRGBA);
			}
			else {
				DWORD dwBGColorRGBA = GetGDIMng()->GetBitmapMng()->ConvertRGBToRGBA(m_stInfo.crItemBack);
				GetGDIMng()->GetBitmapMng()->DrawIdBmpEx(&memDC, rcBox, m_nNormal, dwBGColorRGBA);
			}
		}

		CString strText;
		GetText(nItemIndex, strText);

		if (!strText.IsEmpty())
		{
			COLORREF textColor = bSelected ? m_stInfo.crSelText : m_stInfo.crText;
			memDC.SetTextColor(textColor);
			memDC.SetBkMode(TRANSPARENT);

			CFont* pOldFont = memDC.SelectObject(m_pFont);

			CRect textRect = rcItem;
			if (m_bCheckImage) 
				textRect.left += 24;
			else 
				textRect.left += 8;
			textRect.right -= 16;
			textRect.top += 2;
			textRect.bottom -= 2;

			memDC.DrawText(strText, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

			memDC.SelectObject(pOldFont);
		}
	}

	// 커스텀 스크롤바 그리기
	if (m_bScrollBarVisible)
	{
		Graphics g(memDC.GetSafeHdc());  // GDI+ Graphics 객체 생성
		//DrawCustomScrollbar(&memDC);
		DrawCustomScrollBarGDIPlus(&g);
	}

	// === 실제 DC에 복사 ===
	dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	// 정리
	memDC.SelectObject(pOldBitmap);

}

void CEnvListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	return;
//
//	if (lpDrawItemStruct->itemID == -1)
//		return;
//
//	Graphics g(lpDrawItemStruct->hDC);
//	g.SetSmoothingMode(SmoothingModeAntiAlias);
//	g.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
//
//	// 아이템 영역
//	Rect itemRect(lpDrawItemStruct->rcItem.left,
//		lpDrawItemStruct->rcItem.top,
//		lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left,
//		lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top);
//
//	// 스크롤바 영역 제외
//	if (m_bScrollBarVisible)
//	{
//		itemRect.Width -= m_nScrollBarWidth;
//	}
//
//	// 선택 상태
//	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED);
//
//	// 배경 색상
//	Color bgColor = bSelected ? Color(29, 31, 37, 255) : Color(29, 71, 142, 255);
//	SolidBrush bgBrush(bgColor);
//	g.FillRectangle(&bgBrush, itemRect);
//
//	// 체크박스 네모 그리기 (첫 번째 컬럼에만 표시)
//	if (m_bCheckImage)
//	{
//		CRect rcBox(
//			itemRect.X,
//			itemRect.Y,
//			itemRect.X + itemRect.Width,
//			itemRect.Y + itemRect.Height
//		);
//
//		rcBox.OffsetRect(0, 4);
//		rcBox.right = rcBox.left + 16;
//		rcBox.bottom = rcBox.top + 16;
//
//		CDC dc;
//		dc.Attach(lpDrawItemStruct->hDC);  // HDC를 CDC에 붙임
//
//		DWORD dwBGColorRGBA = GetGDIMng()->GetBitmapMng()->ConvertRGBToRGBA(RGB(29, 31, 37));
////		if (p->bChecked)
//			GetGDIMng()->GetBitmapMng()->DrawIdBmpEx(&dc, rcBox, m_nNormal, dwBGColorRGBA);
////		else
////			GetGDIMng()->GetBitmapMng()->DrawIdBmpEx(&dc, rcBox, m_nChecked, dwBGColorRGBA);
//
//		dc.Detach();
//	}
//
//	// 텍스트 가져오기
//	CString strText;
//	GetText(lpDrawItemStruct->itemID, strText);
//
//	if (!strText.IsEmpty())
//	{
//		// 텍스트 색상
//		Color textColor = bSelected ? Color(238, 238, 238, 255) : Color(238, 238, 238, 255);
//		SolidBrush textBrush(textColor);
//
//		// 폰트 설정
//		LOGFONT lf;
//		m_pFont->GetLogFont(&lf);
//		Gdiplus::FontFamily fontFamily(lf.lfFaceName);
//		Gdiplus::Font font(
//			&fontFamily,
//			static_cast<REAL>(-lf.lfHeight),
//			lf.lfWeight >= FW_BOLD ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular,
//			Gdiplus::UnitPixel
//		);
//
//		// 텍스트 영역에 여백 추가
//		RectF textRect(
//			(REAL)itemRect.X + 16,
//			(REAL)itemRect.Y + 2,
//			(REAL)itemRect.Width - 16,
//			(REAL)itemRect.Height - 4);
//
//		// 텍스트 그리기
//		g.DrawString(strText, -1, &font, textRect, nullptr, &textBrush);
//	}
}

void CEnvListBox::DrawCustomScrollbar(CDC* pDC)
{
	if (!m_bScrollBarVisible) return;

	// 스크롤바 배경
	CBrush bgBrush(m_stInfo.crBack);
	pDC->FillRect(&m_rcScrollBar, &bgBrush);

	int nTotalItems = GetCount();
	if (nTotalItems <= m_nVisibleItems) return;

	int nScrollHeight = m_rcScrollBar.Height() - 4;
	int nScrollableItems = nTotalItems - m_nVisibleItems;
	if (nScrollableItems <= 0) return;

	int nThumbHeight = max(20, (m_nVisibleItems * nScrollHeight) / nTotalItems);
	int nThumbTop = (m_nTopIndex * (nScrollHeight - nThumbHeight)) / nScrollableItems;

	int x = m_rcScrollBar.left + 2;
	int y = m_rcScrollBar.top + 2 + nThumbTop;
	int width = m_rcScrollBar.Width() - 4;
	int height = nThumbHeight;

	// 둥근 썸 그리기
	const int radius = 6;

	CRgn rgn;
	rgn.CreateRoundRectRgn(x, y, x + width, y + height, radius * 2, radius * 2);

	CBrush thumbBrush(RGB(103, 108, 128));
	pDC->FillRgn(&rgn, &thumbBrush);

	// 선택적으로 테두리 추가
	// pDC->FrameRgn(&rgn, &thumbBrush, 1, 1);
}

void CEnvListBox::DrawCustomScrollBarGDIPlus(Graphics* g)
{
	if (!m_bScrollBarVisible) return;

	// 스크롤바 배경만 (테두리 없음)
	Rect scrollBarRect(m_rcScrollBar.left, m_rcScrollBar.top,
		m_rcScrollBar.Width(), m_rcScrollBar.Height());

	SolidBrush bgBrush(Color(GetRValue(m_stInfo.crBack), 
							 GetGValue(m_stInfo.crBack),
							 GetBValue(m_stInfo.crBack)));
	g->FillRectangle(&bgBrush, scrollBarRect);

	int nTotalItems = GetCount();
	if (nTotalItems <= m_nVisibleItems) return;

	// 썸 계산
	int nScrollHeight = m_rcScrollBar.Height() - 4;
	int nScrollableItems = nTotalItems - m_nVisibleItems;
	if (nScrollableItems <= 0) return;

	int nThumbHeight = max(20, (m_nVisibleItems * nScrollHeight) / nTotalItems);
	int nThumbTop = (m_nTopIndex * (nScrollHeight - nThumbHeight)) / nScrollableItems;

	int x = m_rcScrollBar.left + 2;
	int y = m_rcScrollBar.top + 2 + nThumbTop;
	int width = m_rcScrollBar.Width() - 4;
	int height = nThumbHeight;

	// 둥근 썸 경로 생성
	const int radius = 6; // 둥근 정도
	GraphicsPath path;
	path.AddArc(x, y, radius * 2, radius * 2, 180, 90);                         // 좌상
	path.AddArc(x + width - radius * 2, y, radius * 2, radius * 2, 270, 90);   // 우상
	path.AddArc(x + width - radius * 2, y + height - radius * 2, radius * 2, radius * 2, 0, 90); // 우하
	path.AddArc(x, y + height - radius * 2, radius * 2, radius * 2, 90, 90);   // 좌하
	path.CloseFigure();

	// 썸 그리기
	SolidBrush thumbBrush(Color(103,108,128));
	g->SetSmoothingMode(SmoothingModeAntiAlias);
	g->FillPath(&thumbBrush, &path);

}


BOOL CEnvListBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= LBS_NOINTEGRALHEIGHT | LBS_OWNERDRAWFIXED | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS | WS_BORDER;

	return CListBox::Create(dwStyle, rect, pParentWnd, nID);
}

void CEnvListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 20;
}

void CEnvListBox::DrawItemBack(CPaintDC* pdc, COLORREF crBack, COLORREF crDivideLine, CRect rArea)
{
	pdc->FillRect(rArea, new CBrush(crBack));

	DrawLine(pdc, CPoint(rArea.left, rArea.bottom - 1), CPoint(rArea.right, rArea.bottom - 1), crDivideLine, 1);
}

void CEnvListBox::DrawLine(CPaintDC* pdc, CPoint po1, CPoint po2, COLORREF crLine, int nLineTick)
{
	CPen* pOldPen, pnNew(PS_SOLID, nLineTick, crLine);
	pOldPen = pdc->SelectObject(&pnNew);

	pdc->MoveTo(po1);
	pdc->LineTo(po2);

	pdc->SelectObject(pOldPen);
}

void CEnvListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CRect rcClient;
	//GetClientRect(&rcClient);
	//CRect rcScroll(rcClient.right - 12, rcClient.top, rcClient.right, rcClient.bottom);
	//
	//if (rcScroll.PtInRect(point))
	//{
	//	// 스크롤바 클릭 처리
	//	m_nThumbPos = point.y - rcScroll.top;
	//	int nItemCount = GetCount();
	//	int nScrollPos = m_nThumbPos * nItemCount / rcScroll.Height();
	//
	//	SetTopIndex(nScrollPos);
	//	Invalidate();
	//}
	//
	//CListBox::OnLButtonDown(nFlags, point);

	SetFocus();

	if (m_bScrollBarVisible && m_rcScrollBar.PtInRect(point))
	{
		HandleScrollBarClick(point);
		m_bScrollBarDragging = TRUE;
		SetCapture();
		TRACE(_T("==> OnLButtonDown() 마우스캡쳐...\n"));
		return;
	}

	// 아이템 선택
	int nItem = ItemFromPoint(point);
	if (nItem != -1)
	{
		SetCurSel(nItem);
		Invalidate();

		GetParent()->SendMessage(WM_COMMAND,
			MAKEWPARAM(GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)m_hWnd);
	}
}

void CEnvListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bScrollBarDragging)
	{
		m_bScrollBarDragging = FALSE;
		ReleaseCapture();
		TRACE(_T("==> OnLButtonUp() 마우스릴리즈...\n"));
	}
}

void CEnvListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bScrollBarDragging && m_bScrollBarVisible)
	{
		HandleScrollBarClick(point);
	}
}

BOOL CEnvListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (!m_bScrollBarVisible) return FALSE;

	int nTotalItems = GetCount();
	int nScrollableItems = nTotalItems - m_nVisibleItems;
	if (nScrollableItems <= 0) return FALSE;

	// 시스템 설정에 따라 휠 한 번에 몇 줄 스크롤할지 결정
	UINT nScrollLines = 3; // 기본값
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0);

	int nDeltaLines = (zDelta / WHEEL_DELTA) * nScrollLines;
	int nNewTopIndex = m_nTopIndex - nDeltaLines;

	// 범위 제한
	nNewTopIndex = max(0, min(nNewTopIndex, nScrollableItems));

	// 스크롤 위치 갱신
	m_nTopIndex = nNewTopIndex;

	// 썸 위치도 갱신
	Invalidate(); // 다시 그리기
	return TRUE;
}

void CEnvListBox::SetFontColor(COLORREF crFont)
{
	m_stInfo.crText = crFont;

	Invalidate(FALSE);
}

COLORREF CEnvListBox::GetFontColor()
{
	return m_stInfo.crText;
}

void CEnvListBox::SetItemBackColor(COLORREF crItemBack)
{
	m_stInfo.crItemBack = crItemBack;

	Invalidate(FALSE);
}

COLORREF CEnvListBox::GetItemBackColor()
{
	return m_stInfo.crItemBack;
}

void CEnvListBox::SetItemDivideLineColor(COLORREF crItemDivideLine)
{
	m_stInfo.crItemDivideLine = crItemDivideLine;

	Invalidate(FALSE);
}

COLORREF CEnvListBox::GetItemDivideLineColor()
{
	return m_stInfo.crItemDivideLine;
}

void CEnvListBox::SetSelFontColor(COLORREF crSelFont)
{
	m_stInfo.crSelText = crSelFont;

	Invalidate(FALSE);
}

COLORREF CEnvListBox::GetSelFontColor()
{
	return m_stInfo.crSelText;
}

void CEnvListBox::SetSelItemBackColor(COLORREF crSelItemBack)
{
	m_stInfo.crSelItemBack = crSelItemBack;

	Invalidate(FALSE);
}

COLORREF CEnvListBox::GetSelItemBackColor()
{
	return m_stInfo.crSelItemBack;
}

void CEnvListBox::SetBackColor(COLORREF crBack)
{
	m_stInfo.crBack = crBack;

	Invalidate(TRUE);
}

void CEnvListBox::SetFont(CFont* pFont)
{
	m_pFont = pFont;
}


COLORREF CEnvListBox::GetBackColor()
{
	return m_stInfo.crBack;
}

BOOL CEnvListBox::OnEraseBkgnd(CDC* pDC)
{
	CRect rScreen;
	GetClientRect(&rScreen);

	pDC->FillRect(rScreen, new CBrush(m_stInfo.crBack));

	return TRUE;
	//return CListBox::OnEraseBkgnd(pDC);
}

void CEnvListBox::SetFontBold(BOOL bBold)
{
	m_stInfo.bFontBold = bBold;

	Invalidate(FALSE);
}

void CEnvListBox::SetFontSize(int nFontSize)
{
	m_stInfo.nFontSize = nFontSize;

	Invalidate(FALSE);
}

void CEnvListBox::SetAlignFormat(UINT nAlignFormat)
{
	m_stInfo.nAlignFormat = nAlignFormat;

	Invalidate(FALSE);
}

void CEnvListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 기본 스크롤 이벤트 무시하고 직접 처리
	int nTop = GetTopIndex();
	switch (nSBCode)
	{
	case SB_LINEUP:
		if (nTop > 0) SetTopIndex(nTop - 1);
		break;
	case SB_LINEDOWN:
		if (nTop < GetCount() - 1) SetTopIndex(nTop + 1);
		break;
	}

	Invalidate();
}

int CEnvListBox::GetVisibleItemCount()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	// 항목 높이 가져오기
	int itemHeight = GetItemHeight(0); // 모든 항목이 동일한 높이라고 가정

	if (itemHeight <= 0)
		return 0;

	return rcClient.Height() / itemHeight;
}
BOOL CEnvListBox::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CListBox::PreCreateWindow(cs);
}


void CEnvListBox::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// Owner Draw Fixed로 설정
	//ModifyStyle(0, LBS_OWNERDRAWFIXED | LBS_HASSTRINGS);

	// 스크롤바 제거 - 간단한 방법
	//ModifyStyle(WS_VSCROLL | WS_HSCROLL, 0);

	// 윈도우 스타일 강제 갱신
	//SetWindowPos(NULL, 0, 0, 0, 0,
	//	SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	m_bScrollBarVisible = FALSE;
	m_nScrollBarWidth = 16;

	m_bInitialized = TRUE;
	UpdateScrollInfo();

	CListBox::PreSubclassWindow();
}


void CEnvListBox::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// 스크롤바 영역을 클라이언트 영역으로 포함시켜 스크롤바가 나타나지 않도록 함
	// 기본 처리를 건너뛰고 스크롤바 영역을 제거
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CEnvListBox::OnNcPaint()
{
	// Non-client 영역 페인팅을 건너뛰어 스크롤바가 그려지지 않도록 함
	Default();
}

LRESULT CEnvListBox::OnNcHitTest(CPoint point)
{
	// 스크롤바 영역에 대한 히트 테스트를 무시
	LRESULT result = CListBox::OnNcHitTest(point);

	// 스크롤바 관련 히트 테스트 결과를 클라이언트 영역으로 변경
	if (result == HTVSCROLL || result == HTHSCROLL)
		return HTCLIENT;

	return result;
}

void CEnvListBox::HandleScrollBarClick(CPoint point)
{
	int nTotalItems = GetCount();
	int nScrollableItems = nTotalItems - m_nVisibleItems;

	if (nScrollableItems <= 0) return;

	int nRelativePos = point.y - m_rcScrollBar.top - 2; // 테두리 여백
	int nScrollHeight = m_rcScrollBar.Height() - 4;

	if (nScrollHeight > 0)
	{
		int nNewTopIndex = (nRelativePos * nScrollableItems) / nScrollHeight;
		m_nTopIndex = max(0, min(nNewTopIndex, nScrollableItems));
		Invalidate();
	}
}

int CEnvListBox::ItemFromPoint(CPoint point)
{
	if (m_bScrollBarVisible && point.x >= m_rcScrollBar.left)
		return -1;  // 스크롤바 영역

	int nItemIndex = point.y / m_nItemHeight + m_nTopIndex;
	if (nItemIndex >= 0 && nItemIndex < GetCount())
		return nItemIndex;

	return -1;
}

void CEnvListBox::SetImageCheckable(LPCTSTR lpszFileNormal, LPCTSTR lpszFileChecked)
{
	m_bCheckImage = TRUE;
	m_nNormal	= -1;
	m_nChecked	= -1;
	GetGDIMng()->GetBitmapMng()->GetImage(m_nNormal, lpszFileNormal);
	GetGDIMng()->GetBitmapMng()->GetImage(m_nChecked, lpszFileChecked);
}

//////////////////////////////////////////////////////////////////////////////////////////////



CEnvPopupList::CEnvPopupList() : m_selectedIndex(-1), m_hoverIndex(-1) 
{
	m_pParent = nullptr;
	m_itemHeight = 30;
}

CEnvPopupList::~CEnvPopupList() {}

BEGIN_MESSAGE_MAP(CEnvPopupList, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

BOOL CEnvPopupList::Create(CWnd* pParentWnd, const CRect& rect)
{
	LPCTSTR className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
		::LoadCursor(nullptr, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		nullptr);

	BOOL result = CWnd::CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		className,
		_T(""),
		WS_POPUP | WS_VISIBLE,
		rect,
		pParentWnd,
		0,
		nullptr);

	//m_font.CreatePointFont(90, _T("Segoe UI"));
	m_pParent = pParentWnd;

	return result;
}

void CEnvPopupList::SetItems(const CStringArray& items)
{
	m_items.Copy(items);
	Invalidate();
}

void CEnvPopupList::SetSelectedIndex(int index)
{
	m_selectedIndex = index;
	Invalidate();
}

void CEnvPopupList::OnPaint()
{
	CPaintDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);

	dc.SelectObject(m_pFont);

	// 전체 배경 먼저 칠함
	dc.FillSolidRect(clientRect, RGB(62, 66, 80));// RGB(29, 31, 37));

	for (int i = 0; i < m_items.GetSize(); ++i)
	{
		CRect itemRect(0, i * m_itemHeight, clientRect.Width(), (i + 1) * m_itemHeight);

		COLORREF bgColor = RGB(62, 66, 80);// RGB(29, 31, 37); // 기본 배경

		if (i == m_selectedIndex)
			bgColor = RGB(29, 31, 37); // 선택 강조
		else if (i == m_hoverIndex)
			bgColor = RGB(45, 48, 60); // 마우스 오버

		dc.FillSolidRect(itemRect, bgColor);

		// 텍스트 위치 조정
		CRect textRect = itemRect;
		textRect.left += 12;

		dc.SetTextColor(RGB(238, 238, 238));
		dc.DrawText(m_items[i], textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
}

void CEnvPopupList::OnLButtonDown(UINT nFlags, CPoint point)
{
	int index = point.y / m_itemHeight;

	if (index >= 0 && index < m_items.GetSize())
	{
		m_selectedIndex = index;
		Invalidate();

		if (m_pParent)	
			m_pParent->SendMessage(WMU_POPUP_MENU, index, 0);

		//DestroyWindow(); // 선택 후 닫기
		ShowWindow(SW_HIDE);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CEnvPopupList::OnMouseMove(UINT nFlags, CPoint point)
{
	int index = point.y / m_itemHeight;

	if (index != m_hoverIndex && index >= 0 && index < m_items.GetSize())
	{
		m_hoverIndex = index;
		Invalidate();
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CEnvPopupList::OnKillFocus(CWnd* pNewWnd)
{
	//DestroyWindow(); // 포커스 잃으면 닫기
	ShowWindow(SW_HIDE);
	CWnd::OnKillFocus(pNewWnd);
}



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

CStaticUX::CStaticUX()
{
	m_bkColor = RGB(219, 219, 239);      // 기본 배경색
	m_borderColor = RGB(29, 31, 37);  // 기본 테두리색
	m_textColor = RGB(0, 0, 0);          // 기본 텍스트색
	m_cornerRadius = 9;                 // 기본 둥근 정도
}

BEGIN_MESSAGE_MAP(CStaticUX, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CStaticUX::SetBkColor(COLORREF color)
{
	m_bkColor = color;
	Invalidate();
}

void CStaticUX::SetBorderColor(COLORREF color)
{
	m_borderColor = color;
	Invalidate();
}

void CStaticUX::SetTextColor(COLORREF color)
{
	m_textColor = color;
	Invalidate();
}

void CStaticUX::SetCornerRadius(int radius)
{
	m_cornerRadius = radius;
	Invalidate();
}

void CStaticUX::OnPaint()
{
#if 1
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	// 더블 버퍼링
	CDC memDC;
	CBitmap bmp;
	memDC.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);

	// 배경을 라운드 영역만 칠함
	memDC.SetBkMode(TRANSPARENT);
	memDC.FillSolidRect(rc, RGB(0, 0, 0)); // 디버깅용: 바깥 영역 확인

	// 라운드 영역만 클리핑
	CRgn rgn;
	rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, m_cornerRadius, m_cornerRadius);
	memDC.SelectClipRgn(&rgn);

	// 둥근 배경 그리기
	CBrush brushBk(m_bkColor);
	CPen penBorder(PS_SOLID, 1, m_borderColor);
	memDC.SelectObject(&brushBk);
	memDC.SelectObject(&penBorder);
	memDC.RoundRect(rc, CPoint(m_cornerRadius, m_cornerRadius));

	// 텍스트 그리기
	CString text;
	GetWindowText(text);
	memDC.SetTextColor(m_textColor);

	CFont* pFont = GetFont();
	if (pFont) memDC.SelectObject(pFont);

	memDC.DrawText(text, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 화면에 출력
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	// 정리
	memDC.SelectObject(pOldBmp);
	memDC.SelectClipRgn(nullptr);
#else
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	// 더블 버퍼링
	CDC memDC;
	CBitmap bmp;
	memDC.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);

	// 배경 지우기
	memDC.FillSolidRect(rc, GetSysColor(COLOR_3DFACE));

	// 둥근 배경 그리기
	CBrush brushBk(m_bkColor);
	CPen penBorder(PS_SOLID, 1, m_borderColor);
	memDC.SelectObject(&brushBk);
	memDC.SelectObject(&penBorder);
	memDC.RoundRect(rc, CPoint(m_cornerRadius, m_cornerRadius));

	// 텍스트 그리기
	CString text;
	GetWindowText(text);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(m_textColor);

	CFont* pFont = GetFont();
	if (pFont) memDC.SelectObject(pFont);

	memDC.DrawText(text, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 화면에 출력
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	// 정리
	memDC.SelectObject(pOldBmp);
#endif
}

void CStaticUX::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(&rc);

	CRgn rgn;
	rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, m_cornerRadius, m_cornerRadius);
	SetWindowRgn(rgn, TRUE);
}

//-----------------------------------------------------------------------------------------------

using namespace Gdiplus;

CRoundedColorBox::CRoundedColorBox(COLORREF color)
	: m_color(color)
{
}

void CRoundedColorBox::SetColor(COLORREF color)
{
	m_color = color;
	Invalidate();
}

BEGIN_MESSAGE_MAP(CRoundedColorBox, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CRoundedColorBox::OnPaint()
{
	CPaintDC dc(this);
	Graphics graphics(dc);

	CRect rect;
	GetClientRect(&rect);

	// GDI+ 색상 변환
	Color fillColor(GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));

	// 둥근 사각형 그리기
	SolidBrush brush(fillColor);
	int radius = 6;

	GraphicsPath path;
	path.AddArc(rect.left, rect.top, radius, radius, 180, 90);
	path.AddArc(rect.right - radius, rect.top, radius, radius, 270, 90);
	path.AddArc(rect.right - radius, rect.bottom - radius, radius, radius, 0, 90);
	path.AddArc(rect.left, rect.bottom - radius, radius, radius, 90, 90);
	path.CloseFigure();

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.FillPath(&brush, &path);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CEnvEdit, CWnd)

CEnvEdit::CEnvEdit()
{
	m_pEdit = nullptr;
	m_nCornerRadius = 8;        // 기본 모서리 반지름
	m_nBorderWidth = 2;         // 기본 테두리 두께
	m_clrBorder = RGB(128, 128, 128);      // 기본 테두리 색상 (회색)
	m_clrBackground = RGB(255, 255, 255);  // 기본 배경색상 (흰색)
	m_bSubclassed = FALSE;
}

CEnvEdit::~CEnvEdit()
{
	if (m_pEdit)
	{
		delete m_pEdit;
		m_pEdit = nullptr;
	}
}

BEGIN_MESSAGE_MAP(CEnvEdit, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CEnvEdit::SubclassDlgItem(UINT nID, CWnd* pParent)
{
	if (!CWnd::SubclassDlgItem(nID, pParent))
		return FALSE;

	m_bSubclassed = TRUE;

	// 기존 스타일에서 테두리 제거
	ModifyStyle(WS_BORDER | SS_SUNKEN, 0);
	ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 0);

	CreateInternalEdit();

	return TRUE;
}

void CEnvEdit::CreateInternalEdit()
{
	if (m_pEdit)
		return;

	CRect rect;
	GetClientRect(&rect);

	// 기존 스타일 가져오기
	DWORD dwStyle = GetStyle();
	DWORD dwExStyle = GetExStyle();

	m_pEdit = new CEdit();
	if (m_pEdit->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL |
		(dwStyle & ES_MULTILINE) | (dwStyle & ES_PASSWORD) |
		(dwStyle & ES_READONLY) | (dwStyle & ES_NUMBER) |
		(dwStyle & ES_UPPERCASE) | (dwStyle & ES_LOWERCASE),
		CRect(0, 0, 0, 0), this, GetDlgCtrlID() + 1000))
	{
		// Edit 컨트롤의 테두리 제거
		m_pEdit->ModifyStyle(WS_BORDER, 0);
		m_pEdit->ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 0);

		UpdateEditPosition();

		// 기존 텍스트 복사
		CString strText;
		CWnd::GetWindowText(strText);
		m_pEdit->SetWindowText(strText);

		// 폰트 복사
		CFont* pFont = GetFont();
		if (pFont)
			m_pEdit->SetFont(pFont);
	}
}

BOOL CEnvEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// 기본 윈도우 생성
	if (!CWnd::Create(nullptr, _T(""), dwStyle | WS_CHILD | WS_VISIBLE,
		rect, pParentWnd, nID))
		return FALSE;

	CreateInternalEdit();
	return TRUE;
}

int CEnvEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_bSubclassed)
		CreateInternalEdit();

	return 0;
}

void CEnvEdit::UpdateEditPosition()
{
	if (!m_pEdit || !::IsWindow(m_pEdit->m_hWnd))
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	// 테두리와 여백을 고려한 Edit 영역 계산
	int margin = m_nBorderWidth + 4; // 테두리 + 추가 여백
	CRect editRect = clientRect;
	editRect.DeflateRect(margin, margin);

	m_pEdit->MoveWindow(editRect);
}

void CEnvEdit::OnPaint()
{
	CPaintDC dc(this);

	CRect clientRect;
	GetClientRect(&clientRect);

	//// 배경을 투명하게 설정
	//dc.SetBkMode(TRANSPARENT);
	//
	//// 둥근 사각형 그리기
	//DrawRoundedRect(&dc, clientRect, m_nCornerRadius);

	
}

void CEnvEdit::DrawRoundedRect(CDC* pDC, const CRect& rect, int nRadius)
{
	// 배경 브러시 생성
	CBrush brushBg(m_clrBackground);
	CBrush* pOldBrush = pDC->SelectObject(&brushBg);

	// 테두리 펜 생성
	CPen penBorder(PS_SOLID, m_nBorderWidth, m_clrBorder);
	CPen* pOldPen = pDC->SelectObject(&penBorder);

	// 둥근 사각형 그리기
	pDC->RoundRect(rect, CPoint(nRadius * 2, nRadius * 2));

	// 리소스 복원
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

BOOL CEnvEdit::OnEraseBkgnd(CDC* pDC)
{
	// 배경 지우기를 하지 않음 (OnPaint에서 처리)
	return TRUE;
}

void CEnvEdit::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	UpdateEditPosition();
}

void CEnvEdit::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// 포커스를 내부 Edit 컨트롤로 전달하고 캐럿 표시
	if (m_pEdit && ::IsWindow(m_pEdit->m_hWnd))
	{
		m_pEdit->SetFocus();
		// 캐럿이 제대로 표시되도록 강제로 설정
		m_pEdit->SetSel(0, -1);  // 전체 선택 후
		m_pEdit->SetSel(-1);     // 끝으로 이동하여 캐럿 표시
	}
}

void CEnvEdit::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	Invalidate(); // 포커스 상태 변경 시 다시 그리기
}

void CEnvEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 클릭 시 포커스를 내부 Edit 컨트롤로 설정하고 캐럿 위치 조정
	if (m_pEdit && ::IsWindow(m_pEdit->m_hWnd))
	{
		m_pEdit->SetFocus();

		// 클릭 위치를 Edit 컨트롤 좌표로 변환
		CRect editRect;
		m_pEdit->GetWindowRect(&editRect);
		ScreenToClient(&editRect);

		if (editRect.PtInRect(point))
		{
			CPoint editPoint = point;
			editPoint.x -= editRect.left;
			editPoint.y -= editRect.top;

			// Edit 컨트롤에 마우스 클릭 메시지 전달
			m_pEdit->SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(editPoint.x, editPoint.y));
		}
		else
		{
			// Edit 영역 외부 클릭 시 텍스트 끝으로 캐럿 이동
			m_pEdit->SetSel(-1);
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CEnvEdit::SetText(LPCTSTR lpszText)
{
	if (m_pEdit && ::IsWindow(m_pEdit->m_hWnd))
		m_pEdit->SetWindowText(lpszText);
}

CString CEnvEdit::GetText()
{
	CString strText;
	if (m_pEdit && ::IsWindow(m_pEdit->m_hWnd))
		m_pEdit->GetWindowText(strText);
	return strText;
}

void CEnvEdit::GetText(CString& strText)
{
	if (m_pEdit && ::IsWindow(m_pEdit->m_hWnd))
		m_pEdit->GetWindowText(strText);
}

void CEnvEdit::SetFont(CFont* pFont, BOOL bRedraw)
{
	if (m_pEdit && ::IsWindow(m_pEdit->m_hWnd))
		m_pEdit->SetFont(pFont, bRedraw);
}

HBRUSH CEnvEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT && pWnd == m_pEdit)
	{
		// 내부 Edit 컨트롤의 배경색을 설정
		pDC->SetBkColor(m_clrBackground);
		static CBrush brush;
		brush.DeleteObject();
		brush.CreateSolidBrush(m_clrBackground);
		return brush;
	}

	return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}

HWND CEnvEdit::GetEditHwnd()
{
	if (m_pEdit != nullptr)
		return m_pEdit->GetSafeHwnd();

	return nullptr;
}

//-----------------------------------------------------------------------------------

// CEnvStatic.cpp 구현부
IMPLEMENT_DYNAMIC(CEnvStatic, CWnd)

CEnvStatic::CEnvStatic()
	: m_strText(_T(""))
	, m_colorBack(RGB(240, 240, 240))
	, m_colorText(RGB(0, 0, 0))
	, m_colorBorder(RGB(128, 128, 128))
	, m_nRoundRadius(10)
	, m_pFont(nullptr)
	, m_gdiplusToken(0)
{
	InitGdiPlus();
}

CEnvStatic::~CEnvStatic()
{
	CleanupGdiPlus();
}

BEGIN_MESSAGE_MAP(CEnvStatic, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CEnvStatic::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// 기본 스타일 설정
	dwStyle |= WS_CHILD | WS_VISIBLE;

	// 윈도우 클래스 등록
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW));

	if (!CWnd::Create(lpszClassName, lpszText, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	SetText(lpszText);
	return TRUE;
}

BOOL CEnvStatic::SubclassDlgItem(UINT nID, CWnd* pParent)
{
	ASSERT(pParent != nullptr);
	ASSERT(::IsWindow(pParent->GetSafeHwnd()));

	// 기존 컨트롤 핸들 가져오기
	HWND hWndControl = ::GetDlgItem(pParent->GetSafeHwnd(), nID);
	if (!hWndControl)
		return FALSE;

	// 기존 컨트롤의 텍스트 먼저 가져오기
	TCHAR szText[256] = { 0 };
	::GetWindowText(hWndControl, szText, 255);
	m_strText = szText;

	// 서브클래싱 수행
	if (!SubclassWindow(hWndControl))
		return FALSE;

	return TRUE;
}

void CEnvStatic::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	// 서브클래싱된 경우 기존 텍스트 보존
	if (m_strText.IsEmpty() && GetSafeHwnd())
	{
		CString strText;
		GetWindowText(strText);
		if (!strText.IsEmpty())
			m_strText = strText;
	}
}

void CEnvStatic::SetText(LPCTSTR lpszText)
{
	m_strText = lpszText ? lpszText : _T("");
	if (GetSafeHwnd())
		Invalidate();
}

CString CEnvStatic::GetText() const
{
	return m_strText;
}

void CEnvStatic::GetText(CString& strText)
{
	strText = m_strText;
}

void CEnvStatic::SetBackColor(COLORREF color)
{
	m_colorBack = color;
	if (GetSafeHwnd())
		Invalidate();
}

void CEnvStatic::SetTextColor(COLORREF color)
{
	m_colorText = color;
	if (GetSafeHwnd())
		Invalidate();
}

void CEnvStatic::SetBorderColor(COLORREF color)
{
	m_colorBorder = color;
	if (GetSafeHwnd())
		Invalidate();
}

void CEnvStatic::SetRoundRadius(int radius)
{
	m_nRoundRadius = max(0, radius);
	if (GetSafeHwnd())
		Invalidate();
}

void CEnvStatic::SetFont(CFont* pFont, BOOL bRedraw)
{
	m_pFont = pFont;
	if (bRedraw && GetSafeHwnd())
		Invalidate();
}

void CEnvStatic::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	// GDI+ Graphics 객체 생성
	Gdiplus::Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	// 라운드 박스 그리기
	Gdiplus::RectF rectF(static_cast<float>(rect.left), static_cast<float>(rect.top),
		static_cast<float>(rect.Width()), static_cast<float>(rect.Height()));

	Gdiplus::Color fillColor(GetRValue(m_colorBack), GetGValue(m_colorBack), GetBValue(m_colorBack));
	Gdiplus::Color borderColor(GetRValue(m_colorBorder), GetGValue(m_colorBorder), GetBValue(m_colorBorder));

	DrawRoundRect(graphics, rectF, static_cast<float>(m_nRoundRadius), fillColor, borderColor);

	// 텍스트 그리기
	if (!m_strText.IsEmpty())
	{
		// 폰트 설정
		Gdiplus::Font* pGdiFont = nullptr;
		if (m_pFont && m_pFont->GetSafeHandle())
		{
			LOGFONT lf;
			m_pFont->GetLogFont(&lf);
			pGdiFont = new Gdiplus::Font(dc.GetSafeHdc(), &lf);
		}
		else
		{
			pGdiFont = new Gdiplus::Font(L"Arial", 12);
		}

		// 텍스트 색상 설정
		Gdiplus::SolidBrush textBrush(Gdiplus::Color(GetRValue(m_colorText),
			GetGValue(m_colorText),
			GetBValue(m_colorText)));

		// 텍스트 포맷 설정 (중앙 정렬)
		Gdiplus::StringFormat stringFormat;
		stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		// 유니코드 변환
		CStringW wideText(m_strText);

		// 텍스트 그리기
		graphics.DrawString(wideText, -1, pGdiFont, rectF, &stringFormat, &textBrush);

		delete pGdiFont;
	}
}

BOOL CEnvStatic::OnEraseBkgnd(CDC* pDC)
{
	// 배경 지우기 방지 (OnPaint에서 모든 그리기 처리)
	return TRUE;
}

void CEnvStatic::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	Invalidate();
}

void CEnvStatic::InitGdiPlus()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

void CEnvStatic::CleanupGdiPlus()
{
	if (m_gdiplusToken)
	{
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
		m_gdiplusToken = 0;
	}
}

void CEnvStatic::DrawRoundRect(Gdiplus::Graphics& graphics, const Gdiplus::RectF& rect, float radius,
	const Gdiplus::Color& fillColor, const Gdiplus::Color& borderColor)
{
	if (radius <= 0)
	{
		// 일반 사각형
		Gdiplus::SolidBrush fillBrush(fillColor);
		Gdiplus::Pen borderPen(borderColor, 1.0f);

		graphics.FillRectangle(&fillBrush, rect);
		graphics.DrawRectangle(&borderPen, rect);
		return;
	}

	// 라운드 사각형을 위한 GraphicsPath 생성
	Gdiplus::GraphicsPath path;

	float diameter = radius * 2;
	Gdiplus::RectF arcRect(rect.X, rect.Y, diameter, diameter);

	// 왼쪽 위 모서리
	path.AddArc(arcRect, 180, 90);

	// 오른쪽 위 모서리
	arcRect.X = rect.GetRight() - diameter;
	path.AddArc(arcRect, 270, 90);

	// 오른쪽 아래 모서리
	arcRect.Y = rect.GetBottom() - diameter;
	path.AddArc(arcRect, 0, 90);

	// 왼쪽 아래 모서리
	arcRect.X = rect.GetLeft();
	path.AddArc(arcRect, 90, 90);

	path.CloseFigure();

	// 채우기 및 테두리 그리기
	Gdiplus::SolidBrush fillBrush(fillColor);
	Gdiplus::Pen borderPen(borderColor, 1.0f);

	graphics.FillPath(&fillBrush, &path);
	graphics.DrawPath(&borderPen, &path);
}


//--------------------------------------------------------------------------


BEGIN_MESSAGE_MAP(CEnvHeader, CHeaderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CEnvHeader::OnCustomDraw)
END_MESSAGE_MAP()

void CEnvHeader::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pDraw = reinterpret_cast<NMCUSTOMDRAW*>(pNMHDR);

	switch (pDraw->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{
		CDC dc;
		dc.Attach(pDraw->hdc);

		// 헤더 영역 가져오기
		CRect rc = pDraw->rc;

		// 텍스트 가져오기
		TCHAR szText[256] = { 0 };
		HDITEM item = { 0 };
		item.mask = HDI_TEXT;
		item.pszText = szText;
		item.cchTextMax = 255;
		GetItem(pDraw->dwItemSpec, &item);

		CFont* pFont = nullptr;
		CFont* pOldFont = nullptr;
		pFont = GetGDIMng()->GetFont(78);// m_nFontIndex);
		pOldFont = dc.SelectObject(pFont);

		// 텍스트 그리기
		dc.SetTextColor(m_clrText);
		dc.SetBkColor(m_clrBk);
		dc.SetBkMode(OPAQUE);
		dc.FillSolidRect(&rc, m_clrBk);
		dc.DrawText(szText, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		dc.SelectObject(pOldFont);

		dc.Detach();
		*pResult = CDRF_SKIPDEFAULT;
		break;
	}
	break;
	case CDDS_ITEMPOSTPAINT:
	{
		// 여기서 텍스트를 다시 그려주는 것도 가능
		// 또는 배경을 덮어쓰지 않도록 처리
		*pResult = CDRF_SKIPDEFAULT;
	}
	break;
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}
}



//--------------------------------------------------------------------------

CEnvList::CEnvList()
{
	m_clrHeaderBk = RGB(200, 200, 255);  // 헤더 배경색
	m_clrText = RGB(0, 0, 0);        // 텍스트 색상
	m_clrBk = RGB(240, 240, 240);  // 바디 배경색

	m_pScrollbar = nullptr;
}

CEnvList::~CEnvList() {}

BEGIN_MESSAGE_MAP(CEnvList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CEnvList::OnCustomDraw)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_USER + 100, OnUpdateScrollbar)
END_MESSAGE_MAP()

void CEnvList::InitList()
{
	// LVS_EX_GRIDLINES 제거하고 직접 그리기
	SetExtendedStyle(LVS_EX_FULLROWSELECT);  // GRIDLINES 제거

	// 헤더 서브클래싱
	CHeaderCtrl* pRawHeader = GetHeaderCtrl();
	if (pRawHeader && ::IsWindow(pRawHeader->GetSafeHwnd()))
	{
		m_Header.SubclassWindow(pRawHeader->GetSafeHwnd());
		m_Header.m_clrText = m_clrText;
		m_Header.m_clrBk = m_clrHeaderBk;
	}

	SetSelBkColor(RGB(29, 71, 142));
}

// 구현
void CEnvList::UpdateHeaderColors()
{
	if (::IsWindow(m_Header.GetSafeHwnd()))
	{
		m_Header.m_clrText = m_clrText;
		m_Header.m_clrBk = m_clrHeaderBk;
		m_Header.Invalidate();
	}
}


void CEnvList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// 기본적인 배경 그리기
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;

	// 배경색
	pDC->FillSolidRect(&rect, RGB(240, 240, 240));

	// 텍스트 예시
	CString text = _T("Custom Draw Item");
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(rect.left + 5, rect.top + 2, text);
}

void CEnvList::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{
		*pResult = CDRF_NOTIFYPOSTPAINT; // 모든 항목에 대해 POSTPAINT 요청
		//int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		//BOOL bSelected = GetItemState(nItem, LVIS_SELECTED) & LVIS_SELECTED;
		//
		//if (bSelected)
		//{
		//	// 선택된 항목은 직접 그리기 위해 POSTPAINT 요청
		//	*pResult = CDRF_NOTIFYPOSTPAINT;
		//}
		//else
		//{
		//	// 일반 항목은 기본 배경/텍스트 색상 적용
		//	pLVCD->clrTextBk = m_clrBk;
		//	pLVCD->clrText = m_clrText;
		//	*pResult = CDRF_DODEFAULT;
		//}
		break;
	}

	case CDDS_ITEMPOSTPAINT:
	{
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		BOOL bSelected = GetItemState(nItem, LVIS_SELECTED) & LVIS_SELECTED;

		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

		CRect rc;
		GetItemRect(nItem, &rc, LVIR_BOUNDS);
		rc.left += 2;

		// 선택된 배경색으로 채우기
		if (bSelected)
			pDC->FillSolidRect(&rc, m_clrSelBk); // 선택 배경색
		else 
			pDC->FillSolidRect(&rc, m_clrBk);

		// 컬럼 수 가져오기
		int nColCount = GetHeaderCtrl()->GetItemCount();

		CFont* pFont = nullptr;
		CFont* pOldFont = nullptr;
		pFont = GetGDIMng()->GetFont(78);// m_nFontIndex);
		pOldFont = pDC->SelectObject(pFont);

		for (int iCol = 0; iCol < nColCount; ++iCol)
		{
			CRect rcSubItem;
			GetSubItemRect(nItem, iCol, LVIR_LABEL, rcSubItem);

			CString text = GetItemText(nItem, iCol);

			pDC->SetTextColor(m_clrText);
			pDC->SetBkMode(TRANSPARENT);
			// 여백
			if (iCol == 0)	rcSubItem.left += 2;
			else			rcSubItem.left += 6;
			pDC->DrawText(text, &rcSubItem, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
		}

		pDC->SelectObject(pOldFont);

		*pResult = CDRF_SKIPDEFAULT; // 기본 그리기 무시
		break;
	}
	}
}

void CEnvList::OnPaint()
{
	CListCtrl::OnPaint();

}

BOOL CEnvList::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, m_clrBk);
	return TRUE; // 배경 지움 완료
}

void CEnvList::AttachScrollbar(CEnvListScrollbar* pScrollbar)
{
	m_pScrollbar = pScrollbar;
	if (m_pScrollbar) {
		m_pScrollbar->SetOwner(this);

		 // 1단계: ShowScrollBar
		ShowScrollBar(SB_BOTH, FALSE);

		// 2단계: 스타일 제거
		DWORD dwStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
		dwStyle &= ~(WS_VSCROLL | WS_HSCROLL);
		SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle);

		// 3단계: 프레임 변경 적용
		SetWindowPos(nullptr, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		// 4단계: 비활성화
		EnableScrollBar(SB_BOTH, ESB_DISABLE_BOTH);
	}
}

void CEnvList::UpdateScrollbar()
{
	if (!m_pScrollbar) return;

	int totalItems = GetItemCount();
	int visibleItems = GetCountPerPage();

	if (totalItems <= visibleItems) {
		m_pScrollbar->ShowWindow(SW_HIDE);
		return;
	}

	m_pScrollbar->ShowWindow(SW_SHOW);
	m_pScrollbar->SetScrollRange(0, totalItems - visibleItems);
	m_pScrollbar->SetPageSize(visibleItems);

	// 현재 스크롤 위치 동기화
	int currentPos = GetTopIndex();
	m_pScrollbar->SetScrollPos(currentPos);
}

void CEnvList::OnScrollbarThumbMoved(int newPos)
{
	// 리스트를 해당 위치로 스크롤
	EnsureVisible(newPos, FALSE);

	// 또는 더 정확한 제어를 원한다면:
	// Scroll(CSize(0, (newPos - GetTopIndex()) * GetItemHeight()));
}

void CEnvList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 기본 스크롤 처리
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

	// 커스텀 스크롤바와 동기화
	if (m_pScrollbar && ::IsWindow(m_pScrollbar->GetSafeHwnd()))
	{
		int currentPos = GetTopIndex();
		m_pScrollbar->SetScrollPos(currentPos);
	}
}

BOOL CEnvList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL result = CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
	if (m_pScrollbar) UpdateScrollbar(); // 마우스 휠 후 스크롤바 동기화
	return result;
}

void CEnvList::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
	if (m_pScrollbar) UpdateScrollbar(); // 크기 변경 시 스크롤바 업데이트
}

void CEnvList::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);

	// 스크롤바 영역을 0으로 만들어버리기
	if (bCalcValidRects)
	{
		// 클라이언트 영역을 전체 윈도우와 같게 (스크롤바 영역 제거)
		//	rgrc[0] : 새로운 클라이언트 영역(결과값),
		//	rgrc[1] : 새로운 윈도우 전체 영역,
		//	rgrc[2] : 이전 윈도우 전체 영역
		//	rgrc[0]는 rgrc[1]에서 테두리, 타이틀바, 스크롤바 등을 뺀 영역이므로.. rgrc[0] = rgrc[1]를 하면 스크롤바가 사라짐.
		lpncsp->rgrc[0] = lpncsp->rgrc[1];
	}

	// 이렇게 해도 되는데.. 위의 방법이 더 깔끔.
	//lpncsp->rgrc[0].right += GetSystemMetrics(SM_CXVSCROLL); // 스크롤바 너비만큼 확장
	//lpncsp->rgrc[0].bottom += GetSystemMetrics(SM_CYHSCROLL); // 스크롤바 높이만큼 확장
}

void CEnvList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	// 키보드로 스크롤 시 커스텀 스크롤바 동기화
	switch (nChar)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:  // Page Up
	case VK_NEXT:   // Page Down
	case VK_HOME:
	case VK_END:
		if (m_pScrollbar && ::IsWindow(m_pScrollbar->GetSafeHwnd()))
		{
			// 키 처리 후 약간의 지연을 두고 동기화
			PostMessage(WM_USER + 100); // 커스텀 메시지
		}
		break;
	}
}

LRESULT CEnvList::OnUpdateScrollbar(WPARAM wParam, LPARAM lParam)
{
	if (m_pScrollbar) UpdateScrollbar();
	return 0;
}

int CEnvList::InsertItem(int nItem, LPCTSTR lpszItem) 
{
	int result = CListCtrl::InsertItem(nItem, lpszItem);
	if (result != -1 && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}

int CEnvList::InsertItem(int nItem, LPCTSTR lpszItem, int nImage) 
{
	int result = CListCtrl::InsertItem(nItem, lpszItem, nImage);
	if (result != -1 && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}
int CEnvList::InsertItem(const LVITEM* pItem)
{
	int result = CListCtrl::InsertItem(pItem);
	if (result != -1 && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}

BOOL CEnvList::DeleteItem(int nItem)
{
	BOOL result = CListCtrl::DeleteItem(nItem);
	if (result && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}

BOOL CEnvList::DeleteAllItems()
{
	BOOL result = CListCtrl::DeleteAllItems();
	if (result && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}

void CEnvList::SetItemCount(int nItems)
{
	CListCtrl::SetItemCount(nItems);
	if (m_pScrollbar) {
		UpdateScrollbar();
	}
}

BOOL CEnvList::SetItemCountEx(int iCount, DWORD dwFlags)
{
	BOOL result = CListCtrl::SetItemCountEx(iCount, dwFlags);
	if (result && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}

//---------------------------------------------------------------------------


CEnvListScrollbar::CEnvListScrollbar() : 
	m_pOwner(nullptr), m_nMin(0), m_nMax(100), m_nPos(0), m_bDragging(false), m_nPageSize(10)
{
}

BEGIN_MESSAGE_MAP(CEnvListScrollbar, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CEnvListScrollbar::SetOwner(CEnvList* pList)
{
	m_pOwner = pList;
}

void CEnvListScrollbar::SetScrollRange(int min, int max)
{
	m_nMin = min;
	m_nMax = max;
	UpdateThumbRect();
	Invalidate();
}

void CEnvListScrollbar::SetScrollPos(int pos)
{
	m_nPos = max(m_nMin, min(pos, m_nMax));
	UpdateThumbRect();
	Invalidate();
}

// 스크롤바에서
void CEnvListScrollbar::SetPageSize(int pageSize)
{
	m_nPageSize = pageSize;
	UpdateThumbRect();
	Invalidate();
}

int CEnvListScrollbar::GetScrollPos() const
{
	return m_nPos;
}

void CEnvListScrollbar::UpdateThumbRect()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int trackLength = rcClient.Height();  // 세로 스크롤 기준
	int range = m_nMax - m_nMin;

	if (range <= 0 || m_nPageSize <= 0) {
		m_rcThumb.SetRectEmpty();
		return;
	}

	// Thumb 크기 계산
	int thumbSize = max(10, trackLength * m_nPageSize / (range + m_nPageSize));

	// Thumb 위치 계산
	int thumbPos = (trackLength - thumbSize) * (m_nPos - m_nMin) / range;

	m_rcThumb = CRect(
		rcClient.left,
		rcClient.top + thumbPos,
		rcClient.right,
		rcClient.top + thumbPos + thumbSize
	);
}

void CEnvListScrollbar::OnPaint()
{
	CPaintDC dc(this);

	// 전체 배경
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, RGB(29, 31, 37));

	// Thumb
	if (!m_rcThumb.IsRectEmpty())
	{
		// 라운드 영역 생성
		CRgn rgn;
		rgn.CreateRoundRectRgn(
			m_rcThumb.left,
			m_rcThumb.top,
			m_rcThumb.right + 1,  // +1 안 하면 오른쪽/하단이 잘릴 수 있음
			m_rcThumb.bottom + 1,
			8, 8  // 둥근 모서리 반지름
		);

		// 클리핑 영역 설정
		dc.SelectClipRgn(&rgn);

		// Thumb 색상 채우기
		dc.FillSolidRect(m_rcThumb, RGB(103, 108, 128));

		// 클리핑 해제
		dc.SelectClipRgn(nullptr);
	}
}

void CEnvListScrollbar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_rcThumb.PtInRect(point)) {
		m_bDragging = true;
		m_nDragOffset = point.y - m_rcThumb.top;
		SetCapture();
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CEnvListScrollbar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging) {
		CRect rcClient;
		GetClientRect(&rcClient);
		int height = rcClient.Height();
		int thumbHeight = m_rcThumb.Height();
		int newY = point.y - m_nDragOffset;
		newY = max(0, min(newY, height - thumbHeight));

		int range = m_nMax - m_nMin;
		int newPos = m_nPos;

		if (height > thumbHeight && range > 0) {
			newPos = m_nMin + (range * newY) / (height - thumbHeight);
			newPos = max(m_nMin, min(newPos, m_nMax)); // 범위 보정
			SetScrollPos(newPos);
			//            if (m_pOwner) m_pOwner->ScrollTo(newPos);
			TRACE(_T("===> MyScroll newPos : %d\n"), newPos);
			OnThumbDrag(newPos);
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CEnvListScrollbar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging) {
		m_bDragging = false;
		ReleaseCapture();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CEnvListScrollbar::OnThumbDrag(int newPos)
{
	if (m_pOwner)
		m_pOwner->OnScrollbarThumbMoved(newPos);
}

