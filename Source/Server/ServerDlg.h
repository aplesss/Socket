
// ServerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "stdlib.h"
#include "time.h"
#include<iostream>
#include<vector>
#include<fstream>
using namespace std;
#include<afxsock.h>

const CString syntaxpos(_T("\r\n*."));
const CString syntaxfix(_T("\r\n.*"));
const CString syntaxdownload(_T("|:::|"));
const int FILE_CHUNK_SPEED = 10000;
#define PORT 8808
#define WM_SOCKET WM_USER+5
#define LOGIN 1
#define MESSAGE 2
#define LOGOUT 3
#define CLIENTDOWNLOAD 4
#define SUCESSDOWNLOAD 5
// CServerDlg dialog
 
class CServerDlg : public CDialogEx
{
// Construction
public:
	CServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CListBox* listbox;
	vector<CString> listfilename;
	// Generated message map functions
	WSADATA wsaData;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	LRESULT Mess(WPARAM wParam, LPARAM lParam);
	char* ConvertToChar(const CString &s);
	void Split(CString src, CString des[2]);
	void GetUserDownLoad(CString src, CString& user, CString& Path);
	static void mSend(SOCKET sk, CString Command);
	int mRecv(SOCKET sk, CString &Command);
	WSADATA wsa;
	struct SockName
	{
		bool ProcessDownload = FALSE;
		SOCKET sockClient = INVALID_SOCKET;
		bool Login = FALSE;
		HANDLE hThread;
		char Name[200];
		vector <CString> queueMess;
	};
	SOCKET sockServer, sockClient, flag, sclient;
	struct sockaddr_in serverAdd;
	int msgType;
	int buffLength, t, lenguser, flagsend, kq, count_sock;
	int number_Socket;
	int first_send;
	SockName *pSock;
	CString strResult[2];
	CString Command;
	CString m_msgString;
public:
		string ConverttoString(CString);
		void DeleteDataListBox();
		void SendAllListFile();
		void UpDateListBox();
		static UINT _cdecl ThreadDownLoad(LPVOID param);
		static string currentPath;
		afx_msg void OnBnClickedStart();
		afx_msg void OnBnClickedCancel();
		afx_msg void OnBnClickedChoosefile();
};
