#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>

#include "vector.h"
#include "XorString.h"
#include "resource.h"

namespace DMA
{
	typedef class _Process
	{
	private:
		HANDLE DMA = NULL;
		std::wstring procName = L"";
		FARPROC pLcCreateEx = NULL,
			pLcClose = NULL,
			pVMMDLL_PidGetFromName = NULL,
			pVMMDLL_ProcessGetModuleBaseW = NULL,
			pVMMDLL_MemReadEx = NULL,
			pVMMDLL_MemWrite = NULL,
			pVMMDLL_Initialize = NULL,
			pVMMDLL_Close = NULL;

		typedef struct tdLC_CONFIG_ERRORINFO {
			DWORD dwVersion;
			DWORD cbStruct;
			DWORD _FutureUse[16];
			BOOL fUserInputRequest;
			DWORD cwszUserText;
			WCHAR wszUserText[MAX_PATH];
		} LC_CONFIG_ERRORINFO, * PLC_CONFIG_ERRORINFO, ** PPLC_CONFIG_ERRORINFO;

		typedef struct LC_CONFIG {
			DWORD dwVersion;
			DWORD dwPrintfVerbosity;
			CHAR szDevice[MAX_PATH];
			CHAR szRemote[MAX_PATH];
			_Check_return_opt_ int(*pfn_printf_opt)(_In_z_ _Printf_format_string_ char const* const _Format, ...);
			INT64 paMax;
			BOOL fVolatile;
			BOOL fWritable;
			BOOL fRemote;
			BOOL fRemoteDisableCompress;
			CHAR szDeviceName[MAX_PATH];
		} LC_CONFIG, * PLC_CONFIG;

		HANDLE LcCreateEx(_Inout_ PLC_CONFIG pLcCreateConfig, _Out_opt_ PPLC_CONFIG_ERRORINFO ppLcCreateErrorInfo) {
			typedef HANDLE(*Proto)(_Inout_ PLC_CONFIG pLcCreateConfig, _Out_opt_ PPLC_CONFIG_ERRORINFO ppLcCreateErrorInfo);
			if (pLcCreateEx)
				return ((Proto)pLcCreateEx)(pLcCreateConfig, ppLcCreateErrorInfo);
			return NULL;
		}
		VOID LcClose(_In_opt_ _Post_ptr_invalid_ HANDLE hLC) {
			typedef VOID(*Proto)(_In_opt_ _Post_ptr_invalid_ HANDLE hLC);
			if (pLcClose)
				return ((Proto)pLcClose)(hLC);
			return;
		}
		BOOL VMMDLL_PidGetFromName(_In_ LPSTR szProcName, _Out_ PDWORD pdwPID) {
			typedef BOOL(*Proto)(_In_ LPSTR szProcName, _Out_ PDWORD pdwPID);
			if (pVMMDLL_PidGetFromName)
				return ((Proto)pVMMDLL_PidGetFromName)(szProcName, pdwPID);
			return false;
		}
		ULONG64 VMMDLL_ProcessGetModuleBaseW(_In_ DWORD dwPID, _In_ LPWSTR wszModuleName) {
			typedef ULONG64(*Proto)(_In_ DWORD dwPID, _In_ LPWSTR wszModuleName);
			if (pVMMDLL_ProcessGetModuleBaseW)
				return ((Proto)pVMMDLL_ProcessGetModuleBaseW)(dwPID, wszModuleName);
			return NULL;
		}
		BOOL VMMDLL_MemReadEx(_In_ DWORD dwPID, _In_ ULONG64 qwA, _Out_writes_(cb) PBYTE pb, _In_ DWORD cb, _Out_opt_ PDWORD pcbReadOpt, _In_ ULONG64 flags) {
			typedef BOOL(*Proto)(_In_ DWORD dwPID, _In_ ULONG64 qwA, _Out_writes_(cb) PBYTE pb, _In_ DWORD cb, _Out_opt_ PDWORD pcbReadOpt, _In_ ULONG64 flags);
			if (pVMMDLL_MemReadEx)
				return ((Proto)pVMMDLL_MemReadEx)(dwPID, qwA, pb, cb, pcbReadOpt, flags);
			return false;
		}
		BOOL VMMDLL_MemWrite(_In_ DWORD dwPID, _In_ ULONG64 qwA, _In_reads_(cb) PBYTE pb, _In_ DWORD cb) {
			typedef BOOL(*Proto)(_In_ DWORD dwPID, _In_ ULONG64 qwA, _In_reads_(cb) PBYTE pb, _In_ DWORD cb);
			if (pVMMDLL_MemWrite)
				return ((Proto)pVMMDLL_MemWrite)(dwPID, qwA, pb,cb);
			return false;
		}
		BOOL VMMDLL_Initialize(_In_ DWORD argc, _In_ LPSTR argv[]) {
			typedef BOOL(*Proto)(_In_ DWORD argc, _In_ LPSTR argv[]);
			if (pVMMDLL_Initialize)
				return ((Proto)pVMMDLL_Initialize)(argc, argv);
			return false;
		}
		BOOL VMMDLL_Close() {
			typedef BOOL(*Proto)();
			if (pVMMDLL_Close)
				return ((Proto)pVMMDLL_Close)();
			return false;
		}
	public:
		uint32_t pid = NULL;
		_Process(std::wstring procName) : procName(procName)
		{
		START:
			HMODULE vmm = LoadLibraryA(XorString("vmm.dll"));
			if (!vmm)
			{
				MessageBoxA(NULL, XorString("vmm.dll could not be located"), XorString("Missing File"), MB_ICONWARNING);
				TerminateProcess(GetCurrentProcess(), -1);
			}
			HMODULE leechcore = LoadLibraryA(XorString("leechcore.dll"));
			if (!leechcore)
			{
				MessageBoxA(NULL, XorString("leechcore.dll could not be located"), XorString("Missing File"), MB_ICONWARNING);
				TerminateProcess(GetCurrentProcess(), -1);
			}
			pVMMDLL_PidGetFromName = GetProcAddress(vmm, XorString("VMMDLL_PidGetFromName"));
			pVMMDLL_ProcessGetModuleBaseW = GetProcAddress(vmm, XorString("VMMDLL_ProcessGetModuleBaseW"));
			pVMMDLL_MemReadEx = GetProcAddress(vmm, XorString("VMMDLL_MemReadEx"));
			pVMMDLL_MemWrite = GetProcAddress(vmm, XorString("VMMDLL_MemWrite"));
			pVMMDLL_Initialize = GetProcAddress(vmm, XorString("VMMDLL_Initialize"));
			pVMMDLL_Close = GetProcAddress(vmm, XorString("VMMDLL_Close"));
			pLcCreateEx = GetProcAddress(leechcore, XorString("LcCreateEx"));
			pLcClose = GetProcAddress(leechcore, XorString("LcClose"));

			PLC_CONFIG_ERRORINFO pLcErrorInfo = NULL;
			LC_CONFIG cfg;
			ZeroMemory(&cfg, sizeof(cfg));
			cfg.dwVersion = 0xc0fd0002;
			cfg.dwPrintfVerbosity = 0x1;
			strcpy_s(cfg.szDevice, MAX_PATH, XorString("FPGA"));
			cfg.paMax = 0;
			DMA = LcCreateEx(&cfg, &pLcErrorInfo);
			if (DMA)
			{
				LPSTR argv[] = { (LPSTR)"", (LPSTR)("-device"), (LPSTR)("existing") };
				VMMDLL_Initialize(3, argv);
				std::string name = "";
				for (int i = 0; i < procName.size(); i++)
					name.push_back((const char)procName.at(i));
				VMMDLL_PidGetFromName((LPSTR)name.c_str(), (PDWORD) & pid);
				if (!pid)
				{
					LcClose(DMA);
					VMMDLL_Close();
					MessageBox(NULL, (procName + L" isn't running").c_str(), L"Error", MB_ICONWARNING);
					TerminateProcess(GetCurrentProcess(), -1);
				}
			}
			else
			{
				MessageBoxA(NULL, XorString("Error Communicating with DMA"), XorString("Error"), MB_ICONWARNING);
				TerminateProcess(GetCurrentProcess(), -1);
			}

		}

		uint64_t modBase(std::wstring modName)
		{
			if(pid)
				return (uint64_t)VMMDLL_ProcessGetModuleBaseW(pid, (LPWSTR)modName.c_str());
			return 0x0;
		}

		bool rpm(_In_ uint64_t addr, _Out_ PBYTE buffer, _In_ DWORD size)
		{
			DWORD readstat = NULL;
			if (pid)
				return (bool)VMMDLL_MemReadEx((DWORD)pid, (ULONG64)addr, buffer, size, &readstat, 0x1);
			return 0;
		}
		bool wpm(_In_ uint64_t addr, _In_ PBYTE buffer, _In_ DWORD size)
		{
			if (pid)
				return (bool)VMMDLL_MemWrite((DWORD)pid, (ULONG64)addr, buffer, size);
			return 0;
		}
	}Process, *pProcess;
	pProcess proc = NULL;
};
/// <summary>
///	Selects a target process
/// </summary>
/// <param name="FileName">e.g. "notepad.exe"</param>
void SelectProcessA(std::string FileName)
{
	std::wstring name = L"";
	for (int i = 0; i < FileName.size(); i++)
		name.push_back(FileName.at(i));
	DMA::proc = new DMA::Process(name);
}
/// <summary>
///	Selects a target process to r/w memory
/// </summary>
/// <param name="FileName">e.g. L"notepad.exe"</param>
void SelectProcessW(std::wstring FileName)
{
	DMA::proc = new DMA::Process(FileName);
}
/// <summary>
/// Retrieves a module's image base
/// </summary>
/// <param name="str">e.g. "notepad.exe"</param>
/// <returns>return successful if non zero</returns>
uint64_t ModuleBaseA(std::string str)
{
	std::wstring name = L"";
	for (int i = 0; i < str.size(); i++)
		name.push_back(str.at(i));
	if (DMA::proc)
		return DMA::proc->modBase(name);
	return 0x0;
}
/// <summary>
/// Retrieves a module's image base
/// </summary>
/// <returns>return successful if non zero</returns>
uint32_t ProcessPid()
{
	return DMA::proc->pid;
}
/// <summary>
/// Retrieves a module's image base
/// </summary>
/// <param name="str">e.g. L"notepad.exe"</param>
/// <returns>return successful if non zero</returns>
uint64_t ModuleBaseW(std::wstring str)
{
	if (DMA::proc)
		return DMA::proc->modBase(str);
	return 0x0;
}
/// <summary>
/// Retrieves memory from selected process
/// </summary>
/// <param name="va">Target's virtual memory</param>
/// <param name="pb">buffer to retrieve memory</param>
/// <param name="bs">buffer size</param>
/// <returns>return successful if true</returns>
bool read(IN uint64_t va, OUT uint64_t pb, IN uint32_t bs)
{
	if(DMA::proc || va == 0x0)
		return DMA::proc->rpm(va, (PBYTE)pb, (DWORD)bs);
	return 0x0;
}
/// <summary>
/// Retrieves memory from selected process
/// </summary>
/// <param name="va">Target's virtual memory</param>
/// <returns>Success if returns non zero</returns>
template <class AUTO>
AUTO read(IN uint64_t va)
{
	AUTO buffer = { NULL };
	if (DMA::proc || va == 0x0)
	{
		if (DMA::proc->rpm(va, (PBYTE)&buffer, (DWORD)sizeof(AUTO)))
			return buffer;
		else
			return buffer;
	}
	return buffer;
}
/// <summary>
/// Retrieves memory from selected process
/// </summary>
/// <param name="va">Target's virtual memory</param>
/// <returns>Success if returns non zero</returns>
template <>
std::string read(IN uint64_t va)
{
	if (DMA::proc || va == 0x0)
	{
		char buffer[MAX_PATH] = { 0x0 };
		if (DMA::proc->rpm(va, (PBYTE)&buffer[0], (DWORD)MAX_PATH))
			return std::string(buffer);
		else
			return "";
	}
	return "";
}
/// <summary>
/// Retrieves memory from selected process
/// </summary>
/// <param name="va">Target's virtual memory</param>
/// <returns>Success if returns non zero</returns>
template <>
std::wstring read(IN uint64_t va)
{
	if (DMA::proc || va == 0x0)
	{
		wchar_t buffer[MAX_PATH] = { 0x0 };
		if (DMA::proc->rpm(va, (PBYTE)&buffer[0], (DWORD)(MAX_PATH * sizeof(wchar_t))))
			return std::wstring(buffer);
		else
			return  L"";
	}
	return L"";
}
/// <summary>
/// Retrieves memory from selected process
/// </summary>
/// <param name="vac">Target's virtual memory chain</param>
/// <returns>Success if returns non zero</returns>
template <class AUTO>
AUTO read(IN std::vector<uint64_t> vac)
{
	uint64_t addr = 0x0;
	for (DWORD i = 0; i < vac.size() - 1; i++)
	{
		addr = read<uint64_t>(addr + vac.at(i));
	}
	return read<AUTO>(addr + vac.at(vac.size() - 1));
}



bool write(IN uint64_t va, IN uint64_t pb, IN uint32_t bs)
{
	if (DMA::proc || va == 0x0)
		return DMA::proc->wpm(va, (PBYTE)pb, (DWORD)bs);
	return 0x0;
}

template <class AUTO>
bool write(_In_ uint64_t va, AUTO pb)
{
	SIZE_T size = sizeof(AUTO);
	return write((uint64_t)va, (uint64_t)&pb, (DWORD)size);
}

template <class AUTO>
bool write(_In_ std::vector<uint64_t> chain, AUTO pb)
{
	uint64_t Address = 0x0;
	for (DWORD i = 0; i < chain.size() - 1; i++)
	{
		Address = read<uint64_t>(Address + chain.at(i));
	}
	return write<AUTO>(Address + chain.at(chain.size() - 1), pb);
}