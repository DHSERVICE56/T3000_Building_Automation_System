// DialogT3.cpp : implementation file
//

#include "stdafx.h"
#include "DialogT3.h"
#include "../globle_function.h"
#include "../T3000.h"
#include "../MainFrm.h"
#include "../ado/ADO.h"
#include "../Dialog_Progess.h"

#define   T3_MODEL   2000

static BOOL ift3db = TRUE;

// CDialogT3

IMPLEMENT_DYNCREATE(CDialogT3, CFormView)

CDialogT3::CDialogT3()
	: CFormView(CDialogT3::IDD)
	, m_address(0)
	, m_firmware(0)
	, m_serial(0)
	, m_hardware(0)
	, m_model(_T(""))
	, m_picVersion(0)
{

}

CDialogT3::~CDialogT3()
{

}

void CDialogT3::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_T3ADDRESS, m_address);
	DDX_Text(pDX, IDC_EDIT_T3FIRMWARE, m_firmware);
	DDX_Text(pDX, IDC_EDIT_T3SERIAL, m_serial);
	DDX_Text(pDX, IDC_EDIT_T3HARDWARE, m_hardware);
	DDX_Text(pDX, IDC_EDIT_T3MODEL, m_model);
	DDX_Text(pDX, IDC_EDIT_T3PICVERSION, m_picVersion);
	DDX_Control(pDX, IDC_MSFLEXGRID_INPUT, m_msflexgrid_input);
	DDX_Control(pDX, IDC_MSFLEXGRID_OUTPUT, m_msflexgrid_output);
	DDX_Control(pDX, IDC_PROGRESS2, m_progress);
	DDX_Control(pDX, IDC_EDIT_NAME, m_inNameEdt);
	DDX_Control(pDX, IDC_RANGECOMBO, m_comboxRange);
}

BEGIN_MESSAGE_MAP(CDialogT3, CFormView)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_RANGECOMBO, &CDialogT3::OnCbnSelchangeType)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDialogT3, CFormView)
	ON_EVENT(CDialogT3, IDC_MSFLEXGRID_INPUT, DISPID_CLICK, CDialogT3::ClickInputMsflexgrid, VTS_NONE)
	ON_EVENT(CDialogT3, IDC_MSFLEXGRID_OUTPUT, DISPID_CLICK, CDialogT3::ClickOutputMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()
// CDialogT3 diagnostics

#ifdef _DEBUG
void CDialogT3::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDialogT3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}


#endif
#endif //_DEBUG


// CDialogT3 message handlers

void CDialogT3::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	 	 
	#if 1	
	//设置排/行数量
	m_msflexgrid_input.put_Cols(5);
	m_msflexgrid_input.put_Rows(11);//包括标题栏
	//显示横标题
	m_msflexgrid_input.put_TextMatrix(0,0,_T("Input Name"));
	m_msflexgrid_input.put_TextMatrix(0,1,_T("Register Value"));
	m_msflexgrid_input.put_TextMatrix(0,2,_T("Date Stamp"));
	m_msflexgrid_input.put_TextMatrix(0,3,_T("Range"));
	m_msflexgrid_input.put_TextMatrix(0,4,_T("Filter"));




	//设置列宽	
	m_msflexgrid_input.put_ColWidth(0,1000);
	m_msflexgrid_input.put_ColWidth(1,1500);
	m_msflexgrid_input.put_ColWidth(2,1500);
	m_msflexgrid_input.put_ColWidth(3,1500);
	m_msflexgrid_input.put_ColWidth(4,1000);
	//居中显示
	for (int col=0;col<5;col++)
	{ 
		m_msflexgrid_input.put_ColAlignment(col,4);
	}

	//彩色显示
	for(int i=1;i<11;i++)		//排数量
	{

		for(int k=0;k<5;k++)	//列数量
		{
			if (i%2==1)
			{
				m_msflexgrid_input.put_Row(i);m_msflexgrid_input.put_Col(k);m_msflexgrid_input.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_msflexgrid_input.put_Row(i);m_msflexgrid_input.put_Col(k);m_msflexgrid_input.put_CellBackColor(COLOR_CELL);
			}
		}
	}




	//显示纵标题
	CString str;
	for(int i=1;i<11;i++)
	{

		str.Format(_T("Input%d"),i);

		m_msflexgrid_input.put_TextMatrix(i,0,str);	 
	}

	//============================================================================================================界面Output DO部份列表框

	//设置行/列数量
	m_msflexgrid_output.put_Rows(9);
	m_msflexgrid_output.put_Cols(4);
	//设置列宽	


	//显示横标题
	m_msflexgrid_output.put_TextMatrix(0,0,_T("Output Name"));
	m_msflexgrid_output.put_TextMatrix(0,1,_T("Relay 0N/OFF"));
	m_msflexgrid_output.put_TextMatrix(0,2,_T("Analog Output"));
	m_msflexgrid_output.put_TextMatrix(0,3,_T("Auto/Manual"));

	m_msflexgrid_output.put_ColWidth(0,1000);
	m_msflexgrid_output.put_ColWidth(1,1500);
	m_msflexgrid_output.put_ColWidth(2,1500);
	m_msflexgrid_output.put_ColWidth(3,1500);//居中显示
	for (int col=0;col<4;col++)
	{ 
		m_msflexgrid_output.put_ColAlignment(col,4);
	}

	//彩色显示
	for(int i=1;i<9;i++)		//排数量
	{

		for(int k=0;k<4;k++)	//列数量
		{
			if (i%2==1)
			{
				m_msflexgrid_output.put_Row(i);m_msflexgrid_output.put_Col(k);m_msflexgrid_output.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_msflexgrid_output.put_Row(i);m_msflexgrid_output.put_Col(k);m_msflexgrid_output.put_CellBackColor(COLOR_CELL);
			}
		}
	}
	//显示纵标题
	CString str_output;
	for(int i=1;i<9;i++)
	{

		str_output.Format(_T("Output%d"),i);
		m_msflexgrid_output.put_TextMatrix(i,0,str_output);	
 
		 
	}




	#endif  
}

void CDialogT3::Fresh()
{
	
	 
 
	//KillTimer(T3_MODEL);





float progress;
if (is_connect())
{  
	CDialog_Progess* pDlg = new CDialog_Progess(this,1,100);
	pDlg->Create(IDD_DIALOG10_Progress, this);
	pDlg->ShowProgress(0,0);
	pDlg->ShowWindow(SW_SHOW);
	RECT RECT_SET1;
	GetClientRect(&RECT_SET1);
	pDlg->MoveWindow(RECT_SET1.left+400,RECT_SET1.bottom-19,RECT_SET1.right/2+20,20);
	memset(&shmPRegister,0,sizeof(shmPRegister)); 
	for (int i=0;i<3;i++)
	{
		if (pDlg!=NULL)
		{
			progress=float((i+1)*(100/3));
			pDlg->ShowProgress(int(progress),(int)progress);
		} 
		Read_Multi(g_tstat_id,&shmPRegister[i*100],i*100,100);
	}
	pDlg->ShowWindow(SW_HIDE);
	if(pDlg!=NULL)
	{delete pDlg;
	pDlg=NULL;}



	Initial_RegisterList();
#if 1
	//读取串口号，从数据库
	int sumret = 0;
	BOOL flag = FALSE;

	//读取miniPanel数据
	BOOL ret_bool = FALSE;
	long start = clock(),end(0); 		


	
	 
	SetTimer(T3_MODEL,1000,NULL);
#endif
	ShowDialogData();
	SetTimer(T3_MODEL,1000,NULL);
 
}
else
{
	SetPaneString(1,_T("Disconnection"));
	AfxMessageBox(_T("Please Connect your device"));
}
}
void CDialogT3::Initial_RegisterList()
{
#if 1
SN_LOW							=  0  ;
SN_HI							=   2 ;
EPROM_VER_NUMBER					=  4  ;
FIRMWARE_VER_NUMBER					=   5 ;
MODBUS_ID						=  6  ;
PRODUCT_MODEL						=   7 ;
HARDWARE_VER_NUMBER					=  8  ;
PIC_VER_NUMBER						=  9  ;
CALIBRATION_OUTPUTS					= 13   ;
BAUDRATE						=  15  ;
RESPONSE_DELAY						=  19  ;
OUTPUT1_RELAY						=  100  ;
OUTPUT2_RELAY						=  101  ;
OUTPUT3_RELAY						=  102  ;
OUTPUT4_RELAY						=  103  ;
OUTPUT5_RELAY						=  104  ;
OUTPUT6_RELAY						=  105  ;
OUTPUT7_RELAY						=  106  ;
OUTPUT8_RELAY						=  107  ;
OUTPUT1_ANALOG						=  108  ;
OUTPUT2_ANALOG						=  109  ;
OUTPUT3_ANALOG						=  110  ;
OUTPUT4_ANALOG						=  111  ;
SWITCH1_STATUS						=  116  ;
SWITCH2_STATUS						=  117  ;
SWITCH3_BANK						=  118  ;
INPUT1_PULSE_COUNT_HIGHT				= 119   ;
INPUT1_PULSE_COUNT_LOW					= 120   ;
INPUT2_PULSE_COUNT_HIGHT				= 121   ;
INPUT2_PULSE_COUNT_LOW					= 122   ;
INPUT3_PULSE_COUNT_HIGHT				= 123   ;
INPUT3_PULSE_COUNT_LOW					= 124   ;
INPUT4_PULSE_COUNT_HIGHT				= 125   ;
INPUT4_PULSE_COUNT_LOW					= 126   ;
INPUT5_PULSE_COUNT_HIGHT				= 127   ;
INPUT5_PULSE_COUNT_LOW					= 128   ;
INPUT6_PULSE_COUNT_HIGHT				= 129   ;
INPUT6_PULSE_COUNT_LOW					= 130   ;
INPUT7_PULSE_COUNT_HIGHT				= 131   ;
INPUT7_PULSE_COUNT_LOW					= 132   ;
INPUT8_PULSE_COUNT_HIGHT				= 133   ;
INPUT8_PULSE_COUNT_LOW					= 134   ;
INPUT9_PULSE_COUNT_HIGHT				= 135   ;
INPUT9_PULSE_COUNT_LOW					= 136   ;
INPUT10_PULSE_COUNT_HIGHT				= 137   ;
INPUT10_PULSE_COUNT_LOW					= 138   ;
DATE_STAMP_INPUT1_YEAR					= 139   ;
DATE_STAMP_INPUT1_MONTH					= 140   ;
DATE_STAMP_INPUT1_DAY					= 141   ;
DATE_STAMP_INPUT1_HOUR					= 142   ;
DATE_STAMP_INPUT1_MINUTE				= 143   ;
DATE_STAMP_INPUT2_YEAR					= 144   ;
DATE_STAMP_INPUT2_MONTH					= 145   ;
DATE_STAMP_INPUT2_DAY					= 146   ;
DATE_STAMP_INPUT2_HOUR					= 147   ;
DATE_STAMP_INPUT2_MINUTE				= 148   ;
DATE_STAMP_INPUT3_YEAR					= 149   ;
DATE_STAMP_INPUT3_MONTH					= 150   ;
DATE_STAMP_INPUT3_DAY					= 151   ;
DATE_STAMP_INPUT3_HOUR					= 152   ;
DATE_STAMP_INPUT3_MINUTE				= 153   ;
DATE_STAMP_INPUT4_YEAR					= 154   ;
DATE_STAMP_INPUT4_MONTH					= 155   ;
DATE_STAMP_INPUT4_DAY					= 156   ;
DATE_STAMP_INPUT4_HOUR					= 157   ;
DATE_STAMP_INPUT4_MINUTE				= 158   ;
DATE_STAMP_INPUT5_YEAR					= 159   ;
DATE_STAMP_INPUT5_MONTH					= 160   ;
DATE_STAMP_INPUT5_DAY					= 161   ;
DATE_STAMP_INPUT5_HOUR					= 162   ;
DATE_STAMP_INPUT5_MINUTE				= 163   ;
DATE_STAMP_INPUT6_YEAR					= 164   ;
DATE_STAMP_INPUT6_MONTH					= 165   ;
DATE_STAMP_INPUT6_DAY					= 166   ;
DATE_STAMP_INPUT6_HOUR					= 167   ;
DATE_STAMP_INPUT6_MINUTE				= 168   ;
DATE_STAMP_INPUT7_YEAR					= 169   ;
DATE_STAMP_INPUT7_MONTH					= 170   ;
DATE_STAMP_INPUT7_DAY					= 171 ;
DATE_STAMP_INPUT7_HOUR					= 172 ;
DATE_STAMP_INPUT7_MINUTE				= 173 ;
DATE_STAMP_INPUT8_YEAR					= 174 ;
DATE_STAMP_INPUT8_MONTH					= 175 ;
DATE_STAMP_INPUT8_DAY					= 176 ;
DATE_STAMP_INPUT8_HOUR					= 177 ;
DATE_STAMP_INPUT8_MINUTE				= 178 ;
DATE_STAMP_INPUT9_YEAR					= 179 ;
DATE_STAMP_INPUT9_MONTH					= 180 ;
DATE_STAMP_INPUT9_DAY					= 181 ;
DATE_STAMP_INPUT9_HOUR					= 182 ;
DATE_STAMP_INPUT9_MINUTE				= 183 ;
DATE_STAMP_INPUT10_YEAR					= 184 ;
DATE_STAMP_INPUT10_MONTH				= 185 ;
DATE_STAMP_INPUT10_DAY					= 186 ;
DATE_STAMP_INPUT10_HOUR					= 187 ;
DATE_STAMP_INPUT10_MINUTE				= 188 ;
AUTO_MANUAL_OUTPUTS					    = 189 ;
ORIGIGINAL_AD_DATA_INPUT1				= 190;
ORIGIGINAL_AD_DATA_INPUT2				= 191   ;
ORIGIGINAL_AD_DATA_INPUT3				= 192   ;
ORIGIGINAL_AD_DATA_INPUT4				= 193   ;
ORIGIGINAL_AD_DATA_INPUT5				= 194   ;
ORIGIGINAL_AD_DATA_INPUT6				= 195   ;
ORIGIGINAL_AD_DATA_INPUT7				= 196   ;
ORIGIGINAL_AD_DATA_INPUT8				= 197   ;
ORIGIGINAL_AD_DATA_INPUT9				= 198   ;
ORIGIGINAL_AD_DATA_INPUT10				= 199   ;
RANGE_INPUT1						=   200 ;
RANGE_INPUT2						=   201 ;
RANGE_INPUT3						=   202 ;
RANGE_INPUT4						=   203 ;
RANGE_INPUT5						=   204 ;
RANGE_INPUT6						=   205 ;
RANGE_INPUT7						=   206 ;
RANGE_INPUT8						=   207 ;
RANGE_INPUT9						=   208 ;
RANGE_INPUT10						=   209 ;
FILTER_INPUT1						=   210 ;
FILTER_INPUT2						=   211 ;
FILTER_INPUT3						=   212 ;
FILTER_INPUT4						=   213 ;
FILTER_INPUT5						=   214 ;
FILTER_INPUT6						=   215 ;
FILTER_INPUT7						=   216 ;
FILTER_INPUT8						=   217 ;
FILTER_INPUT9						=   218 ;
FILTER_INPUT10						=   219 ;
LIGHTING_ZONE_TIME_INPUT1				= 220   ;
LIGHTING_ZONE_TIME_INPUT2				= 221   ;
LIGHTING_ZONE_TIME_INPUT3				= 222   ;
LIGHTING_ZONE_TIME_INPUT4				= 223   ;
LIGHTING_ZONE_TIME_INPUT5				= 224   ;
LIGHTING_ZONE_TIME_INPUT6				= 225   ;
LIGHTING_ZONE_TIME_INPUT7				= 226   ;
LIGHTING_ZONE_TIME_INPUT8				= 227   ;
LIGHTING_ZONE_TIME_INPUT9				= 228   ;
LIGHTING_ZONE_TIME_INPUT10				= 229   ;
ZONE_TIME_LEFT_INPUT1					= 230   ;
ZONE_TIME_LEFT_INPUT2					= 231   ;
ZONE_TIME_LEFT_INPUT3					= 232   ;
ZONE_TIME_LEFT_INPUT4					= 233   ;
ZONE_TIME_LEFT_INPUT5					= 234   ;
ZONE_TIME_LEFT_INPUT6					= 235   ;
ZONE_TIME_LEFT_INPUT7					= 236   ;
ZONE_TIME_LEFT_INPUT8					= 237   ;
ZONE_TIME_LEFT_INPUT9					= 238   ;
ZONE_TIME_LEFT_INPUT10					= 239   ;
LC_DISABLE_ENABLE					    = 240   ;
SELECT_INPUT_AS_TRIGGER_1				= 241   ;
SELECT_INPUT_AS_TRIGGER_2				= 242   ;
SELECT_INPUT_AS_TRIGGER_3				= 243   ;
SELECT_INPUT_AS_TRIGGER_4				= 244   ;
SELECT_INPUT_AS_TRIGGER_5				= 245   ;
SELECT_INPUT_AS_TRIGGER_6				= 246   ;
SELECT_INPUT_AS_TRIGGER_7				= 247   ;
SELECT_INPUT_AS_TRIGGER_8				= 248   ;
#endif
T3Register temp;
CADO m_ado;
m_ado.OnInitADOConn();
#if 1
CString SQL = _T("select * from T3_RegisterList");
m_ado.m_pRecordset = m_ado.OpenRecordset(SQL);
_variant_t vartemp;
while(!m_ado.m_pRecordset->EndOfFile)
{
	temp.regID=m_ado.m_pRecordset->GetCollect(_T("RegID"));
	vartemp =m_ado.m_pRecordset->GetCollect(_T("T3-4AO"));
	if (vartemp.vt==VT_NULL)
		temp.regName=_T("");
	else
		temp.regName =vartemp;
	m_ado.m_pRecordset->MoveNext();
	m_vecT3Register.push_back(temp);
}
m_ado.CloseRecordset();
m_ado.CloseConn();
#endif 


#if 1
SN_LOW							=  Get_RegID(_T("SN_LOW"))  ;
SN_HI							=    Get_RegID(_T("SN_HI")) ;
EPROM_VER_NUMBER					=   Get_RegID(_T("EPROM_VER_NUMBER"))  ;
FIRMWARE_VER_NUMBER					=    Get_RegID(_T("FIRMWARE_VER_NUMBER")) ;
MODBUS_ID						=  Get_RegID(_T("MODBUS_ID"))  ;
PRODUCT_MODEL						=   Get_RegID(_T("PRODUCT_MODEL")) ;
HARDWARE_VER_NUMBER					=   Get_RegID(_T("HARDWARE_VER_NUMBER")) ;
PIC_VER_NUMBER						=   Get_RegID(_T("PIC_VER_NUMBER"))  ;
CALIBRATION_OUTPUTS					=  Get_RegID(_T("CALIBRATION_OUTPUTS"))   ;
BAUDRATE						=   Get_RegID(_T("BAUDRATE"))  ;
RESPONSE_DELAY						=  Get_RegID(_T("RESPONSE_DELAY"))  ;
OUTPUT1_RELAY						=  Get_RegID(_T("OUTPUT1_RELAY")) ;
OUTPUT2_RELAY						=  Get_RegID(_T("OUTPUT2_RELAY"))  ;
OUTPUT3_RELAY						=   Get_RegID(_T("OUTPUT3_RELAY"))  ;
OUTPUT4_RELAY						=   Get_RegID(_T("OUTPUT4_RELAY")) ;
OUTPUT5_RELAY						=   Get_RegID(_T("OUTPUT5_RELAY"))  ;
OUTPUT6_RELAY						=  Get_RegID(_T("OUTPUT6_RELAY"))  ;
OUTPUT7_RELAY						=   Get_RegID(_T("OUTPUT7_RELAY"))  ;
OUTPUT8_RELAY						=   Get_RegID(_T("OUTPUT8_RELAY"))  ;
OUTPUT1_ANALOG						=   Get_RegID(_T("OUTPUT1_ANALOG"))  ;
OUTPUT2_ANALOG						=   Get_RegID(_T("OUTPUT2_ANALOG"))  ;
OUTPUT3_ANALOG						=  Get_RegID(_T("OUTPUT3_ANALOG")) ;
OUTPUT4_ANALOG						=   Get_RegID(_T("OUTPUT4_ANALOG"))  ;
SWITCH1_STATUS						=   Get_RegID(_T("SWITCH1_STATUS")) ;
SWITCH2_STATUS						=  Get_RegID(_T("SWITCH2_STATUS")) ;
SWITCH3_BANK						=   Get_RegID(_T("SWITCH3_BANK"))  ;
INPUT1_PULSE_COUNT_HIGHT				=  Get_RegID(_T("INPUT1_PULSE_COUNT_HIGHT"))   ;
INPUT1_PULSE_COUNT_LOW					=  Get_RegID(_T("INPUT1_PULSE_COUNT_LOW"))   ;
INPUT2_PULSE_COUNT_HIGHT				= Get_RegID(_T("INPUT2_PULSE_COUNT_HIGHT"))   ;
INPUT2_PULSE_COUNT_LOW					=  Get_RegID(_T("INPUT2_PULSE_COUNT_LOW"))   ;
INPUT3_PULSE_COUNT_HIGHT				=  Get_RegID(_T("INPUT3_PULSE_COUNT_HIGHT"))   ;
INPUT3_PULSE_COUNT_LOW					=  Get_RegID(_T("INPUT3_PULSE_COUNT_LOW"))   ;
INPUT4_PULSE_COUNT_HIGHT				=  Get_RegID(_T("INPUT4_PULSE_COUNT_HIGHT"))   ;
INPUT4_PULSE_COUNT_LOW					=  Get_RegID(_T("INPUT4_PULSE_COUNT_LOW"))   ;
INPUT5_PULSE_COUNT_HIGHT				=  Get_RegID(_T("INPUT5_PULSE_COUNT_HIGHT"))   ;
INPUT5_PULSE_COUNT_LOW					=  Get_RegID(_T("INPUT5_PULSE_COUNT_LOW"))   ;
INPUT6_PULSE_COUNT_HIGHT				=  Get_RegID(_T("INPUT6_PULSE_COUNT_HIGHT"))   ;
INPUT6_PULSE_COUNT_LOW					=  Get_RegID(_T("INPUT6_PULSE_COUNT_LOW"))   ;
INPUT7_PULSE_COUNT_HIGHT				=  Get_RegID(_T("INPUT7_PULSE_COUNT_HIGHT"))   ;
INPUT7_PULSE_COUNT_LOW					=  Get_RegID(_T("INPUT7_PULSE_COUNT_LOW"))   ;
INPUT8_PULSE_COUNT_HIGHT				=  Get_RegID(_T("INPUT8_PULSE_COUNT_HIGHT"))   ;
INPUT8_PULSE_COUNT_LOW					=  Get_RegID(_T("INPUT8_PULSE_COUNT_LOW"))   ;
INPUT9_PULSE_COUNT_HIGHT				=  Get_RegID(_T("INPUT9_PULSE_COUNT_HIGHT"))   ;
INPUT9_PULSE_COUNT_LOW					=  Get_RegID(_T("INPUT9_PULSE_COUNT_LOW"))   ;
INPUT10_PULSE_COUNT_HIGHT				=  Get_RegID(_T("INPUT10_PULSE_COUNT_HIGHT"))   ;
INPUT10_PULSE_COUNT_LOW					=  Get_RegID(_T("INPUT10_PULSE_COUNT_LOW"))   ;
DATE_STAMP_INPUT1_YEAR					=  Get_RegID(_T("DATE_STAMP_INPUT1_YEAR"))   ;
DATE_STAMP_INPUT1_MONTH					=  Get_RegID(_T("DATE_STAMP_INPUT1_MONTH"))   ;
DATE_STAMP_INPUT1_DAY					=  Get_RegID(_T("DATE_STAMP_INPUT1_DAY"))   ;
DATE_STAMP_INPUT1_HOUR					=  Get_RegID(_T("DATE_STAMP_INPUT1_HOUR"))   ;
DATE_STAMP_INPUT1_MINUTE				=  Get_RegID(_T("DATE_STAMP_INPUT1_MINUTE"))   ;
DATE_STAMP_INPUT2_YEAR					=  Get_RegID(_T("DATE_STAMP_INPUT2_YEAR"))   ;
DATE_STAMP_INPUT2_MONTH					=  Get_RegID(_T("DATE_STAMP_INPUT2_MONTH"))   ;
DATE_STAMP_INPUT2_DAY					=  Get_RegID(_T("DATE_STAMP_INPUT2_DAY"))   ;
DATE_STAMP_INPUT2_HOUR					=  Get_RegID(_T("DATE_STAMP_INPUT2_HOUR"))   ;
DATE_STAMP_INPUT2_MINUTE				=  Get_RegID(_T("DATE_STAMP_INPUT2_MINUTE"))   ;
DATE_STAMP_INPUT3_YEAR					=  Get_RegID(_T("DATE_STAMP_INPUT3_YEAR"))   ;
DATE_STAMP_INPUT3_MONTH					=  Get_RegID(_T("DATE_STAMP_INPUT3_MONTH"))   ;
DATE_STAMP_INPUT3_DAY					=  Get_RegID(_T("DATE_STAMP_INPUT3_DAY"))   ;
DATE_STAMP_INPUT3_HOUR					=  Get_RegID(_T("DATE_STAMP_INPUT3_HOUR"))   ;
DATE_STAMP_INPUT3_MINUTE				=  Get_RegID(_T("DATE_STAMP_INPUT3_MINUTE"))   ;
DATE_STAMP_INPUT4_YEAR					=  Get_RegID(_T("DATE_STAMP_INPUT4_YEAR"))   ;
DATE_STAMP_INPUT4_MONTH					=  Get_RegID(_T("DATE_STAMP_INPUT4_MONTH"))   ;
DATE_STAMP_INPUT4_DAY					=  Get_RegID(_T("DATE_STAMP_INPUT4_DAY"))   ;
DATE_STAMP_INPUT4_HOUR					=  Get_RegID(_T("DATE_STAMP_INPUT4_HOUR"))   ;
DATE_STAMP_INPUT4_MINUTE				=  Get_RegID(_T("DATE_STAMP_INPUT4_MINUTE"))   ;
DATE_STAMP_INPUT5_YEAR					=  Get_RegID(_T("DATE_STAMP_INPUT5_YEAR"))   ;
DATE_STAMP_INPUT5_MONTH					=  Get_RegID(_T("DATE_STAMP_INPUT5_MONTH"))   ;
DATE_STAMP_INPUT5_DAY					=  Get_RegID(_T("DATE_STAMP_INPUT5_DAY"))   ;
DATE_STAMP_INPUT5_HOUR					=  Get_RegID(_T("DATE_STAMP_INPUT5_HOUR"))   ;
DATE_STAMP_INPUT5_MINUTE				=  Get_RegID(_T("DATE_STAMP_INPUT5_MINUTE"))   ;
DATE_STAMP_INPUT6_YEAR					=  Get_RegID(_T("DATE_STAMP_INPUT6_YEAR"))   ;
DATE_STAMP_INPUT6_MONTH					=  Get_RegID(_T("DATE_STAMP_INPUT6_MONTH"))   ;
DATE_STAMP_INPUT6_DAY					=  Get_RegID(_T("DATE_STAMP_INPUT6_DAY"))   ;
DATE_STAMP_INPUT6_HOUR					=  Get_RegID(_T("DATE_STAMP_INPUT6_HOUR"))   ;
DATE_STAMP_INPUT6_MINUTE				=  Get_RegID(_T("DATE_STAMP_INPUT6_MINUTE"))   ;
DATE_STAMP_INPUT7_YEAR					=  Get_RegID(_T("DATE_STAMP_INPUT7_YEAR"))   ;
DATE_STAMP_INPUT7_MONTH					=  Get_RegID(_T("DATE_STAMP_INPUT7_MONTH"))   ;
DATE_STAMP_INPUT7_DAY					=  Get_RegID(_T("DATE_STAMP_INPUT7_DAY")) ;
DATE_STAMP_INPUT7_HOUR					=  Get_RegID(_T("DATE_STAMP_INPUT7_HOUR")) ;
DATE_STAMP_INPUT7_MINUTE				=  Get_RegID(_T("DATE_STAMP_INPUT7_MINUTE")) ;
DATE_STAMP_INPUT8_YEAR					=  Get_RegID(_T("DATE_STAMP_INPUT8_YEAR")) ;
#endif
#if 1
DATE_STAMP_INPUT8_MONTH					=  Get_RegID(_T("DATE_STAMP_INPUT8_MONTH")) ;
DATE_STAMP_INPUT8_DAY					=  Get_RegID(_T("DATE_STAMP_INPUT8_DAY")) ;
DATE_STAMP_INPUT8_HOUR					=  Get_RegID(_T("DATE_STAMP_INPUT8_HOUR")) ;
DATE_STAMP_INPUT8_MINUTE				=  Get_RegID(_T("DATE_STAMP_INPUT8_MINUTE")) ;
DATE_STAMP_INPUT9_YEAR					=  Get_RegID(_T("DATE_STAMP_INPUT9_YEAR")) ;
DATE_STAMP_INPUT9_MONTH					=  Get_RegID(_T("DATE_STAMP_INPUT9_MONTH")) ;
DATE_STAMP_INPUT9_DAY					=  Get_RegID(_T("DATE_STAMP_INPUT9_DAY")) ;
DATE_STAMP_INPUT9_HOUR					=  Get_RegID(_T("DATE_STAMP_INPUT9_HOUR")) ;
DATE_STAMP_INPUT9_MINUTE				=  Get_RegID(_T("DATE_STAMP_INPUT9_MINUTE")) ;
DATE_STAMP_INPUT10_YEAR					=  Get_RegID(_T("DATE_STAMP_INPUT10_YEAR")) ;
DATE_STAMP_INPUT10_MONTH				=  Get_RegID(_T("DATE_STAMP_INPUT10_MONTH")) ;
DATE_STAMP_INPUT10_DAY					=  Get_RegID(_T("DATE_STAMP_INPUT10_DAY")) ;
DATE_STAMP_INPUT10_HOUR					=  Get_RegID(_T("DATE_STAMP_INPUT10_HOUR")) ;
DATE_STAMP_INPUT10_MINUTE				=  Get_RegID(_T("DATE_STAMP_INPUT10_MINUTE")) ;
AUTO_MANUAL_OUTPUTS					    =  Get_RegID(_T("AUTO_MANUAL_OUTPUTS")) ;
ORIGIGINAL_AD_DATA_INPUT1				=  Get_RegID(_T("ORIGIGINAL_AD_DATA_INPUT1"));
ORIGIGINAL_AD_DATA_INPUT2				=  Get_RegID(_T("ORIGIGINAL_AD_DATA_INPUT2"))   ;
ORIGIGINAL_AD_DATA_INPUT3				=  Get_RegID(_T("ORIGIGINAL_AD_DATA_INPUT3"))   ;
ORIGIGINAL_AD_DATA_INPUT4				=  Get_RegID(_T("ORIGIGINAL_AD_DATA_INPUT4"))   ;
ORIGIGINAL_AD_DATA_INPUT5				=  Get_RegID(_T("ORIGIGINAL_AD_DATA_INPUT5"))   ;
ORIGIGINAL_AD_DATA_INPUT6				=  Get_RegID(_T("ORIGIGINAL_AD_DATA_INPUT6"))   ;
ORIGIGINAL_AD_DATA_INPUT7				=  Get_RegID(_T("ORIGIGINAL_AD_DATA_INPUT7"))   ;
ORIGIGINAL_AD_DATA_INPUT8				=  Get_RegID(_T("ORIGIGINAL_AD_DATA_INPUT8"))   ;
ORIGIGINAL_AD_DATA_INPUT9				=  Get_RegID(_T("ORIGIGINAL_AD_DATA_INPUT9"))   ;
ORIGIGINAL_AD_DATA_INPUT10				=  Get_RegID(_T("ORIGIGINAL_AD_DATA_INPUT10"))   ;
RANGE_INPUT1						=    Get_RegID(_T("RANGE_INPUT1")) ;
RANGE_INPUT2						=    Get_RegID(_T("RANGE_INPUT2")) ;
RANGE_INPUT3						=    Get_RegID(_T("RANGE_INPUT3")) ;
RANGE_INPUT4						=    Get_RegID(_T("RANGE_INPUT4")) ;
RANGE_INPUT5						=    Get_RegID(_T("RANGE_INPUT5")) ;
RANGE_INPUT6						=    Get_RegID(_T("RANGE_INPUT6")) ;
RANGE_INPUT7						=    Get_RegID(_T("RANGE_INPUT7")) ;
RANGE_INPUT8						=    Get_RegID(_T("RANGE_INPUT8")) ;
RANGE_INPUT9						=    Get_RegID(_T("RANGE_INPUT9")) ;
RANGE_INPUT10						=    Get_RegID(_T("RANGE_INPUT10")) ;
FILTER_INPUT1						=    Get_RegID(_T("FILTER_INPUT1")) ;
FILTER_INPUT2						=    Get_RegID(_T("FILTER_INPUT2")) ;
FILTER_INPUT3						=    Get_RegID(_T("FILTER_INPUT3")) ;
FILTER_INPUT4						=    Get_RegID(_T("FILTER_INPUT4")) ;
FILTER_INPUT5						=    Get_RegID(_T("FILTER_INPUT5")) ;
FILTER_INPUT6						=    Get_RegID(_T("FILTER_INPUT6")) ;
FILTER_INPUT7						=    Get_RegID(_T("FILTER_INPUT7")) ;
FILTER_INPUT8						=    Get_RegID(_T("FILTER_INPUT8")) ;
FILTER_INPUT9						=    Get_RegID(_T("FILTER_INPUT9")) ;
FILTER_INPUT10						=    Get_RegID(_T("FILTER_INPUT10")) ;
LIGHTING_ZONE_TIME_INPUT1				=  Get_RegID(_T("LIGHTING_ZONE_TIME_INPUT1"))   ;
LIGHTING_ZONE_TIME_INPUT2				=  Get_RegID(_T("LIGHTING_ZONE_TIME_INPUT2"))   ;
LIGHTING_ZONE_TIME_INPUT3				=  Get_RegID(_T("LIGHTING_ZONE_TIME_INPUT3"))   ;
LIGHTING_ZONE_TIME_INPUT4				=  Get_RegID(_T("LIGHTING_ZONE_TIME_INPUT4"))   ;
LIGHTING_ZONE_TIME_INPUT5				=  Get_RegID(_T("LIGHTING_ZONE_TIME_INPUT5"))   ;
LIGHTING_ZONE_TIME_INPUT6				=  Get_RegID(_T("LIGHTING_ZONE_TIME_INPUT6"))   ;
LIGHTING_ZONE_TIME_INPUT7				=  Get_RegID(_T("LIGHTING_ZONE_TIME_INPUT7"))   ;
LIGHTING_ZONE_TIME_INPUT8				=  Get_RegID(_T("LIGHTING_ZONE_TIME_INPUT8"))   ;
LIGHTING_ZONE_TIME_INPUT9				=  Get_RegID(_T("LIGHTING_ZONE_TIME_INPUT9"))   ;
LIGHTING_ZONE_TIME_INPUT10				=  Get_RegID(_T("LIGHTING_ZONE_TIME_INPUT10"))   ;
ZONE_TIME_LEFT_INPUT1					=  Get_RegID(_T("ZONE_TIME_LEFT_INPUT1"))   ;
ZONE_TIME_LEFT_INPUT2					=  Get_RegID(_T("ZONE_TIME_LEFT_INPUT2"))   ;
ZONE_TIME_LEFT_INPUT3					=  Get_RegID(_T("ZONE_TIME_LEFT_INPUT3"))   ;
ZONE_TIME_LEFT_INPUT4					=  Get_RegID(_T("ZONE_TIME_LEFT_INPUT4"))   ;
ZONE_TIME_LEFT_INPUT5					=  Get_RegID(_T("ZONE_TIME_LEFT_INPUT5"))   ;
ZONE_TIME_LEFT_INPUT6					=  Get_RegID(_T("ZONE_TIME_LEFT_INPUT6"))   ;
ZONE_TIME_LEFT_INPUT7					=  Get_RegID(_T("ZONE_TIME_LEFT_INPUT7"))   ;
ZONE_TIME_LEFT_INPUT8					=  Get_RegID(_T("ZONE_TIME_LEFT_INPUT8"))   ;
ZONE_TIME_LEFT_INPUT9					=  Get_RegID(_T("ZONE_TIME_LEFT_INPUT9"))   ;
ZONE_TIME_LEFT_INPUT10					=  Get_RegID(_T("ZONE_TIME_LEFT_INPUT10"))   ;
LC_DISABLE_ENABLE					    =  Get_RegID(_T("LC_DISABLE_ENABLE"))   ;
SELECT_INPUT_AS_TRIGGER_1				=  Get_RegID(_T("SELECT_INPUT_AS_TRIGGER_1"))   ;
SELECT_INPUT_AS_TRIGGER_2				=  Get_RegID(_T("SELECT_INPUT_AS_TRIGGER_2"))   ;
SELECT_INPUT_AS_TRIGGER_3				=  Get_RegID(_T("SELECT_INPUT_AS_TRIGGER_3"))   ;
SELECT_INPUT_AS_TRIGGER_4				=  Get_RegID(_T("SELECT_INPUT_AS_TRIGGER_4"))   ;
SELECT_INPUT_AS_TRIGGER_5				=  Get_RegID(_T("SELECT_INPUT_AS_TRIGGER_5"))   ;
SELECT_INPUT_AS_TRIGGER_6				=  Get_RegID(_T("SELECT_INPUT_AS_TRIGGER_6"))   ;
SELECT_INPUT_AS_TRIGGER_7				=  Get_RegID(_T("SELECT_INPUT_AS_TRIGGER_7"))   ;
SELECT_INPUT_AS_TRIGGER_8				=  Get_RegID(_T("SELECT_INPUT_AS_TRIGGER_8"))   ;
#endif




}

int  CDialogT3::Get_RegID(CString Name)
{
	Name.TrimLeft();
	Name.TrimRight();
   int regid=-1;
  vector<T3Register>::iterator iter;
  for (iter=m_vecT3Register.begin();iter!=m_vecT3Register.end();iter++)
  {
	  if (iter->regName==Name)
	  {
		  regid=iter->regID;
		  break;
	  }
  }
  return regid;
}
void CDialogT3::ShowDialogData()
{

	m_model = _T("T3-4AO");
	m_address= shmPRegister[MODBUS_ID];
	m_firmware=shmPRegister[FIRMWARE_VER_NUMBER];
	m_serial= shmPRegister[SN_LOW]+shmPRegister[SN_LOW+1]*256+shmPRegister[SN_HI]*256*256;+shmPRegister[SN_HI+1]*256*256*256;//get_serialnumber(); //Address 0-3
	m_hardware=shmPRegister[HARDWARE_VER_NUMBER];
 
	m_picVersion=shmPRegister[PIC_VER_NUMBER];



	
	 //==================================================Input============================================================
	//T3-4AO
	//address: input 119~138  INPUT1-10 119(high word) 120(low word)
	//address:Switch 
	//Range Setting for each input  200
	/*
	m_comboxRange.AddString(_T("RAW DATA"));
	m_comboxRange.AddString(_T("10K C"));
	m_comboxRange.AddString(_T("10K F"));
	m_comboxRange.AddString(_T("0-100%"));
	m_comboxRange.AddString(_T("ON/OFF"));
	m_comboxRange.AddString(_T("OFF/ON"));
	m_comboxRange.AddString(_T("Pulse Input"));
	m_comboxRange.AddString(_T("Lighting Control"));
	m_comboxRange.AddString(_T("TYPE3 10K C"));
	m_comboxRange.AddString(_T("TYPE3 10K F"));
	m_comboxRange.AddString(_T("NO USE"));
	m_comboxRange.AddString(_T("0-5V"));
	m_comboxRange.AddString(_T("0-10V"));
	m_comboxRange.AddString(_T("0-20I"));
	*/
	CString strresult;
	int regValue;
	for(int i = 1;i<=10;i++)
	{  
		 regValue=shmPRegister[INPUT1_PULSE_COUNT_HIGHT+2*(i-1)]*65535+shmPRegister[INPUT1_PULSE_COUNT_LOW+2*(i-1)];
		 strresult.Format(_T("%d"),regValue);
		 m_msflexgrid_input.put_TextMatrix(i,1,strresult);

		 strresult.Format(_T("%4d/%4d/%4d  %2d:%2d"),shmPRegister[DATE_STAMP_INPUT1_YEAR+3*(i-1)],shmPRegister[DATE_STAMP_INPUT1_MONTH+3*(i-1)],shmPRegister[DATE_STAMP_INPUT1_DAY+3*(i-1)],shmPRegister[DATE_STAMP_INPUT1_HOUR+3*(i-1)],shmPRegister[DATE_STAMP_INPUT1_MINUTE+3*(i-1)]);
		 m_msflexgrid_input.put_TextMatrix(i,2,strresult);
		 strresult.Format(_T("%d"),shmPRegister[RANGE_INPUT1+i-1]);
		 if (0==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("RAW DATA");
		 } 
		 else if (1==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("T10K C");
		 }
		 else if (2==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("10K F");
		 }
		 else if (3==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("0-100%");
		 }
		 else if (4==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("ON/OFF");
		 }
		 else if (5==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("OFF/ON");
		 }
		 else if (6==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("Pulse Input");
		 }
		 else if (7==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("Lighting Control");
		 }
		 else if (8==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("TYPE3 10K C");
		 }
		 else if (9==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("TYPE3 10K F");
		 }
		 else if (10==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("NO USE");
		 }
		 else if (11==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("0-5V");
		 }
		 else if (12==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("0-10V");
		 }
		 else if (13==shmPRegister[RANGE_INPUT1+i-1])
		 {
		 strresult=_T("0-20I");
		 }

	    m_msflexgrid_input.put_TextMatrix(i,3,strresult);
		strresult.Format(_T("%d"),shmPRegister[FILTER_INPUT1+i-1]);
		m_msflexgrid_input.put_TextMatrix(i,4,strresult);
	}


	//====================================================Output============================================================	
	//T3-4AO OUTPUT
	//address: input 100~110   OUTPUT1-12
	//100~111

	CString CstresultDO;
	for(int i = 1;i<=8;i++)
	{  
		// 		CstresultDO.Format(_T("%d"),10);
		// 		m_msflexgridDO.put_TextMatrix(i,2,CstresultDO);
		// 		m_msflexgridDO.put_TextMatrix(i,1,_T("east door"));
		// 		m_msflexgridDO.put_TextMatrix(i,3,_T("AUTO"));
		if(shmPRegister[OUTPUT1_RELAY+i-1]!=0)
		{
		  CstresultDO=_T("ON");
		}
		else{
		  CstresultDO=_T("OFF");
		}
		
		m_msflexgrid_output.put_TextMatrix(i,1,CstresultDO);
		//m_msflexgrid_output.put_TextMatrix(i,2,vect3model.at(100-1+i).CStName);
		
		if (i<=4)
		{
		  CstresultDO.Format(_T("%d"),shmPRegister[OUTPUT1_ANALOG+i-1]);
		  m_msflexgrid_output.put_TextMatrix(i,2,CstresultDO);
		}

	}
   UpdateData(FALSE);


#if 0
		//183					1		Range Setting for each input.
		//184					1		183 correspond to input1,
		//185					1		184 correspond to input2, etc.
		//186					1		0 = raw data,
		//187					1		1 = 10K Celsius,
		//188					1		2 = 10K Fahrenheit,
		//189					1		3 = 0-100%
		//190					1		4 = ON/OFF,
		//								5 = OFF/ON
		//								6 = Pulse Input
		//								7 = Lighting Control
	for (int i = 183;i<=190;i++)
	{	
		int retRangSet =write_one(g_tstat_id,i,1);
		TRACE(_T("retRangSet=%d\n"),retRangSet);
	}
	


	CString strinput,stroutput,strfilter;
	//Input Register address 118-133  eg:input1-> 118 high word   119 low word
	int numrow = 1;
	for (int i = 1;i<=16;i++)
	{
		int temphigh = multi_register_value[i+117]<<16;
		i++;
		int templow = multi_register_value[i+117]&0x0000ffff;
		int temp = temphigh|templow;

		strinput.Format(_T("%d°C"),temp);
		m_msflexgrid_input.put_TextMatrix(numrow,2,strinput);
		//Input Filter address 191-198
		strfilter.Format(_T("%d"),multi_register_value[190+numrow]);
		m_msflexgrid_input.put_TextMatrix(numrow,3,strfilter);
		numrow++;
		
	

	}

	//Output Register address 100-112
	for (int i = 1;i<=13;i++)
	{
		if (multi_register_value[i+99] == 0)
		{
			m_msflexgrid_output.put_TextMatrix(i,2,_T("Off"));
		} 
		else if(multi_register_value[i+99] > 0)
		{
			m_msflexgrid_output.put_TextMatrix(i,2,_T("On"));
		}

	}

	//显示Auto/Manual
	//116	2	Switch Status, 1 Register output 1-8
	//117	2	Switch Status, 2 Register output 9-13
	//当output全按至HAND时，116 117的二进制值：
	//					 high 8 bit   |	  low 8 bit							
	//HAND INPUT		 1   2   3   4   5   6  7    8			  9  10  11  12  13
//register:116 117     0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1			0 1 0 1 0 1 0 1 0 1 0 0 0 0 0 0


    //当output全按至AUTO时，116 117的二进制值：
	//AUTO INPUT	   1   2   3   4   5   6   7   8			9  10  11  12  13
//register;116 117	   1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0			1 0 1 0 1 0 1 0 1 0 0 0 0 0 0 0

	int temp = 1; 

	for (int i = 8;i>=1;i--)
	{
		//Manual
		if((multi_register_value[116]&temp)!= 0)
			m_msflexgrid_output.put_TextMatrix(i,3,_T("Manual"));
			temp = temp<<1;
		if ((multi_register_value[116]&temp)!=0)
			m_msflexgrid_output.put_TextMatrix(i,3,_T("Auto"));
		temp = temp<<1;
	}

	temp = 1;
	temp = temp<<6;
	for (int i = 13;i>=9;i--)
	{
		
		//Manual
		if((multi_register_value[117]&temp)!= 0)
			m_msflexgrid_output.put_TextMatrix(i,3,_T("Manual"));	
		temp = temp<<1;
		if ((multi_register_value[117]&temp)!=0)
			m_msflexgrid_output.put_TextMatrix(i,3,_T("Auto"));
		temp = temp<<1;
	}

#endif

}

 
void CDialogT3::OnTimer(UINT_PTR nIDEvent)
{
// 	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
//	CView* pNewView = pMain->m_pViews[5];
//	CView* pActiveView = pMain->GetActiveView();
//
//	if ( pNewView != pActiveView )    // current is minipanel dialog
//	{
//		KillTimer(T3_MODEL);
//	}
//
//#if 1
//	static int nCount = 0,nCount2 = 0;
//
//	nCount++;
//	nCount2++;
//
//	if (T3_MODEL == nIDEvent)
//	{
//		CTime now;
//		now = CTime::GetCurrentTime();
//		//m_datetime = now.Format(_T("%A,%B %d,%Y  %H:%M:%S"));		// "Saturday, April 12, 2012"		
//
//		//UpdateData(FALSE);
//
//		if (nCount% 30 == 0)
//		{
//
//			nCount = 0;
//			BOOL flag=MiniUpdateData();
//
//			if (flag)
//			{
//				ShowDialogData();
//				SetPaneString(2,_T("Online!"));
//
//			}
//
//		}
//		if (nCount2%60*10 == 0)
//		{
//			nCount2 = 0;
//			//Updatedatabase();
//
//		}
//
//
//	}
//
//#endif

	CFormView::OnTimer(nIDEvent);
}


BOOL CDialogT3::MiniUpdateData()
{
	KillTimer(T3_MODEL);
#if 1
	//读取串口号，从数据库
	//open_com(comnum);

	int sumret = 0;
BOOL flag = FALSE;

	////读取miniPanel数据
	BOOL ret_bool = FALSE;
	//long start = clock(),end(0); 		


	memset(&shmPRegister,0,sizeof(shmPRegister)); 
	for(int i=0;i<4;i++)
	{
		int ret = Read_Multi(g_tstat_id,&shmPRegister[i*64],i*64,64,2);
		sumret += ret;

		if (sumret<0)
		{
			TRACE(_T("write_multi ret sum = %d\n"),sumret);
			SetTimer(MiniPanelTimer,1000,NULL);
			return FALSE;
		}

	}



	


	SetTimer(T3_MODEL,1000,NULL);
#endif
	return flag;

}

void CDialogT3::Updatedatabase()
{
	TRACE(_T("Updatedatabase()start!\n"));
	KillTimer(T3_MODEL);
	//存入数据库中
	CADO saveADO;
	saveADO.OnInitADOConn();
	CString sql = _T("select * from T3_4AO");
	saveADO.m_pRecordset = saveADO.OpenRecordset(sql);
	if (!saveADO.m_pRecordset->EndOfFile)
	{
		saveADO.m_pRecordset->MoveFirst();

		for(int i = 0;i<230;i++)
		{
			try 
			{

				//saveADO.m_pRecordset->AddNew();
				//saveADO.m_pRecordset->Move(pos,vtMissing);
				// 			cstnum.Format(_T("%d"),i+1);
				// 			saveADO.m_pRecordset->PutCollect("Address",(_bstr_t)cstnum);
				// 			saveADO.m_pRecordset->PutCollect("Name",(_bstr_t)_T("output"));

				saveADO.m_pRecordset->PutCollect("Address",(_bstr_t)(i+1));
				saveADO.m_pRecordset->PutCollect("Name",(_bstr_t)vect3model.at(i).CStName);
				saveADO.m_pRecordset->PutCollect("Data",(_bstr_t)shmPRegister[i]);
				saveADO.m_pRecordset->PutCollect("Range",(_bstr_t)vect3model.at(i).CStrange);


				if (i>=124&&i<=147)
				{
					switch ((int)(_variant_t)shmPRegister[i])
					{
					case 0:   //0- off 1 - mannel 2 - auto
						saveADO.m_pRecordset->PutCollect("AutoManual",(_bstr_t)"OFF");
						break;
					case 1:
						saveADO.m_pRecordset->PutCollect("AutoManual",(_bstr_t)"Manual");
						break;
					case 2:
						saveADO.m_pRecordset->PutCollect("AutoManual",(_bstr_t)"Auto");
						break;
					default:
						break;

					}
				}


				//  		saveADO.m_pRecordset->PutCollect("Auto/Manual",(_bstr_t)"auto");
				//  		saveADO.m_pRecordset->PutCollect("Range",(_bstr_t)"10%-100%");
				saveADO.m_pRecordset->Update();
				saveADO.m_pRecordset->MoveNext();
			}


			catch(...)
			{
				//MessageBox(_T("Write dababase false!"));

				SetPaneString(2,_T("Write dababase false!"));
				return ;
			}
		}
	}
	else
	{
		for(int i = 0;i<230;i++)
		{
			try 
			{

				//saveADO.m_pRecordset->AddNew();
				//saveADO.m_pRecordset->Move(pos,vtMissing);
				// 			cstnum.Format(_T("%d"),i+1);
				// 			saveADO.m_pRecordset->PutCollect("Address",(_bstr_t)cstnum);
				// 			saveADO.m_pRecordset->PutCollect("Name",(_bstr_t)_T("output"));
				saveADO.m_pRecordset->AddNew();
				saveADO.m_pRecordset->PutCollect("Address",(_bstr_t)(i+1));
				saveADO.m_pRecordset->PutCollect("Name",(_bstr_t)vect3model.at(i).CStName);
				saveADO.m_pRecordset->PutCollect("Data",(_bstr_t)shmPRegister[i]);
				saveADO.m_pRecordset->PutCollect("Range",(_bstr_t)vect3model.at(i).CStrange);


				if (i>=124&&i<=147)
				{
					switch ((int)(_variant_t)shmPRegister[i])
					{
					case 0:   //0- off 1 - mannel 2 - auto
						saveADO.m_pRecordset->PutCollect("AutoManual",(_bstr_t)"OFF");
						break;
					case 1:
						saveADO.m_pRecordset->PutCollect("AutoManual",(_bstr_t)"Manual");
						break;
					case 2:
						saveADO.m_pRecordset->PutCollect("AutoManual",(_bstr_t)"Auto");
						break;
					default:
						break;

					}
				}


				//  		saveADO.m_pRecordset->PutCollect("Auto/Manual",(_bstr_t)"auto");
				//  		saveADO.m_pRecordset->PutCollect("Range",(_bstr_t)"10%-100%");
				saveADO.m_pRecordset->Update();

			}


			catch(...)
			{
				//MessageBox(_T("Write dababase false!"));

				SetPaneString(2,_T("Write dababase false!"));
				return ;
			}
		}

	}

	saveADO.CloseRecordset();
	saveADO.CloseConn(); 

	SetTimer(T3_MODEL,1000,NULL);

}

void CDialogT3::ClickInputMsflexgrid()
{
	KillTimer(T3_MODEL);
	m_comboxRange.ShowWindow(FALSE);
	UpdateData(FALSE);

	long lRow,lCol;
	lRow = m_msflexgrid_input.get_RowSel();//获取点击的行号	
	lCol = m_msflexgrid_input.get_ColSel(); //获取点击的列号
	TRACE(_T("Click input grid!\n"));

	CRect rect;
	m_msflexgrid_input.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_msflexgrid_input.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_msflexgrid_input.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_msflexgrid_input.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_msflexgrid_input.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_msflexgrid_input.get_TextMatrix(lRow,lCol);


	
	if((0==lCol||4==lCol) && lRow != 0)
	{
		 
		m_inNameEdt.MoveWindow(&rcCell,1);
		m_inNameEdt.ShowWindow(SW_SHOW);
		m_inNameEdt.SetWindowText(strValue);
		m_inNameEdt.SetFocus();
		m_inNameEdt.SetCapture();//LSC
		int nLenth=strValue.GetLength();
		m_inNameEdt.SetSel(nLenth,nLenth); //全选//

	}
	if((3==lCol)&&lRow!=0)
	{


		m_comboxRange.MoveWindow(&rcCell,1);
	    m_comboxRange.ResetContent();
		m_comboxRange.AddString(_T("RAW DATA"));
		m_comboxRange.AddString(_T("10K C"));
		m_comboxRange.AddString(_T("10K F"));
		m_comboxRange.AddString(_T("0-100%"));
		m_comboxRange.AddString(_T("ON/OFF"));
		m_comboxRange.AddString(_T("OFF/ON"));
		m_comboxRange.AddString(_T("Pulse Input"));
		m_comboxRange.AddString(_T("Lighting Control"));
		m_comboxRange.AddString(_T("TYPE3 10K C"));
		m_comboxRange.AddString(_T("TYPE3 10K F"));
		m_comboxRange.AddString(_T("NO USE"));
		m_comboxRange.AddString(_T("0-5V"));
		m_comboxRange.AddString(_T("0-10V"));
		m_comboxRange.AddString(_T("0-20I"));

		m_comboxRange.ShowWindow(SW_SHOW);

 
 
		m_comboxRange.BringWindowToTop();
	 
		m_comboxRange.SetFocus(); //获取焦点
		m_comboxRange.SetWindowText(strValue);

		//m_comboxRange.SetWindowText(strValue);
		//m_comboxRange.SetFocus();
		//m_comboxRange.SetCapture();//LSC
		//int nLenth=strValue.GetLength();
//		m_comboxRange.SetSel(nLenth,nLenth); //全选//


	 

	}
}
void CDialogT3::ClickOutputMsflexgrid()
{
	long lRow,lCol;
	lRow = m_msflexgrid_output.get_RowSel();//获取点击的行号	
	lCol = m_msflexgrid_output.get_ColSel(); //获取点击的列号
	TRACE(_T("Click input grid!\n"));

	CRect rect;
	m_msflexgrid_output.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_msflexgrid_output.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_msflexgrid_output.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_msflexgrid_output.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_msflexgrid_output.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_msflexgrid_output.get_TextMatrix(lRow,lCol);


	if(1==lCol && lRow != 0)
	{
		//return; // 2012.2.7老毛说不允许修改
		m_inNameEdt.MoveWindow(&rcCell,1);
		m_inNameEdt.ShowWindow(SW_SHOW);
		m_inNameEdt.SetWindowText(strValue);
		m_inNameEdt.SetFocus();
		m_inNameEdt.SetCapture();//LSC
		int nLenth=strValue.GetLength();
		m_inNameEdt.SetSel(nLenth,nLenth); //全选//

	}

}

void CDialogT3::OnCbnSelchangeType(){
	BeginWaitCursor();
	CString strTemp;
	int lRow = m_msflexgrid_input.get_RowSel();	
	int lCol = m_msflexgrid_input.get_ColSel();
	//if ((strText.GetLength()<=0)||strText.CompareNoCase(m_tempGridString)==0)
	//{
	//	return;
	//}
	int sel=m_comboxRange.GetCurSel();
	m_comboxRange.ShowWindow(FALSE);
	if (shmPRegister[RANGE_INPUT1+lRow-1]==sel)
	{
	return;
	}
	int ret=write_one(g_tstat_id,RANGE_INPUT1+lRow-1,sel);
	if (ret>0)
	{
	   shmPRegister[RANGE_INPUT1+lRow-1]=sel;
	}
	ShowDialogData();
	EndWaitCursor();
}