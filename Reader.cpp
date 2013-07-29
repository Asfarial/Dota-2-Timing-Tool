#include "Reader.h"
namespace CReader
{
	TCHAR* RDLL::ReadConfigForWindow()
	{
		HANDLE hr;
		hr = CreateFile(L"config.cfg", GENERIC_READ, 0,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hr==INVALID_HANDLE_VALUE)
		{
			CloseHandle(hr);
			return L"";
		}
		LARGE_INTEGER dwSize1;
		if(!GetFileSizeEx(hr, &dwSize1))
		{
			CloseHandle(hr);
			return L"";
		}
		int dwSize = (int)dwSize1.LowPart;
		DWORD size=0;
		char* buf=new char[dwSize];
		ReadFile(hr, buf, dwSize, &size, NULL);
		CloseHandle(hr);
		buf[dwSize]='\0';
		TCHAR* tBuf=new TCHAR[dwSize];
		MultiByteToWideChar(CP_ACP, 0,  buf, -1, tBuf,size);
		buf=0;
		delete [] buf;
		char i=0;
		char entry=0;
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
		return retBuf;
	}
	TCHAR* RDLL::ReadConfig()
	{
		HANDLE hr;
		hr = CreateFile(L"config.cfg", GENERIC_READ, 0,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hr==INVALID_HANDLE_VALUE)
		{
			CloseHandle(hr);
			return L"";
		}
		LARGE_INTEGER dwSize1;
		if(!GetFileSizeEx(hr, &dwSize1))
		{
			CloseHandle(hr);
			return L"";
		}
		int dwSize = (int)dwSize1.LowPart;
		DWORD size=0;
		char* buf=new char[dwSize+1];
		ReadFile(hr, buf, dwSize+1, &size, NULL);
		CloseHandle(hr);
		buf[dwSize]='\0';
		TCHAR* tvBuf=new TCHAR[dwSize];
		MultiByteToWideChar(CP_ACP, 0,  buf, -1, tvBuf,size);
		buf=0;
		delete [] buf;
		
		char i=0;
		static TCHAR retvBuf[110];
		ZeroMemory(&retvBuf, 110);

		while(i!=size+1)
		{
			if(tvBuf[i]=='\r')
			{
				tvBuf[i]=' ';
			}else if(tvBuf[i]=='\n')
			{
				tvBuf[i]=' ';
			}
			++i;
		}

		_tcscat_s(retvBuf, 110, tvBuf);
		tvBuf=0;
		delete [] tvBuf;
		return retvBuf;
	}
	void RDLL::CreateConfig()
	{
		HANDLE hr = CreateFile(L"config.cfg", GENERIC_WRITE, 0,NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD written;
		char data[] = "<volume level=\"80\"/>\r\n<rate speed=\"0\"/>\r\n<voice required=\"Gender=Male;Age=Adult\"/>";
		DWORD len  = (DWORD)strlen(data);
		WriteFile(hr, data, len, &written, NULL);
		CloseHandle(hr);
		return;
	}
	void RDLL::ChangeConfig(LPCWSTR dataTransfer, char len)
	{
		HANDLE hr;
		hr = CreateFile(L"config.cfg", GENERIC_WRITE, 0,NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hr==INVALID_HANDLE_VALUE)
		{
			CloseHandle(hr);
			CreateConfig();
			return;
		}
		char* data = new char[len];
		ZeroMemory(data, len);
		WideCharToMultiByte(CP_ACP,0,dataTransfer, -1, data, len+1,NULL,NULL);
		DWORD written;
		DWORD sendSize  = (DWORD)strlen(data);
		WriteFile(hr, data, sendSize, &written, NULL);
		CloseHandle(hr);
		data=0;
		delete[]data;
		return;
	}
	TCHAR* RDLL::returnVersion()
	{
		return L"1.2.2";
	}
}
