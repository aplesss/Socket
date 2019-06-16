
// ClientDlg.h : header file
//

#pragma once
#include<afxsock.h>
#include <iostream>
#include<string>
#include<vector>
#include<fstream>
#include<filesystem>
using namespace std;
using namespace std::experimental::filesystem;

#define PORT 8808
#define WM_SOCKET WM_USER+5
const CString syntaxpos(_T("\r\n*."));
const CString syntaxfix(_T("\r\n.*"));
const CString syntaxdownload (_T("|:::|"));
const int FILE_CHUNK_SPEED = 10000;
#define LOGIN 1
#define MESSAGE 2
#define UPDATELIST 3
#define DOWNLOAD 4
 
// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	SOCKET sClient;
	sockaddr_in servAdd;
	CString Command;
	int	buffLength;
	CString strResult[2];
	CString userOnline;
	int level;
	int isLogon;
	WSADATA wsa;
	CString m_msgString;
	CString IP;
	CString m_userName;
	vector<CString> listfilename;
	CListBox*  listbox;
// Implementation
protected:
	HICON m_hIcon;
	ofstream infile;
	bool RecvFileName = FALSE;
	static int SizeFileName;
	static int SizeDownLoad;
	static int bytes_read;
	string PathDownLoad = "";
	bool FlagDownload = FALSE;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void ReadListFile(string);
	LRESULT Mess(WPARAM wParam, LPARAM lParam);
	char* ConvertToChar(const CString &s);
	void Split(CString src, CString des[2]);
	void mSend(CString Command);
	int mRecv(CString &Command);
	string ConvertCstringtoString(CString);
	void UpdateListFileName(int);
	void UpDateListBoxClient();
	CEdit m_chatbox;
	void DeleteDataListBox();
	afx_msg void OnBnClickedLogout();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedDownload();
};
