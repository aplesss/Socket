
// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About
string CServerDlg::currentPath = "";
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CServerDlg dialog



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent), m_msgString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INFO, m_msgString);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, Mess)
	ON_BN_CLICKED(IDC_START, &CServerDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_CANCEL, &CServerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ChooseFile, &CServerDlg::OnBnClickedChoosefile)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	listbox = (CListBox*)GetDlgItem(IDC_LISTFILE);
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// Tach chuoi nhan thanh cac ki hieu nhan biet
void CServerDlg::Split(CString src, CString des[2])
{
	int p1, p2;

	p1 = src.Find(syntaxpos, 0);
	des[0] = src.Mid(0, p1);
	p2 = src.Find(syntaxfix, p1 + 1);
	des[1] = src.Mid(p1 + 4, p2 - (p1 + 4));

}
char* CServerDlg::ConvertToChar(const CString &s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}
void CServerDlg::mSend(SOCKET sk, CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[2000];
	memset(sendBuff, 0,2000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sk, (char*)&Len, sizeof(Len), 0);
	send(sk, (char*)sendBuff, Len, 0);
	delete sendBuff;
}
int CServerDlg::mRecv(SOCKET sk, CString &Command)
{
	PBYTE buffer = new BYTE[2000];
	memset(buffer, 0, 2000);
	recv(sk, (char*)&buffLength, sizeof(int), 0);
	recv(sk, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if (Command.GetLength() == 0)
		return -1;
	return 0;

}


void CServerDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	// NOTE Code 
	CString testFile;
	GetDlgItem(IDC_Direct)->GetWindowText(testFile);
	if (testFile == _T(""))
	{
		MessageBox(_T("Vui long chon noi luu tru file"));
	}
	else
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			MessageBox(_T("Error code. Build failed"), _T("ERROR"), 0);
		}
		if ((sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		{
			MessageBox(_T("Cant call WSAAsyncSelect"));

		}
		serverAdd.sin_family = AF_INET;
		serverAdd.sin_port = htons(PORT);
		serverAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	
		bind(sockServer, (SOCKADDR*)&serverAdd, sizeof(serverAdd));
		listen(sockServer, 5);
		int err = WSAAsyncSelect(sockServer, m_hWnd, WM_SOCKET, FD_READ | FD_ACCEPT | FD_CLOSE);
		if (err)
			MessageBox(_T("Cant call WSAAsyncSelect"));
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
		number_Socket = 0;
		pSock = new SockName[200];
		srand((unsigned)time(NULL));
		m_msgString += _T("Waiting Client Connect...\r\n");
		UpdateData(FALSE);
	}
}
LRESULT CServerDlg::Mess(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		pSock[number_Socket].sockClient = accept(wParam, NULL, NULL);
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
		break;
	}
	case FD_READ:
	{

		int post = -1, dpos = -1;

		for (int i = 0; i < number_Socket; i++)
		{
			if (pSock[i].sockClient == wParam)
			{
				if (i < number_Socket)
					post = i;
			}
		}

		CString temp;
		if (mRecv(wParam, temp) < 0)
			break;
		Split(temp, strResult);
		int flag = _ttoi(strResult[0]);
		char* tem = ConvertToChar(strResult[1]);
		switch (flag)
		{
		case LOGIN://Login
		{
			int t = 0;
			if (number_Socket > 0)
			{
				for (int i = 0; i < number_Socket; i++)
				{
					if (strcmp(tem, pSock[i].Name) == 0)//Trung ten user
					{
						t = 1;
						break;
					}
				}
			}
			bool  SuccessLogin = FALSE;
			if (t == 0)
			{
				strcpy(pSock[number_Socket].Name, tem);
				pSock[number_Socket].Login = TRUE;
				SuccessLogin = TRUE;
				UpdateData(FALSE);
				// Dang nhap thang cong
				CString MessLogin;
				MessLogin = _T("2") + syntaxpos;
				MessLogin += CString(tem)  + _T(": Login");
				MessLogin += syntaxfix;
				for (int i = 0; i < number_Socket; i++)
				{
					if (!pSock[i].ProcessDownload)
					{
						mSend(pSock[i].sockClient, MessLogin);
					}
					else
					{
						pSock[i].queueMess.push_back(MessLogin);
					}
				}
				m_msgString += tem;
				m_msgString += " Login\r\n";
				UpdateData(FALSE);
				number_Socket++;
				Command = _T("1\r\n*.1\r\n.*");
				
			}
			else Command = _T("1\r\n*.0\r\n.*");
			mSend(wParam, Command);
			// List File DownLoad
			if (listfilename.size() > 1 && SuccessLogin)
			{
				SuccessLogin = FALSE;
				CString str;
				str.Format(_T("%d"), listfilename.size());
				CString mess = _T("3") + syntaxpos + str + syntaxfix;
				mSend(wParam, mess);
				for (int i = 0; i < listfilename.size(); i++)
				{
				 
					mSend(wParam, listfilename[i]);
				
				}

			}
		
			

			UpdateData(FALSE);
			break;
		}
		// Gui thong diep trong messbox
		case MESSAGE:
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}
			CString mess(_T(""));
			mess += pSock[post].Name;
			mess+=": ";
			mess += tem;
			m_msgString += mess;
			m_msgString += "\r\n";
			Command = _T("2") + syntaxpos;
			Command += mess;
			Command += syntaxfix;
			for (int i = 0; i < number_Socket; i++)
			{
				if (!pSock[i].ProcessDownload)
				{
					mSend(pSock[i].sockClient, Command);
				}
				else
				{
					pSock[i].queueMess.push_back(Command);
				}
			}
			UpdateData(FALSE);
			break;
		}
		case LOGOUT:
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}

			CString mess(_T(""));
			mess += pSock[post].Name;
			mess += ": Logout";
			
			m_msgString += pSock[post].Name;
			m_msgString += ": Logout\r\n";
			closesocket(wParam);
			for (int j = post; j < number_Socket; j++)
			{
				pSock[post].sockClient = pSock[post + 1].sockClient;
				strcpy(pSock[post].Name, pSock[post + 1].Name);
			}
			number_Socket--;
			Command = _T("2") + syntaxpos;
			Command += mess;
			Command += syntaxfix;
			for (int i = 0; i < number_Socket; i++)
			{
				if (!pSock[i].ProcessDownload)
				{
					mSend(pSock[i].sockClient, Command);
				}
				else
				{
					pSock[i].queueMess.push_back(Command);
				}
			}
			UpdateData(FALSE);
			break;
		}
		case CLIENTDOWNLOAD:
		{
			CString UserDonwload;
			CString Path;
			GetUserDownLoad(CString(strResult[1]), UserDonwload, Path);
			char *flag = ConvertToChar(UserDonwload);
			m_msgString += UserDonwload + _T(": tien hanh download.\r\n ");
			UpdateData(FALSE);
			int index = 0;
			if (number_Socket > 0)
			{
				for (int i = 0; i < number_Socket; i++)
				{
					if (strcmp(flag, pSock[i].Name) == 0)
					{
						index = i;
						break;
					}
				}
			}
			pSock[index].ProcessDownload = TRUE;
			// Chuyen thanh dang duong dan chuan trong C,C++
			string temppath;
			temppath = ConverttoString(Path);
			currentPath = "";
			for (int i = 0; i < temppath.size(); i++)
			{
				if (temppath[i] == '\\')
				{
					currentPath += '//';
				}
				else currentPath += temppath[i];

			}

			ifstream file(currentPath, ios::binary);
			file.seekg(0, ios::end);
			int sizes = file.tellg();
			file.close();
			CString strsize;
			strsize.Format(_T("%d"), sizes);
			CString messtrans = _T("4") + syntaxpos + strsize + syntaxfix;
			mSend(pSock[index].sockClient, messtrans);
			CWinThread *cThread = AfxBeginThread(ThreadDownLoad,
				(LPVOID)pSock[index].sockClient);
			cThread->SetThreadPriority(1);
			break;
		}
		case SUCESSDOWNLOAD:
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
					break;
				}
			}
			CString mess(_T(""));
			mess += tem;
			m_msgString += mess;
			m_msgString += "\r\n";
			pSock[post].ProcessDownload = FALSE;
			for (CString i : pSock[post].queueMess)
			{
				mSend(pSock[post].sockClient, i);
				Sleep(10);
			}
			pSock[post].queueMess.clear();
			UpdateData(FALSE);
			break;
		}
		}

		break;
	}

	case FD_CLOSE:
	{
		UpdateData();
		int post = -1;
		for (int i = 0; i < number_Socket; i++)
		{
			if (pSock[i].sockClient == wParam)
			{
				if (i < number_Socket)
					post = i;
			}
		}

		m_msgString += pSock[post].Name;
		m_msgString += " logout\r\n";
		CString mess(_T(""));
		mess += pSock[post].Name;
		mess += ": logout";
		closesocket(wParam);
		for (int j = post; j < number_Socket; j++)
		{
			pSock[post].sockClient = pSock[post + 1].sockClient;
			strcpy(pSock[post].Name, pSock[post + 1].Name);
		}
		number_Socket--;
		Command = _T("2") + syntaxpos;
		Command += mess;
		Command += syntaxfix;
		for (int i = 0; i < number_Socket; i++)
		{
			if (!pSock[i].ProcessDownload)
			{
				mSend(pSock[i].sockClient, Command);
			}
			else
			{
				pSock[i].queueMess.push_back(Command);
			}
		}
		UpdateData(FALSE);
		break;
	}

	}
	return 0;
}
void CServerDlg::UpDateListBox()
{
	for (int i = 1; i < listfilename.size(); i++)
	{
		if (listbox->FindStringExact(0, listfilename[i]) == LB_ERR)
		{
			listbox->AddString(listfilename[i]);

		}
	}
}
string CServerDlg::ConverttoString(CString Command)
{
	CT2CA pszConvertedAnsiString(Command);
	string message(pszConvertedAnsiString);
	return message;
}

/// FUNCTION THREAD DOWNLOAD
UINT __cdecl CServerDlg::ThreadDownLoad(LPVOID pParam)
{
	SOCKET socket = reinterpret_cast<SOCKET>(pParam);
	char name[1024];
	char FileName[1024];
	string PathDownload = currentPath;
	int result = 0;
	ifstream file(PathDownload, ios::binary);
	file.seekg(0, ios::end);
	 unsigned int size = file.tellg();
	file.close();
	char *fileBuffer = new char[size];
	file.open(PathDownload, ios::binary);
	file.seekg(0, ios::beg);
	file.read(fileBuffer, size);
	file.close();
	unsigned int bytesSent = 0;
	int bytesToSend = 0;
	int sizes = size;
	while (bytesSent < sizes)
	{
		if (sizes - bytesSent >= FILE_CHUNK_SPEED)
		{
			bytesToSend = FILE_CHUNK_SPEED;
		}
		else
		{
			bytesToSend = sizes - bytesSent;
		}
		send(socket, fileBuffer + bytesSent, bytesToSend, 0);
		Sleep(10);
		bytesSent += bytesToSend;
	}

	delete[] fileBuffer;
	return 0;
}

void CServerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
// Xoa list download hien co 
void CServerDlg::DeleteDataListBox()
{
   int 	numOfItems = listbox->GetCount();
	for (int i = 0; i <= numOfItems; i++) {
		listbox->DeleteString(i);
	}
}
// Gui di danh sach file co the download cho tat ca client 
void CServerDlg::SendAllListFile()
{
	CString str;
	str.Format(_T("%d"), listfilename.size());
	Command = _T("3") + syntaxpos+ str+ syntaxfix;

	for (int i = 0; i < number_Socket; i++)
	{
		mSend(pSock[i].sockClient, Command);
		for (int i = 0; i < listfilename.size(); i++)
		{
			mSend(pSock[i].sockClient, listfilename[i]);
		}
	}
}
// Lay user gui lenh download 
void CServerDlg::GetUserDownLoad(CString src, CString& user, CString& Path)
{
	int p1, p2;

	p1 = src.Find(syntaxdownload, 0);
	user = src.Mid(0, p1);
	Path = src.Mid(p1 +  syntaxdownload.GetLength(), src.GetLength());
}
void CServerDlg::OnBnClickedChoosefile()
{
	MessageBox(_T("Click vao cac file can tai"));

	// File Browser 
	//int nLnBuff = 32767;
	int nLnBuff = 33000;
	CString filefull;
	CString filename;
	wchar_t* p = filename.GetBuffer(nLnBuff);
	TCHAR file[] = _T("Text file (*.txt)|*.txt|Rar file(*.rar)|*.rar|All Files (*.*)|*.*||");
	CFileDialog openfile(TRUE, _T("rar"), _T("*.rar"), OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, file);
	OPENFILENAME& ofn = openfile.GetOFN();
	ofn.Flags |= OFN_ALLOWMULTISELECT;// cho phep select multi file
	ofn.lpstrFile = p;
	if (openfile.DoModal() == IDOK)
	{
		 
		listfilename.clear();
		DeleteDataListBox();
		listfilename.push_back(CString(openfile.GetPathName()));
		wchar_t * pBuffend = p + nLnBuff - 2;
		wchar_t* start = p;
		int fileCount = 1;
		while ((p < pBuffend) && (*p)) p++;
		if (p > start)
		{
			p++;
			
			while ((p < pBuffend) && (*p))
			{
				start = p;
				while ((p < pBuffend) && (*p))p++;
				if (p > start) filefull = openfile.GetFileName();
				filefull.Append(_T("\\"));
				 filefull.Append(openfile.GetPathName());
		 		 filefull .Append(start);
				 if (start != _T("")) listfilename.push_back(CString(start));
				p++;
				fileCount++;
			}
		}
		if (fileCount == 1)
		{
			CString test;
			// Truong hop chi chon 1 file duong dan chua ca ten file va duong dan
			for (int i = listfilename[0].GetLength() - 1; i >= 0; i--)
			{
				if (listfilename[0][i] == _T('\\'))
				{
					test =  (listfilename[0].Mid(i + 1, listfilename[0].GetLength() - 1));
					break;
				}
			}
			listfilename.push_back(test);
		}

		if (listfilename.size() > 1)
		{
			GetDlgItem(IDC_Direct)->SetWindowText(listfilename[0]);
			UpDateListBox();
			GetDlgItem(IDC_ChooseFile)->EnableWindow(FALSE);
		}
		
	}

}
