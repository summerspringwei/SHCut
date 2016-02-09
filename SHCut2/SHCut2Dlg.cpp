
// SHCut2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SHCut2.h"
#include "SHCut2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSHCut2Dlg �Ի���



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


// CSHCut2Dlg ��Ϣ�������

BOOL CSHCut2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	firstClick = true;
	stopDraw = false;
	startDrawRect = false;
	count = 0;



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSHCut2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSHCut2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//���˫�����ƶ���κ;���
void CSHCut2Dlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���ƶ����
	if (!stopDraw)//�����û�л��ƶ����
	{
		CClientDC dc(this);//����
		CPen pen(0, 2, RGB(255, 0, 0));//���û��ʺͻ�����ɫ
		dc.SelectObject(&pen);


		if (firstClick)//��һ�ε��û�л���
		{
			poly[numOfPoint] = point;
			dc.Ellipse(point.x, point.y, point.x + 4, point.y + 4);
			firstClick = false;
			numOfPoint++;
		}
		else//����ÿ���һ�λ�һ����
		{
			dc.MoveTo(poly[numOfPoint - 1]);
			dc.LineTo(point);
			poly[numOfPoint] = point;
			numOfPoint++;
		}
	}

	//���Ʋü�����
	if (startDrawRect)
	{
		if (count == 0)
		{
			rect_topLeft = point;//��¼���ϽǶ�������
		}

		else if (count == 1)
		{
			rect_bottomRight = point;//��¼���½Ƕ�������
			CClientDC dc(this);
			CPen pen(0, 2, RGB(0, 255, 0));

			rect = new CRect(rect_topLeft, rect_bottomRight);
			dc.SelectObject(&pen);

			rect_topRight.x = rect_bottomRight.x;//���ϵ�
			rect_topRight.y = rect_topLeft.y;
			
			rect_bottomLeft.x = rect_topLeft.x;//���µ�
			rect_bottomLeft.y = rect_bottomRight.y;

			//���ƾ��βü�����
			dc.MoveTo(rect_topLeft);
			dc.LineTo(rect_topRight);
			dc.LineTo(rect_bottomRight);
			dc.LineTo(rect_bottomLeft);
			dc.LineTo(rect_topLeft);
			
		}
		count++;

		if (count > 1)//��¼���Ѿ���������Σ�����
		{
			startDrawRect = false;
			return;
		}
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

//�Ҽ�������Ϣ��Ӧ����
void CSHCut2Dlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (numOfPoint < 3)//
	{
		MessageBox(_T("��˫������������㣡"));
		return;
	}

	stopDraw = true;
	startDrawRect = true;
	CClientDC dc(this);
	CPen pen(0, 2, RGB(255, 0, 0));
	dc.SelectObject(&pen);

	//�������һ����
	dc.MoveTo(poly[numOfPoint - 1]);
	dc.LineTo(poly[0]);
	CDialogEx::OnRButtonDblClk(nFlags, point);
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CSHCut2Dlg::OnBnClickedButtonShcut()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	POINT p1, p2, p3, p4, p;
	//��ü�
	p3 = rect_topLeft;
	p4 = rect_bottomLeft;
	noCutP = 0;
	for (int i = 0; i < numOfPoint; i++)
	{
		//������ʼ�߶�
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

		//�жϴ�ֱ
		if (p1.x == p2.x)
		{
			if (p1.x>p3.x)
			cut1[noCutP++] = p2;//�ѽ���ӵ�������
		}
		else//����ֱ�Ϳ���ͨ��б�ʼ����ӳ��߽���
		{
			int x = p3.x;
			int y = p2.y - (double)(p2.y - p1.y)*(double)(p2.x - x) / (double)(p2.x - p1.x);
			//�н���
			if (y > min(p1.y, p2.y) && y < max(p1.y, p2.y))
			{
				p.x = x;
				p.y = y;
				cut1[noCutP++] = p;
			}
			if (p2.x > p3.x)//�������ڻ��ߴ��ڵ���
			{
				cut1[noCutP++] = p2;
			}
		}
	}

	//�Ҳü�
	p3 = rect_topRight;
	p4 = rect_bottomRight;
	numOfPoint = noCutP;
	noCutP = 0;
	for (int i = 0; i < numOfPoint; i++)
	{
		//�����߶���ʼ��
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
		//�жϴ�ֱ
		if (p1.x == p2.x)
		{
			if (p1.x<p3.x)
			cut2[noCutP++] = p2;
		}
		else
		{
			int x = p3.x;
			int y = p2.y - (double)(p2.y - p1.y)*(double)(p2.x - x) / (double)(p2.x - p1.x);
			//�н���
			if (y > min(p1.y, p2.y) && y < max(p1.y, p2.y))
			{
				p.x = x;
				p.y = y;
				cut2[noCutP++] = p;
			}
			if (p2.x < p3.x)//�������ڻ���ڵ���
			{
				cut2[noCutP++] = p2;
			}
		}
	}

	//�²ü�
	p3 = rect_bottomLeft;
	p4 = rect_bottomRight;
	numOfPoint = noCutP;
	noCutP = 0;
	for (int i = 0; i < numOfPoint; i++)
	{
		//�����߶���ʼ��
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

		//�ж�ƽ��
		if (p1.y == p2.y)//���ڵ���
		{
			if (p1.y<p3.y)
			cut3[noCutP++] = p2;
		}
		else
		{
			if (p1.x == p2.x)//��ֱֱ�Ӽ��㽻��
			{
				if (p3.y>min(p1.y, p2.y) && p3.y<max(p1.y, p2.y))//��������
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
			if (p2.y < p3.y)//��������
			{
				cut3[noCutP++] = p2;
			}
		}
	}

	//�ϲü�
	p3 = rect_topLeft;
	p4 = rect_topRight;
	numOfPoint = noCutP;
	noCutP = 0;
	for (int i = 0; i < numOfPoint; i++)
	{
		//�����߶���ʼ��
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
		//�ж�ƽ��
		if (p1.y == p2.y)//���ڵ���
		{
			if (p1.y>p3.y)
			cut4[noCutP++] = p2;
		}
		else
		{
			if (p1.x == p2.x)//��ֱֱ�Ӽ��㽻��
			{
				if (p3.y>min(p1.y,p2.y)&&p3.y<max(p1.y,p2.y))//��������
				{
					p.x = p1.x;
					p.y = p3.y;
					cut4[noCutP++] = p;
				}
			}
			else
			{
				//�󽻵�
				int y = p3.y;
				int x = p2.x - (double)(p2.y - y)*(double)(p2.x - p1.x) / (double)(p2.y - p1.y);
				if (x > min(p1.x, p2.x) && x < max(p1.x, p2.x))
				{
					p.x = x;
					p.y = y;
					cut4[noCutP++] = p;
				}
			}
			if (p2.y > p3.y)//��������
			{
				cut4[noCutP++] = p2;
			}
		}
	}

	//��ʼ����
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Invalidate();
	firstClick = true;
	stopDraw = false;
	startDrawRect = false;
	count = 0;
	numOfPoint = 0;
}
/*
�󽻵㺯��**ֻ�жϴ��ڴ�ֱ��ƽ�е���� tag=1\2\3\4�ֱ��Ӧ��������
p1Ϊ���ü��߶���㣬p2Ϊ�յ㡣p3Ϊ�϶˵㣨��˵㣩��p4Ϊ�¶ϵ㣨�Ҷ˵㣩

������һ������!
*/
/*
POINT CSHCut2Dlg::intersectP(POINT p1, POINT p2, POINT p3, POINT p4,int tag)
{
POINT p;
p.x = -1;
p.y = -1;

//���ұ߽�
if (p3.x == p4.x)
{
//���ü��߶δ�ֱ
if (p1.x == p2.x)
{
//p3p4Ϊ���ұ߽���p1��p2��ֱ
if (p1.x != p3.x)
{
return p;
}

//�����һ��ֱ����,�൱�ڴ��ڵ��ڣ�
if (p1.x == p3.x)
{
return p2;
}
}

//���ü��߶β���ֱ
else
{
int x = p3.x;
int y = p2.y - (double)(p2.y - p1.y)*(double)(p2.x - x) / (double)(p2.x - p1.x);

//�����ڲü��߶�֮��
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
//���±߽�
if (p3.y == p4.y)
{
if (p1.y == p2.y)
{

}
}
}
*/
