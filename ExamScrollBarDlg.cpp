
// ExamScrollBarDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ExamScrollBar.h"
#include "ExamScrollBarDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*!
	@brief		수정할 윈도우의 평면 혹은 보더 처리
	@param      pModifyWnd : 속성 변경할 윈도우
	@param      bFlat : 평면 처리 여부
	@param      bBorder : 보더 처리 여부
*/
void NK_SM_ModifyFlatStyle(CWnd* pModifyWnd, BOOL bFlat, BOOL bBorder)
{
	if (pModifyWnd && pModifyWnd->GetSafeHwnd())
	{
		if (bBorder)
			pModifyWnd->ModifyStyle(0, WS_BORDER, SWP_DRAWFRAME);
		else
			pModifyWnd->ModifyStyle(WS_BORDER, 0, SWP_DRAWFRAME);

		if (bFlat)
			pModifyWnd->ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME, 0, SWP_DRAWFRAME);
		else
			pModifyWnd->ModifyStyleEx(0, WS_EX_STATICEDGE | WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME, SWP_DRAWFRAME);
	}
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

// CExamScrollBarDlg 대화 상자

CExamScrollBarDlg::CExamScrollBarDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXAMSCROLLBAR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExamScrollBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_LinkList);
}

BEGIN_MESSAGE_MAP(CExamScrollBarDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CExamScrollBarDlg 메시지 처리기

BOOL CExamScrollBarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetDialogFont(10, _T("맑은 고딕"));

	CDC* pDC = GetDC();
	if (pDC) {
		CFont* pFont = GetFont();
		CFont* pOldFont = nullptr;
		if (pFont) pOldFont = pDC->SelectObject(pFont);

		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);

		int nPadding = 24;  // 위아래 여백 픽셀
		int nItemHeight = tm.tmHeight + tm.tmExternalLeading + nPadding;

		//m_wndTmLstBox.SetItemHeight(0, nItemHeight);

		if (pOldFont) pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
	}
	
	//m_wndTmLstBox.AddSampleItem();
	//----------------------------------------------------------------
	// 선택화면 리스트 초기화
	LV_COLUMN	lvcolumn;
	TCHAR* strTitle[] = { _T("화면번호"),_T("화면명")};
	
	CRect rc;
	m_LinkList.GetClientRect(rc);

	for (int i = 0; i < 2; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = strTitle[i];
		lvcolumn.iSubItem = i;
		if (i == 0)
			lvcolumn.cx = rc.Width() / 3;
		else
			lvcolumn.cx = lvcolumn.cx = rc.Width()*2 / 3;

		m_LinkList.InsertColumn(i, &lvcolumn);
	}

	NK_SM_ModifyFlatStyle(&m_LinkList, TRUE, FALSE);
	m_LinkList.InitList();
	m_LinkList.SetBkColor(RGB(29, 31, 37));
	m_LinkList.SetHeaderBkColor(RGB(34, 36, 43));
	m_LinkList.SetTxtColor(RGB(238, 238, 238));
	m_LinkList.UpdateHeaderColors();
	m_LinkList.ModifyStyle(WS_VSCROLL, 0);
	m_LinkList.ModifyStyle(WS_HSCROLL, 0);
	//m_LinkList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_LinkList.SetFont(&m_dlgFont);

	//----------------------------------------------------------------

	// 리스트컨트롤의 스크롤바 생성
	CRect rcList;
	m_LinkList.GetWindowRect(&rcList);
	ScreenToClient(&rcList);
	CRect rcScroll(rcList.right + 2, rcList.top, rcList.right + 24, rcList.bottom);
	m_ListScrollbar.Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE, rcScroll, this, 998);
	m_LinkList.AttachScrollbar(&m_ListScrollbar);
	m_LinkList.UpdateScrollbar();

	//----------------------------------------------------------------
	
	for (int i = 0; i < 100; i++)
	{
		CString strNo, strName;
		strNo.Format(_T("%04d"), i + 1);
		strName.Format(_T("화면명 %04d"), i + 1);
		InsertListItem(strNo, strName);
	}
	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CExamScrollBarDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

}

void CExamScrollBarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CExamScrollBarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CExamScrollBarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CExamScrollBarDlg::SetDialogFont(int nPtSize, LPCTSTR lpszFaceName)
{
	CDC* pDC = GetDC();
	if (!pDC)	return;

	int nFontHeight = -MulDiv(nPtSize, pDC->GetDeviceCaps(LOGPIXELSY), 72);
	ReleaseDC(pDC);

	if (m_dlgFont.GetSafeHandle())
		m_dlgFont.DeleteObject();

	m_dlgFont.CreateFont(
		nFontHeight, 0, 0, 0,
		FW_NORMAL, FALSE, FALSE, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
		lpszFaceName
	);
	
	SetFont(&m_dlgFont);

	CWnd* pChild = GetWindow(GW_CHILD);
	while (pChild) {
		pChild->SetFont(&m_dlgFont);
		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}
}

void CExamScrollBarDlg::InsertListItem(CString strNo, CString strName)
{
	int nRow = m_LinkList.GetItemCount();

	LV_ITEM		lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iSubItem = 0;
	lvItem.pszText = (LPTSTR)(LPCTSTR)strNo;
	lvItem.iItem = nRow;
	lvItem.lParam = nRow;
	m_LinkList.InsertItem(&lvItem);
	m_LinkList.SetItemText(nRow, 1, (LPTSTR)(LPCTSTR)strName);
	//m_LinkList.SetItemData(nRow, (DWORD_PTR)pMDBItem);
	nRow++;
}
