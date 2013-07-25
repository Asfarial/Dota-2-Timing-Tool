#include "Reader.h"
namespace CReader
{
  TCHAR* MyClass::ReadConfigForWindow()
	{
		HANDLE hr;
		try
		{
			hr = CreateFile(L"config.cfg", GENERIC_READ, 0,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD dwSize = GetFileSize(hr, &dwSize);
			DWORD size=0;
			char* buf=new char[dwSize];
			ReadFile(hr, buf, dwSize, &size, NULL);
			buf[dwSize]='\0';
			TCHAR* tBuf=new TCHAR[dwSize];
			MultiByteToWideChar(CP_ACP, 0,  buf, -1, tBuf,size+1);
			buf=0;
			delete [] buf;
			int i=0;
			int entry=0;
			static TCHAR retBuf[50];
			ZeroMemory(&retBuf, 50);
			while(i!=size+1)
			{
				if(tBuf[i]=='"')
				{
					if(tBuf[i+1]=='/')
					{
						_tcsncat_s(retBuf, 50, &tBuf[entry+1], i-entry-1);
						_tcscat_s(retBuf, 50, L"|");
					}else
					{
						entry=i;
					}
				}
				++i;
			}
			tBuf=0;
			delete [] tBuf;
			CloseHandle(hr);
			return retBuf;
		}
		catch(...)
		{
			CloseHandle(hr);
			return L"";
		}
	}
	TCHAR* MyClass::ReadConfig()
	{
		HANDLE hr;
		try
		{
			hr = CreateFile(L"config.cfg", GENERIC_READ, 0,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD dwSize = GetFileSize(hr, &dwSize);
			DWORD size=0;
			char* buf=new char[dwSize];
			ReadFile(hr, buf, dwSize, &size, NULL);
			buf[dwSize]='\0';
			TCHAR* tBuf=new TCHAR[dwSize];
			MultiByteToWideChar(CP_ACP, 0,  buf, -1, tBuf,size+1);
			buf=0;
			delete [] buf;
			int i=0;
			static TCHAR retBuf[100];
			ZeroMemory(&retBuf, 100);
			while(i!=size+1)
			{
				if(tBuf[i]=='\r')
				{
					tBuf[i]=' ';
				}else if(tBuf[i]=='\n')
				{
					tBuf[i]=' ';
				}
				++i;
			}
			_tcscat_s(retBuf, 100, tBuf);
			tBuf=0;
			delete [] tBuf;
			CloseHandle(hr);
			return retBuf;
		}
		catch(...)
		{
			CloseHandle(hr);
			return L"";
		}
	}
	void MyClass::CreateConfig()
	{
		HANDLE hr = CreateFile(L"config.cfg", GENERIC_WRITE, 0,NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD written;
		char data[] = "<volume level=\"80\"/>\r\n<rate speed=\"0\"/>\r\n<voice required=\"Gender=Male;Age=Adult\"/>";
		DWORD len  = (DWORD)strlen(data);
		WriteFile(hr, data, len, &written, NULL);
		CloseHandle(hr);
	}
	void MyClass::ChangeConfig(LPCWSTR dataTransfer, int len)
	{
		HANDLE hr;
		try
		{
			hr = CreateFile(L"config.cfg", GENERIC_WRITE, 0,NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			char* data = new char[len];
			ZeroMemory(data, len);
			WideCharToMultiByte(CP_ACP,0,dataTransfer, -1, data, len+1,NULL,NULL);
			DWORD written;
			DWORD sendSize  = (DWORD)strlen(data);
			WriteFile(hr, data, sendSize, &written, NULL);
			data=0;
			delete[]data;
			CloseHandle(hr);
		}
		catch(...)
		{
			CloseHandle(hr);
			CreateConfig();
		}
	}
}
