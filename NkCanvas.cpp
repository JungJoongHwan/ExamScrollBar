#include "stdafx.h"
#include "NKCommonUI/NKNewUX/NKCanvas.h"
#include "NKCommonUI/NKNewUX/NKList.h"
#include "NKCommonUI/NKNewUX/NKColorButton.h"
#include "NKCommonUI/NKNewUX/NKCombo.h"
#include "NKCommonUI/NKNewUX/NKTreeList.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CALLBACK ShowChildWndProc(HWND hWnd, LPARAM lParam)
{
	::ShowWindow(hWnd, (int)lParam);
	return 1;
}


// CNKCanvas
CNKCanvas::CNKCanvas()
{
	m_bIsLayered = FALSE;
	m_pBackImage = nullptr;
	m_pHoverCtrl = nullptr;
	m_pOldCtrl = nullptr;
	m_pNewCtrl = nullptr;
	m_nFocusFlag = WA_INACTIVE;
	m_dwLastLClickTimne = 0;
}


CNKCanvas::~CNKCanvas()
{
	ClearControls();
}


void CNKCanvas::SetFont(Gdiplus::Font* pFont)
{
}


void CNKCanvas::Draw(Graphics& g)
{
	if (m_pBackImage)
	{
		g.DrawImage(m_pBackImage, 0, 0);
	}
	else
	{
		Rect rcBrush;
		g.GetClipBounds(&rcBrush);

		SolidBrush brBack(m_clBack);
		g.FillRectangle(&brBack, rcBrush);
	}

	for (int i = 0; i < m_arrControls.GetCount(); ++i)
		m_arrControls[i]->Draw(g);

	DrawBorder(g);
}


void CNKCanvas::OnDraw(CDC* pDC)
{
	if (!m_pParent) return;

	Graphics orgG(pDC->m_hDC);

	CRect rcClient;
	m_pParent->GetClientRect(&rcClient);
/*
	Bitmap memBmp(rcClient.Width(), rcClient.Height());
	Graphics* pG = Graphics::FromImage(&memBmp);
	if (!pG) return;

	RectF rcBound;
	Unit unit;
	memBmp.GetBounds(&rcBound, &unit);

	if (!m_pBackImage)
	{
		SolidBrush brBack(m_clBack);
		if (m_bIsLayered)
			brBack.SetColor(Color(192, 0, 0, 0));

		pG->FillRectangle(&brBack, rcBound);

	}
	else
		pG->DrawImage(m_pBackImage, 0, 0);

	for (int i = 0; i < m_arrControls.GetCount(); ++i)
		m_arrControls[i]->Draw(*pG);

	SetControlRect((int)rcBound.X, (int)rcBound.Y, int(rcBound.Width - 1), int(rcBound.Height - 1));

	DrawBorder(*pG);
	orgG.DrawImage(&memBmp, 0, 0, rcClient.Width(), rcClient.Height());

	delete pG;
*/

	Region rgnClip(Rect(0, 0, 0, 0));
	Rect rcClip;
	CRect rcEdit;
	LPCTSTR lpszClassName = NULL;

	int nCount = GetControlCount();
	for (int i = 0; i < nCount; ++i)
	{
		lpszClassName = GetAt(i)->GetClassName();

		if (!_tcscmp(lpszClassName, _T("spinedit")) || !_tcscmp(lpszClassName, _T("edit")))
		{
			GetAt(i)->GetEditControlRect(rcEdit);
			rcClip.X = rcEdit.left;
			rcClip.Y = rcEdit.top;
			rcClip.Width = rcEdit.Width();
			rcClip.Height = rcEdit.Height();
			rgnClip.Union(rcClip);
		}
	}

	if (!rgnClip.IsEmpty(&orgG))
		orgG.ExcludeClip(&rgnClip);

	RectF rcBound((REAL)rcClient.left, (REAL)rcClient.top, (REAL)rcClient.Width(), (REAL)rcClient.Height());
	if (!m_pBackImage)
	{
		SolidBrush brBack(m_clBack);
		if (m_bIsLayered)
			brBack.SetColor(Color(192, 0, 0, 0));

		orgG.FillRectangle(&brBack, rcBound);
	}
	else
	{
		orgG.DrawImage(m_pBackImage, 0, 0);
	}

	for (int i = 0; i < m_arrControls.GetCount(); ++i)
		m_arrControls[i]->Draw(orgG);

	SetControlRect((int)rcBound.X, (int)rcBound.Y, int(rcBound.Width - 1), int(rcBound.Height - 1));
	DrawBorder(orgG);
}


void CNKCanvas::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bIsLayered)
	{
		if (m_pHoverCtrl)
		{
			m_pHoverCtrl->SetMouseStatus(MouseStatusDown);
			UpdateLayeredWindow();
		}
	}

	for (int i = 0; i < m_arrControls.GetCount(); ++i)
	{
		if (!m_arrControls[i]->IsEnableControl() || !m_arrControls[i]->IsVisibleControl())
			continue;

		m_arrControls[i]->OnLButtonDown(nFlags, point);
	}
}


void CNKCanvas::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsLayered)
	{
		if (m_pHoverCtrl)
		{
			m_pHoverCtrl->SetMouseStatus(MouseStatusHover);
			UpdateLayeredWindow();
		}
	}

	for (int i = 0; i < m_arrControls.GetCount(); ++i)
	{
		if (!m_arrControls[i]->IsEnableControl() || !m_arrControls[i]->IsVisibleControl())
			continue;

		m_arrControls[i]->OnLButtonUp(nFlags, point);
	}
}


void CNKCanvas::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	for (int i = 0; i < m_arrControls.GetCount(); ++i)
	{
		if (!m_arrControls[i]->IsEnableControl() || !m_arrControls[i]->IsVisibleControl())
			continue;

		m_arrControls[i]->OnLButtonDblClk(nFlags, point);
	}
}


void CNKCanvas::OnRButtonDown(UINT nFlags, CPoint point)
{
	for (int i = 0; i < m_arrControls.GetCount(); ++i)
	{
		if (!m_arrControls[i]->IsEnableControl() || !m_arrControls[i]->IsVisibleControl())
			continue;
		m_arrControls[i]->OnRButtonDown(nFlags, point);
	}
}


void CNKCanvas::OnRButtonUp(UINT nFlags, CPoint point)
{
	for (int i = 0; i < m_arrControls.GetCount(); ++i)
	{
		if (!m_arrControls[i]->IsEnableControl() || !m_arrControls[i]->IsVisibleControl())
			continue;
		m_arrControls[i]->OnRButtonUp(nFlags, point);
	}
}


void CNKCanvas::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	for (int i = 0; i < m_arrControls.GetCount(); ++i)
	{
		if (!m_arrControls[i]->IsEnableControl() || !m_arrControls[i]->IsVisibleControl())
			continue;
		m_arrControls[i]->OnRButtonDblClk(nFlags, point);
	}
}


void CNKCanvas::OnMouseMove(UINT nFlags, CPoint point)
{
	TrackMouseEventHover(10, m_pParent->m_hWnd, FALSE);
	TrackMouseEventLeave(20, m_pParent->m_hWnd, FALSE);

	if (!m_bIsLayered)
	{
		for (int i = 0; i < m_arrControls.GetCount(); ++i)
			m_arrControls[i]->OnMouseMove(nFlags, point);
	}
	else
	{
		if (m_pHoverCtrl)
		{
			if (m_pHoverCtrl->GetControlRect().PtInRect(point))
				return;
		}

		CNKControl* pCtrl = nullptr;
		for (int i = 0; i < m_arrControls.GetCount(); ++i)
		{
			if (m_arrControls[i]->GetControlRect().PtInRect(point))
			{
				pCtrl = m_arrControls[i];
				break;
			}
		}

		if (pCtrl)
		{
			pCtrl->SetMouseStatus(MouseStatusHover);
			if (m_pHoverCtrl)
				m_pHoverCtrl->SetMouseStatus(MouseStatusNormal);

			m_pHoverCtrl = pCtrl;
			UpdateLayeredWindow();
		}
		else
		{
			if (m_pHoverCtrl)
			{
				m_pHoverCtrl->SetMouseStatus(MouseStatusNormal);
				UpdateLayeredWindow();
				m_pHoverCtrl = nullptr;
			}
		}
	}
}


void CNKCanvas::OnMouseHover(UINT nFlags, CPoint point)
{
	if (!m_bIsLayered)
	{
		for (int i = 0; i < m_arrControls.GetCount(); ++i)
			m_arrControls[i]->OnMouseHover(nFlags, point);
	}
}


void CNKCanvas::OnMouseLeave()
{
	if (!m_bIsLayered)
	{
		for (int i = 0; i < m_arrControls.GetCount(); ++i)
			m_arrControls[i]->OnMouseLeave();
	}
	else
	{
		if (m_pHoverCtrl)
		{
			m_pHoverCtrl->SetMouseStatus(MouseStatusNormal);
			UpdateLayeredWindow();
			m_pHoverCtrl = nullptr;
		}
	}
}


BOOL CNKCanvas::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	for (int i = 0; i < m_arrControls.GetCount(); ++i)
		m_arrControls[i]->OnMouseWheel(nFlags, zDelta, point);

	return TRUE;
}


void CNKCanvas::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	m_nFocusFlag = nState;

	if(m_pNewCtrl)
		m_pNewCtrl->SetFocusFlag(m_nFocusFlag);
}


void CNKCanvas::OnSetFocus(CNKControl* pOldCtrl)
{
	if (pOldCtrl)
		pOldCtrl->SetFocusFlag(WA_ACTIVE);
}


void CNKCanvas::OnKillFocus(CNKControl* pNewCtrl)
{
	if (m_pNewCtrl)
		m_pNewCtrl->SetFocusFlag(WA_INACTIVE);
}


void CNKCanvas::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (VK_TAB == nChar)
	{
		MoveTabOrder(GetKeyState(VK_SHIFT) & 0x8000);
	}

	if(m_pNewCtrl)
		m_pNewCtrl->OnKeyDown(nChar, nRepCnt, nFlags);
}


void CNKCanvas::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (VK_RETURN == nChar || VK_SPACE == nChar)
	{
		if (m_pNewCtrl)
			m_pNewCtrl->OnKeyUp(nChar, nRepCnt, nFlags);
	}
}


CNKControl* CNKCanvas::AllocationControl(LPCTSTR lpszCtrlName)
{
	CString strCtrlName = lpszCtrlName;
	strCtrlName.MakeLower();

	CNKControl* pControl = nullptr;

	if (_T("button") == strCtrlName)
	{
		pControl = new CNKButton;
	}
	else if (_T("circlebutton") == strCtrlName)
	{
		pControl = new CNKCircleButton;
	}
	else if (_T("roundbutton") == strCtrlName)
	{
		pControl = new CNKRoundButton;
	}
	else if (_T("imagebutton") == strCtrlName)
	{
		pControl = new CNKImageButton;
	}
	else if (_T("langimagebutton") == strCtrlName)
	{
		pControl = new CNKLangImageButton;
	}
	else if (_T("spin") == strCtrlName)
	{
		pControl = new CNKSpin;
	}
	else if (_T("label") == strCtrlName)
	{
		pControl = new CNKLabel;
	}
	else if (_T("langlabel") == strCtrlName)
	{
		pControl = new CNKLangLabel;
	}
	else if (_T("scrolllabel") == strCtrlName)
	{
		pControl = new CNKScrollLabel;
	}
	else if (_T("radio") == strCtrlName)
	{
		pControl = new CNKRadio;
	}
	else if (_T("switch") == strCtrlName)
	{
		pControl = new CNKSwitch;
	}
	else if (_T("edit") == strCtrlName)
	{
		pControl = new CNKEdit;
	}
	else if (_T("multilineedit") == strCtrlName)
	{
		pControl = new CNKMultilineEdit;
	}
	else if (_T("spinedit") == strCtrlName)
	{
		pControl = new CNKSpinEdit;
	}
	else if (_T("animator") == strCtrlName)
	{
		pControl = new CNKAnimator;
	}
	else if (_T("progress") == strCtrlName)
	{
		pControl = new CNKProgress;
	}
	else if (_T("list") == strCtrlName)
	{
		pControl = new CNKList;
	}
	else if (_T("combo") == strCtrlName)
	{
		pControl = new CNKCombo;
	}
	else if (_T("timepicker") == strCtrlName)
	{
		pControl = new CNKTimePicker;
	}
	else if (_T("combobar") == strCtrlName)
	{
		pControl = new CNKComboBar;
	}
	else if (_T("check") == strCtrlName)
	{
		pControl = new CNKCheck;
	}
	else if (_T("langcheck") == strCtrlName)
	{
		pControl = new CNKLangCheck;
	}
	else if (_T("svgcheck") == strCtrlName)
	{
		pControl = new CNKSVGCheck;
	}
	else if (_T("tab") == strCtrlName)
	{
		pControl = new CNKTabControl;
	}
	else if (_T("line") == strCtrlName)
	{
		pControl = new CNKLine;
	}
	else if (_T("gradientline") == strCtrlName)
	{
		pControl = new CNKGradientLine;
	}
	else if (_T("colorbutton") == strCtrlName)
	{
		pControl = new CNKColorButton;
	}
	else if (_T("treelist") == strCtrlName)
	{
		pControl = new CNKTreeList;
	}
	else if (_T("hexagonpalette") == strCtrlName)
	{
		pControl = new CNKHexagonPalette;
	}
	else if (_T("rectanglepalette") == strCtrlName)
	{
		pControl = new CNKRectanglePalette;
	}
	else if (_T("colorcombo") == strCtrlName)
	{
		pControl = new CNKColorCombo;
	}
	else if (_T("searchedit") == strCtrlName)
	{
		pControl = new CNKSearchEdit;
	}
	
	return pControl;
}


CNKControl* CNKCanvas::AddControl(LPCTSTR lpszClassName, LPCTSTR lpszCaption, CRect rcControl, DWORD dwStyle, UINT nID, CWnd* pParent)
{
	CNKControl* pControl = AllocationControl(lpszClassName);
	if (pControl)
	{
		pControl->InitControl(lpszClassName, lpszCaption, rcControl, dwStyle, nID, pParent);
		m_arrControls.Add(pControl);
	}

	return pControl;
}


CNKControl* CNKCanvas::AddControlEx(LPCTSTR lpszTag, LPCTSTR lpszClassName, LPCTSTR lpszCaption, CRect rcControl, DWORD dwStyle, UINT nID, CWnd* pParent)
{
	CNKControl* pControl = AllocationControl(lpszClassName);
	if (pControl)
	{
		pControl->InitControl(lpszClassName, lpszCaption, rcControl, dwStyle, nID, pParent);
		m_arrControls.Add(pControl);

		CNKControl* pVal = nullptr;
		m_mapControls.Lookup(lpszTag, pVal);
		ASSERT(!pVal);
		m_mapControls.SetAt(lpszTag, pControl);
	}

	return pControl;
}


CNKControl* CNKCanvas::AddControl(LPCTSTR lpszClassName, LPCTSTR lpszCaption, DWORD dwStyle, UINT nID, CWnd* pParent)
{
	if (!pParent)
		return nullptr;

	CWnd* pCtrl = pParent->GetDlgItem(nID);
	if (!pCtrl)
		return nullptr;

	CRect rcControl;
	pCtrl->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);

	CNKControl* pControl = AllocationControl(lpszClassName);
	if (pControl)
	{
		pControl->InitControl(lpszClassName, lpszCaption, rcControl, dwStyle, nID, pParent);
		m_arrControls.Add(pControl);
	}

	return pControl;
}


void CNKCanvas::AddControl(CNKControl* pControl)
{
	if (pControl)
		m_arrControls.Add(pControl);
}


CNKControl* CNKCanvas::FindControl(UINT nID)
{
	CNKControl* pControl = nullptr;
	for (int i = 0; i < m_arrControls.GetCount(); ++i)
	{
		pControl = m_arrControls[i];
		if (pControl->GetControlID() == nID)
			return pControl;
	}

	return nullptr;
}


CNKControl* CNKCanvas::FindControl(LPCTSTR lpszTag)
{
	CNKControl* pControl = nullptr;
	m_mapControls.Lookup(lpszTag, pControl);
	return pControl;
}


CNKControl* CNKCanvas::GetAt(int nIndex)
{
	CNKControl* pControl = nullptr;
	if (0 <= nIndex && nIndex <= m_arrControls.GetCount() - 1)
		pControl = m_arrControls[nIndex];

	return pControl;
}


void CNKCanvas::ClearControls()
{
	for (int i = 0; i < m_arrControls.GetCount(); ++i)
		delete m_arrControls[i];

	m_arrControls.RemoveAll();
	m_mapControls.RemoveAll();
}


void CNKCanvas::DrawBackground(Graphics& g)
{
	if (!m_pParent)
		return;

	CRect rcClient;
	m_pParent->GetClientRect(&rcClient);

	SolidBrush brBack(m_clBack);
	g.FillRectangle(&brBack, 0, 0, rcClient.Width(), rcClient.Height());
}


void CNKCanvas::SetBorderColor(Color clBorder)
{
	CNKControl::SetBorderColor(clBorder);

	for (int i = 0; i < m_arrControls.GetCount(); ++i)
	{
		if (!_tcscmp(m_arrControls[i]->GetClassName(), _T("button")) ||
			!_tcscmp(m_arrControls[i]->GetClassName(), _T("colorbutton")) ||
			!_tcscmp(m_arrControls[i]->GetClassName(), _T("edit")) ||
			!_tcscmp(m_arrControls[i]->GetClassName(), _T("combo")))
			continue;

		m_arrControls[i]->SetBorderColor(clBorder);
	}
}


void CNKCanvas::SetTextColor(Color clText)
{
	CNKControl::SetTextColor(clText);

	for (int i = 0; i < m_arrControls.GetCount(); ++i)
		m_arrControls[i]->SetTextColor(clText);
}


void CNKCanvas::SetBackColor(Color clBack)
{
	CNKControl::SetBackColor(clBack);

	for (int i = 0; i < m_arrControls.GetCount(); ++i)
	{
		if (!_tcscmp(m_arrControls[i]->GetClassName(), _T("button")) ||
			!_tcscmp(m_arrControls[i]->GetClassName(), _T("colorbutton")) ||
			!_tcscmp(m_arrControls[i]->GetClassName(), _T("edit")) ||
			!_tcscmp(m_arrControls[i]->GetClassName(), _T("combo")) ||
			m_arrControls[i]->GetBkMode() == OPAQUE)
			continue;

		m_arrControls[i]->SetBackColor(clBack);
	}
}


void CNKCanvas::RelayEvent(LPMSG lpMsg)
{
	if (!m_pParent)
		return;

	CPoint pt = lpMsg->pt;
	m_pParent->ScreenToClient(&pt);

	switch (lpMsg->message)
	{
	case WM_LBUTTONDOWN:	
	{
		DWORD dwCurrentTime = lpMsg->time;  // 메시지 생성 시각을 이용

		if (dwCurrentTime - m_dwLastLClickTimne <= GetDoubleClickTime() &&
			abs(pt.x - m_ptlastClickPoint.x) <= 5 && abs(pt.y - m_ptlastClickPoint.y) <= 5)
			OnLButtonDblClk(lpMsg->wParam, pt);
		else
			OnLButtonDown(lpMsg->wParam, pt);

		m_dwLastLClickTimne = dwCurrentTime;
		m_ptlastClickPoint = pt;
	}
	break;
	case WM_LBUTTONUP:		OnLButtonUp(lpMsg->wParam, pt);		break;
	case WM_LBUTTONDBLCLK:	OnLButtonDblClk(lpMsg->wParam, pt); break;
	case WM_RBUTTONDOWN:	OnRButtonDown(lpMsg->wParam, pt);	break;
	case WM_RBUTTONUP:		OnRButtonUp(lpMsg->wParam, pt);		break;
	case WM_RBUTTONDBLCLK:	break;
	case WM_MOUSEMOVE:		OnMouseMove(lpMsg->wParam, pt);		break;
	case WM_MOUSEHOVER:		OnMouseHover(lpMsg->wParam, pt);	break;
	case WM_MOUSELEAVE:		OnMouseLeave();						break;
	case WM_MOUSEWHEEL:		OnMouseWheel(GET_KEYSTATE_WPARAM(lpMsg->wParam), GET_WHEEL_DELTA_WPARAM(lpMsg->wParam), pt); break;
	case WM_KEYDOWN:		OnKeyDown(lpMsg->wParam, LOWORD(lpMsg->lParam), HIWORD(lpMsg->lParam));	break;
	case WM_KEYUP: {
		OnKeyUp(lpMsg->wParam, LOWORD(lpMsg->lParam), HIWORD(lpMsg->lParam)); 
	}
				 break;
	default:				break;
	}
}


BOOL CNKCanvas::LoadImageFromFile(CString& strFileName)
{
	if (m_pBackImage)
		SAFE_DELETE(m_pBackImage);

	BOOL bResult = FALSE;
	m_pBackImage = Image::FromFile(strFileName);

	if (!m_pBackImage)
		return bResult;

	if (Ok == m_pBackImage->GetLastStatus())
		bResult = TRUE;

	if(m_pParent)
		m_pParent->SetWindowPos(NULL, 0, 0, m_pBackImage->GetWidth(), m_pBackImage->GetHeight(), SWP_NOMOVE);

	return bResult;
}


HBITMAP CNKCanvas::PrepareDIBSection(CDC& memDC, int nWidth, int nHeight)
{
	BITMAPINFO bmInfo;
	ZeroMemory(&bmInfo, sizeof(BITMAPINFO));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = nWidth;
	bmInfo.bmiHeader.biHeight = nHeight;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 32;
	bmInfo.bmiHeader.biCompression = BI_RGB;

	BYTE* lpBits = NULL;
	HBITMAP hDIB = CreateDIBSection(NULL, &bmInfo, DIB_RGB_COLORS, (void**)&lpBits, NULL, 0L);
	SetDIBits(memDC.m_hDC, hDIB, 0, nHeight, lpBits, &bmInfo, DIB_RGB_COLORS);

	return hDIB;
}


void CNKCanvas::UpdateLayeredWindow()
{
	CRect rcWindow;
	m_pParent->GetWindowRect(rcWindow);

	CDC dcScreen;
	dcScreen.Attach(::GetDC(nullptr));

	CDC* pDC = m_pParent->GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	HBITMAP hmemBmp = PrepareDIBSection(dcMem, rcWindow.Width(), rcWindow.Height());
	CBitmap* pOldBmp = dcMem.SelectObject(CBitmap::FromHandle(hmemBmp));

	OnDraw(&dcMem);

	CPoint ptDest(rcWindow.TopLeft());
	CSize size(rcWindow.Width(), rcWindow.Height());
	CPoint ptSrc(0, 0);

	BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	m_pParent->UpdateLayeredWindow(&dcScreen, &ptDest, &size, &dcMem, &ptSrc, RGB(255, 0, 255), &blendPixelFunction, ULW_ALPHA);

	::DeleteObject(hmemBmp);
	dcMem.SelectObject(pOldBmp);
	m_pParent->ReleaseDC(pDC);
	m_pParent->ReleaseDC(&dcScreen);
}


void CNKCanvas::SetParent(CWnd* pParent)
{
	CNKControl::SetParent(pParent);
	CreateDummyButton();
}


CSize CNKCanvas::GetBackImageSize()
{
	if (!m_pBackImage)
		return CSize(0, 0);

	if(Ok != m_pBackImage->GetLastStatus())
		return CSize(0, 0);

	return CSize(m_pBackImage->GetWidth(), m_pBackImage->GetHeight());
}


CNKControl* CNKCanvas::CreateControl(LPCTSTR lpszCtrlName)
{
	return AllocationControl(lpszCtrlName);
}


CNKControl* CNKCanvas::GetFocus()
{
	return m_pNewCtrl;
}


void CNKCanvas::SetFocus(LPCTSTR lpszTag)
{
	if (0 >= _tcslen(lpszTag))
	{
		if (m_btnDummy.GetSafeHwnd())
			m_btnDummy.SetFocus();
		return;
	}

	m_pOldCtrl = m_pNewCtrl;
	if (m_pOldCtrl)
	{
		SetFocusDummyButton(m_pOldCtrl);
		m_pOldCtrl->SetFocusFlag(WA_INACTIVE);
	}

	m_pNewCtrl = FindControl(lpszTag);
	if (m_pNewCtrl)
	{
		m_pNewCtrl->SetFocusFlag(WA_ACTIVE);
	}
}

void CNKCanvas::SetFocus(CNKControl* pNewCtrl)
{
	if (nullptr == pNewCtrl)
		return;

	m_pOldCtrl = m_pNewCtrl;
	if (m_pOldCtrl)
	{
		SetFocusDummyButton(m_pOldCtrl);
		m_pOldCtrl->SetFocusFlag(WA_INACTIVE);
	}

	m_pNewCtrl = pNewCtrl;
	if (m_pNewCtrl)
	{
		m_pNewCtrl->SetFocusFlag(WA_ACTIVE);
	}
}


void CNKCanvas::MoveTabOrder(BOOL bReverse)
{
	if (!m_pNewCtrl)
		return;

	int nPos = 0;
	int nID = m_pNewCtrl->GetControlID();

	for (int i = 0; i < m_arrControls.GetCount(); ++i)
	{
		if (m_arrControls[i]->GetControlID() == nID)
		{
			nPos = i;
			break;
		}
	}

	m_pOldCtrl = m_pNewCtrl;
	if (m_pOldCtrl)
	{
		SetFocusDummyButton(m_pOldCtrl);
		m_pOldCtrl->SetFocusFlag(WA_INACTIVE);
	}

	m_pNewCtrl = nullptr;
	if (!bReverse)
	{
		MoveTabOrder(nPos, m_arrControls.GetCount(), bReverse);
		if (!m_pNewCtrl)
			MoveTabOrder(-1, m_arrControls.GetCount(), bReverse);
	}
	else
	{
		MoveTabOrder(nPos, 0, bReverse);
		if (!m_pNewCtrl)
			MoveTabOrder(m_arrControls.GetCount(), 0, bReverse);
	}
}


void CNKCanvas::MoveTabOrder(int nStart, int nEnd, BOOL bReverse)
{
	if (!bReverse)
	{
		nStart += 1;
		if (m_arrControls.GetCount() <= nStart)
			nStart = 0;

		for (int i = nStart; i < nEnd; ++i)
		{
			CNKControl* pCtrl = GetAt(i);
			if (pCtrl && !_tcscmp(_T("edit"), pCtrl->GetClassName()) ||
				!_tcscmp(_T("imagebutton"), pCtrl->GetClassName()) ||
				!_tcscmp(_T("langimagebutton"), pCtrl->GetClassName()))
			{
				m_pNewCtrl = pCtrl;
				if (m_pNewCtrl)
					m_pNewCtrl->SetFocusFlag(m_nFocusFlag);
				break;
			}
		}
	}
	else
	{
		nStart -= 1;
		if (0 > nStart)
			nStart = m_arrControls.GetCount() - 1;

		for (int i = nStart; nEnd <= i; --i)
		{
			CNKControl* pCtrl = GetAt(i);
			if (pCtrl && !_tcscmp(_T("edit"), pCtrl->GetClassName()) ||
				!_tcscmp(_T("imagebutton"), pCtrl->GetClassName()) ||
				!_tcscmp(_T("langimagebutton"), pCtrl->GetClassName()))
			{
				m_pNewCtrl = pCtrl;
				if (m_pNewCtrl)
					m_pNewCtrl->SetFocusFlag(m_nFocusFlag);
				break;
			}
		}
	}
}

BOOL CNKCanvas::CreateDummyButton()
{
	return m_btnDummy.Create(_T("dummy"), WS_CHILD | WS_TABSTOP, CRect(0, 0, 0, 0), this->m_pParent, 1001);
}


void CNKCanvas::SetFocusDummyButton(CNKControl* pCtrl)
{
	if (!m_btnDummy.GetSafeHwnd() || nullptr == pCtrl)
		return;

	if (!_tcscmp(_T("edit"), m_pOldCtrl->GetClassName()))
	{
		if (m_btnDummy.GetSafeHwnd())
		{
			m_pParent->SetCapture();
			m_btnDummy.SetFocus();
			ReleaseCapture();
		}
	}
}
