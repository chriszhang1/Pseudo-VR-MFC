// MFC-UCL-MI3-SettingsDlg.cpp : IMPLEMENTATION FILE
// Author: Chris Zhang & Anelia Gaydardzhieva
#include "pch.h"
#include "math.h"
#include "framework.h"
#include "MFC-UCL-MI3-Settings.h"
#include "MFC-UCL-MI3-SettingsDlg.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "../packages/nlohmann.json.3.10.5/build/native/include/nlohmann/json.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// define json library
using json = nlohmann::json;
using namespace std;

// define global variables
string globalCurrentMode;
bool globalSpeech;
int globalCameraNr;
int globalAoi;
double globalSensitivity;
bool globalShowFPS;

// Define grids
#define MODE_LEFT_1 "left_fps_mode_1_speech"
#define MODE_LEFT_2 "left_fps_mode_2_speech"
#define MODE_LEFT_3 "left_fps_mode_3_speech"

#define MODE_RIGHT_1 "right_fps_mode_1_speech"
#define MODE_RIGHT_2 "right_fps_mode_2_speech"
#define MODE_RIGHT_3 "right_fps_mode_3_speech"

//parameters

#define MAX_CAMERA_INDEX 9

// CMFCUCLMI3SettingsDlg Dialog - MFC VARIABLES
CMFCUCLMI3SettingsDlg::CMFCUCLMI3SettingsDlg(CWnd* pParent) : CDialogEx(IDD_MFCUCLMI3SETTINGS_DIALOG, pParent) {
	//UCL LOGO ICON
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	// uncomment below for the MFC default logo (and comment the line above)
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCUCLMI3SettingsDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEFAULTCAMERA_COMBO, m_camera);
	DDX_Control(pDX, IDC_AOI_SLIDER, m_aoi);
	DDX_Control(pDX, IDC_FPS_BUTTON, m_showFPS);
	DDX_Control(pDX, IDC_MODE1_BUTTON, m_mode1);
	DDX_Control(pDX, IDC_MODE2_BUTTON, m_mode2);
	DDX_Control(pDX, IDC_MODE3_BUTTON, m_mode3);
	DDX_Control(pDX, IDC_HANDCONTROL_LEFT_BUTTON, m_handLeft);
	DDX_Control(pDX, IDC_HANDCONTROL_RIGHT_BUTTON, m_handRight);
	DDX_Control(pDX, IDC_WALK_SENSITIVITY_SLIDER, m_sensitivity);
	DDX_Control(pDX, IDC_SPEECH_BUTTON, m_speech);
	DDX_Control(pDX, IDC_WALK_SENSITIVITY_COUNTER, m_sensitivityValue);
}

BEGIN_MESSAGE_MAP(CMFCUCLMI3SettingsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CMFCUCLMI3SettingsDlg::ShowAbout)
	ON_BN_CLICKED(IDOK, &CMFCUCLMI3SettingsDlg::Save)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CMFCUCLMI3SettingsDlg::ShowHelp)
	ON_WM_HSCROLL()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_FPS_BUTTON, &CMFCUCLMI3SettingsDlg::UpdateShowFPS)
	ON_BN_CLICKED(IDC_MODE1_BUTTON, &CMFCUCLMI3SettingsDlg::UpdateMode1)
	ON_BN_CLICKED(IDC_MODE2_BUTTON, &CMFCUCLMI3SettingsDlg::UpdateMode2)
	ON_BN_CLICKED(IDC_MODE3_BUTTON, &CMFCUCLMI3SettingsDlg::UpdateMode3)
	ON_BN_CLICKED(IDC_HANDCONTROL_LEFT_BUTTON, &CMFCUCLMI3SettingsDlg::UpdateHandLeft)
	ON_BN_CLICKED(IDC_HANDCONTROL_RIGHT_BUTTON, &CMFCUCLMI3SettingsDlg::UpdateHandRight)
	ON_BN_CLICKED(IDC_BUTTON_INFO_MODE, &CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoMode)
	ON_BN_CLICKED(IDC_BUTTON_INFO_HANDCONTROL, &CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoMethod)
	ON_BN_CLICKED(IDC_BUTTON_INFO_FPS, &CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoFps)
	ON_BN_CLICKED(IDC_BUTTON_INFO_AOI, &CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoAOI)
	ON_BN_CLICKED(IDC_BUTTON_INFO_WALKSENSITIVITY, &CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoSensitivity)
	ON_BN_CLICKED(IDC_BUTTON_INFO_SPEECH, &CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoSpeech)
	ON_BN_CLICKED(IDC_SPEECH_BUTTON, &CMFCUCLMI3SettingsDlg::UpdateSpeech)
	ON_BN_CLICKED(IDC_BUTTON_INFO_DEFAULT_CAMERA, &CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoDefaultCamera)

END_MESSAGE_MAP()

// drag window cursor
HCURSOR CMFCUCLMI3SettingsDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

// Paint
void CMFCUCLMI3SettingsDlg::OnPaint() {
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


// Init
BOOL CMFCUCLMI3SettingsDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon


	// CONFIG data
	wstring pathConfigS = L"..\\..\\config.json";
	LPCWSTR pathConfig = pathConfigS.c_str();

	ifstream ifs_config(pathConfig);
	string content_config((istreambuf_iterator<char>(ifs_config)), (istreambuf_iterator<char>()));

	json myjson_config = json::parse(content_config);
	auto& general = myjson_config["general"];
	auto& modules = myjson_config["modules"];
	auto& events = myjson_config["events"];

	// JSON MODES PATH
	wstring pathModesS = L"..\\..\\\mode_controller.json";
	LPCWSTR pathModes = pathModesS.c_str();

	ifstream ifs_modes(pathModes);
	string content_modes((istreambuf_iterator<char>(ifs_modes)), (istreambuf_iterator<char>()));

	json myjson_modes = json::parse(content_modes);
	auto& current_mode = myjson_modes["current_mode"];

	// Set mode data
	globalCurrentMode = current_mode;
	m_mode1.SetCheck(current_mode == MODE_LEFT_1 || current_mode == MODE_RIGHT_1);
	m_mode2.SetCheck(current_mode == MODE_LEFT_2 || current_mode == MODE_RIGHT_2);
	m_mode3.SetCheck(current_mode == MODE_LEFT_3 || current_mode == MODE_RIGHT_3);
	m_handLeft.SetCheck(current_mode == MODE_LEFT_1 || current_mode == MODE_LEFT_2 || current_mode == MODE_LEFT_3);
	m_handRight.SetCheck(current_mode == MODE_RIGHT_1 || current_mode == MODE_RIGHT_2 || current_mode == MODE_RIGHT_3);


	// Set config data
	globalShowFPS = general["view"]["show_fps"]; // FPS
	globalSpeech = modules["speech"]["enabled"]; // SPEECH
	globalAoi = events["fps_mode_keys"]["aoi_coordinates"]["x"]; // AOI
	globalSensitivity = modules["body"]["min_confidence_threshold"]; // SENSITIVITY
	globalCameraNr = general["camera"]["camera_nr"]; // CAMERA

	// FPS
	m_showFPS.SetWindowTextW(globalShowFPS ? L"ON" : L"OFF");
	// Speech
	m_speech.SetWindowTextW(globalSpeech ? L"ON" : L"OFF");
	// Aoi
	m_aoi.SetRange(-150, 150);
	m_aoi.SetPos(globalAoi);
	CString strSliderValue;
	strSliderValue.Format(_T("%d"), globalAoi);
	// Sensitivity
	m_sensitivity.SetRange(1, 5);
	m_sensitivity.SetPos(6 - globalSensitivity);
	strSliderValue.Format(_T("%d"), int(6 - globalSensitivity));
	m_sensitivityValue.SetWindowText(strSliderValue);
	// Camera
	for (int i = 0; i < MAX_CAMERA_INDEX; i++)
	{
		CString curIndex;
		curIndex.Format(_T("Camera %d"), i + 1);
		m_camera.AddString(curIndex);
	}
	m_camera.SetCurSel(globalCameraNr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}



// Save 
void CMFCUCLMI3SettingsDlg::Save() {
	// Update values
	if (m_mode1.GetCheck() && m_handLeft.GetCheck()) globalCurrentMode = MODE_LEFT_1;
	else if (m_mode2.GetCheck() && m_handLeft.GetCheck()) globalCurrentMode = MODE_LEFT_2;
	else if (m_mode3.GetCheck() && m_handLeft.GetCheck()) globalCurrentMode = MODE_LEFT_3;
	else if (m_mode1.GetCheck() && m_handRight.GetCheck()) globalCurrentMode = MODE_RIGHT_1;
	else if (m_mode2.GetCheck() && m_handRight.GetCheck()) globalCurrentMode = MODE_RIGHT_2;
	else if (m_mode3.GetCheck() && m_handRight.GetCheck()) globalCurrentMode = MODE_RIGHT_3;

	globalAoi = m_aoi.GetPos();
	globalSensitivity = m_sensitivity.GetPos();
	globalCameraNr = m_camera.GetCurSel();

	// --------------------- WRITE JSON ---------------------

	// Save Modes
	wstring tempStrModes = L"..\\..\\mode_controller.json";
	LPCWSTR pathModes = tempStrModes.c_str();
	ifstream ifs_modes(pathModes);
	string content_modes((istreambuf_iterator<char>(ifs_modes)), (istreambuf_iterator<char>()));
	json myjson_modes = json::parse(content_modes);
	myjson_modes["current_mode"] = globalCurrentMode;

	ofstream outputModesFile(pathModes);
	outputModesFile << setw(4) << myjson_modes << endl;

	// Save Configs
	wstring tempStrConfig = L"..\\..\\configMFC.json";
	LPCWSTR pathConfig = tempStrConfig.c_str();
	ifstream ifs_config(pathConfig);
	string content_config((istreambuf_iterator<char>(ifs_config)), (istreambuf_iterator<char>()));
	json myjson_config = json::parse(content_config);
	auto& general = myjson_config["general"];
	auto& modules = myjson_config["modules"];
	auto& events = myjson_config["events"];
	modules["speech"]["enabled"] = globalSpeech;
	general["view"]["show_fps"] = globalShowFPS;
	events["fps_mode_keys"]["aoi_coordinates"]["x"] = globalAoi;
	modules["body"]["min_confidence_threshold"] = 6 - globalSensitivity;
	general["camera"]["camera_nr"] = globalCameraNr;


	// WRITE INTO CONFIG JSON ALL CHANGES
	ofstream outputConfigFile(pathConfig);
	outputConfigFile << setw(4) << myjson_config << endl;

	//MessageBox(_T("UCL MotionInput will now restart and apply the new setting."), _T("Information"));
	MessageBox(_T("Any changes made have now been saved.\n\nMotionInput will now be restarted to apply the new settings."), _T("Changes Saved"));

	// 1. Exit MI
	system("TASKKILL /IM MI3-PseudoVR-3.11.exe");

	// 2. Copy amended configMFC.json file from MFC app to config.json
	Sleep(1000);	// 1 seconds delay
	ifstream src(L"..\\configMFC.json", ios::binary);
	ofstream dst(L"..\\..\\config.json", ios::binary);
	dst << src.rdbuf();

	// 3. Run MI again
	wstring tempStr2 = L"..\\..\\..\\MI3-PseudoVR-3.2.exe";
	LPCWSTR finalPythonPath2 = tempStr2.c_str();

	SHELLEXECUTEINFO si2 = { sizeof(SHELLEXECUTEINFO) };
	si2.hwnd = GetSafeHwnd();
	si2.lpVerb = L"open";
	si2.lpFile = finalPythonPath2;
	si2.nShow = SW_SHOW;
	ShellExecuteEx(&si2);


	CDialogEx::OnOK();
}

// About
void CMFCUCLMI3SettingsDlg::ShowAbout()
{
	m_aboutDlg.DoModal();
}
// Show Help
void CMFCUCLMI3SettingsDlg::ShowHelp()
{
	// OPEN help.txt
	CComHeapPtr<WCHAR> pszPath;
	if (SHGetKnownFolderPath(FOLDERID_Windows, KF_FLAG_CREATE, nullptr, &pszPath) == S_OK)
	{
		// relative path
		wstring tempStr = L"..\\..\\help\\general\\help.txt";
		LPCWSTR finalP = tempStr.c_str();

		// open .txt file
		SHELLEXECUTEINFO si = { sizeof(SHELLEXECUTEINFO) };
		si.hwnd = GetSafeHwnd();
		si.lpVerb = L"open";
		si.lpFile = finalP;
		si.nShow = SW_SHOW;
		ShellExecuteEx(&si);
	}
}


void CMFCUCLMI3SettingsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);

	// Camera
	if (pSlider == &m_aoi) {
		CString strSliderValue;
		int iValue = m_aoi.GetPos(); // Get Slider value
		strSliderValue.Format(_T("%d"), iValue);
	}
	else if (pSlider == &m_sensitivity) {
		CString strSliderValue;
		int iValue = m_sensitivity.GetPos(); // Get Slider value
		strSliderValue.Format(_T("%d"), iValue);
		m_sensitivityValue.SetWindowText(strSliderValue);
	}
}

// FPS 
void CMFCUCLMI3SettingsDlg::UpdateShowFPS() {
	CString temp2;
	m_showFPS.GetWindowText(temp2);
	if (temp2 == "ON") {
		m_showFPS.SetWindowText(L"OFF");
		globalShowFPS = false;
	}
	else {
		m_showFPS.SetWindowTextW(L"ON");
		globalShowFPS = true;
	}
}

// Do not show on/off eye gaze option if Nose Facial selected
void CMFCUCLMI3SettingsDlg::UpdateMode1() {
	m_mode1.SetCheck(1);
	m_mode2.SetCheck(0);
	m_mode3.SetCheck(0);
}

// Show all options on Eyes Facial
void CMFCUCLMI3SettingsDlg::UpdateMode2() {
	m_mode1.SetCheck(0);
	m_mode2.SetCheck(1);
	m_mode3.SetCheck(0);
}

void CMFCUCLMI3SettingsDlg::UpdateMode3() {
	m_mode1.SetCheck(0);
	m_mode2.SetCheck(0);
	m_mode3.SetCheck(1);
}

// Show options only when Facial selected
void CMFCUCLMI3SettingsDlg::UpdateHandLeft() {
	m_handLeft.SetCheck(1);
	m_handRight.SetCheck(0);
	m_aoi.SetPos(-150);
}
void CMFCUCLMI3SettingsDlg::UpdateHandRight() {
	m_handLeft.SetCheck(0);
	m_handRight.SetCheck(1);
	m_aoi.SetPos(150);
}


void CMFCUCLMI3SettingsDlg::UpdateSpeech() {
	CString temp2;
	m_speech.GetWindowText(temp2);
	if (temp2 == "ON") {
		m_speech.SetWindowText(L"OFF");
		globalSpeech = false;
	}
	else {
		m_speech.SetWindowTextW(L"ON");
		globalSpeech = true;
	}
}


void CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoMode() {
	MessageBox(_T("Mode 1: Touch the four d-pad hit targets, it sets it as last known direction, and walk to activate a character walking in that direction (WSAD). \nMode 2: Hold on hit targets and walk, to activate a character walking in those directions. \nMode 3 : When you walk, key W will be triggered.You only need to hold on the hit targets for walking in other directions.There will be only 3 hit targets on screen for A, S, D(left, down, right)."), _T("Mode Information"));
}


void CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoMethod() {
	MessageBox(_T("Selecting the left or right hand indicates the dominant hand which will be used for controling the view. All hand gestures are done using the selected hand."), _T("Hand Information"));
}


void CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoFps() {
	MessageBox(_T("FPS, or Frames per second, is the rate of frames (pictures) a computer screen produces every second. Naturally, the higher this number is the smoother and better our experience interacting with the device would be. By default, this setting is set to 'ON' and a number shows at the bottom right corner of MotionInput's camera screen. Setting this option to 'OFF' will hide the number. "), _T("FPS Information"));
}


void CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoAOI() {
	MessageBox(_T("Area of Interest (AOI) is displayed as a blue rectangle in the webcam window. Your hands need to be in this area when controlling the mouse. \nIt's recommended that AOI is set to left when you are using your left hand to control the view and vice versa."), _T("Area of Interest Information"));
}

void CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoSensitivity() {
	MessageBox(_T("This option allows the user to adjust the sensitivity of the detection of the walking gesture. This options might be useful if you find that walking is recognised too easily or if it is too difficult to trigger the action you need."), _T("Walk Sensitivity Information"));
}


void CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoSpeech() {
	MessageBox(_T("Whatever mode you have selected, and regardless of the hand you are using, there is the option to enable Speech. Speech commands allow users to trigger various actions without any physical movement.  \n\nUsers control various computer actions by saying phrases that are linked to actions. \nFor instance, saying 'click' triggers a left mouse click, and saying 'double click' triggers a double left mouse click. Currently, with Speech enabled, we can do more than Classic Mouse or Multitouch alone. This is something our team will likely improve in the future so that all features are reachable one way or another. \n\nSpeech Commands are designed to be used by people who are mostly comfortable with speech and clear word pronunciation. "), _T("Speech Information"));
}



void CMFCUCLMI3SettingsDlg::OnBnClickedButtonInfoDefaultCamera()
{
	MessageBox(_T("Some computer devices may have two or more webcams available for MotionInput to use (such as Microsoft Surface devices which often have a front and a rear camera). Some users might also prefer attaching an additional camera(s) to their computer devices. \n\nThe default camera value is initially set to 0. If you are experiencing difficulties with MotionInput camera detection, set this option to a different number. In most cases, changing 0 to 1 or 2 is likely to be a solution. \nRestart MotionInput to check if the new number selected has resolved the problem. If not adjust the setting again until MotionInput is connected to the desired camera."), _T("Default Camera Information"));
}



