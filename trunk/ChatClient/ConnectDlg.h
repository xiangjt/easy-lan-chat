#pragma once


// CConnectDlg �Ի���

class CConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConnectDlg();

// �Ի�������
	enum { IDD = IDD_CONNECTSET };
public://���ڷ���
	CString m_strName;//�û���
	CString m_strIP;//�������˵�IP
	int m_image;//ѡ���ͼ��

private://�ڲ�ʹ��
	CImageList m_imgList;//ͼ���б�
	CComboBoxEx m_comboType;//Ҫ������ͼ��ѡ���
	CArray<LPSTR,LPSTR> m_strArray;
	BOOL m_bIsChange;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnSetfocusName();
};
