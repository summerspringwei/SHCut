
// SHCut2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SHCut2.h"
#include "SHCut2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSHCut2Dlg 对话框



CSHCut2Dlg::CSHCut2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSHCut2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSHCut2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSHCut2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SHCUT, &CSHCut2Dlg::OnBnClickedButtonShcut)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSHCut2Dlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CSHCut2Dlg 消息处理程序

BOOL CSHCut2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	firstClick = true;
	stopDraw = false;
	startDrawRect = false;
	count = 0;



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSHCut2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSHCut2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSHCut2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//左键双击绘制多边形和矩形
void CSHCut2Dlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//绘制多边形
	if (!stopDraw)//如果还没有绘制多边形
	{
		CClientDC dc(this);//画笔
		CPen pen(0, 2, RGB(255, 0, 0));//设置画笔和画笔颜色
		dc.SelectObject(&pen);


		if (firstClick)//第一次点击没有画线
		{
			poly[numOfPoint] = point;
			dc.Ellipse(point.x, point.y, point.x + 4, point.y + 4);
			firstClick = false;
			numOfPoint++;
		}
		else//后面每点击一次画一条线
		{
			dc.MoveTo(poly[numOfPoint - 1]);
			dc.LineTo(point);
			poly[numOfPoint] = point;
			numOfPoint++;
		}
	}

	//绘制裁剪窗口
	if (startDrawRect)
	{
		if (count == 0)
		{
			rect_topLeft = point;//记录左上角顶点坐标
		}

		else if (count == 1)
		{
			rect_bottomRight = point;//记录右下角顶点坐标
			CClientDC dc(this);
			CPen pen(0, 2, RGB(0, 255, 0));

			rect = new CRect(rect_topLeft, rect_bottomRight);
			dc.SelectObject(&pen);

			rect_topRight.x = rect_bottomRight.x;//右上点
			rect_topRight.y = rect_topLeft.y;
			
			rect_bottomLeft.x = rect_topLeft.x;//左下点
			rect_bottomLeft.y = rect_bottomRight.y;

			//绘制矩形裁剪窗口
			dc.MoveTo(rect_topLeft);
			dc.LineTo(rect_topRight);
			dc.LineTo(rect_bottomRight);
			dc.LineTo(rect_bottomLeft);
			dc.LineTo(rect_topLeft);
			
		}
		count++;

		if (count > 1)//记录下已经绘制完矩形，返回
		{
			startDrawRect = false;
			return;
		}
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

//右键单击消息响应函数
void CSHCut2Dlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (numOfPoint < 3)//
	{
		MessageBox(_T("请双击鼠标左键输入点！"));
		return;
	}

	stopDraw = true;
	startDrawRect = true;
	CClientDC dc(this);
	CPen pen(0, 2, RGB(255, 0, 0));
	dc.SelectObject(&pen);

	//绘制最后一条边
	dc.MoveTo(poly[numOfPoint - 1]);
	dc.LineTo(poly[0]);
	CDialogEx::OnRButtonDblClk(nFlags, point);
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CSHCut2Dlg::OnBnClickedButtonShcut()
{
	// TODO:  在此添加控件通知处理程序代码
	POINT p1, p2, p3, p4, p;
	//左裁剪
	p3 = rect_topLeft;
	p4 = rect_bottomLeft;
	noCutP = 0;
	for (int i = 0; i < numOfPoint; i++)
	{
		//设置起始线段
		if (i < numOfPoint - 1)
		{
			p1 = poly[i];
			p2 = poly[i + 1];
		}
		else if (i == numOfPoint-1)
		{
			p1 = poly[i];
			p2 = poly[0];
		}

		//判断垂直
		if (p1.x == p2.x)
		{
			if (p1.x>p3.x)
			cut1[noCutP++] = p2;//把交点加到队列中
		}
		else//不垂直就可以通过斜率计算延长线交点
		{
			int x = p3.x;
			int y = p2.y - (double)(p2.y - p1.y)*(double)(p2.x - x) / (double)(p2.x - p1.x);
			//有交点
			if (y > min(p1.y, p2.y) && y < max(p1.y, p2.y))
			{
				p.x = x;
				p.y = y;
				cut1[noCutP++] = p;
			}
			if (p2.x > p3.x)//从外向内或者从内到内
			{
				cut1[noCutP++] = p2;
			}
		}
	}

	//右裁剪
	p3 = rect_topRight;
	p4 = rect_bottomRight;
	numOfPoint = noCutP;
	noCutP = 0;
	for (int i = 0; i < numOfPoint; i++)
	{
		//设置线段起始点
		if (i < numOfPoint - 1)
		{
			p1 = cut1[i];
			p2 = cut1[i + 1];
		}
		else if (i == numOfPoint - 1)
		{
			p1 = cut1[i];
			p2 = cut1[0];
		}
		//判断垂直
		if (p1.x == p2.x)
		{
			if (p1.x<p3.x)
			cut2[noCutP++] = p2;
		}
		else
		{
			int x = p3.x;
			int y = p2.y - (double)(p2.y - p1.y)*(double)(p2.x - x) / (double)(p2.x - p1.x);
			//有交点
			if (y > min(p1.y, p2.y) && y < max(p1.y, p2.y))
			{
				p.x = x;
				p.y = y;
				cut2[noCutP++] = p;
			}
			if (p2.x < p3.x)//从外向内或从内到内
			{
				cut2[noCutP++] = p2;
			}
		}
	}

	//下裁剪
	p3 = rect_bottomLeft;
	p4 = rect_bottomRight;
	numOfPoint = noCutP;
	noCutP = 0;
	for (int i = 0; i < numOfPoint; i++)
	{
		//设置线段起始点
		if (i < numOfPoint - 1)
		{
			p1 = cut2[i];
			p2 = cut2[i + 1];
		}
		else if (i == numOfPoint - 1)
		{
			p1 = cut2[i];
			p2 = cut2[0];
		}

		//判断平行
		if (p1.y == p2.y)//从内到内
		{
			if (p1.y<p3.y)
			cut3[noCutP++] = p2;
		}
		else
		{
			if (p1.x == p2.x)//垂直直接计算交点
			{
				if (p3.y>min(p1.y, p2.y) && p3.y<max(p1.y, p2.y))//从外向内
				{
					p.x = p1.x;
					p.y = p3.y;
					cut3[noCutP++] = p;
				}
			}
			else
			{
				int y = p3.y;
				int x = p2.x - (double)(p2.y - y)*(double)(p2.x - p1.x) / (double)(p2.y - p1.y);
				if (x > min(p1.x, p2.x) && x < max(p1.x, p2.x))
				{
					p.x = x;
					p.y = y;
					cut3[noCutP++] = p;
				}
			}
			if (p2.y < p3.y)//从外向内
			{
				cut3[noCutP++] = p2;
			}
		}
	}

	//上裁剪
	p3 = rect_topLeft;
	p4 = rect_topRight;
	numOfPoint = noCutP;
	noCutP = 0;
	for (int i = 0; i < numOfPoint; i++)
	{
		//设置线段起始点
		if (i < numOfPoint - 1)
		{
			p1 = cut3[i];
			p2 = cut3[i + 1];
		}
		else if (i == numOfPoint - 1)
		{
			p1 = cut3[i];
			p2 = cut3[0];
		}
		//判断平行
		if (p1.y == p2.y)//从内到内
		{
			if (p1.y>p3.y)
			cut4[noCutP++] = p2;
		}
		else
		{
			if (p1.x == p2.x)//垂直直接计算交点
			{
				if (p3.y>min(p1.y,p2.y)&&p3.y<max(p1.y,p2.y))//从外向内
				{
					p.x = p1.x;
					p.y = p3.y;
					cut4[noCutP++] = p;
				}
			}
			else
			{
				//求交点
				int y = p3.y;
				int x = p2.x - (double)(p2.y - y)*(double)(p2.x - p1.x) / (double)(p2.y - p1.y);
				if (x > min(p1.x, p2.x) && x < max(p1.x, p2.x))
				{
					p.x = x;
					p.y = y;
					cut4[noCutP++] = p;
				}
			}
			if (p2.y > p3.y)//从外向内
			{
				cut4[noCutP++] = p2;
			}
		}
	}

	//开始绘制
	CClientDC dc(this);
	CPen pen(0, 2, RGB(0, 0, 255));
	dc.SelectObject(&pen);
	for (int i = 0; i < noCutP-1; i++)
	{
		dc.MoveTo(cut4[i]);
		dc.LineTo(cut4[i + 1]);
	}
	dc.MoveTo(cut4[noCutP-1]);
	dc.LineTo(cut4[0]);

}


void CSHCut2Dlg::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	Invalidate();
	firstClick = true;
	stopDraw = false;
	startDrawRect = false;
	count = 0;
	numOfPoint = 0;
}
/*
求交点函数**只判断窗口垂直和平行的情况 tag=1\2\3\4分别对应左右下上
p1为带裁剪线段起点，p2为终点。p3为上端点（左端点），p4为下断点（右端点）

函数毁一生！！!
*/
/*
POINT CSHCut2Dlg::intersectP(POINT p1, POINT p2, POINT p3, POINT p4,int tag)
{
POINT p;
p.x = -1;
p.y = -1;

//左右边界
if (p3.x == p4.x)
{
//待裁剪线段垂直
if (p1.x == p2.x)
{
//p3p4为左右边界且p1、p2垂直
if (p1.x != p3.x)
{
return p;
}

//如果在一条直线上,相当于从内到内，
if (p1.x == p3.x)
{
return p2;
}
}

//待裁剪线段不垂直
else
{
int x = p3.x;
int y = p2.y - (double)(p2.y - p1.y)*(double)(p2.x - x) / (double)(p2.x - p1.x);

//交点在裁剪线段之外
if (y<min(p1.y, p2.y) || y>max(p1.y, p2.y))
return p;

else
{
p.x = x;
p.y = y;
return p;
}
}
}
//上下边界
if (p3.y == p4.y)
{
if (p1.y == p2.y)
{

}
}
}
*/
