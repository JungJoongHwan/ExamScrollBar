#pragma once

//#include "NKControl.h"
#include "NKButton.h"
#include "NKEdit.h"
#include "NKAnimator.h"
#include "NKList.h"
#include "NKTabControl.h"
#include "NKColorButton.h"
#include "NKCombo.h"


#ifdef _NKCOMMONUI_EXPORT
#define NKCOMMONUI_CLASS	__declspec(dllexport)
#define NKCOMMONUI_API		__declspec(dllexport)
#else
#define NKCOMMONUI_CLASS	__declspec(dllimport)
#define NKCOMMONUI_API		__declspec(dllimport)
#endif


NKCOMMONUI_CLASS BOOL CALLBACK ShowChildWndProc(HWND hWnd, LPARAM lParam);


class NKCOMMONUI_CLASS CNKCanvas : public CNKControl
{
public:
	CNKCanvas();
	~CNKCanvas();

	virtual void SetFont(Gdiplus::Font* pFont);
	virtual void Draw(Graphics& g);
	virtual void UpdateLayeredWindow();
	virtual void SetParent(CWnd* pParent);

	virtual void OnDraw(CDC* pDC);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnRButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnMouseHover(UINT nFlags, CPoint point);
	virtual void OnMouseLeave();
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	virtual void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual void OnSetFocus(CNKControl* pOldCtrl);
	virtual void OnKillFocus(CNKControl* pNewCtrl);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void SetBorderColor(Color clBorder);
	virtual void SetTextColor(Color clText);
	virtual void SetBackColor(Color clBack);
	virtual CNKControl* GetFocus();
	virtual void SetFocus(LPCTSTR lpszTag);

	virtual void RelayEvent(LPMSG lpMsg);
	virtual BOOL LoadImageFromFile(CString& strFileName);

	CNKControl* AddControl(LPCTSTR lpszClassName, LPCTSTR lpszCaption, CRect rcControl, DWORD dwStyle, UINT nID, CWnd* pParent);
	CNKControl* AddControlEx(LPCTSTR lpszTag, LPCTSTR lpszClassName, LPCTSTR lpszCaption, CRect rcControl, DWORD dwStyle, UINT nID, CWnd* pParent);
	CNKControl* AddControl(LPCTSTR lpszClassName, LPCTSTR lpszCaption, DWORD dwStyle, UINT nID, CWnd* pParent);
	void AddControl(CNKControl* pControl);
	CNKControl* FindControl(UINT nID);
	CNKControl* FindControl(LPCTSTR lpszTag);
	CNKControl* GetAt(int nIndex);
	void MoveTabOrder(BOOL bReverse = FALSE);

	void SetLayered(BOOL bLayered) { m_bIsLayered = bLayered; }
	BOOL IsLayerdWindow() { return m_bIsLayered; }
	int GetControlCount() { return m_arrControls.GetCount(); }
	CSize GetBackImageSize();
	CNKControl* CreateControl(LPCTSTR lpszCtrlName);
	void SetFocus(CNKControl* pNewCtrl);
protected:
	CNKControl* AllocationControl(LPCTSTR lpszCtrlName);
	void ClearControls();
	HBITMAP PrepareDIBSection(CDC& memDC, int nWidth, int nHeight);
	void MoveTabOrder(int nStart, int nEnd, BOOL bReverse = FALSE);
	BOOL CreateDummyButton();
	void SetFocusDummyButton(CNKControl* pCtrl);
	virtual void DrawBackground(Graphics& g);

protected:
	CArrayNKControl m_arrControls;
	CMapNKControl m_mapControls;
	BOOL m_bIsLayered;
	Image* m_pBackImage;
	CNKControl* m_pHoverCtrl;
	CNKControl* m_pOldCtrl;
	CNKControl* m_pNewCtrl;
	UINT m_nFocusFlag;
	CButton m_btnDummy;

	DWORD m_dwLastLClickTimne;
	CPoint m_ptlastClickPoint;
};

