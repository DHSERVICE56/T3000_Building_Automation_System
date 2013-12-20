
// CLightingController message handlers
// E:\T3000\trunk\T3000\LightingController\LightingController.cpp : implementation file
//

#include "stdafx.h"
#include "..\T3000.h"
#include "..\LightingController\LightingController.h"
#include "..\MainFrm.h"

static BOOL ifLCdb = TRUE;
static BOOL ifUpdatedb = TRUE;

//#define  IDC_BUTTON_Configure 7001
UINT _Read_LightingStatus(LPVOID pParam){
CLightingController* pFrame=(CLightingController*)(pParam);
while(TRUE){

if (pFrame)
{
	if (!is_connect())
	{
		Sleep(5000);
		continue;
	}
	Sleep(2000);
	int  Status1=read_one(g_tstat_id,14600);
	int  Status2=read_one(g_tstat_id,14601);
	for (int i=0;i<16;i++)
	{
		pFrame->m_light[i]=(Status1>>i) &0x01;
	}
	for (int i=16;i<32;i++)
	{
		pFrame->m_light[i]=(Status2>>(i-16)) &0x01;
	}
}
break;
}
return 1;
}




// CLightingController
#define LIGHTINGCONTROLLERTIMER 1

IMPLEMENT_DYNCREATE(CLightingController, CFormView)

CLightingController::CLightingController()
	: CFormView(CLightingController::IDD)
	, m_inaddress(0)
	, m_inSerialNum(0)
	, m_flFirmware(0)
	, m_inHardware(0)
	, m_CStrModel(_T(""))
	, m_inBaudrate(0)
	, m_datetime(_T(""))
	, m_strcomboBox(_T(""))
	, row(0)
	, col(0)
	,m_outputsum(0)
 	, m_date(COleDateTime::GetCurrentTime())
 	,m_time(COleDateTime::GetCurrentTime())
	,m_strcomboboxpanel(_T(""))
	,m_strcomboboxoutput(_T(""))
	,m_strcomboboxoutputboard(_T(""))
	,outputno(0)
	,comnum(0)
	, m_nListenPort(6001)
{
m_GridNo=-1;
LC_Thread=NULL;
}

CLightingController::~CLightingController()
{

}

void CLightingController::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_inaddress);
	DDX_Text(pDX, IDC_EDIT3, m_inSerialNum);
	DDX_Text(pDX, IDC_EDIT2, m_flFirmware);
	DDX_Text(pDX, IDC_EDIT4, m_inHardware);
	DDX_Text(pDX, IDC_EDIT5, m_CStrModel);
	DDX_Text(pDX, IDC_EDIT6, m_inBaudrate);
	DDX_Text(pDX, IDC_STATIC1, m_datetime);
	DDX_Text(pDX, IDC_EDIT_ListeningPort, m_nListenPort);
	DDX_Control(pDX, IDC_CHECKEnableEdit, m_ReadOnlyCheckBtn);
	DDX_Control(pDX, IDC_LIST_INPUT, m_ListBox);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_msflexgrid1to96);

	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
	DDX_CBString(pDX, IDC_COMBO1, m_strcomboBox);
	DDX_Control(pDX, IDC_EDIT_MODIFYNAME, m_editName);

	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DATE, m_date);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME, m_time);
	DDX_Control(pDX,IDC_COMBO_panel,m_comboboxpanel);
	DDX_Control(pDX,IDC_COMBO_outputboard,m_comboboxouputboard);
	DDX_Control(pDX,IDC_COMBO_output,m_comboboxoutput);

	DDX_Text(pDX,IDC_COMBO_panel,m_strcomboboxpanel);
	DDX_Text(pDX,IDC_COMBO_outputboard,m_strcomboboxoutputboard);
	DDX_Text(pDX,IDC_COMBO_output,m_strcomboboxoutput);

	DDX_Control(pDX, IDC_IPADDRESS_IP, m_ip_addressCtrl);
	DDX_Control(pDX, IDC_IPADDRESS_Subnet, m_subnet_addressCtrl);
	DDX_Control(pDX, IDC_IPADDRESS_Gateway, m_gateway_addressCtrl);
	DDX_Control(pDX, IDC_EDIT_ListeningPort, m_listenPortEdit);

	DDX_Control(pDX, IDC_COMBO_IPModel, m_ipModelComBox);
	DDX_Control(pDX, IDC_COMBO_TIMESERVERLIST2, m_timeserver);
	DDX_Control(pDX, IDC_MAC_ADDRESS, m_macaddress);

	DDX_Control(pDX, IDC_MSFLEXGRID4, m_inputs_grid);
	DDX_Control(pDX, IDC_COMBO5, m_combox_controler);
	DDX_Control(pDX,IDC_SAVEDELAYTIME,m_savedelaytime);
}

BEGIN_MESSAGE_MAP(CLightingController, CFormView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_LIST_INPUT, &CLightingController::OnLbnDblclkListInput)
	ON_LBN_SELCHANGE(IDC_LIST_INPUT, &CLightingController::OnLbnSelchangeListInput)
	ON_WM_CONTEXTMENU()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLightingController::OnCbnSelchangeCombo1)
	ON_EN_KILLFOCUS(IDC_EDIT_MODIFYNAME, &CLightingController::OnEnKillfocusEditModifyname)
	ON_EN_KILLFOCUS(IDC_SAVEDELAYTIME, &CLightingController::OnEnKillfocusEditTimeSaveDelay)
	ON_COMMAND(ID_SETMAPPING_ADDOUTPUTBAROD, &CLightingController::OnSetmappingAddoutputbarod)
	ON_COMMAND(ID_SETMAPPING_PREVIOUS, &CLightingController::OnSetmappingPrevious)
	ON_COMMAND(ID_SETMAPPING_NEXT, &CLightingController::OnSetmappingNext)
	ON_COMMAND(ID_SETMAPPING_READ, &CLightingController::OnSetmappingRead)
	ON_COMMAND(ID_SETMAP_SETTINGSAVE, &CLightingController::OnSetmapSettingsave)
	ON_COMMAND(ID_SETMAPPING_SAND, &CLightingController::OnSetmappingSand)
	ON_BN_CLICKED(IDC_BUTTON_Lightingcontorl_WeeklyS, &CLightingController::OnBnClickedButtonLightingcontorlWeeklys)
	ON_BN_CLICKED(IDC_BUTTON_LightingControlAnnualS, &CLightingController::OnBnClickedButtonLightingcontorlAnnuals)
	ON_BN_CLICKED(IDC_BUTTON_LightingControlGroupS, &CLightingController::OnBnClickedButtonLightingcontorlGroups)
	ON_BN_CLICKED(IDC_BUTTON_Syncwithpc, &CLightingController::OnBnClickedButtonLightingcontorlSyncwithPC)
	ON_BN_CLICKED(IDC_SAVEALL, &CLightingController::OnBnClickedButtonSaveAll)

	ON_BN_CLICKED(IDC_BUTTON_ADD, &CLightingController::OnBnClickedButtonAdd)
	ON_MESSAGE(WM_WRITE_MESSAGE,OnWriteMessage)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CLightingController::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_Reboot, &CLightingController::OnBnClickedButtonReboot)
	ON_BN_CLICKED(IDC_BUTTON_ResetToFactory, &CLightingController::OnBnClickedButtonResetToFactory)
		 
	ON_BN_CLICKED(IDC_CHECKEnableEdit, &CLightingController::OnBnClickedCheckEnableEdit)
	ON_BN_CLICKED(IDC_BUTTON_Configure, &CLightingController::OnBnClickedButtonConfigure)


	ON_CBN_SELCHANGE(IDC_COMBO5, &CLightingController::OnCbnSelchangeValuecombo)

//	ON_BN_CLICKED(IDC_BUTTON_Configureswitch, &CLightingController::OnBnClickedButtonConfigureswitch)
END_MESSAGE_MAP()


// CLightingController diagnostics

#ifdef _DEBUG
void CLightingController::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLightingController::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLightingController message handlers

void CLightingController::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	for(int i=0;i<32;i++){
	m_light[i]=FALSE;
	}
	CString strtmp;
	for(int i = 1;i<=200;i++)
	{
		strtmp.Format(_T("%d"),i);
		m_comboboxpanel.AddString(strtmp);

	}
	for (int i = 1;i<=24;i++)
	{
		strtmp.Format(_T("%d"),i);
		m_comboboxouputboard.AddString(strtmp);

	}
	for (int i = 1;i<=20;i++)
	{
		strtmp.Format(_T("%d"),i);
		m_comboboxoutput.AddString(strtmp);

	}




	//初始化MSFLXGRID控件

#if 1
	//设置排/行数量
	m_msflexgrid1to96.put_Cols(4);//这句一定要有，如果下面再增加显示一外，而这里不变还是5，则会出错
	m_msflexgrid1to96.put_Rows(200);//包括标题栏


// 	long n1,n2(0);
// 	n1 = clock();
	//显示横标题
	m_msflexgrid1to96.put_TextMatrix(0,0,_T("Panels"));
	m_msflexgrid1to96.put_TextMatrix(0,1,_T("Card_ID"));
	m_msflexgrid1to96.put_TextMatrix(0,2,_T("No."));
	m_msflexgrid1to96.put_TextMatrix(0,3,_T("OutputName"));

	//设置列宽	
	/*m_msflexgrid1to96.put_ColWidth(0,0);*/
	m_msflexgrid1to96.put_ColWidth(1,700);
	m_msflexgrid1to96.put_ColWidth(2,500);
 	m_msflexgrid1to96.put_ColWidth(3,1500);


	for (int col=0;col<4;col++)//11
	{ 
		m_msflexgrid1to96.put_ColAlignment(col,4);
		//第二个参数数是功能码，就是表示文字居中显示而已，没别的意思
	}




#endif


//Inputs 
#if 1
	//设置排/行数量
	m_inputs_grid.put_Cols(3);//这句一定要有，如果下面再增加显示一外，而这里不变还是5，则会出错
	m_inputs_grid.put_Rows(25);//包括标题栏


	// 	long n1,n2(0);
	// 	n1 = clock();
	//显示横标题
	m_inputs_grid.put_TextMatrix(0,0,_T("Inputs"));
	m_inputs_grid.put_TextMatrix(0,1,_T("Status"));
	m_inputs_grid.put_TextMatrix(0,2,_T("Auto/Manual"));
	 

	//设置列宽	
	m_inputs_grid.put_ColWidth(0,1500);
	m_inputs_grid.put_ColWidth(1,700);
	m_inputs_grid.put_ColWidth(2,1500);
 


	for (int col=0;col<3;col++)//11
	{ 
		m_inputs_grid.put_ColAlignment(col,4);//第二个参数数是功能码，就是表示文字居中显示而已，没别的意思
	}
#endif

}
void CLightingController::Fresh_ListBox(){
    unsigned short sys_data[4];
    CString str_input;
	Read_Multi(g_tstat_id,&sys_data[0],0,4);
	m_sn=sys_data[0]+sys_data[1]*256+sys_data[2]*256*256+sys_data[3]*256*256*256;
	for(int i=0;i<24;i++)
	{

		str_input=Get_Table_Name(m_sn,_T("Input"),i+1);
		m_ListBox.InsertString(i,str_input);
	}
}
void CLightingController::Fresh()
{
	CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pMain->m_pFreshMultiRegisters->SuspendThread();
	pMain->m_pRefreshThread->SuspendThread();

	//m_inaddress = 255;//界面上Address的值,在这里除显示没任何意义
	UpdateData(FALSE);
	
	if(ifLCdb)//T3000启动仅执行一次
	{


		m_strcomboBox = _T("Input-Output-Map");
		m_comboBox.SetCurSel(1);//input 1 group 0
		m_ListBox.SetCurSel(0);//列表默认项设为第一项
		m_CStrModel = _T("Lighting Controller");
		GetDlgItem(IDC_EDIT5)->SetWindowText(m_CStrModel);
		
		ifLCdb = FALSE;
	}


	Fresh_ListBox();
	SetPaneString(2,_T("Online!"));

	m_comboboxoutput.SetCurSel(0);
	m_comboboxouputboard.SetCurSel(0);
	m_comboboxpanel.SetCurSel(0);
	 if (!is_connect())
	{
	SetPaneString(1,_T("NO CONNECTION"));
	}
	Fresh_System();
	Fresh_Inputs();

	//Read_Input_Group_Mapping();

 //   LC_Thread=AfxBeginThread(_Read_LightingStatus,this);
	SetTimer(LIGHTINGCONTROLLERTIMER,1000,NULL);

}
void CLightingController::Read_Input_Group_Mapping()
{
	unsigned short Output_Units_Data[60];
	Mapping_Struct temp;
	int OutputNameAddress=0;
	for (int swith=0;swith<24;swith++)
	{
		Read_Multi(g_tstat_id,Output_Units_Data,300+60*swith,60);
		temp.Address=swith+1;
		for (int out=0;out<20;out++)
		{
			temp.Card_ID=Output_Units_Data[3*out];
			for (int OutputAddress=1;OutputAddress<=32;OutputAddress++)
			{     
				int RegisterValue;
				int Position;
				if (OutputAddress<=16)
				{
					RegisterValue=Output_Units_Data[3*out+1];
					Position=OutputAddress;
				}
				else
				{
					RegisterValue=Output_Units_Data[3*out+2];
					Position=OutputAddress-16;
				}
				if(Get_Bit_FromRegister(RegisterValue,Position))//如果是1的话，那么就加入到Mapping中去
				{
					temp.OuputNo=OutputAddress;
					temp.OutputNameAddress=OutputNameAddress;
					OutputNameAddress++;
					m_input_output_mapping.push_back(temp);
				}
			}
		}
	
	}



	for (int swith=0;swith<40;swith++)
	{
		Read_Multi(g_tstat_id,Output_Units_Data,1740+60*swith,60);
		temp.Address=swith+1;
		for (int out=0;out<20;out++)
		{
			temp.Card_ID=Output_Units_Data[3*out];
			for (int OutputAddress=1;OutputAddress<=32;OutputAddress++)
			{     
				int RegisterValue;
				int Position;
				if (OutputAddress<=16)
				{
					RegisterValue=Output_Units_Data[3*out+1];
					Position=OutputAddress;
				}
				else
				{
					RegisterValue=Output_Units_Data[3*out+2];
					Position=OutputAddress-16;
				}
				if(Get_Bit_FromRegister(RegisterValue,Position))//如果是1的话，那么就加入到Mapping中去
				{
					temp.OuputNo=OutputAddress;
					temp.OutputNameAddress=OutputNameAddress;
					OutputNameAddress++;
					m_group_output_mapping.push_back(temp);
				}
			}
		}
	}

	 
}
void CLightingController::Fresh_Inputs(){
CString temp;

 
	unsigned short temp_MB[24],temp_AM[24];
   Read_Multi(g_tstat_id,&temp_MB[0],INPUT_MB,24);
   Read_Multi(g_tstat_id,&temp_AM[0],INPUT_AM,24);
  for (int row=1;row<=24;row++)
  {
   temp=Get_Table_Name(m_sn,_T("Input"),row);
   m_inputs_grid.put_TextMatrix(row,0,temp);

   if (temp_MB[row-1]!=0)
   {
	   temp=_T("ON");
   } 
   else
   {
	   temp=_T("OFF");
   }
   m_inputs_grid.put_TextMatrix(row,1,temp);

  if (temp_AM[row-1]!=0)
  {
    temp=_T("MAN");
  } 
  else
  {
    temp=_T("AUTO");
  }
   m_inputs_grid.put_TextMatrix(row,2,temp);
  }


}
void CLightingController::Fresh_System(){
unsigned short sys_data[100];
CString strtemp;
Read_Multi(g_tstat_id,&sys_data[0],0,20);
strtemp.Format(_T("%d"),sys_data[6]);
GetDlgItem(IDC_EDIT1)->SetWindowTextW(strtemp);//Address ID

strtemp.Format(_T("%d"),sys_data[0]+sys_data[1]*256+sys_data[2]*256*256+sys_data[3]*256*256*256);

GetDlgItem(IDC_EDIT3)->SetWindowTextW(strtemp);//Serial NO
strtemp.Format(_T("%d"),sys_data[8]);
GetDlgItem(IDC_EDIT4)->SetWindowTextW(strtemp);//Hardware
 
 
float firmware=((float) sys_data[4])/100.0+(float)sys_data[5];
strtemp.Format(_T("%0.2f"),firmware);
GetDlgItem(IDC_EDIT2)->SetWindowTextW(strtemp);

m_timeserver.SetCurSel(0);
m_timeserver.EnableWindow(FALSE);


Read_Multi(g_tstat_id,&sys_data[0],100,21);
strtemp.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),sys_data[0],sys_data[1],sys_data[102],sys_data[3],sys_data[4],sys_data[5]);
m_macaddress.SetWindowText(strtemp);
m_ipModelComBox.SetCurSel(sys_data[6]);
m_ip_addressCtrl.SetAddress((BYTE)sys_data[7],(BYTE)sys_data[8],(BYTE)sys_data[9],(BYTE)sys_data[10]);
m_subnet_addressCtrl.SetAddress((BYTE)sys_data[11],(BYTE)sys_data[12],(BYTE)sys_data[13],(BYTE)sys_data[14]);
m_gateway_addressCtrl.SetAddress((BYTE)sys_data[15],(BYTE)sys_data[16],(BYTE)sys_data[17],(BYTE)sys_data[18]);
strtemp.Format(_T("%d"),sys_data[20]);
m_listenPortEdit.SetWindowText(strtemp);
OnBnClickedCheckEnableEdit();
strtemp.Format(_T("%d"),read_one(g_tstat_id,92));
m_savedelaytime.SetWindowText(strtemp);

}


void CLightingController::ShowLighContDlg()
{
	m_CStrModel = _T("Lighting Controller");

	m_inaddress = _wtoi(veclightingcontroller.at(6).CStvalue);

	UINT nSelectSerialNumber = _wtoi(veclightingcontroller.at(0).CStvalue)+_wtoi(veclightingcontroller.at(1).CStvalue)*256+_wtoi(veclightingcontroller.at(2).CStvalue)*256*256+_wtoi(veclightingcontroller.at(3).CStvalue)*256*256*256;
	m_inSerialNum=nSelectSerialNumber;

	m_flFirmware = ((float)(_wtoi(veclightingcontroller.at(5).CStvalue)*256+_wtoi(veclightingcontroller.at(4).CStvalue)))/10;

	m_inHardware = _wtoi(veclightingcontroller.at(8).CStvalue);
	int intemp = _wtoi(veclightingcontroller.at(12).CStvalue);
	if (intemp == 0)
		m_inBaudrate = 9600;
	else if (intemp == 1)
		m_inBaudrate = 19200;


	//显示输出名称
	for (int i = 1;i<33;i++)
	{	
		m_msflexgrid1to96.put_TextMatrix(i,1,m_vecONS.at(i-1).Name);
		m_msflexgrid1to96.put_TextMatrix(i,2,m_vecONS.at(i-1).Status);
	}
	for (int i = 33;i<65;i++)
	{	
		m_msflexgrid1to96.put_TextMatrix(i-32,5,m_vecONS.at(i-1).Name);
		m_msflexgrid1to96.put_TextMatrix(i-32,6,m_vecONS.at(i-1).Status);
	}
	for (int i = 65;i<97;i++)
	{	
		m_msflexgrid1to96.put_TextMatrix(i-64,9,m_vecONS.at(i-1).Name);
		m_msflexgrid1to96.put_TextMatrix(i-64,10,m_vecONS.at(i-1).Status);
	}

	UpdateData(FALSE);




}
void CLightingController::OnTimer(UINT_PTR nIDEvent)
{
	CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CView* pNewView = pMain->m_pViews[8];
	CView* pActiveView = pMain->GetActiveView();
	static int counter = 59,counter2 = 600*1000;
	counter++;
	counter2++;
	if (pNewView != pActiveView)
	{
		KillTimer(LIGHTINGCONTROLLERTIMER);
		//恢复T3000主线程
		pMain->m_pFreshMultiRegisters->ResumeThread();
		pMain->m_pRefreshThread->ResumeThread();

	}

	if(LIGHTINGCONTROLLERTIMER == nIDEvent)// )&& (counter%60 == 0)
	{

		CTime now;
		now = CTime::GetCurrentTime();
		m_datetime = now.Format(_T("%A,%B,%d,%Y  %H:%M:%S"));//
	
		GetDlgItem(IDC_STATIC1)->SetWindowText(m_datetime);
	}





 



	//////////////////
	//if(counter%1000*1000 == 0)
	//{
	//	int iret = Read_Multi(254,lightingController_time,4140,8,3);
	//	if(iret>0)
	//	{
	//		m_date.SetDate(lightingController_time[0]*100+lightingController_time[1],lightingController_time[2],lightingController_time[4]);
	//		m_time.SetTime(lightingController_time[5],lightingController_time[6],lightingController_time[7]);
	//		//UpdateData(FALSE);
	//	}
	//}

	//////////////////



	





	


	CFormView::OnTimer(nIDEvent);
}

BOOL CLightingController::prodtopcData()
{
KillTimer(LIGHTINGCONTROLLERTIMER);
#if 1
//读取串口号，从数据库

//open_com(comnum);

int sumret = 0;
BOOL flag = FALSE;

//读取AirQuality数据
BOOL ret_bool = FALSE;
long start = clock(),end(0); 		


memset(&LightCregister,0,sizeof(LightCregister)); 
for(int i=0;i<4;i++)
{
	int ret = Read_Multi(g_tstat_id,&LightCregister[i*64],i*64,64,2);
	sumret += ret;

	if (sumret<0)
	{
		TRACE(_T("write_multi ret sum = %d\n"),sumret);
		SetTimer(LIGHTINGCONTROLLERTIMER,1000,NULL);
		return FALSE;
	}

}
for (int i = 0;i<400;i++)
{
	
	veclightingcontroller.at(i).CStvalue.Format(_T("%d"),LightCregister[i]);	


}



if (sumret>200)
{
	flag = TRUE;
}
TRACE(_T("write_multi ret sum = %d\n"),sumret);

end = clock();
long resultvalue = (end - start)/1000;
TRACE(_T("GetData() usetime %d second\n"),resultvalue);


SetTimer(LIGHTINGCONTROLLERTIMER,1000,NULL);
#endif
return flag;
}

BOOL CLightingController::checkDB( CString DBname,CString strSQL )
{
	CADO m_ado;
	m_ado.OnInitADOConn();


		//判断数据库是否存在

		bool m_judge = m_ado.IsHaveTable(m_ado,DBname);
		//不存在 创建数据库表

		if(DBname.CompareNoCase(_T("LightingController")) == 0)
		{

			if (!m_judge) 
			{	
				m_ado.m_pRecordset = m_ado.OpenRecordset(strSQL);
				//初始化表
				if(DBname.CompareNoCase(_T("LightingController")) == 0)
				{

					CString sql = _T("select * from LightingController");
					m_ado.m_pRecordset = m_ado.OpenRecordset(sql);
					if (m_ado.m_pRecordset->EndOfFile)
					{

						for(int i = 0;i<400;i++)
						{
							try 
							{
								m_ado.m_pRecordset->AddNew();
								m_ado.m_pRecordset->PutCollect("Address",(_bstr_t)(i+1));
								m_ado.m_pRecordset->PutCollect("Data",(_bstr_t)"1");
								m_ado.m_pRecordset->Update();

							}


							catch(...)
							{
								return FALSE;

							}
						}
						m_ado.CloseRecordset();
					}
				}

			}
		}
		else if(DBname.CompareNoCase(_T("LightingController_Name")) == 0)
		{
			if(!m_judge)
			{
				m_ado.m_pRecordset = m_ado.OpenRecordset(strSQL);
				CString StrOutputName,StrStatus = _T("OFF");
				CString sql = _T("select * from LightingController_Name");
				m_ado.m_pRecordset = m_ado.OpenRecordset(sql);
#if 1
				if (m_ado.m_pRecordset->EndOfFile)
				{
					long start(0),end(0);
					start = clock();
					//初始化input-output-mapping
					CString CSTYPE;
					CString strtype = _T("Input");
					BOOL flg = TRUE;
					int maxin = 24;
					for (int i =1;i<=maxin;i++)
					{	
						CSTYPE.Format(_T("%s%d"),strtype,i);
						for (int j = 1;j<=20;j++)
						{	
							for(int k = 1;k<=32;k++)
							{
								StrOutputName.Format(_T("Output%d"),k);
								try 
								{
									m_ado.m_pRecordset->AddNew();
									m_ado.m_pRecordset->PutCollect("Address",(_bstr_t)j);
									m_ado.m_pRecordset->PutCollect("Type",(_bstr_t)CSTYPE);
									m_ado.m_pRecordset->PutCollect("ID_No",(_bstr_t)k);
									m_ado.m_pRecordset->PutCollect("OutputName",(_bstr_t)StrOutputName);
									m_ado.m_pRecordset->PutCollect("Status",(_bstr_t)StrStatus);
									m_ado.m_pRecordset->Update();

								}


								catch(...)
								{
									return FALSE;

								}
							}
						}
// 						if(i == 24&&flg)//这样要慢将近15ms
// 						{
// 							i = 0;
// 							strtype = _T("GroupMap");
// 							flg = FALSE;
// 							maxin = 20;
// 
// 						}

					}

#if 1

					for (int i =1;i<=20;i++)
					{	
						CSTYPE.Format(_T("Group%d"),i);
						for (int j = 1;j<=20;j++)
						{	
							for(int k = 1;k<=32;k++)
							{
								StrOutputName.Format(_T("Output%d"),k);
								try 
								{
									m_ado.m_pRecordset->AddNew();
									m_ado.m_pRecordset->PutCollect("Address",(_bstr_t)j);
									m_ado.m_pRecordset->PutCollect("Type",(_bstr_t)CSTYPE);
									m_ado.m_pRecordset->PutCollect("ID_No",(_bstr_t)k);
									m_ado.m_pRecordset->PutCollect("OutputName",(_bstr_t)StrOutputName);
									m_ado.m_pRecordset->PutCollect("Status",(_bstr_t)StrStatus);
									m_ado.m_pRecordset->Update();

								}


								catch(...)
								{
									return FALSE;

								}
							}
						}

					}
#endif
					end = clock();
					long resul = (end-start);
					CString strshow;
					strshow.Format(_T("initl DB spend\n%d seconds"),resul);
		//			AfxMessageBox(strshow);

					m_ado.CloseRecordset();
				}
#endif

			}


		}

		m_ado.CloseConn();


	return TRUE;
}

BOOL CLightingController::ReadDB()
{

	CADO m_ado;
	m_ado.OnInitADOConn();

	CString SQL = _T("select * from LightingController"); 
	m_ado.m_pRecordset = m_ado.OpenRecordset(SQL);
	int num =0;
	_variant_t vartemp;
	veclightingcontroller.clear();
	while(!m_ado.m_pRecordset->EndOfFile)
	{
		m_veclightingcontroller.iaddress =m_ado.m_pRecordset->GetCollect(_T("Address"));


		vartemp =m_ado.m_pRecordset->GetCollect(_T("Data"));
		if (vartemp.vt==VT_NULL)
			m_veclightingcontroller.CStvalue =_T("");
		else
			m_veclightingcontroller.CStvalue =vartemp;


		m_ado.m_pRecordset->MoveNext();

		veclightingcontroller.push_back(m_veclightingcontroller);
	}

	//读取本电脑当前正在使用的串口。
#if 1
//会出问题，就是如果在数据存的这个串口与电脑的串口号不对应，则本类永远无法有正确数据采集？
	CString CSTcompot;
	BOOL BOdefault;
	SQL = _T("select * from Building");
	m_ado.m_pRecordset = m_ado.OpenRecordset(SQL);
	while(!m_ado.m_pRecordset->EndOfFile)
	{
		CSTcompot =(_variant_t)m_ado.m_pRecordset->GetCollect(_T("Com_Port"));
		BOdefault =(_variant_t)m_ado.m_pRecordset->GetCollect(_T("Default_SubBuilding"));//TRUE = -1即非0，FALSE就是0
		m_ado.m_pRecordset->MoveNext();

		if (BOdefault)
		{
			comnum = _wtoi(CSTcompot.Mid(3));
		}

	}

#endif

	m_ado.CloseRecordset();
	m_ado.CloseConn();
	return TRUE;
}

BOOL CLightingController::UpdateDBALL()
{
	TRACE(_T("Updatedatabase()start!\n"));
	KillTimer(LIGHTINGCONTROLLERTIMER);
	//存入数据库中
	CADO saveADO;
	saveADO.OnInitADOConn();
	CString sql = _T("select * from LightingController");
	saveADO.m_pRecordset = saveADO.OpenRecordset(sql);
	if (!saveADO.m_pRecordset->EndOfFile)
	{
		saveADO.m_pRecordset->MoveFirst();

		for(int i = 0;i<400;i++)
		{
			try 
			{

				saveADO.m_pRecordset->PutCollect("Address",(_bstr_t)veclightingcontroller.at(i).iaddress);
				saveADO.m_pRecordset->PutCollect("Data",(_bstr_t)veclightingcontroller.at(i).CStvalue);


				saveADO.m_pRecordset->Update();
				saveADO.m_pRecordset->MoveNext();
			}


			catch(...)
			{
				SetPaneString(1,_T("Write dababase false!"));
				return FALSE;
			}
		}
	}
	else
	{
		for(int i = 0;i<400;i++)
		{
			try 
			{

				saveADO.m_pRecordset->AddNew();
				saveADO.m_pRecordset->PutCollect("Address",(_bstr_t)veclightingcontroller.at(i).iaddress);
				saveADO.m_pRecordset->PutCollect("Data",(_bstr_t)veclightingcontroller.at(i).CStvalue);

				saveADO.m_pRecordset->Update();

			}


			catch(...)
			{
				SetPaneString(1,_T("Write dababase false!"));
				return FALSE;
			}
		}

	}

	saveADO.CloseRecordset();
	saveADO.CloseConn(); 

	SetTimer(LIGHTINGCONTROLLERTIMER,1000,NULL);
	return TRUE;
}

void CLightingController::OnLbnDblclkListInput()
{
	int Index = m_ListBox.GetCurSel();
	switch(Index)
	{
	case 0:


		break;
	default:

		break;
	}





}
///////Alex/Fresh_Ouputs 表///////////////////////////////////////////////////////////////////////////

void CLightingController::OnLbnSelchangeListInput()
{
	//m_msflexgrid1to96.AddItem(_T("d"),1);RemoveItem(1);减少一行//Refresh();//Clear();
	 Read_Input_Group_Mapping();
	int startaddr;//input1=200;group1=2600
	m_msflexgrid1to96.Clear();
	m_msflexgrid1to96.put_TextMatrix(0,0,_T("Panels"));
	m_msflexgrid1to96.put_TextMatrix(0,1,_T("Card_ID"));
	m_msflexgrid1to96.put_TextMatrix(0,2,_T("No."));
	m_msflexgrid1to96.put_TextMatrix(0,3,_T("OutputName"));



	int itmp = m_ListBox.GetCurSel();//获取编号
	itmp+=1;
	CString strtmp,strtmp1;

	GetDlgItem(IDC_COMBO1)->GetWindowText(strtmp);
	 vector<Mapping_Struct>::iterator iter;

	 m_msflexgrid1to96.put_WordWrap(TRUE);//允许换行 和下面\n对应一起使用才有效果	
	 m_msflexgrid1to96.put_MergeCells(1); //表按任意方式进行组合单元格内容	

	 m_msflexgrid1to96.put_MergeCol(0,TRUE);//其中第一人参数0，表示的是第几列.不是表示需要合并的行数
	 m_msflexgrid1to96.put_MergeCol(1,TRUE);
	 
	if (strtmp.CompareNoCase(_T("Input-Output-Map")) == 0)//确认是input还是group
	{    int i=1;  
		for (iter=m_input_output_mapping.begin();iter!=m_input_output_mapping.end();iter++)
		{
			if (iter->Address==itmp)
			{
				m_msflexgrid1to96.put_TextMatrix(i,0,Get_Table_Name(m_sn,_T("Input"),itmp));
				strtmp.Format(_T("Card%d"),iter->Card_ID);
				m_msflexgrid1to96.put_TextMatrix(i,1,strtmp);
				strtmp.Format(_T("%d"),iter->OuputNo);
				m_msflexgrid1to96.put_TextMatrix(i,2,strtmp);
				strtmp=Read_OutputName(iter->OutputNameAddress);
				if (strtmp.IsEmpty())
				{
                 strtmp.Format(_T("Output%d"),iter->OuputNo);
				} 
				m_msflexgrid1to96.put_TextMatrix(i,3,strtmp);


				 i++;
			}
		   
		}
	}
	else
	{
		int i=1;  
		for (iter=m_group_output_mapping.begin();iter!=m_group_output_mapping.end();iter++)
		{
			if (iter->Address==itmp)
			{
				m_msflexgrid1to96.put_TextMatrix(i,0,Get_Table_Name(m_sn,_T("Group"),itmp));
				strtmp.Format(_T("Card%d"),iter->Address);
				m_msflexgrid1to96.put_TextMatrix(i,1,strtmp);
				strtmp.Format(_T("%d"),iter->OuputNo);
				m_msflexgrid1to96.put_TextMatrix(i,2,strtmp);
				strtmp=Read_OutputName(iter->OutputNameAddress);
				if (strtmp.IsEmpty())
				{
					strtmp.Format(_T("Output%d"),iter->OuputNo);
				} 
				m_msflexgrid1to96.put_TextMatrix(i,3,strtmp);
			}
			i++;
		}
	}
}

void CLightingController::OnContextMenu(CWnd* pWnd, CPoint point)
{
#if 1
	CMenu m_popmenu;

	if(!m_popmenu.LoadMenu(IDR_MENU_MAP))
	{
		TRACE(_T("载入IDR_MENU_PRINT失败！\n"));
		return;
	}

	CMenu* m_submenu=m_popmenu.GetSubMenu(0);
	m_submenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this);
	m_popmenu.DestroyMenu();
#endif
}

WORD* CLightingController::ConvertOutput()
{
//	WORD WordTemp[6];//注：unsigned char 8bit 0~255  放在.h中才对

	memset(WordTemp,0,sizeof(WordTemp));
	BOOL OutputVal = TRUE;
//	BYTE  store,compare=255;
#if 0
	//WORD低8位
	WordTemp[0] = (WORD)m_CheckOutput1|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput2))<<1)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput3))<<2)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput4))<<3)|WordTemp[0];			
	WordTemp[0] = ((((WORD)m_CheckOutput5))<<4)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput6))<<5)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput7))<<6)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput8))<<7)|WordTemp[0];

	//WORD高8位
	WordTemp[0] = ((((WORD)m_CheckOutput9))<<8)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput10))<<9)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput11))<<10)|WordTemp[0];			
	WordTemp[0] = ((((WORD)m_CheckOutput12))<<11)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput13))<<12)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput14))<<13)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput15))<<14)|WordTemp[0];
	WordTemp[0] = ((((WORD)m_CheckOutput16))<<15)|WordTemp[0];



	//WORD低8位
	WordTemp[1] = (WORD)m_CheckOutput17|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput18))<<1)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput19))<<2)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput20))<<3)|WordTemp[1];			
	WordTemp[1] = ((((WORD)m_CheckOutput21))<<4)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput22))<<5)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput23))<<6)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput24))<<7)|WordTemp[1];

	//WORD高8位
	WordTemp[1] = ((((WORD)m_CheckOutput25))<<8)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput26))<<9)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput27))<<10)|WordTemp[1];			
	WordTemp[1] = ((((WORD)m_CheckOutput28))<<11)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput29))<<12)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput30))<<13)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput31))<<14)|WordTemp[1];
	WordTemp[1] = ((((WORD)m_CheckOutput32))<<15)|WordTemp[1];



	//WORD低8位
	WordTemp[2] = (WORD)m_CheckOutput33|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput34))<<1)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput35))<<2)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput36))<<3)|WordTemp[2];			
	WordTemp[2] = ((((WORD)m_CheckOutput37))<<4)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput38))<<5)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput39))<<6)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput40))<<7)|WordTemp[2];

	//WORD高8位
	WordTemp[2] = ((((WORD)m_CheckOutput41))<<8)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput42))<<9)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput43))<<10)|WordTemp[2];			
	WordTemp[2] = ((((WORD)m_CheckOutput44))<<11)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput45))<<12)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput46))<<13)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput47))<<14)|WordTemp[2];
	WordTemp[2] = ((((WORD)m_CheckOutput48))<<15)|WordTemp[2];
#endif
	return WordTemp;
}


BOOL CLightingController::UpdateDBPART( int startnum,int endnum,WORD*savedata )
{
	KillTimer(LIGHTINGCONTROLLERTIMER);
	//存入数据库中
	CADO saveADO;
	saveADO.OnInitADOConn();
	CString sql = _T("select * from LightingController");
	saveADO.m_pRecordset = saveADO.OpenRecordset(sql);
	if (!saveADO.m_pRecordset->EndOfFile)
	{
		//saveADO.m_pRecordset->MoveFirst();

//		inline HRESULT Recordset15::Move ( ADO_LONGPTR NumRecords, const _variant_t & Start ) 
		saveADO.m_pRecordset->Move(startnum);


		for(int i = startnum;i<=endnum;i++)
		{
			try 
			{

				//saveADO.m_pRecordset->PutCollect("Address",(_bstr_t)veclightingcontroller.at(i).iaddress);
				WORD temp = savedata[i-startnum];
				saveADO.m_pRecordset->PutCollect("Data",(_bstr_t)temp);


				saveADO.m_pRecordset->Update();
		 		saveADO.m_pRecordset->MoveNext();
			}


			catch(...)
			{
				SetPaneString(1,_T("Write dababase false!"));
				return FALSE;
			}
		}
	}
	else
	{
		for(int i = 0;i<400;i++)
		{
			try 
			{

				saveADO.m_pRecordset->AddNew();
				saveADO.m_pRecordset->PutCollect("Address",(_bstr_t)i);
				saveADO.m_pRecordset->PutCollect("Data",(_bstr_t)savedata[0]);

				saveADO.m_pRecordset->Update();

			}


			catch(...)
			{
				SetPaneString(1,_T("Write dababase false!"));
				return FALSE;
			}
		}

	}

	saveADO.CloseRecordset();
	saveADO.CloseConn(); 

	SetTimer(LIGHTINGCONTROLLERTIMER,1000,NULL);
	return TRUE;

}

BOOL CLightingController::InitializeArray( BOOL*ARRAY )
{
	ASSERT(ARRAY != NULL);
	UpdateData(TRUE);
#if 0
	ARRAY[0] = m_CheckOutput1;
	ARRAY[1] = m_CheckOutput2;
	ARRAY[2] = m_CheckOutput3;
	ARRAY[3] = m_CheckOutput4;
	ARRAY[4] = m_CheckOutput5;
	ARRAY[5] = m_CheckOutput6;
	ARRAY[6] = m_CheckOutput7;
	ARRAY[7] = m_CheckOutput8;

#endif


	return TRUE;

}

//////选择--Input-output-mapping 或者是 group-output -mapping/////////////////
void CLightingController::OnCbnSelchangeCombo1()
{
	int Index = m_comboBox.GetCurSel();
	if(Index == 1)
	{
		m_ListBox.ResetContent();
		//更新input-output-map 组合框
		CString strtemp;
		/*for (int i=0;i<24;i++)
		{
			strtemp.Format(_T("Input%d"),i+1);
			m_ListBox.InsertString(i,strtemp);
		}*/

		for(int i=0;i<24;i++)
		{

			strtemp=Get_Table_Name(m_sn,_T("Input"),i+1);
			m_ListBox.InsertString(i,strtemp);
		}
		m_ListBox.SetCurSel(0);
		m_strcomboBox = _T("Input-Output-Map");


		//更新Group-output-map output列表


	}
	else
	{
		m_ListBox.ResetContent();
		//更新Group-output-map 组合框
		CString strtemp;
		for (int i=0;i<40;i++)
		{
			//strtemp.Format(_T("Group%d"),i+1);
			strtemp=Get_Table_Name(m_sn,_T("Group"),i+1);
			m_ListBox.InsertString(i,strtemp);
		}
		 
		m_ListBox.SetCurSel(0);
		m_strcomboBox = _T("Group-Output-Map");
		//更新input-output-map output列表



	}

}

BEGIN_EVENTSINK_MAP(CLightingController, CFormView)
	ON_EVENT(CLightingController, IDC_MSFLEXGRID1, DISPID_CLICK, CLightingController::ClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CLightingController, IDC_MSFLEXGRID4, DISPID_CLICK, CLightingController::ClickMsflexgrid4, VTS_NONE)
	ON_EVENT(CLightingController, IDC_MSFLEXGRID1, DISPID_DBLCLICK, CLightingController::DblClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()
//Output table
void CLightingController::ClickMsflexgrid1()
{
    m_input_output=1;
	long lRow,lCol;
	lRow = m_msflexgrid1to96.get_RowSel();//获取点击的行号	
	lCol = m_msflexgrid1to96.get_ColSel(); //获取点击的列号

	if(lRow==0)
		return;
	CRect rect;
	m_msflexgrid1to96.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_msflexgrid1to96.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_msflexgrid1to96.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_msflexgrid1to96.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_msflexgrid1to96.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息	
	CString strValue = m_msflexgrid1to96.get_TextMatrix(lRow,lCol);
	m_oldname=strValue;
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	if(3==lCol)
	{
		m_editName.ShowWindow(SW_SHOW);
		m_editName.SetWindowText(strValue); //显示文本

		m_editName.SetFocus();
		int nLenth=strValue.GetLength();

		m_editName.SetFocus(); //获取焦点
		m_editName.MoveWindow(rc); //移动到选中格的位置，覆盖
		return;
	}
	
}

//Alex Add input table
void CLightingController::ClickMsflexgrid4()
{
    m_input_output=2;
    m_GridNo=1;
	long lRow,lCol;
	lRow = m_inputs_grid.get_RowSel();//获取点击的行号	
	lCol = m_inputs_grid.get_ColSel(); //获取点击的列号
	
	if(lRow==0)
		return;
	CRect rect;
	m_inputs_grid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_inputs_grid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_inputs_grid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_inputs_grid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_inputs_grid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息	
	CString strValue = m_inputs_grid.get_TextMatrix(lRow,lCol);
	m_oldname=strValue;
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	if(lCol==0)
	{
		m_editName.ShowWindow(SW_SHOW);
		m_editName.SetWindowText(strValue); //显示文本

		m_editName.SetFocus();
		int nLenth=strValue.GetLength();

		m_editName.SetFocus(); //获取焦点
		m_editName.MoveWindow(rc); //移动到选中格的位置，覆盖
		return;
	}
	if (lCol==1)
	{
	   m_combox_controler.ResetContent();
	   m_combox_controler.InsertString(0,_T("OFF"));
	   m_combox_controler.InsertString(1,_T("ON"));
	   m_combox_controler.MoveWindow(rc); //移动到选中格的位置，覆盖
	   m_combox_controler.ShowWindow(SW_SHOW);
	   m_combox_controler.BringWindowToTop();
	   m_combox_controler.SelectString(-1,strValue);
	   m_combox_controler.SetFocus(); //获取焦点
	}

	if (lCol==2)
	{
		m_combox_controler.ResetContent();
		m_combox_controler.InsertString(0,_T("AUTO"));
		m_combox_controler.InsertString(1,_T("MAN"));
		m_combox_controler.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_combox_controler.ShowWindow(SW_SHOW);
		m_combox_controler.BringWindowToTop();
		m_combox_controler.SelectString(-1,strValue);
		m_combox_controler.SetFocus(); //获取焦点
	}


}

void CLightingController::DblClickMsflexgrid1()
{
#if 0
	BOOL flg = FALSE;
	long Getrow = m_msflexgrid1to96.get_RowSel();
	long Getcol = m_msflexgrid1to96.get_ColSel();
	CString CStr = m_msflexgrid1to96.get_TextMatrix(Getrow,Getcol);
	switch(Getcol)
	{
	case 2:
		if(m_outputsum  >=1)
			flg = TRUE;

		break;
	case 6:
		if(m_outputsum >=2)
			flg = TRUE;
		break;
	case 10:
		if(m_outputsum >=3)
			flg = TRUE;
	}
	//	if (Getcol == 2||Getcol == 6||Getcol == 10)
	if(flg)
	{	
		if (CStr.CompareNoCase(_T("ON")) == 0)
		{
			m_msflexgrid1to96.put_TextMatrix(Getrow,Getcol,_T("OFF"));
#if 0
			switch (Getcol)
			{
			case 2:
				m_vecONS.at(Getrow-1).Status = _T("OFF");
				break;
			case 6:
				m_vecONS.at(Getrow+32-1).Status = _T("OFF");
				break;
			case 10:
				m_vecONS.at(Getrow+64-1).Status = _T("OFF");
				break;
			}
#endif


		}
		else
		{
			m_msflexgrid1to96.put_TextMatrix(Getrow,Getcol,_T("ON"));
#if 0			
			m_vecONS.at(0).Status = _T("ON");
			switch (Getcol)
			{
			case 2:
				m_vecONS.at(Getrow-1).Status = _T("ON");
				break;
			case 6:
				m_vecONS.at(Getrow+32-1).Status = _T("ON");
				break;
			case 10:
				m_vecONS.at(Getrow+64-1).Status = _T("ON");
				break;
			}
#endif
		}

		m_msflexgrid1to96.put_CellForeColor(RGB(255,0,0));
	}

#endif


}
//Alex 修改 用来改变InputName OutputName
void CLightingController::OnEnKillfocusEditModifyname()
{ 
	m_editName.GetWindowText(m_newname);
	if (m_input_output==2)
	{
		m_inputs_grid.put_TextMatrix(m_nCurRow,m_nCurCol,m_newname);
		if (m_oldname.CompareNoCase(m_newname)==0)
		{
			return;//只要是相同的，都不进行改变
		} 
		else
		{
			unsigned char lable_buffer[GROUP_LABLE_SIZE]={0};
			for (int k=0;k<m_newname.GetLength();k++)
			{
				if (k>=10)
					break;
				lable_buffer[k]=(char)m_newname.GetAt(k);
			}
			Write_Multi(g_tstat_id,lable_buffer,INPUT_DESCRIPTION+(m_nCurRow-1)*GROUP_LABLE_SIZE,GROUP_LABLE_SIZE);
			Insert_Update_Table_Name(m_sn,_T("Input"),m_nCurRow,m_newname);  
			Fresh_Inputs();
		}
	}


	
}
CString CLightingController::Read_OutputName(int Address){
if (Address>480)
{
return _T("");
}
CString OutputName;
unsigned short Lable[10];
unsigned char  Char_Lable[10];
Read_Multi(g_tstat_id,Lable,9336+Address*10,10);
if(Lable[0]==255)
{
	for(int itt=0;itt<10;itt++)
		Lable[itt]=0;
}
for(int itt=0;itt<10;itt++)
{
	if (Lable[itt]!=255)
	{
		Char_Lable[itt]=Lable[itt];
	}
	else
	{

	break;
	}
}
OutputName=(CString)Char_Lable;
return OutputName;
}
void CLightingController::OnEnKillfocusEditTimeSaveDelay(){
	CString delaytime;
	m_savedelaytime.GetWindowText(delaytime);
	write_one(g_tstat_id,92,_wtoi(delaytime));
}

//屏蔽
void CLightingController::ReadDB_OutNameStatus()
{
#if 0
	CADO m_ado;
	m_ado.OnInitADOConn();

	CString SQL = _T("select * from LightingController_Name"); 
	m_ado.m_pRecordset = m_ado.OpenRecordset(SQL);
	int num =0;
	_variant_t vartemp;
	m_vecONS.clear();
	while(!m_ado.m_pRecordset->EndOfFile)
	{
		vartemp =m_ado.m_pRecordset->GetCollect(_T("Name"));
		if (vartemp.vt==VT_NULL)
			m_structONS.Name =_T("");
		else
			m_structONS.Name =vartemp;
		vartemp = m_ado.m_pRecordset->GetCollect(_T("Status"));
		if (vartemp.vt == VT_NULL)
			m_structONS.Status = _T("");
		else
			m_structONS.Status = vartemp;


		m_ado.m_pRecordset->MoveNext();

		m_vecONS.push_back(m_structONS);
	}

	m_ado.CloseRecordset();
	m_ado.CloseConn();
#endif
	
}
//屏蔽
void CLightingController::SaveDB_OutNameStatus()
{
	//存入数据库中
	#if 0
	CADO saveADO;
	saveADO.OnInitADOConn();
	CString sql = _T("select * from LightingController_Name");
	saveADO.m_pRecordset = saveADO.OpenRecordset(sql);
	if (!saveADO.m_pRecordset->EndOfFile)
	{
		saveADO.m_pRecordset->MoveFirst();

		for(int i = 1;i<96;i++)
		{
			try 
			{

				//saveADO.m_pRecordset->PutCollect("Address",(_bstr_t)veclightingcontroller.at(i).iaddress);
				saveADO.m_pRecordset->PutCollect("Name",(_bstr_t)m_vecONS.at(i-1).Name);
				saveADO.m_pRecordset->PutCollect("Status",(_bstr_t)m_vecONS.at(i-1).Status);


				saveADO.m_pRecordset->Update();
				saveADO.m_pRecordset->MoveNext();
			}


			catch(...)
			{
				SetPaneString(1,_T("Write dababase false!"));
				//return FALSE;
			}
		}
	}
	else
	{
		for(int i = 0;i<400;i++)
		{
			try 
			{

				saveADO.m_pRecordset->AddNew();
				saveADO.m_pRecordset->PutCollect("Address",(_bstr_t)veclightingcontroller.at(i).iaddress);
				saveADO.m_pRecordset->PutCollect("Data",(_bstr_t)veclightingcontroller.at(i).CStvalue);

				saveADO.m_pRecordset->Update();

			}


			catch(...)
			{
				SetPaneString(1,_T("Write dababase false!"));
				//return FALSE;
			}
		}

	}

	saveADO.CloseRecordset();
	saveADO.CloseConn(); 
#endif



}


//屏蔽
void CLightingController::OnSetmappingAddoutputbarod()
{
#if 0

	m_msflexgrid1to96.AllowUserResizeSettings;
	CLightingSet dlg;
	
	CString stroutput1,stroutput2,stroutput3,stroutpusum;

	//dlg.m_mapoutputaddress.clear();
	dlg.m_mapoutputaddress = m_mapoutputaddress;//将老的历史记录赋给‘增加地址’界面
	if(dlg.DoModal() == IDOK)
	{
		//m_mapoutaddress.clear();//这里不用清空，下一行代码会将右边完全复制到左边
		m_mapoutputaddress = dlg.m_mapoutputaddress;//map类型之间传值
		m_outputsum = dlg.m_outputboardsum;

	}
	//更新界面，显示输出板的数量
	MAP_OUT_ADDRESS::iterator itera;
	itera = m_mapoutputaddress.begin();
#if 0
	switch(m_outputsum)
	{
	case 0:
		stroutpusum.Format(_T("Output Board: 0/0"));
		stroutput1.Format(_T(""));
		stroutput2.Format(_T(""));
		stroutput3.Format(_T(""));

		break;
	case 1:
		stroutpusum.Format(_T("Output Board: 1/%d"),m_outputsum);
		stroutput1.Format(_T("Output Board %d Address:   %s"),itera->first,itera->second);
		stroutput2.Format(_T(""));
		stroutput3.Format(_T(""));
		break;
	case 2:
		stroutpusum.Format(_T("Output Board: 2/%d"),m_outputsum);
		stroutput1.Format(_T("Output Board %d Address:   %s"),itera->first,itera->second);
		itera++;
		stroutput2.Format(_T("Output Board %d Address:   %s"),itera->first,itera->second);
		stroutput3.Format(_T(""));
		break;
	case 3:
		stroutpusum.Format(_T("Output Board: 3/%d"),m_outputsum);
		stroutput1.Format(_T("Output Board %d Address:   %s"),itera->first,itera->second);
		itera++;
		stroutput2.Format(_T("Output Board %d Address:   %s"),itera->first,itera->second);
		itera++;
		stroutput3.Format(_T("Output Board %d Address:   %s"),itera->first,itera->second);
		break;
// 	default:
// 		stroutpusum.Format(_T("Output Board: 3/%d"),m_outputsum);
// 		stroutput1.Format(_T("Output Board %d Address:   %s"),itera->first,itera->second);
// 		itera++;
// 		stroutput2.Format(_T("Output Board %d Address:   %s"),itera->first,itera->second);
// 		itera++;
// 		stroutput3.Format(_T("Output Board %d Address:   %s"),itera->first,itera->second);
// 		break;
	}
	m_msflexgridTitle.put_TextMatrix(0,0,stroutput1);
	m_msflexgridTitle.put_TextMatrix(0,2,stroutput2);
	m_msflexgridTitle.put_TextMatrix(0,4,stroutput3);
	m_msflexgridTitle.put_TextMatrix(0,5,stroutpusum);
	

// 	m_msflexgridTitle.put_TextMatrix(0,0,_T(" Output Board 1 Address:   210"));
// 	m_msflexgridTitle.put_TextMatrix(0,2,_T(" Output Board 2 Address:   211"));
// 	m_msflexgridTitle.put_TextMatrix(0,4,_T(" Output Board 3 Address:   212"));
// 	m_msflexgridTitle.put_TextMatrix(0,5,_T(" Output Board: 3/3"));



	//确定输出板地址
//	int outputaddr = 100;
// 	m_vecaddrinout.clear();
// 	for (int i = 0;i<2;i++)
// 	{
// 		m_vecaddrinout.push_back(outputaddr);
// 		outputaddr+=50;
// 	} 	
#endif

	CString strinputgroup,strmaptype;
	int indexmap = m_comboBox.GetCurSel();//input索引是1
	if(indexmap == 1)
		strmaptype = _T("input");
	else
		strmaptype = _T("group");
	int input_No =m_ListBox.GetCurSel();//input1 = 0,input2....32

	strinputgroup.Format(_T("%s%d"),strmaptype,input_No+1);

	MAP_INT_OUT::iterator iter;
	iter = m_mapinout.find(strinputgroup);
	if (iter != m_mapinout.end())
		m_mapinout.erase(iter);

	m_mapinout.insert(MAP_INT_OUT::value_type(strinputgroup,m_mapoutputaddress));//存储input-output对应关系：映射关系input_No:如input1,input2..m_mapoutaddress：输出板序号：输出板地址
	//用insert函数插入数据，在数据的插入上涉及到集合的唯一性这个概念，即当map中有这个关键字时，insert操作是插入数据不了的，但是用数组方式就不同了，它可以覆盖以前该关键字对应的值，用程序说明


#endif
}
//屏蔽
void CLightingController::OnSetmappingPrevious()//delete
{
#if 0
	int itmp = m_ListBox.GetCurSel();
	//判断是组还是输入
	CString strtmp,strtmp1;
	GetDlgItem(IDC_COMBO1)->GetWindowText(strtmp);
	if (strtmp.CompareNoCase(_T("Input-Output-Map")) == 0)
		strtmp1.Format(_T("input%d"),itmp+1);
	else
		strtmp1.Format(_T("group%d"),itmp+1);

	map<CString,vecaddoutputs>::iterator iter;
	iter = m_mapaddoutputs.find(strtmp1);
	if (iter != m_mapaddoutputs.end())
	{
		m_mapaddoutputs.erase(iter);
		m_vecaddoutputs.clear();

		m_msflexgrid1to96.Clear();
		m_msflexgrid1to96.put_TextMatrix(0,0,_T("Panels"));
		m_msflexgrid1to96.put_TextMatrix(0,1,_T("Outputs"));
		m_msflexgrid1to96.put_TextMatrix(0,2,_T("No."));
		m_msflexgrid1to96.put_TextMatrix(0,3,_T("OutputName"));
	}

#endif
	



}
//屏蔽
void CLightingController::OnSetmappingNext() //现改成 Set As default 给硬件发送 1,给91寄存器
{
#if 0
	if (g_CommunicationType == 0)
		open_com(comnum);
	int ret =0;
	ret = write_one(g_tstat_id,91,1);
	//ret = write_one(100,200,4);
	if(ret>0)
		AfxMessageBox(_T("Setting successful"));
	else
		AfxMessageBox(_T("Setting fail"));
#endif

}
 

//屏蔽

void CLightingController::OnSetmappingRead()//read from lightingcontroller  to pc
{

	//200…2559	2400	Register 200 to 2731 use for input-output-mappings. The data length is (100) in Write Multiple and the maximum input is 24.

	//2600…4599	2000	Register 2732 to 4691 use for group-output-mapping. The data length is (100) in Write Multiple and the maximum input is 20.



#if 0//测试LightingController批量读。
	//open_com(comnum);
	//memset(SerialNum,0,sizeof(SerialNum));
	
	int num = 0;//确定硬件件中input-output-map中有已设置了多少组（总共24组）

	for (int i = 200;i<2400;)
	{
		int tmp = read_one(g_tstat_id,i);
		if( tmp == 255)
		{
			num+=1;
			break;		
		}
		num+=1;			
		i+=100;
	}

	//根据已知多少有多少组是设置好的后再去读取。
	int flg = 0; 
	for (int i =0;i<num;i++)
	{

		register_critical_section.Lock();
		flg = Read_Multi(254,&SerialNum[i*100],200+i*100,100);
		register_critical_section.Unlock();
	}
	if (flg>0)
	{
		AfxMessageBox(_T("Read successful！"));
	}

#if 0
	CString strpanel,stroutputboard,stroutput,stroutputname;
	stroutput.Format(_T("%d"),SerialNum[0])
	//将选择的数据保存在内存中vector:: m_vecaddoutputs
	m_structaddoutputs.stroutputno = stroutput;
	m_structaddoutputs.stroutputboard = stroutputboard;
	m_structaddoutputs.strpanel = strpanel;
	m_structaddoutputs.stroutputname = stroutputname;
	m_vecaddoutputs.push_back(m_structaddoutputs);





	int itmp = m_ListBox.GetCurSel();
	// 	//m_ListBox.SelectString(itmp,strtmp);
	// 	strtmp.Format(_T(""))
	//	GetDlgItem(IDC_LIST_INPUT)->GetWindowText(strtmp);


	//判断是组还是输入
	CString strtmp,strtmp1;
	GetDlgItem(IDC_COMBO1)->GetWindowText(strtmp);
	if (strtmp.CompareNoCase(_T("Input-Output-Map")) == 0)
		strtmp1.Format(_T("input%d"),itmp+1);
	else
		strtmp1.Format(_T("group%d"),itmp+1);

	//m_mapaddoutputs.clear();
	map<CString,vecaddoutputs>::iterator iter;
	iter = m_mapaddoutputs.find(strtmp1);
	if (iter != m_mapaddoutputs.end())
		m_mapaddoutputs.erase(iter);

	m_mapaddoutputs.insert(map<CString,vecaddoutputs>::value_type(strtmp1,m_vecaddoutputs));


#endif



	//int Read_Multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int retry_times)
// 	register_critical_section.Lock();
// 	Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);
// 	register_critical_section.Unlock();
// 	for (int i =0;i<24;i++)
// 	{
// 
// 	register_critical_section.Lock();
// 	flg = Read_Multi(254,&SerialNum[i*100],200+i*100,100);
// 	register_critical_section.Unlock();
// 	}
// 	if (1)//flg==
// 	{
// 		AfxMessageBox(_T("Read successful！"));
// 	}
#endif




}
//屏蔽
void CLightingController::OnSetmapSettingsave()
{
#if 0
#if 1
	int inporgro = m_comboBox.GetCurSel();//input索引是1
	int num_No =m_ListBox.GetCurSel();//input1 = 0,input2....32
	MAP_OUT_ADDRESS::iterator iter;
	int col;
	int istart = 0,iend;
	for (iter = m_mapoutputaddress.begin();iter != m_mapoutputaddress.end();iter++ )
	{
#if 0
		switch(iter->first)
		{
		case 1:
			col = 1;
			istart

			break;
		case 2:
			col = 5;
			break;
		case 3:
			col = 9;
			break;
		}
#endif

		istart+= iter->first;
		m_vecoutparam.clear();
		for (int i = istart;i<=istart+32;i++)
		{
			m_stru_outparam.outname = m_msflexgrid1to96.get_TextMatrix(i,1);
			m_stru_outparam.outstatus = m_msflexgrid1to96.get_TextMatrix(i,2);
			m_vecoutparam.push_back(m_stru_outparam);

		}

		CString strtemp;
		if (inporgro == 1)
			strtemp.Format(_T("input%d %s"),num_No+1,iter->second);//输出板地址：iter->second；input_No：如input1，input2....如"201+input1"
		else
			strtemp.Format(_T("group%d %s"),num_No+1,iter->second);//输出板地址：iter->second；group_No：如group1，group2....如"201+group1"

		MAP_OUT_PARAM::iterator iterparam;
		iterparam = m_mapoutputparam.find(strtemp);
		if (iterparam != m_mapoutputparam.end())
			m_mapoutputparam.erase(iterparam);
		m_mapoutputparam.insert(MAP_OUT_PARAM::value_type(strtemp,m_vecoutparam));//存储output Address()-outputName and Status（m_vecoutparam）对应关系：映射关系

	}

#endif
#endif
}

void CLightingController::OnSetmappingSand()
{
//新版
	//
	bool r =FALSE;
	//	r = open_com(2);
	if (g_CommunicationType == 0)
		r = open_com(comnum);
	else
		r = TRUE;

	if (r)
	{
		

//	CString s;
//	s.Format(_T("com%d is ok"),comnum);
//	AfxMessageBox(s);
	//要发送的数据
	BYTE SendData[100];
	//int x = sizeof(SendData);
	memset(SendData,0,sizeof(SendData));


	CString straddress,strmap;//strmap:映射类型

	//找到要发送的项
	int itmp = m_ListBox.GetCurSel();//获取编号
	//判断是组还是输入
	CString strtmp,strtmp1;
	GetDlgItem(IDC_COMBO1)->GetWindowText(strtmp);
	int indexmap;
	if (strtmp.CompareNoCase(_T("Input-Output-Map")) == 0)//确认是input还是group
	{
		strtmp1.Format(_T("input%d"),itmp+1);
		indexmap =1;

	}
	else
	{
		strtmp1.Format(_T("group%d"),itmp+1);
		indexmap = 0;
	}

	map<CString,vecaddoutputs>::iterator iter;//查找组中的元素
	iter = m_mapaddoutputs.find(strtmp1);
	if (iter != m_mapaddoutputs.end())
	{
		if (iter->first != _T("") )
		{
		m_vecaddoutputs = iter->second;

 	vecaddoutputs::iterator itervec;
// 	CString ioutputno;
// 	itervec = find(iter->second.begin(),iter->second.end(),iter->second.at(1).stroutputboard);
// 		if (itervec !=iter->second.end())
// 		{
// 			ioutputno = iter->second.at(1).stroutputno;
// 		}

	vector<int> vecout;
	map<int,vector<int>> mapboardoutput;
	mapboardoutput.clear();
	int outsum = m_vecaddoutputs.size();//输出板总数
	int iboard,ioutput;
	CString CBoard,COutput;
	map<int,vector<int>>::iterator itermap;
	for (int i = 0;i<outsum;i++)
	{
		CBoard = m_vecaddoutputs.at(i).stroutputboard;
		iboard = _ttoi(CBoard.Right(3));
		COutput = m_vecaddoutputs.at(i).stroutputno;
		ioutput = _ttoi(COutput);
		vecout.push_back(ioutput);


		itermap = mapboardoutput.find(iboard);
		if (itermap != mapboardoutput.end())
			mapboardoutput.erase(itermap);
		else
		{
			vecout.clear();
			vecout.push_back(ioutput);
		}


		mapboardoutput.insert(map<int,vector<int>>::value_type(iboard,vecout));
	}

	BYTE Byte = 1;
	int ITEMP,index = 0;
	map<int,vector<int>>::iterator itermapp;
	for (itermapp = mapboardoutput.begin();itermapp !=mapboardoutput.end();itermapp++)
	{
	
		for (unsigned int j=0;j<itermapp->second.size();j++)
		{
			ITEMP =itermapp->second.at(j);
			SendData[index] = itermapp->first;
				if(ITEMP <=8)
				{
					SendData[index+1] = (Byte<<(ITEMP-1))|SendData[index+1];
					//SendData[index+1] = Byte|SendData[index+1];
				}else if (ITEMP<=16&&ITEMP>8)
				{
					int intmp=ITEMP-9;
					SendData[index+2] = (Byte<<intmp)|SendData[index+2];
				}else if (ITEMP<=24&&ITEMP>16)
				{
					int intmp=ITEMP-17;
					SendData[index+3] = (Byte<<intmp)|SendData[index+3];
				}else if (ITEMP<=32&&ITEMP>24)
				{
					int intmp=ITEMP-25;
					SendData[index+4] = (Byte<<intmp)|SendData[index+4];
				}
				
		}
		index = index+5;

	}


	memset(&SendData[index],255,sizeof(SendData)-index);
	}else
	memset(&SendData,255,sizeof(SendData));

}else
	{
		memset(&SendData,255,sizeof(SendData));

}



	//发送函数
	int ret = 0;
	int sendmodbus =0;
	sendmodbus=itmp*100;//itmp就是索引
	switch(indexmap)//input = 1/group/indexnum    //1...24(input1 = 0,input2....24)/1..20
	{
	case 1://input map 200…2559 2400 Register 200 to 2731 use for input-output-mappings. The data length is (100) in Write Multiple and the maximum input is 24.
		sendmodbus+=300;
		break;
	case 0://group map
		sendmodbus+=2700;
		break;
	}


	ret = Write_Multi(g_tstat_id,SendData,sendmodbus,100);
	//ret = Write_Multi(100,SendData,sendmodbus,100);

	//发送结果提示
	if (ret == 1)
		AfxMessageBox(_T("Send successful!"));

	else
		AfxMessageBox(_T("Send failed!"));	

// 	CString t;
// 	t.Format(_T("%d"),ret);
// 	AfxMessageBox(t);









#if 0
// 	int indexmap = m_comboBox.GetCurSel();
// 	if(indexmap == 1)
// 		strmap = _T("input");
// 	else
// 		strmap = _T("group");
// 
// 	int indexnum = m_ListBox.GetCurSel();
// 
// 
// 	straddress.Format(_T("%s%d"),strmap,indexnum+1);


	//读取输出板地址
	//typedef std::map<CString,MAP_OUT_ADDRESS> MAP_INT_OUT;
	MAP_INT_OUT::iterator iterfindoutsum;
	//	在往map里面插入了数据，我们怎么知道当前已经插入了多少数据呢，可以用size函数，用法如下：Int nSize = mapStudent.size();
	iterfindoutsum = m_mapinout.find(straddress);
	if(iterfindoutsum != m_mapinout.end())
	{
		//MAP_OUT_ADDRESS::iterator iteraddress = iteraddress->second;//Error	5	error C2440: 'initializing' : cannot convert from 'CString' to 'std::_Tree<_Traits>::iterator'	指针和变量混淆

		CString strtmp = iterfindoutsum->first;
		MAP_OUT_ADDRESS maptmp = iterfindoutsum->second;
		outsum = maptmp.size();

		CString mapaddress;
		int inmoveaddr = 0;
		int j = 0;
		memset(SendData,0,sizeof(SendData));
		for (int i =1;i<=outsum;i++)
		{
			j = inmoveaddr+(i - 1)*5;
			CString str = maptmp[i];
			mapaddress.Format(_T("%s %s"),straddress,str);
			MAP_OUT_PARAM::iterator iterparam;
			iterparam = m_mapoutputparam.find(mapaddress);
			if (iterparam != m_mapoutputparam.end())
			{
				VECTOR_OUT_PARAM vectmp = iterparam->second;

				// 				 (2)    按制定符号分割字符：
				// 
				// 					 int pos=str.Find(strchar);//查找字符，如果没找到，则返回0，找到则返回字符的位置,参数可以是字符也可以是字符串
				// 
				// 				 while(pos>0){
				// 
				// 					 str1=str.Left(pos);//取左,参数为字符串的个数
				// 
				// 					 str=str.Right(str.GetLength-pos-1);//取右，参数同上
				// 
				// 					 tmp.Format(“%s\r\n”,str1);//字符串中\r\n代表回车化行符
				// 
				// 					 strres+=tmp;
				// 
				// 					 pos=str.Find(strchar);
				// 
				// 				 }

				SendData[j] = _ttoi(str);//第一位放地址
				int endnum = vectmp.size();//一个MODBUS的32位数据长度,以下数据将用sendData数组5个内存长度。
				for (int i =0;i<endnum;i++)
				{
					CString str = vectmp.at(i).outstatus;
					BYTE Byte;
					if (str.CompareNoCase(_T("ON")) == 0)
						Byte = 1;
					else
						Byte =0;

					if(i <8)
					{
						SendData[j+1] = (Byte<<i)|SendData[j+1];
					}else if (i<16&&i>=8)
					{
						int intmp=i-8;
						SendData[j+2] = (Byte<<intmp)|SendData[j+2];
					}else if (i<24&&i>=16)
					{
						int intmp=i-16;
						SendData[j+3] = (Byte<<intmp)|SendData[j+3];
					}else if (i<32&&i>=24)
					{
						int intmp=i-24;
						SendData[j+4] = (Byte<<intmp)|SendData[j+4];
					}


				}

			}
		}

	}


	//读取输出板参数
	//typedef std::map<CString,VECTOR_OUT_PARAM> MAP_OUT_PARAM;
	//MAP_OUT_PARAM::iterator iterparam;


	//m_mapoutputparam;



	// 	for (int i = 0;i<100;i++)
	// 	{
	// 		if (m_vecONS.at(i).Status.CompareNoCase(_T("ON")) == 0)
	// 		{
	// 			SendData[i+2] = 1;
	// 			SendData[1]+=1;
	// 		}else
	// 		{
	// 			SendData[i+2] = 0;
	// 		}
	// 	}

	//发送函数
	int ret = 0;
	int sendmodbus =0;
	sendmodbus=indexnum*100;
	switch(indexmap)//input = 1/group/indexnum    //1...24(input1 = 0,input2....24)/1..20
	{
	case 1://input map 200…2559 2400 Register 200 to 2731 use for input-output-mappings. The data length is (100) in Write Multiple and the maximum input is 24.
		sendmodbus+=200;
		break;
	case 0://group map
		sendmodbus+=2600;
		break;
	}


	ret = Write_Multi(g_tstat_id,SendData,sendmodbus,100);

	//发送结果提示
	if (ret == 1)
		AfxMessageBox(_T("Send successful!"));

	else
		AfxMessageBox(_T("Send failed!"));	

	//存于数据库中
	//	SaveDB_OutNameStatus();
#endif

//	200…2559	2400	Register 200 to 2731 use for input-output-mappings. The data length is (100) in Write Multiple and the maximum input is 24.
//假设发送input1-ouput1、2、3;Adress(1个BYTE)-output4-output3-..output1(0-8bit) 共20组共100BYTE
#if 0
	open_com(comnum);
	//要发送的数据
	BYTE SendData[100];
	//memset(SendData,0,sizeof(SendData));

	CString straddress,strmap;//strmap:映射类型
	int outsum;//输出板总数
	int indexmap = m_comboBox.GetCurSel();
	if(indexmap == 1)
		strmap = _T("input");
		else
		strmap = _T("group");

	int indexnum = m_ListBox.GetCurSel();

	
	straddress.Format(_T("%s%d"),strmap,indexnum+1);


	//读取输出板地址
	//typedef std::map<CString,MAP_OUT_ADDRESS> MAP_INT_OUT;
	MAP_INT_OUT::iterator iterfindoutsum;
//	在往map里面插入了数据，我们怎么知道当前已经插入了多少数据呢，可以用size函数，用法如下：Int nSize = mapStudent.size();
	iterfindoutsum = m_mapinout.find(straddress);
	if(iterfindoutsum != m_mapinout.end())
	{
		//MAP_OUT_ADDRESS::iterator iteraddress = iteraddress->second;//Error	5	error C2440: 'initializing' : cannot convert from 'CString' to 'std::_Tree<_Traits>::iterator'	指针和变量混淆

		CString strtmp = iterfindoutsum->first;
		MAP_OUT_ADDRESS maptmp = iterfindoutsum->second;
		outsum = maptmp.size();

		CString mapaddress;
		int inmoveaddr = 0;
		int j = 0;
		 memset(SendData,0,sizeof(SendData));
		for (int i =1;i<=outsum;i++)
		{
			j = inmoveaddr+(i - 1)*5;
			CString str = maptmp[i];
			 mapaddress.Format(_T("%s %s"),straddress,str);
			 MAP_OUT_PARAM::iterator iterparam;
			 iterparam = m_mapoutputparam.find(mapaddress);
			 if (iterparam != m_mapoutputparam.end())
			 {
				 VECTOR_OUT_PARAM vectmp = iterparam->second;
				
// 				 (2)    按制定符号分割字符：
// 
// 					 int pos=str.Find(strchar);//查找字符，如果没找到，则返回0，找到则返回字符的位置,参数可以是字符也可以是字符串
// 
// 				 while(pos>0){
// 
// 					 str1=str.Left(pos);//取左,参数为字符串的个数
// 
// 					 str=str.Right(str.GetLength-pos-1);//取右，参数同上
// 
// 					 tmp.Format(“%s\r\n”,str1);//字符串中\r\n代表回车化行符
// 
// 					 strres+=tmp;
// 
// 					 pos=str.Find(strchar);
// 
// 				 }

				 SendData[j] = _ttoi(str);//第一位放地址
				 int endnum = vectmp.size();//一个MODBUS的32位数据长度,以下数据将用sendData数组5个内存长度。
				 for (int i =0;i<endnum;i++)
				 {
					 CString str = vectmp.at(i).outstatus;
					 BYTE Byte;
					 if (str.CompareNoCase(_T("ON")) == 0)
						 Byte = 1;
					 else
						 Byte =0;
					
					if(i <8)
					{
					 SendData[j+1] = (Byte<<i)|SendData[j+1];
					}else if (i<16&&i>=8)
					{
						int intmp=i-8;
						SendData[j+2] = (Byte<<intmp)|SendData[j+2];
					}else if (i<24&&i>=16)
					{
						int intmp=i-16;
						SendData[j+3] = (Byte<<intmp)|SendData[j+3];
					}else if (i<32&&i>=24)
					{
						int intmp=i-24;
						SendData[j+4] = (Byte<<intmp)|SendData[j+4];
					}


				 }

			 }
		}
			
	}


	//读取输出板参数
	//typedef std::map<CString,VECTOR_OUT_PARAM> MAP_OUT_PARAM;
	//MAP_OUT_PARAM::iterator iterparam;


	//m_mapoutputparam;



// 	for (int i = 0;i<100;i++)
// 	{
// 		if (m_vecONS.at(i).Status.CompareNoCase(_T("ON")) == 0)
// 		{
// 			SendData[i+2] = 1;
// 			SendData[1]+=1;
// 		}else
// 		{
// 			SendData[i+2] = 0;
// 		}
// 	}

	//发送函数
	int ret = 0;
	int sendmodbus =0;
	sendmodbus=indexnum*100;
	switch(indexmap)//input = 1/group/indexnum    //1...24(input1 = 0,input2....24)/1..20
	{
	case 1://input map 200…2559 2400 Register 200 to 2731 use for input-output-mappings. The data length is (100) in Write Multiple and the maximum input is 24.
		sendmodbus+=200;
		break;
	case 0://group map
		sendmodbus+=2600;
		break;
	}
	

	ret = Write_Multi(g_tstat_id,SendData,sendmodbus,100);

	//发送结果提示
	if (ret == 1)
		AfxMessageBox(_T("Send successful!"));

	else
		AfxMessageBox(_T("Send failed!"));	

	//存于数据库中
//	SaveDB_OutNameStatus();

#endif

#if 0
open_com(comnum);
//要发送的数据
BYTE SendData[98];
memset(SendData,0,sizeof(SendData));

for (int i = 0;i<96;i++)
{
	if (m_vecONS.at(i).Status.CompareNoCase(_T("ON")) == 0)
	{
		SendData[i+2] = 1;
		SendData[1]+=1;
	}else
	{
		SendData[i+2] = 0;
	}
}

//发送函数
int ret = 0;
ret = Write_Multi(g_tstat_id,SendData,200,98);

//发送结果提示
if (ret == 1)
AfxMessageBox(_T("Send successful!"));

else
AfxMessageBox(_T("Send failed!"));	

//存于数据库中
SaveDB_OutNameStatus();
#endif

#if 0
SaveDB_OutNameStatus();

InitializeArray(m_ArrayOutput);

open_com(comnum);

//将界面用户选择转换成WORD数据
WORD *SendData;
SendData = ConvertOutput();

//查找要发送的地址，从输入名称中可找到
int Curindex = m_ListBox.GetCurSel();
unsigned short StartAddress;
switch(Curindex)
{
case 0:
	StartAddress = 200;
	break;
case 1:
	break;
default:
	break;
}

//发送数据给产品
int RETWRITE = 0;
for (int i = 0;i<3;i++)
{
	RETWRITE = write_one(g_tstat_id,StartAddress,SendData[i]);

	StartAddress++;
}
if(RETWRITE == 1)
{
	AfxMessageBox(_T("Set successful!"));

	//发送如果成功则将所发送的数据写入数据库
	// 		if(UpdateDBPART(200,202,SendData))
	// 			AfxMessageBox(_T("Write DB successful!"));
}
else
{
	AfxMessageBox(_T("Set failed!"));
}
#endif

#if 0
BYTE chartemp[12];//注：unsigned char 8bit 0~255
for (int i=0;i<12;i++)
{
	chartemp[i] = 0;
}
open_com(comnum);
int flag;
//flag = write_multi(254,chartemp,4700,12);

flag = write_one(254,4700,0);
#endif


#if 0
open_com(comnum);
int ret = Read_Multi(254,LightCregister,1380,64,2);

#endif




#if 0 //测试LightingController写，注批量写。已测试OK
open_com(comnum);
unsigned char addr2 = 'a';

CString str_temp;
description temp_descri;
memset(&temp_descri,50,sizeof(description));

unsigned char *p;
p=(unsigned char *) &temp_descri;
int flg = Write_Multi(254,p,4520,WR_DESCRIPTION_SIZE);
if (flg==1)
{
	AfxMessageBox(_T("写入成功！"));
}

#endif

#if 0 //测试LightingController写，注批量读。
open_com(comnum);
unsigned short SerialNum[100];


//int Read_Multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int retry_times)
register_critical_section.Lock();
int flg = Read_Multi(254,&SerialNum[0],100,100);
register_critical_section.Unlock();
if (flg==1)
{
	AfxMessageBox(_T("写入成功！"));
}

#endif





//test_0K 
// 	unsigned short StartAddress = 200;
// 	open_com(comnum);
// 	unsigned char Chartemp[6];//注：unsigned char 8bit 0~255
// 	for (int i=0;i<6;i++)
// 	{
// 		Chartemp[i] = 1;
// 	}
// 	open_com(comnum);
// 	for (int i = 0;i<6;i++)
// 	{
// 	int flg = write_one(g_tstat_id,StartAddress,100);
// 		StartAddress++;
// 
// 	}



//test_no multi write flg =1,但是硬件则没有真正写入Chartemp的值，在T3产品也测试过出现同样问题 ,NC产品还没测试 
//不用测试了，是因为产品硬件没有开辟这么多内存空间来存放这些字符导致的，schedule则可以一次性多写，YF。

// 	BYTE chartemp[12];//注：unsigned char 8bit 0~255
// 	for (int i=0;i<12;i++)
// 	{
// 		chartemp[i] = 10;
// 	}
// 	open_com(comnum);
// 	int flag;
// 	flag = write_multi(254,chartemp,200,12);
}else

AfxMessageBox(_T("serial 2 open fail"));

}

void CLightingController::OnBnClickedButtonLightingcontorlWeeklys()
{
	if(g_NetWorkLevel==1)
		return;

	g_bEnableRefreshTreeView = FALSE;
// 	open_com(comnum);
// 	SetPaneString(1,_T("Online!"));



	g_bPauseMultiRead=TRUE;
	CString str = _T("Lightingcontroller");
	Weekly_Routines dlg(this,str);
// 	g_tstat_id = 9;
// 	Weekly_Routines dlg;
	dlg.DoModal();

	g_bPauseMultiRead=FALSE;


	g_bEnableRefreshTreeView = TRUE;

}

void CLightingController::OnBnClickedButtonLightingcontorlAnnuals()
{
	if(g_NetWorkLevel==1)
		return;
// 	open_com(comnum);
// 	SetPaneString(1,_T("Online!"));

	g_bPauseMultiRead=TRUE;
	CString str = _T("Lightingcontroller");
	Annual_Routines dlg(this,str);
//	Annual_Routines dlg;
// 	g_tstat_id = 9;
 	dlg.DoModal();
	g_bPauseMultiRead=FALSE;

}

void CLightingController::OnBnClickedButtonLightingcontorlGroups()
{

	if(g_NetWorkLevel==1)
		return;
// 	open_com(comnum);
// 	SetPaneString(1,_T("Online!"));
	g_bPauseMultiRead=TRUE;
	CString str = _T("Lightingcontroller");
	CConfig_Routines dlg(this,str);
	dlg.DoModal();
	g_bPauseMultiRead=FALSE;
}

void CLightingController::OnBnClickedButtonLightingcontorlSyncwithPC()
{
	if (g_CommunicationType == 0)
     open_com(comnum);
	 UpdateData();
	BYTE t_time[8]={0};
	WORD temp_us=0;
	CTime time = CTime::GetCurrentTime();
	temp_us=time.GetYear();
	t_time[0]=temp_us/100;
	t_time[1]=temp_us%100;
	t_time[2]=time.GetMonth();
	t_time[3]=time.GetDayOfWeek();

	if(t_time[3]==1)
		t_time[3]=7;
	else 
		t_time[3]-=1;

	t_time[4]=time.GetDay();
	t_time[5]=time.GetHour();
	t_time[6]=time.GetMinute();
	t_time[7]=time.GetSecond();
	int iretw = Write_Multi(g_tstat_id,t_time,4700,8);
	//int iretw = Write_Multi(g_tstat_id,t_time,200,8);
	if (iretw>0)
	{
		int iret = Read_Multi(g_tstat_id,lightingController_time,4700,8,3);
		//int iret = Read_Multi(g_tstat_id,lightingController_time,200,8,3);
		if(iret>0)
		{
			m_date.SetDate(lightingController_time[0]*100+lightingController_time[1],lightingController_time[2],lightingController_time[4]);
			m_time.SetTime(lightingController_time[5],lightingController_time[6],lightingController_time[7]);
		}
	}

	

}
void CLightingController::OnBnClickedButtonSaveAll(){
write_one(g_tstat_id,91,1);
AfxMessageBox(_T("Save Successfully"));

}
void CLightingController::Automationflexrow()
{

	//	m_msflexgrid1to96.put_Rows(100);	
	m_msflexgrid1to96.put_WordWrap(TRUE);//允许换行 和下面\n对应一起使用才有效果	
	m_msflexgrid1to96.put_MergeCells(1); //表按任意方式进行组合单元格内容	

	m_msflexgrid1to96.put_MergeCol(0,TRUE);//其中第一人参数0，表示的是第几列.不是表示需要合并的行数
	m_msflexgrid1to96.put_MergeCol(1,TRUE);

	//用代码进行自动的增加单元格内容
		CString str;
	 	for (long i = 1;i<=32*200;i++)
	 	{
	 		m_msflexgrid1to96.AddItem(_T(""),(COleVariant)i);
	 	
//	 	}

		//显示纵标题
	
// 		for(int i=1;i<=32;i++)
// 		{
			str.Format(_T("%d"),i);
			m_msflexgrid1to96.put_TextMatrix(i,2,str);	
			str = _T("Output ")+str;
			m_msflexgrid1to96.put_TextMatrix(i,3,str);
		//	m_msflexgrid1to96.put_TextMatrix(i,4,_T("OFF"));	
//		}

		//panel和ouput列显示
//		for (int i = 1;i<=32;i++)
//		{
			m_msflexgrid1to96.put_TextMatrix(i,0,_T("2"));
			m_msflexgrid1to96.put_TextMatrix(i,1,_T("2--\nout 1"));
//		}


		//背景色刷新
//		for(int i=1;i<=32;i++)		//排数量
//		{
// 			for(int k=2;k<5;k++)	//列数量
// 			{
// 				if (i%2==1)
// 				{
// 					m_msflexgrid1to96.put_Row(i);m_msflexgrid1to96.put_Col(k);m_msflexgrid1to96.put_CellBackColor(RGB(255,255,255));
// 				}
// 				else
// 				{
// 					m_msflexgrid1to96.put_Row(i);m_msflexgrid1to96.put_Col(k);m_msflexgrid1to96.put_CellBackColor(COLOR_CELL);
// 				}
// 			}
//		}

		}

}
void CLightingController::OnBnClickedButtonAdd()
{	


	//flexcontrol参数设置
	m_msflexgrid1to96.put_WordWrap(TRUE);//允许换行 和下面\n对应一起使用才有效果	
	m_msflexgrid1to96.put_MergeCells(1); //表按任意方式进行组合单元格内容	

	m_msflexgrid1to96.put_MergeCol(0,TRUE);//其中第一人参数0，表示的是第几列.不是表示需要合并的行数
	m_msflexgrid1to96.put_MergeCol(1,TRUE);
	


	//获取panel,outputboard,output组合框所选择的当前值；
	CString strpanel,stroutputboard,stroutput,stroutputname;
	GetDlgItem(IDC_COMBO_panel)->GetWindowText(strpanel);
	GetDlgItem(IDC_COMBO_outputboard)->GetWindowText(stroutputboard);
	GetDlgItem(IDC_COMBO_output)->GetWindowText(stroutput);


	//将选择的数据显示在flexgrid控件上

	int itmp_ = m_ListBox.GetCurSel();//获取编号
	//判断是组还是输入
	CString strtmp_,strtmp1_;
	GetDlgItem(IDC_COMBO1)->GetWindowText(strtmp_);
	if (strtmp_.CompareNoCase(_T("Input-Output-Map")) == 0)//确认是input还是group
		strtmp1_=Get_Table_Name(m_sn,_T("Input"),itmp_+1);
	else
		strtmp1_=Get_Table_Name(m_sn,_T("Group"),itmp_+1);

	m_vecaddoutputs.clear();
	map<CString,vecaddoutputs>::iterator iter_;//查找组中的元素
	iter_ = m_mapaddoutputs.find(strtmp1_);
	if (iter_ != m_mapaddoutputs.end())
		m_vecaddoutputs = iter_->second;




	int i  = m_vecaddoutputs.size()+1;
	m_msflexgrid1to96.put_TextMatrix(i,0,strpanel);
	int ctoitmp = _ttoi(stroutputboard);
	if (ctoitmp<10)
		stroutputboard.Format(_T("00%d"),ctoitmp);
	else if (ctoitmp<100)
		stroutputboard.Format(_T("0%d"),ctoitmp);
	
	stroutputboard = strpanel+ _T("--out\n") + stroutputboard;
	m_msflexgrid1to96.put_TextMatrix(i,1,stroutputboard);
	m_msflexgrid1to96.put_TextMatrix(i,2,stroutput);
	stroutputname =  _T("output")+stroutput;
	m_msflexgrid1to96.put_TextMatrix(i,3,stroutputname);
	


	//将选择的数据保存在内存中vector:: m_vecaddoutputs
	m_structaddoutputs.stroutputno = stroutput;
	m_structaddoutputs.stroutputboard = stroutputboard;
	m_structaddoutputs.strpanel = strpanel;
	m_structaddoutputs.stroutputname = stroutputname;
	m_vecaddoutputs.push_back(m_structaddoutputs);

	



 	int itmp = m_ListBox.GetCurSel();

	//判断是组还是输入
	CString strtmp,strtmp1;
	GetDlgItem(IDC_COMBO1)->GetWindowText(strtmp);
	if (strtmp.CompareNoCase(_T("Input-Output-Map")) == 0)
		strtmp1.Format(_T("input%d"),itmp+1);
	else
		strtmp1.Format(_T("group%d"),itmp+1);

	//m_mapaddoutputs.clear();
	map<CString,vecaddoutputs>::iterator iter;
	iter = m_mapaddoutputs.find(strtmp1);
	if (iter != m_mapaddoutputs.end())
		m_mapaddoutputs.erase(iter);

	m_mapaddoutputs.insert(map<CString,vecaddoutputs>::value_type(strtmp1,m_vecaddoutputs));

	outputno = _ttoi(stroutput);
	if (outputno != 32)
	{
		outputno+=1;
		stroutput.Format(_T("%d"),outputno);
		GetDlgItem(IDC_COMBO_output)->SetWindowText(stroutput);
	}
}

BOOL CLightingController::GetSerialComm( vector<CString>& szComm )
{
	LPCTSTR strRegEntry = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");

	HKEY   hKey;   
	LONG   lReturnCode=0;   
	lReturnCode=::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegEntry, 0, KEY_READ, &hKey);  
	BOOL ret = FALSE;

	if(lReturnCode==ERROR_SUCCESS)   
	{  
		DWORD dwIndex = 0;
		WCHAR lpValueName[MAX_PATH];
		ZeroMemory(lpValueName, sizeof(lpValueName));
		DWORD lpcchValueName = MAX_PATH; 
		DWORD lpReserved = 0; 
		DWORD lpType = REG_SZ; 
		BYTE		lpData[MAX_PATH]; 
		ZeroMemory(lpData, MAX_PATH);
		DWORD lpcbData = MAX_PATH;
		dwIndex = 0;
		while(RegEnumValue(	hKey, dwIndex, lpValueName, &lpcchValueName, 0, &lpType, lpData, &lpcbData ) != ERROR_NO_MORE_ITEMS)
		{   
			//TRACE("Registry's   Read!");   
			dwIndex++;

			lpcchValueName = MAX_PATH; 
			lpValueName[0] = '\0'; 

			WCHAR ch[MAX_PATH];
			ZeroMemory(ch, MAX_PATH);
			memcpy(ch, lpData, lpcbData);
			CString str = CString(ch);
			szComm.push_back(str);

			ZeroMemory(lpData, MAX_PATH);
			lpcbData = MAX_PATH;
			ret = TRUE;

		}   
		::RegCloseKey(hKey);   		   

		return ret;
	}

	return ret;  


}

LRESULT CLightingController::OnWriteMessage( WPARAM wParam,LPARAM lParam)
{
	int ret =0;
	ret = write_one(254,wParam,(short)lParam);
	if(ret>0)
		AfxMessageBox(_T("Setting successful"));
	else
		AfxMessageBox(_T("Setting fail"));
	return TRUE;
}

void CLightingController::readSerial()
{
	CADO m_ado;
	m_ado.OnInitADOConn();

// 	CString SQL = _T("select * from LightingController"); 
// 	m_ado.m_pRecordset = m_ado.OpenRecordset(SQL);
// 	int num =0;
// 	_variant_t vartemp;
// 	veclightingcontroller.clear();
// 	while(!m_ado.m_pRecordset->EndOfFile)
// 	{
// 		m_veclightingcontroller.iaddress =m_ado.m_pRecordset->GetCollect(_T("Address"));
// 
// 
// 		vartemp =m_ado.m_pRecordset->GetCollect(_T("Data"));
// 		if (vartemp.vt==VT_NULL)
// 			m_veclightingcontroller.CStvalue =_T("");
// 		else
// 			m_veclightingcontroller.CStvalue =vartemp;
// 
// 
// 		m_ado.m_pRecordset->MoveNext();
// 
// 		veclightingcontroller.push_back(m_veclightingcontroller);
// 	}

	//读取本电脑当前正在使用的串口。
#if 1//会出问题，就是如果在数据存的这个串口与电脑的串口号不对应，则本类永远无法有正确数据采集？
	CString CSTcompot;
	BOOL BOdefault;
	CString SQL = _T("select * from Building");
	m_ado.m_pRecordset = m_ado.OpenRecordset(SQL);
	while(!m_ado.m_pRecordset->EndOfFile)
	{
		CSTcompot =(_variant_t)m_ado.m_pRecordset->GetCollect(_T("Com_Port"));
		BOdefault =(_variant_t)m_ado.m_pRecordset->GetCollect(_T("Default_SubBuilding"));//TRUE = -1即非0，FALSE就是0
		m_ado.m_pRecordset->MoveNext();

		if (BOdefault)
		{
			comnum = _wtoi(CSTcompot.Mid(3));
		}

	}

#endif

	m_ado.CloseRecordset();
	m_ado.CloseConn();

}

void CLightingController::OnBnClickedButtonApply()
{
#if 1//原有NC

	if(!CheckSettingChanged()) // if setting changed,return TRUE
	{
		return; // unchanged, don't write register
	}

	if(g_NetWorkLevel==1)
		return;
	BeginWaitCursor();
	//m_nListenPort	
	CString strText;
	m_listenPortEdit.GetWindowText(strText);
	if(!strText.IsEmpty())
		m_nListenPort=_wtoi(strText);
	if(m_nListenPort>=12767)
	{
		AfxMessageBox(_T("The listen port number is too big, please change it."));
		return;
	}
	if(m_nListenPort<=0)
	{
		AfxMessageBox(_T("The listen port number must be greater than 0!"));
		return;
	}
	write_one(g_tstat_id,120,m_nListenPort);
	BYTE address1,address2,address3,address4;
	m_ip_addressCtrl.	GetAddress(address1,address2,address3,address4);
	int n=write_one(g_tstat_id,107,address1);
	n=write_one(g_tstat_id,108,address2);
	n=write_one(g_tstat_id,109,address3);
	n=write_one(g_tstat_id,110,address4);
	m_subnet_addressCtrl.GetAddress(address1,address2,address3,address4);
	write_one(g_tstat_id,111,address1);
	write_one(g_tstat_id,112,address2);
	write_one(g_tstat_id,113,address3);
	write_one(g_tstat_id,114,address4);
	m_gateway_addressCtrl.GetAddress(address1,address2,address3,address4);
	write_one(g_tstat_id,115,address1);
	write_one(g_tstat_id,116,address2);
	write_one(g_tstat_id,117,address3);
	write_one(g_tstat_id,118,address4);
	write_one(g_tstat_id,131,1);
	Sleep(1000);//Sleep(10000); // ???
	write_one(g_tstat_id,133,1);
	Sleep(5000);	//Sleep(5000); // wait for nc restart

	CMainFrame* pPraent=(CMainFrame*)GetParent();
	//pPraent->ReFresh();

	/*
	unsigned short variable[13]={0};
	int nRet=Read_Multi(g_tstat_id,variable,106,13,3);/////////////////////////read
	m_ipModelComBox.SetCurSel(variable[0]);
	m_ip_addressCtrl.SetAddress(variable[1],variable[2],variable[3],variable[4]);
	m_subnet_addressCtrl.SetAddress(variable[5],variable[6],variable[7],variable[8]);
	m_gateway_addressCtrl.SetAddress(variable[9],variable[10],variable[11],variable[12]);

	*/

	CString strBuilding,strSubBuilding;
	strBuilding=pPraent->m_strCurMainBuildingName;
	strSubBuilding=pPraent->m_strCurSubBuldingName;
	CString strIP,strPort;
	strPort.Format(_T("%d"),multi_register_value[120]);
	m_ip_addressCtrl.GetAddress(address1,address2,address3,address4);
	strIP.Format(_T("%d.%d.%d.%d"),address1,address2,address3,address4);
	if(g_CommunicationType==1)//TCP
	{
		int nPort;
		nPort=_wtoi(strPort);
		//m_strIP=build_info.strIp; 
		g_CommunicationType=1;
		SetCommunicationType(g_CommunicationType);
		BOOL bOk=FALSE;
		bOk=Open_Socket2(strIP,nPort);
		CString strInfo;

		strInfo.Format((_T("Open IP:%s successful")),strIP);//prompt info;
		SetPaneString(1,strInfo);
		if(bOk)
		{
		try
		{


			_ConnectionPtr t_pCon;//for ado connection
			t_pCon.CreateInstance(_T("ADODB.Connection"));
			t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
			CString strSql;
			strSql.Format(_T("update Building set Ip_Address='%s' where Ip_Address='%s'"),strIP,pPraent->m_strIP);
			t_pCon->Execute(strSql.GetString(),NULL,adCmdText);

			// 改node
			CString strSID;
			strSID.Format(_T("%d"), m_inaddress);
			strSql.Format(_T("update ALL_NODE set Bautrate='%s' where Serial_ID='%s'"),strIP, strSID); //bautrate 放IP
			t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			if(t_pCon->State)
				t_pCon->Close();

		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}




			pPraent->m_strIP=strIP;
			pPraent->ScanTstatInDB();

		}
		else
		{

		}



		/*
		_ConnectionPtr t_pCon;//for ado connection
		t_pCon.CreateInstance(_T("ADODB.Connection"));
		t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
		CString strSql;
		strSql.Format(_T("update Building set Ip_Address='%s',Ip_Port='%s' where Main_BuildingName='%s' and Building_Name='%s'"),strIP,strPort,strBuilding,strSubBuilding);
		t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
		if(t_pCon->State)
		t_pCon->Close();
		pPraent->OnConnect();
		*/
	}
	//write_one(g_tstat_id,131,1);
	Fresh();
	EndWaitCursor();
#endif
}

BOOL CLightingController::CheckSettingChanged()
{
	BYTE f1,f2,f3,f4;
	m_ip_addressCtrl.GetAddress(f1,f2,f3,f4);

	if (multi_register_value[107] != f1 
		|| multi_register_value[108] != f2 
		|| multi_register_value[109] != f3 
		|| multi_register_value[110] != f4 )
	{
		return TRUE;
	}

	m_subnet_addressCtrl.GetAddress(f1,f2,f3,f4);
	if (multi_register_value[111] != f1 
		|| multi_register_value[112] != f2 
		|| multi_register_value[113] != f3 
		|| multi_register_value[114] != f4 )
	{
		return TRUE;
	}

	m_gateway_addressCtrl.GetAddress(f1,f2,f3,f4);
	if (multi_register_value[115] != f1 
		|| multi_register_value[116] != f2 
		|| multi_register_value[117] != f3 
		|| multi_register_value[118] != f4 )
	{
		return TRUE;
	}

	return FALSE;

}
void CLightingController::OnBnClickedButtonReboot()
{
	if(g_NetWorkLevel==1)
		return;
	write_one(g_tstat_id,133,1);
}

void CLightingController::OnBnClickedButtonResetToFactory()
{
	if(g_NetWorkLevel==1)
		return;
	write_one(g_tstat_id,130,0);
	Sleep(5000);//Sleep because network controller is busy now 

}
void CLightingController::OnBnClickedCheckEnableEdit()
{
	if (m_ReadOnlyCheckBtn.GetCheck()==BST_CHECKED) 
	{
		m_ipModelComBox.EnableWindow(TRUE);
		//m_listenPortEdit.EnableWindow(TRUE);
		//m_ip_addressCtrl.EnableWindow(TRUE);
		//m_subnet_addressCtrl.EnableWindow(TRUE);
		//m_gateway_addressCtrl.EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(TRUE);
	}
	else
	{
		m_ipModelComBox.EnableWindow(FALSE);
		//m_listenPortEdit.EnableWindow(FALSE);
		//m_ip_addressCtrl.EnableWindow(FALSE);
		//m_subnet_addressCtrl.EnableWindow(FALSE);
		//m_gateway_addressCtrl.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(FALSE);
	}
}

void CLightingController::OnBnClickedButtonConfigure()
{
	Cconfigure dlg;
	dlg.DoModal();
}

 

//改变颜色

HBRUSH CLightingController::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);


	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_STATUS1:
	{
		if (m_light[0])
		{
			pDC->SetTextColor(RGB(0,255,255));
			pDC->SetBkColor(RGB(0,255,255));
			pDC->SetBkMode(TRANSPARENT);


			HBRUSH B = CreateSolidBrush(RGB(0,255,255));
			return (HBRUSH)B;
		} 
		else
		{
			pDC->SetTextColor(RGB(130,130,130));
			pDC->SetBkColor(RGB(130,130,130));
			pDC->SetBkMode(TRANSPARENT);


			HBRUSH B = CreateSolidBrush(RGB(130,130,130));
			return (HBRUSH)B;
		}

		
	}
	case IDC_STATUS2:
		{
			if (m_light[1])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));
					return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));
					return (HBRUSH)B;
			}

		
		}
	case IDC_STATUS3:
		{
			if (m_light[2])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));
				return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));
				return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS4:
		{
			if (m_light[3])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS5:
		{
			if (m_light[4])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS6:
		{
			if (m_light[5])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS7:
		{
			if (m_light[6])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS8:
		{
			if (m_light[7])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS9:
		{
			if (m_light[8])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS10:
		{
			if (m_light[9])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS11:
		{
			if (m_light[10])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS12:
		{
			if (m_light[11])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS13:
		{
			if (m_light[12])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS14:
		{
			if (m_light[13])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS15:
		{
			if (m_light[14])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS16:
		{
			if (m_light[15])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS17:
		{
			if (m_light[16])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS18:
		{
			if (m_light[17])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS19:
		{
			if (m_light[18])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}
	case IDC_STATUS20:
		{
			if (m_light[19])
			{
				pDC->SetTextColor(RGB(0,255,255));
				pDC->SetBkColor(RGB(0,255,255));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(0,255,255));return (HBRUSH)B;
			} 
			else
			{
				pDC->SetTextColor(RGB(130,130,130));
				pDC->SetBkColor(RGB(130,130,130));
				pDC->SetBkMode(TRANSPARENT);


				HBRUSH B = CreateSolidBrush(RGB(130,130,130));return (HBRUSH)B;
			}

			
		}

	//case IDC_NIGHT_EDIT:
	//	{
	//		pDC->SetTextColor(RGB(255,255,255));
	//		pDC->SetBkColor(RGB(0,0,255));
	//		pDC->SetBkMode(TRANSPARENT);
	//		HBRUSH B = CreateSolidBrush(RGB(0,0,255));
	//		return (HBRUSH)B;
	//	}
	//case IDC_EDIT_DAYHEAT:
	//	{
	//		pDC->SetTextColor(RGB(255,255,255));
	//		//pDC->SetBkColor(RGB(255,255,255));
	//		pDC->SetBkMode(TRANSPARENT);
	//		HBRUSH B = CreateSolidBrush(RGB(255,0,0));
	//		return (HBRUSH)B;
	//	}
	//case IDC_NIGHTHEAT_EDIT:
	//	{
	//		pDC->SetTextColor(RGB(255,255,255));
	//		//pDC->SetBkColor(RGB(255,255,255));
	//		pDC->SetBkMode(TRANSPARENT);
	//		HBRUSH B = CreateSolidBrush(RGB(255,0,0));
	//		return (HBRUSH)B;
	//	}
	default:
		return CFormView::OnCtlColor(pDC,pWnd, nCtlColor);
	}

}

//选择A/M
void CLightingController::OnCbnSelchangeValuecombo()
{
	int nItem=0;
	nItem=m_combox_controler.GetCurSel();
    if (m_nCurCol==2)
    {
	  write_one(g_tstat_id,INPUT_AM+(m_nCurRow-1),nItem);
    }
	if (m_nCurCol==1)
	{
	  write_one(g_tstat_id,INPUT_MB+(m_nCurRow-1),nItem);
	}
	Fresh_Inputs();

	 
}

void CLightingController::OnDestroy(){
	if (LC_Thread)
	{
		PostThreadMessage(LC_Thread->m_nThreadID,  WM_QUIT,0,0);
		if (WaitForSingleObject(LC_Thread->m_hThread, 1000) != WAIT_OBJECT_0)

			if (WaitForSingleObject(LC_Thread->m_hThread, 100) == WAIT_OBJECT_0)
			{
			}
			else
			{
				TerminateThread(LC_Thread->m_hThread, 0);
			}
	}
CFormView::OnDestroy();
}

