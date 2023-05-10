// MFC-UCL-MI3-SettingsDlg.h : HEADER FILE
#include "CMFCUCLMI3AboutDlg.h"

#pragma once


// CMFCUCLMI3SettingsDlg dialog
class CMFCUCLMI3SettingsDlg : public CDialogEx
{

	// Construction
public:
	CMFCUCLMI3SettingsDlg(CWnd* pParent = nullptr);	// standard constructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCUCLMI3SETTINGS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void ShowAbout();
	afx_msg void ShowHelp();
	afx_msg void Save();

	CComboBox m_mode;
	CComboBox m_method;
	CComboBox m_camera;

	CButton m_showFPS;
	CButton m_lowLightOn;
	afx_msg void UpdateShowFPS();
	CSliderCtrl m_aoi;

	CSliderCtrl m_noseMouseSpeed;
	CEdit m_noseMouseSpeedValue;
	CSliderCtrl m_eyesMouseSpeed;
	CEdit m_eyesMouseSpeedValue;
	CSliderCtrl m_headRotationLeft;
	CEdit m_headRotationLeftValue;
	CSliderCtrl m_headRotationRight;
	CEdit m_headRotationRightValue;

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CComboBox m_smile;
	CComboBox m_fishFace;
	CComboBox m_raisedEyebrows;
	CComboBox m_openMouth;
	CButton m_mode1;
	CButton m_mode2;
	CButton m_mode3;
	CButton m_handLeft;
	CButton m_handRight;
	afx_msg void UpdateMode1();
	afx_msg void UpdateMode2();
	afx_msg void UpdateMode3();
	afx_msg void UpdateHandLeft();
	afx_msg void UpdateHandRight();
	CComboBox m_rotationRight;
	CComboBox m_rotationLeft;
	CSliderCtrl m_sensitivity;
	CEdit m_sensitivityValue;
	afx_msg void OnBnClickedButtonInfoMode();
	afx_msg void OnBnClickedButtonInfoMethod();
	afx_msg void OnBnClickedButtonInfoFps();
	afx_msg void OnBnClickedButtonInfoAOI();
	afx_msg void OnBnClickedButtonInfoNoseMouse();
	afx_msg void OnBnClickedButtonInfoNoseboxBound();
	afx_msg void OnBnClickedButtonInfoHeadRotationLeft();
	afx_msg void OnBnClickedButtonInfoHeadRotationRight();
	afx_msg void OnBnClickedButtonInfoEyesMouse();
	afx_msg void OnBnClickedButtonInfoSmile();
	afx_msg void OnBnClickedButtonInfoFishface();
	afx_msg void OnBnClickedButtonInfoEyebrows();
	afx_msg void OnBnClickedButtonInfoOpenMouth();
	afx_msg void OnBnClickedButtonInfoRotateHeadLeft();
	afx_msg void OnBnClickedButtonInfoRotateHeadRight();
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnCbnSelchangeCombo13();
	afx_msg void OnStnClickedStaticHand5();
	afx_msg void OnBnClickedButtonInfoSensitivity();
	afx_msg void OnBnClickedButtonInfoSpeech();
	afx_msg void UpdateSpeech();
	CButton m_speech;
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonInfoDefaultCamera();
	afx_msg void OnBnClickedCancel();
	CMFCUCLMI3AboutDlg m_aboutDlg;
};

