
// SHCut2Dlg.h : 头文件
//

#pragma once


// CSHCut2Dlg 对话框
class CSHCut2Dlg : public CDialogEx
{
// 构造
public:
	CSHCut2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SHCUT2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	int numOfPoint;//记录多边形点的个数
	POINT poly[50];//记录多边形各个定点
	POINT cut1[50], cut2[50], cut3[50], cut4[50];
	int noCutP;
	CRect rect;//裁剪窗口
	bool firstClick;//是否第一次点击绘制多边形
	bool stopDraw;//停止绘制多边形
	bool startDrawRect;//开始绘制裁剪窗口
	int count = 0;//记录裁剪窗口的点
	POINT rect_topLeft,rect_topRight,rect_bottomLeft, rect_bottomRight;//裁剪窗口的四个点

	POINT intersectP(POINT p1,POINT p2, POINT p3,POINT p4,int tag);//待写！
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonShcut();
	afx_msg void OnBnClickedButtonClear();
};
