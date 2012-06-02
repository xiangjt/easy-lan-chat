// ChatClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "ConnectDlg.h"
#include "UDPClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SAFEDELETE(x) if(x!=NULL){delete x;x=NULL;}//ɾ������� ��
#define PORT 2310//�˿ں�
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


// CChatClientDlg �Ի���




CChatClientDlg::CChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatClientDlg::IDD, pParent)
{
	m_pUDPClient = new CUDPClient(this);
	m_bFilter = FALSE;
	m_bConnected = FALSE;
	crf = RGB(0,0,0);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_LARGEQQ);
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_USERLIST, &CChatClientDlg::OnNMDblclkUserlist)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SEND, &CChatClientDlg::OnBnClickedSend)
	ON_COMMAND(ID_CONNECT, &CChatClientDlg::OnConnect)
	ON_BN_CLICKED(IDC_FILTER, &CChatClientDlg::OnBnClickedFilter)
	ON_COMMAND(ID_DISCONNECT, &CChatClientDlg::OnDisconnect)
	ON_BN_CLICKED(IDC_COLORSELECT, &CChatClientDlg::OnBnClickedColorselect)
	ON_WM_CTLCOLOR()
	ON_WM_INITMENUPOPUP()
	ON_WM_DESTROY()
	ON_COMMAND(ID_EXIT, &CChatClientDlg::OnExit)
	ON_COMMAND(ID_SHOWIP, &CChatClientDlg::OnShowip)
END_MESSAGE_MAP()


// CChatClientDlg ��Ϣ�������

BOOL CChatClientDlg::OnInitDialog()
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

	//��ʼ��LIST
	InitList();
	//��ʼ��TYPE COMBO
	InitType();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatClientDlg::OnPaint()
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
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//list�ؼ�����ʾ�����û��ĺ���

void CChatClientDlg::InitList()
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

//��ʼ��TYPE���������STRING
void CChatClientDlg::InitType()
{
	CComboBox* pTo = (CComboBox*)GetDlgItem(IDC_CURRENT);
	pTo->AddString("������");
	pTo->SetCurSel(0);

	CComboBox* pType = (CComboBox*)GetDlgItem(IDC_TYPE);
	pType->ResetContent();
	CString str;
	for(int i = 0; i <= 32; i++)
	{
		str.LoadString(IDS_S0 + i);
		pType->AddString(str);
	}
	pType->SetCurSel(0);
}

//ΪLIST����µ���չ��Ϣ
void CChatClientDlg::AddExStyleOfList(DWORD dwNewStyle)
{
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	//���LIST�ķ��
	DWORD dwStyle = ::SendMessage (pList->m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= dwNewStyle;
	//��LIST���޸ķ����Ϣ
	::SendMessage (pList->m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}

//��LIST���Ƴ�һ��
void CChatClientDlg::RemoveItemOfList(char* name)
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
void CChatClientDlg::AddItemOfList(short i, char* name, char* text)
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

//���û�˫��LIST��ʱ���CURRENT COMBO�����һ��
void CChatClientDlg::OnNMDblclkUserlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMItem = (NM_LISTVIEW*)pNMHDR;
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_USERLIST);
	CString str;
	int iSel = pNMItem->iItem;
	if (iSel >= 0 ){
		str = pList->GetItemText(iSel,0);
		if(str == m_strName) return;//�㵽���Լ�
		CComboBox* pCurr = (CComboBox*)GetDlgItem(IDC_CURRENT);
		if (pCurr->FindStringExact(-1,(LPCTSTR)str)<0)//��CURRENT COMBO������ͬ��ʱ�����
		{
			pCurr->AddString(str);
			pCurr->SelectString(-1,(LPCTSTR)str);
		}else
			pCurr->SelectString(-1,(LPCTSTR)str);
	}
	*pResult = 0;
}




//��ʾ��Ϣ
void CChatClientDlg::DisplayMsg(LPCTSTR lpszText)
{
	TextOut(lpszText, RGB(128,0,0));
}

//��������ʾ����
void CChatClientDlg::TextOut(LPCTSTR lpszMessage, COLORREF clr)
{
	CRichEditCtrl * pView = (CRichEditCtrl *)GetDlgItem(IDC_CHATVIEW);

	memset(&cfm,   0,   sizeof(cfm));   
	cfm.cbSize   =   sizeof(CHARFORMAT);   
	cfm.dwMask   =   CFM_BOLD   |   CFM_COLOR   |   CFM_FACE   |   CFM_ITALIC   |   CFM_SIZE   |   CFM_STRIKEOUT   |   CFM_UNDERLINE;   
	cfm.crTextColor   =   clr;//��ɫ   
	_tcscpy(cfm.szFaceName,   _T("����"));//����   
	cfm.bPitchAndFamily   =   FF_SWISS;   
	cfm.yHeight   =   200;     

	int len = pView->GetWindowTextLength();
	pView->SetSel(len,len);

	pView->SetSelectionCharFormat(cfm);
	pView->ReplaceSel(lpszMessage);
}

//����
void CChatClientDlg::OnBnClickedSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox* pTo = (CComboBox*)GetDlgItem(IDC_CURRENT);
	CComboBox* pType = (CComboBox*)GetDlgItem(IDC_TYPE);
	CWnd* pText = (CWnd*)GetDlgItem(IDC_MSG);
	CButton* pSecret = (CButton*)GetDlgItem(IDC_QUIET);

	MSG_INFO mi;
	CString tmp;
	strcpy(mi.m_From,(LPCTSTR)m_strName);
	pTo->GetWindowText(tmp);
	if (tmp == "������")		tmp = "";
	strcpy(mi.m_To,(LPCTSTR)tmp);
	tmp.Empty();
	pType->GetWindowText(tmp);
	mi.m_Type = pType->FindString(0, tmp);
	tmp.Empty();
	pText->GetWindowText(tmp);
	strcpy(mi.m_Text,(LPCTSTR)tmp);
	if(tmp.IsEmpty()) return;
	mi.m_Secret = pSecret->GetCheck();
	mi.m_Color = crf;
	pText->SetWindowText(_TEXT(""));
	m_pUDPClient->SendMsg(mi);
}


//��������Ӳ˵�����������
void CChatClientDlg::OnConnect()
{
	CConnectDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_strName = dlg.m_strName;
		this->SetWindowText(m_strName);
		m_pUDPClient->Connect(dlg.m_strName,dlg.m_strIP,dlg.m_image);
	}
}

void CChatClientDlg::OnBnClickedFilter()
{
	CButton* pFilter = (CButton*)GetDlgItem(IDC_FILTER);
	m_bFilter = pFilter->GetCheck();
}


void CChatClientDlg::OnDisconnect()
{
	m_pUDPClient->DisConnect();
}

void CChatClientDlg::OnBnClickedColorselect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CEdit * pEdit  = (CEdit *)GetDlgItem(IDC_MSG);
	CColorDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.GetColor() == RGB(255,255,255))
			AfxMessageBox("��������Ϊ��ɫ�Ϳ������˰�!");
		else
		{
			crf = dlg.GetColor();
			pEdit->Invalidate(TRUE);
			pEdit->UpdateWindow();
		}
	}	
}


//���ñ༭���������ɫ
HBRUSH CChatClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		CEdit * pEdit  = (CEdit *)GetDlgItem(IDC_MSG);
		if (pEdit->m_hWnd == pWnd->m_hWnd)
		{
			pDC->SetTextColor(crf);
		}
	}
	return hbr;
}


//���û�������ӻ�Ͽ�����
void CChatClientDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	if (nIndex == 0)
	{
		if (m_bConnected == false)
		{
			pPopupMenu->EnableMenuItem(ID_CONNECT,MF_BYCOMMAND|MF_ENABLED);
			pPopupMenu->EnableMenuItem(ID_DISCONNECT,MF_BYCOMMAND|MF_GRAYED);
		}
		else
		{
			pPopupMenu->EnableMenuItem(ID_CONNECT,MF_BYCOMMAND|MF_GRAYED);
			pPopupMenu->EnableMenuItem(ID_DISCONNECT,MF_BYCOMMAND|MF_ENABLED);
		}
	}
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

void CChatClientDlg::OnDestroy()
{
	if (m_bConnected)
	{
		m_pUDPClient->DisConnect();
	}
	CDialog::OnDestroy();
}

void CChatClientDlg::OnClose()
{
	if(MessageBox("ȷ��Ҫ�˳���","֪ͨ",MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
	{
		CDialog::OnClose();
	}
}

void CChatClientDlg::OnExit()
{
	PostMessage(WM_CLOSE,0,0);
}

void CChatClientDlg::OnShowip()
{
	PHOSTENT hostinfo;
	char hostname[255];
	CString str,str1,str2;
	LPSTR ip;
	if(gethostname(hostname, sizeof(hostname)) != 0)    //��ñ���������
	{
		MessageBox("��ȡ����������","����",MB_OK | MB_ICONWARNING);
		return;
	}
	if((hostinfo = gethostbyname(hostname)) == NULL)    //��ñ���IP��ַ
	{
		MessageBox("��ȡ����IP����","����",MB_OK | MB_ICONWARNING);
		return;
	}
	str.Format("��������%s\n",hostname);
	while(*(hostinfo->h_addr_list) != NULL)        //���IP��ַ
	{
		ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
		str1.Format("ip Ϊ : %s\n", ip);
		str2+=str1;
		hostinfo->h_addr_list++;
	}
	str+=str2;
	MessageBox(str,"IP��ַ",0);
}
