
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int counti = 0;
// CAboutDlg dialog used for App About
  int CClientDlg::SizeFileName=0;
  int CClientDlg::SizeDownLoad=0;
  int CClientDlg::bytes_read=0;
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


// CClientDlg dialog



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	,m_msgString(_T(""))
	,IP(_T(""))
	,m_userName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOXCHAT, m_chatbox);
	DDX_Text(pDX, IDC_BOXCHAT, m_msgString);
	DDX_Text(pDX, IDC_IP, IP);
	DDX_Text(pDX, IDC_USER, m_userName);

}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, Mess)
	ON_BN_CLICKED(IDC_LOGOUT, &CClientDlg::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_LOGIN, &CClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_SEND, &CClientDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_DOWNLOAD, &CClientDlg::OnBnClickedDownload)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	IP = "127.0.0.1";
	GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
	listbox =(CListBox*) GetDlgItem(IDC_ListFile);
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CClientDlg::Split(CString src, CString des[2])
{
	int p1, p2;

	p1 = src.Find(syntaxpos, 0);
	des[0] = src.Mid(0, p1);
	p2 = src.Find(syntaxfix, p1 + 1);
	des[1] = src.Mid(p1 + 4, p2 - (p1 + 4));

}
void CClientDlg::UpdateListFileName(int size)
{
	CString mess;
	for (int i = 0; i < size; i++)
	{
		mRecv(mess);
		listfilename.push_back(mess);
	}
}
// Read file with directice
void CClientDlg::ReadListFile(string filedirect)
{
	experimental::filesystem::path p(filedirect.c_str());

	for (auto i = directory_iterator(p); i != directory_iterator(); i++)
	{
		if (!is_directory(i->path()))
			listfilename.push_back(CString(i->path().filename().string().c_str()));
	}
}
char* CClientDlg::ConvertToChar(const CString &s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}
string CClientDlg::ConvertCstringtoString(CString Command)
{
	CT2CA pszConvertedAnsiString(Command);
	string message(pszConvertedAnsiString);
	return message;
}
void CClientDlg::mSend(CString Command)
{
	//Send Syntax
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[2000];
	memset(sendBuff, 0, 2000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sClient, (char*)&Len, sizeof(Len), 0);
	send(sClient, (char*)sendBuff, Len, 0);
}
int CClientDlg::mRecv(CString &Command)
{
	PBYTE buffer = new BYTE[2000];
	memset(buffer, 0, 2000);
	recv(sClient, (char*)&buffLength, sizeof(int), 0);
	recv(sClient, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if (Command.GetLength() == 0)
		return -1;
	return 0;
}
void CClientDlg::DeleteDataListBox()
{
	int numOfItems = listbox->GetCount();
	for (int i = 0; i <= numOfItems; i++) {
		listbox->DeleteString(i);
	}
}
LRESULT CClientDlg::Mess(WPARAM wParam, LPARAM lParam)
{
	if (SizeFileName > 0)
	{
		RecvFileName = TRUE;
	}
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		if (FlagDownload)
		{
			int flagrun = TRUE;
			char  filebuf[FILE_CHUNK_SPEED];
		 	if (SizeDownLoad - FILE_CHUNK_SPEED < 0)
			{
				 
				bytes_read = recv(sClient, filebuf, SizeDownLoad, 0);
			}
				// Read data into buffer.  We may not have enough to fill up buffer, so we
				// store how many bytes were actually read in bytes_read.
			else  	bytes_read = recv(sClient, filebuf, FILE_CHUNK_SPEED, 0);
				if (bytes_read == 0)
				{
					// We're done reading from the file
					FlagDownload = FALSE;
					flagrun = FALSE;
					infile.close();
					bytes_read = 0;
					CString Message = _T("5") + syntaxpos + m_userName + _T(": download thanh cong") + syntaxfix;
					mSend(Message);
					m_msgString +=  _T(": Download thanh cong");
					m_msgString += "\r\n";
					UpdateData(FALSE);
					break;
				}
				if(flagrun) infile.write(filebuf, bytes_read);

				if (bytes_read <= 0) {
					// handle errors
					 
				}
				SizeDownLoad -= bytes_read;
				if (SizeDownLoad <= 0)
				{
					FlagDownload = FALSE;
					flagrun = FALSE;
					infile.close();
					bytes_read = 0;
					CString Message = _T("5") + syntaxpos + m_userName + _T(" download thanh cong") + syntaxfix;
					mSend(Message);
					GetDlgItem(IDC_DOWNLOAD)->EnableWindow(TRUE);
					break;
				}
	
		}
		else
		{
		CString temp;
		if (mRecv(temp) < 0)
			break;
	 	
		Split(temp, strResult);
		int flag1 = _ttoi(strResult[0]);
		int flag2 = _ttoi(strResult[1]);
		switch (flag1)
		{
		case LOGIN:
		{
			if (flag2 == 1)
			{
				m_msgString += _T("Dang nhap thanh cong\r\n");
				GetDlgItem(IDC_LOGOUT)->EnableWindow(TRUE);
				GetDlgItem(IDC_LOGIN)->EnableWindow(FALSE);
				GetDlgItem(IDC_USER)->EnableWindow(FALSE);
				GetDlgItem(IDC_PASS)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP)->EnableWindow(FALSE);
			}
			else
				m_msgString += _T("Dang nhap that bai. Username da duoc su dung truoc do.\r\n");

			UpdateData(FALSE);
			break;
		}

		case MESSAGE:
		{
			m_msgString += strResult[1];
			m_msgString += "\r\n";
			UpdateData(FALSE);
			break;
		}
		case UPDATELIST:
		{
			listfilename.clear();
			DeleteDataListBox();
			SizeFileName = flag2;
			break;
		}
		case DOWNLOAD:
		{
			FlagDownload = TRUE;
			SizeDownLoad = flag2;
			GetDlgItem(IDC_DOWNLOAD)->EnableWindow(FALSE);
			break;
		}
		}
		if (SizeFileName > 0 && RecvFileName)
		{
			listfilename.push_back(temp);

			SizeFileName--;
			if (SizeFileName == 0)
			{
				RecvFileName = FALSE;
				if (listfilename.size() > 1)
				{
					UpDateListBoxClient();
				}
			}
		}
		}
		break;
	}
	case FD_CLOSE:
	{
		infile.close();
		closesocket(sClient);
		m_msgString += _T("Server da dong ket noi\r\n");
		GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LOGIN)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	}

	}
	return 0;
}


void CClientDlg::OnBnClickedLogout()
{
	INT_PTR i = MessageBox(_T("Ban muon logout?"), _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;
	infile.close();
	Command = _T("3") + syntaxpos;
	mSend(Command);
	GetDlgItem(IDC_USER)->EnableWindow(TRUE);
	GetDlgItem(IDC_PASS)->EnableWindow(TRUE);
	GetDlgItem(IDC_IP)->EnableWindow(TRUE);
}


void CClientDlg::OnBnClickedLogin()
{
	UpdateData(TRUE);
	if (m_userName == "")
	{
		MessageBox(_T("Vui long nhap ten user"));
		return;
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		MessageBox(_T("Error code. Build failed"), _T("ERROR"), 0);
	}
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	hostent* host = NULL;
	if (sClient == INVALID_SOCKET)
	{
		MessageBox(_T("socket() failed"), _T("ERROR"), 0);
		return;
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_port = htons(PORT);

	char* cIP = ConvertToChar(IP);

	servAdd.sin_addr.s_addr = inet_addr(cIP);
	CStringA cpy_IP(IP);

	if (servAdd.sin_addr.s_addr == INADDR_NONE)
	{
		host = (gethostbyname(cpy_IP));
		if (host == NULL)
		{
			MessageBox(_T("Khong the ket noi den server."), _T("ERROR"), 0);
		}
	 	CopyMemory(&servAdd.sin_addr, host->h_addr_list[0],
	 		host->h_length);
		return;
	}
	if (connect(sClient, (struct sockaddr*)&servAdd, sizeof(servAdd))<0) {
		MessageBox(_T("Ket noi that bai"), _T("ERROR"), 0);
		return;
	}

	Command = _T("1") + syntaxpos;
	Command += m_userName + syntaxfix;
	mSend(Command);
//	listen(sClient, 5);
	WSAAsyncSelect(sClient, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	UpdateData(FALSE);
}
void CClientDlg::UpDateListBoxClient() 
{
	for (int i = listfilename.size()-1; i>=1; i--)
	{
		if (listbox->FindStringExact(0, listfilename[i]) == LB_ERR)
		{
			listbox->AddString(listfilename[i]);
			 
		}
	}
}

void CClientDlg::OnBnClickedSend()
{
	CString sTextData;
	GetDlgItemText(IDC_MESS, sTextData);
	Command = _T("2") + syntaxpos+sTextData+ syntaxfix;
	if (sClient != NULL)
	{

		mSend(Command);
	}

	CWnd* pWnd = GetDlgItem(IDC_MESS);
	pWnd->SetWindowText(_T(""));

}


void CClientDlg::OnBnClickedDownload()
{
	 
	INT_PTR i = MessageBox(_T("Xac nhan tai file? "), _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;
	CString messdownload(_T(""));
	messdownload += _T("4") + syntaxpos + m_userName + syntaxdownload;
	int index = listbox->GetCurSel();
		 if (index != LB_ERR)
		 {
			 CString namefile;
			 listbox->GetText(index, namefile);
			 if (listfilename.size() == 2)
			 {
				 messdownload += listfilename[0];
				
			 }
			 else
			 {
				
				 messdownload += listfilename[0] + _T("\\") + namefile;  
				
			 }
			 
			 PathDownLoad = ConvertCstringtoString(namefile);
			 infile.open(PathDownLoad, ios::binary);
			 m_msgString += m_userName + _T(":Tien hanh download ") + namefile;
			 m_msgString += "\r\n";
			 UpdateData(FALSE);
			 messdownload += syntaxfix;
			 mSend(messdownload);
		 }
}
