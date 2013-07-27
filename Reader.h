#ifndef READER_H
#define READER_H

#include <Windows.h>
#include <tchar.h>
namespace CReader
{
	class RDLL
	{
	public:
		static __declspec(dllexport) TCHAR* ReadConfig();
		static __declspec(dllexport) TCHAR* ReadConfigForWindow();
		static __declspec(dllexport) void CreateConfig();
		static __declspec(dllexport) void ChangeConfig(LPCWSTR dataTransfer, char len);
		static __declspec(dllexport) TCHAR* returnVersion();
	};
}
#endif
