// ChatServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatServer.h"
#include "UDPServer.h"
#include "ChatServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SAFEDELETE(x) if(x!=NULL){delete x;x=NULL;}//ɾ������� ��
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CChatServerDlg �Ի���
CChatServerDlg::CChatServerDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CChatServerDlg::IDD, pParent),UDPSocket(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_START, &CChatServerDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_EXIT, &CChatServerDlg::OnBnClickedExit)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CChatServerDlg ��Ϣ�������

BOOL CChatServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//��ʼ��SOCKET
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		MessageBox(_T("SOCKET��ʼ��û�гɹ�"),_T("����"),MB_ICONWARNING|MB_OK);
		PostQuitMessage(0);
	}
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	InitList();//��ʼ���б��
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChatServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChatServerDlg::OnClose()
{
	if (MessageBox("ȷ��Ҫ�رշ�����?","����",MB_OKCANCEL|MB_ICONWARNING) == IDOK)
	{
		CDialog::OnClose();
	}
}

//���￪ʼ�򿪸ö˿ڵ�������񣬵ȴ��ͻ�������
void CChatServerDlg::OnBnClickedStart()
{
	if(UDPSocket.StartListen())
	{
		this->GetDlgItem(IDC_START)->EnableWindow(FALSE);
		MessageReturn("�ɹ�������������");
	}
	else
		MessageReturn("����������ʧ�ܣ�");
}

void CChatServerDlg::MessageReturn(LPCTSTR lpszMessage)
{
	CRichEditCtrl * pView = (CRichEditCtrl *)GetDlgItem(IDC_VIEW);
	CString strTemp = lpszMessage;
	strTemp += _T("\r\n");
	int len = pView->GetWindowTextLength();
	pView->SetSel(len,len);
	pView->ReplaceSel(strTemp);
}

void CChatServerDlg::Message(LPCTSTR lpszMessage)
{
	CRichEditCtrl * pView = (CRichEditCtrl *)GetDlgItem(IDC_VIEW);
	CString strTemp = lpszMessage;
	int len = pView->GetWindowTextLength();
	pView->SetSel(len,len);
	pView->ReplaceSel(strTemp);
}

//��ʼ�������û��б�
void CChatServerDlg::InitList()
{
	//��ʼ��LIST�ؼ������ϱ���
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	pList->ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	AddExStyleOfList(LVS_EX_FULLROWSELECT | LVS_OWNERDRAWFIXED);

	int i;
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CString	strTemp[2] = {"����", "IP"};
	int size[2] = {60,90};//��ӱ�����,����
	for(i = 0; i < 2; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = (char*)(LPCTSTR)strTemp[i];
		lvc.cx = size[i];
		lvc.fmt = LVCFMT_CENTER;
		pList->InsertColumn(i, &lvc);//���һ��
	}
	//��ʼ��ͼ���б�
	m_imgList.Create(IDB_IMAGE, 16, 1, RGB(0, 255, 0));
	HIMAGELIST him = m_imgList.m_hImageList;
	::SendMessage(GetDlgItem(IDC_USERLIST)->m_hWnd,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)him);
	ImageList_SetBkColor(him,CLR_NONE);
}

//�޸ķ��
void CChatServerDlg::AddExStyleOfList(DWORD dwNewStyle)
{
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	//���LIST�ķ��
	DWORD dwStyle = ::SendMessage (pList->m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= dwNewStyle;
	//��LIST���޸ķ����Ϣ
	::SendMessage (pList->m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}


//��LIST���Ƴ�һ��
void CChatServerDlg::RemoveItemOfList(char* name)
{
	CString str;
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	for(int i = 0; i < pList->GetItemCount(); i++){
		str =  pList->GetItemText(i,0);
		if(strcmp(str, name) == 0){
			pList->DeleteItem(i);
			break;
		}
	}
}

//ΪLIST���һ��,����iΪͼ������ͣ�nameΪ�û�����textΪIP��ַ
void CChatServerDlg::AddItemOfList(short i, char* name, char* text)
{
	LVITEM			lvi;
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.iItem = pList->GetItemCount();
	lvi.iImage = i;
	lvi.iSubItem = 0;
	lvi.pszText = name;
	lvi.cchTextMax = 64;
	lvi.lParam = 0;
	pList->InsertItem(&lvi);

	lvi.mask = LVIF_TEXT;
	lvi.pszText = text;
	lvi.cchTextMax = 32;
	lvi.iSubItem = 1;
	pList->SetItem(&lvi);

}
void CChatServerDlg::OnBnClickedExit()
{
	PostMessage(WM_CLOSE,0,0);
}

void CChatServerDlg::OnDestroy()
{
	UDPSocket.SevverShutDown();
	CDialog::OnDestroy();
}
