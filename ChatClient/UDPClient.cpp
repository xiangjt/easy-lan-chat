#include "StdAfx.h"
#include "UDPClient.h"
#include "ChatClientDlg.h"
#define MAX_LENGTH (8*1024)
#define PORT 6000

char IP[20] = {0};
MSG_INFO mi;
CChatClientDlg *m_pmainwnd;//主窗口
SOCKET m_sUDP;
char m_name[20];

CUDPClient::CUDPClient(CChatClientDlg *pmainwnd)
{
	m_pmainwnd = pmainwnd;
	m_sUDP = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
}

CUDPClient::~CUDPClient(void)
{
	closesocket(m_sUDP);
}

void CUDPClient::Connect(LPCTSTR name,LPCTSTR ip,int image)
{
	memset(&mi,0,sizeof(MSG_INFO));
	strcpy(IP,ip);
	strcpy(mi.m_From,name);
	strcpy(m_name,name);
	mi.m_Image = image;
	mi.m_Type = -1;//上线
	SendMsg(mi);
	CreateThread(NULL,0,CUDPClient::WorkThread,this,0,NULL);
}
void CUDPClient::DisConnect()
{
	memset(&mi,0,sizeof(MSG_INFO));
	strcpy(mi.m_From,m_name);
	mi.m_Type = -2;//
	SendMsg(mi);
}
//中间的函数
DWORD WINAPI CUDPClient::WorkThread(void* param)
{
	((CUDPClient*)param)->ReceiveProc();
	return 1;
}

//接收客户端数据
void CUDPClient::ReceiveProc()
{
	int count = 0;
	SOCKADDR_IN clientaddr;
	int len = sizeof(SOCKADDR);
	char buf[MAX_LENGTH];
	while(TRUE)
	{
		count = recvfrom(m_sUDP,buf,MAX_LENGTH,0,(SOCKADDR*)&clientaddr,&len);
		if(count != sizeof(MSG_INFO)) continue;
		else
			memcpy(&mi,buf,sizeof(MSG_INFO));
		switch(mi.m_Type)
		{
		case -1://上线
			OnLine();
			break;
		case -2://下线
			OffLine();
			break;
		case -3://重名
			MessageBox(NULL,"用户名已存在，请换一个！","提示",MB_OK|MB_ICONINFORMATION);
			break;
		case -4://服务器关闭
			ServerShutDown();
			break;
		default:
			if (mi.m_Type>=0)//正常的交谈
			{
				Talk();
			}
			else
				break;
		}
	}
}

//发送消息
void CUDPClient::SendMsg(MSG_INFO &msg_info)
{
	SOCKADDR_IN addr_in;
	addr_in.sin_addr.S_un.S_addr = inet_addr(IP);
	addr_in.sin_port = htons(PORT);
	addr_in.sin_family = AF_INET;
	int len = sizeof(SOCKADDR);
	len = sendto(m_sUDP,(char *)&msg_info,sizeof(MSG_INFO),0,(SOCKADDR*)&addr_in,len);
}

//上线
void CUDPClient::OnLine()
{
    m_pmainwnd->TextOut(mi.m_From, RGB(0,0,255));
	m_pmainwnd->TextOut(_TEXT("风尘仆仆地推门而入\r\n"),RGB(255,0,0));
	m_pmainwnd->m_bConnected = TRUE;
	m_pmainwnd->AddItemOfList(mi.m_Image,mi.m_From,mi.m_IP);
}


//下线
void CUDPClient::OffLine()
{
	if (strcmp(mi.m_From , m_name)==0)
	{
		m_pmainwnd->m_bConnected = FALSE;
		((CListCtrl *)m_pmainwnd->GetDlgItem(IDC_USERLIST))->DeleteAllItems();
	}
	else
	{
		m_pmainwnd->RemoveItemOfList(mi.m_From);
	}
	m_pmainwnd->TextOut(mi.m_From, RGB(0,0,255));
	m_pmainwnd->TextOut(_TEXT("静静地离开了，孤单的背影显得格外潇洒\r\n"),RGB(255,0,0));
}

//服务器关闭
void CUDPClient::ServerShutDown()
{
	m_pmainwnd->TextOut(_T("系统：服务器已关闭!"),RGB(0,0,0));
	m_pmainwnd->m_bConnected = FALSE;
	((CListCtrl *)m_pmainwnd->GetDlgItem(IDC_USERLIST))->DeleteAllItems();
}

//交谈
void CUDPClient::Talk()
{
	CString from = mi.m_From;
	CString to = mi.m_To;
	CString text = mi.m_Text;
	int type = mi.m_Type;
	COLORREF crf = mi.m_Color;

	if(from != m_name&&to != m_name && mi.m_Secret)
		return;//过滤悄悄话
	if(from != m_name&&to != m_name && m_pmainwnd->m_bFilter)
		return;//过滤不属于自己的消息
	CString temp,first,second;

	if(type > 32 || type < 0) 
		return;  
	temp.LoadString(IDS_TALK0 + type);	
	int i=temp.Find(",");
	if(i != -1){
		first = temp.Left(i);
		if(i != temp.GetLength() - 1){
			second = temp.Mid(i + 1);
			second += "：";
		}
		else{
			second="：";
		}
		if(to == "") to = "所有人";
		m_pmainwnd->TextOut((LPCTSTR)from, RGB(0,0,255));
		m_pmainwnd->TextOut((LPCTSTR)first, RGB(0,0,0));
		m_pmainwnd->TextOut((LPCTSTR)to, RGB(0,0,255));
		m_pmainwnd->TextOut((LPCTSTR)second, RGB(0,0,0));
		m_pmainwnd->TextOut((LPCTSTR)text,crf);
		m_pmainwnd->TextOut((LPCTSTR)"\r\n",crf);
	}
	else{
		first=temp;
		second="： ";
		m_pmainwnd->TextOut(from,RGB(0,0,255));
		m_pmainwnd->TextOut(first,RGB(0,0,0));
		m_pmainwnd->TextOut(second,RGB(0,0,0));
		m_pmainwnd->TextOut(text,crf);
		m_pmainwnd->TextOut("\r\n",crf);
	}
}