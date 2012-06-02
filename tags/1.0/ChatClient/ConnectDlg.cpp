// ConnectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ConnectDlg.h"


// CConnectDlg �Ի���

IMPLEMENT_DYNAMIC(CConnectDlg, CDialog)

CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
{
	m_strName = "";
	m_strIP = "";
	m_image = 0;
}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CConnectDlg::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_NAME, &CConnectDlg::OnEnSetfocusName)
END_MESSAGE_MAP()


// CConnectDlg ��Ϣ�������

void CConnectDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NAME);
	pEdit->GetWindowText(m_strName);
	m_image = m_comboType.GetCurSel();
	((CIPAddressCtrl*)GetDlgItem(IDC_IP))->GetWindowText(m_strIP);

	if (m_strName == "��������!" ||m_strName == "")
	{
		MessageBox("δ��д�û�����\n��������д!","����");
		return;
	}
	if (m_strName.GetLength()>=20)
	{
		MessageBox("�û������ܳ���20���ַ���\n��������д!","����");
		pEdit->SetWindowText("");
		return;
	}
	OnOK();
}

BOOL CConnectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//��ʼ��ͼ���б�
	RECT rc;
	COMBOBOXEXITEM ci;
	SetRect(&rc,134,74,219,180);
	m_imgList.Create(IDB_IMAGE, 16, 1, RGB(0, 255, 0));
	HIMAGELIST him = m_imgList.m_hImageList;
	m_comboType.Create(CBS_DROPDOWNLIST|WS_CHILD|CBS_AUTOHSCROLL|WS_VISIBLE,rc,this,IDC_TYPE);
	m_comboType.SetImageList(&m_imgList);
	ImageList_SetBkColor(him,CLR_NONE);

	//��m_strArray����ַ���
	m_strArray.Add("�к�1");
	m_strArray.Add("Ů��");
	m_strArray.Add("�к�2");
	m_strArray.Add("�����");
	m_strArray.Add("��");
	m_strArray.Add("��");
	m_strArray.Add("���");

	for (int i =0;i<7;i++)//���ͼ��
	{
		ci.mask = CBEIF_IMAGE|CBEIF_TEXT|CBEIF_SELECTEDIMAGE;
		ci.iImage = i;
		ci.iSelectedImage =i;
		ci.pszText = m_strArray.GetAt(i);
		ci.iItem = i;
		m_comboType.InsertItem(&ci);
	}

	m_comboType.SetCurSel(0);

	((CEdit *)GetDlgItem(IDC_NAME))->SetWindowText("��������!");
	m_bIsChange = false;
	((CIPAddressCtrl*)GetDlgItem(IDC_IP))->SetWindowText("127.0.0.1");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CConnectDlg::OnEnSetfocusName()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bIsChange)
	{
		((CEdit *)GetDlgItem(IDC_NAME))->SetWindowText("");
		m_bIsChange = true;
	}
}
