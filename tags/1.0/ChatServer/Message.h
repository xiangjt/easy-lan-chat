#pragma once

struct MSG_INFO
{
	int m_Type;//����
	char m_From[20];//����������
	char m_To[20];//����������
	COLORREF m_Color;//��Ϣ����ɫ
	BOOL m_Secret;//�Ƿ����Ļ�
	int m_Image;//�û�ͷ��
	char m_IP[20];//�û�IP�����ߵ�ʱ���õ�
	char m_Text[1000];
};

struct USER_INFO
{
	char m_User[20];
	char m_IP[20];
	int m_Image;
	SOCKADDR_IN m_SA;
};

//type�ĵĺ���
/*
>=0		����̸��
-1		����
-2		����
-3		����
-4    ϵͳ�ر�
*/
