#include "stdafx.h"
#include "ADOConn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

ADOConn::ADOConn()
{
}


ADOConn::~ADOConn()
{
}

// ��ʼ�����������ݿ�
void ADOConn::OnInitADOConn()
{
	// ��ʼ��OLE/COM�⻷��
	::CoInitialize(NULL);
	try
	{
		// ����Connection����
		m_pConnection.CreateInstance("ADODB.Connection");//(__uuidof(Connection));
		// ���������ַ�����������BSTR�ͻ���_bstr_t����
		//_bstr_t strConnect = "Provider=SQLOLEDB;Persist Security Info=true;Data Source=.;Initial Catalog=AntTribal; User ID=sa; Password=123456;";
		_bstr_t strConnect = "Provider=SQLOLEDB.1; Server=127.0.0.1;Database=AntTribal; uid=sa; pwd=123456;";
		
		m_pConnection->Open(strConnect, "", "", adModeUnknown);
	}
	// ��׽�쳣
	catch (_com_error e)
	{
		// ��ʾ������Ϣ
		AfxMessageBox(e.Description());
	}
}

// ִ�в�ѯ
_RecordsetPtr& ADOConn::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		// �������ݿ⣬���Connection����Ϊ�գ��������������ݿ�
		if (m_pConnection == NULL)
			OnInitADOConn();
		// ������¼������
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// ȡ�ñ��еļ�¼
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	}
	// ��׽�쳣
	catch (_com_error e)
	{
		// ��ʾ������Ϣ
		AfxMessageBox(e.Description());
	}
	// ���ؼ�¼��
	return m_pRecordset;
}

// ִ��SQL��䣬Insert Update _variant_t
BOOL ADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
	// _variant_t RecordsAffected;
	try
	{
		// �Ƿ��Ѿ��������ݿ�
		if (m_pConnection == NULL)
			OnInitADOConn();
		// Connection�����Execute����:(_bstr_t CommandText,
		// VARIANT * RecordsAffected, long Options )
		// ����CommandText�������ִ���ͨ����SQL���
		// ����RecordsAffected�ǲ�����ɺ���Ӱ�������,
		// ����Options��ʾCommandText�����ͣ�adCmdText-�ı����adCmdTable-����
		// adCmdProc-�洢���̣�adCmdUnknown-δ֪
		m_pConnection->Execute(bstrSQL, NULL, adCmdText);
		return true;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

void ADOConn::ExitConnect()
{
	// �رռ�¼��������
	if (m_pRecordset != NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	// �ͷŻ���
	::CoUninitialize();
}