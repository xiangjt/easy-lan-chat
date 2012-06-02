// ChatClientDlg.h : ͷ�ļ�
//

#pragma once


// CChatClientDlg �Ի���
#include "Message.h"
#include "resource.h"
class CUDPClient;

class CChatClientDlg : public CDialog
{
// ����
public:
	CChatClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHATCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

//LIST�ؼ�
public:
	CImageList m_imgList;//ͼ���б���ʾ�����û���ͼ��
	void RemoveItemOfList(char* name);//�Ƴ�
	void AddItemOfList(short i, char* name, char* text);//���
	void AddExStyleOfList(DWORD dwNewStyle);//��USERLIST��ӷ��
	void InitList();//��ʼ��USERLIST
	void InitType();//��ʼ��TYPE COMBO�ؼ�
public:
//���ݵĲ��������ͱ���
	CHARFORMAT cfm;//���ڸ�RICHEDIT��������
	void DisplayMsg(LPCTSTR lpszText);
	void TextOut(LPCTSTR lpszMessage, COLORREF clr);
// ʵ��
protected:
	HICON m_hIcon;
	//
public:
	COLORREF crf;//������ɫ
    BOOL m_bConnected;//�Ƿ�����
	BOOL m_bFilter;
	CUDPClient *m_pUDPClient;
	CString m_strName;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkUserlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnBnClickedSend();
	afx_msg void OnConnect();
	afx_msg void OnBnClickedFilter();//ֱ�Ӹı䱾�ر�������QUIET���Ƿ��͵�ʱ��ı�
	afx_msg void OnDisconnect();
	afx_msg void OnBnClickedColorselect();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnDestroy();
	afx_msg void OnExit();
	afx_msg void OnShowip();
};
