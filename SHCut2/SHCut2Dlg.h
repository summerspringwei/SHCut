
// SHCut2Dlg.h : ͷ�ļ�
//

#pragma once


// CSHCut2Dlg �Ի���
class CSHCut2Dlg : public CDialogEx
{
// ����
public:
	CSHCut2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SHCUT2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	int numOfPoint;//��¼����ε�ĸ���
	POINT poly[50];//��¼����θ�������
	POINT cut1[50], cut2[50], cut3[50], cut4[50];
	int noCutP;
	CRect rect;//�ü�����
	bool firstClick;//�Ƿ��һ�ε�����ƶ����
	bool stopDraw;//ֹͣ���ƶ����
	bool startDrawRect;//��ʼ���Ʋü�����
	int count = 0;//��¼�ü����ڵĵ�
	POINT rect_topLeft,rect_topRight,rect_bottomLeft, rect_bottomRight;//�ü����ڵ��ĸ���

	POINT intersectP(POINT p1,POINT p2, POINT p3,POINT p4,int tag);//��д��
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonShcut();
	afx_msg void OnBnClickedButtonClear();
};
