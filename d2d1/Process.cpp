#include "stdafx.h"
#include "Process.h"

Process* process = new Process();
Module* client = nullptr;
Module* engine = nullptr;

Process::Process() {

}

Process::~Process() {
	Detach();
}

bool Process::Attach(const std::string& ExeName) {
	Detach();

	if (ExeName.empty()) {
		return false;
	}

	m_dwProcessId = GetProcessIdByName(ExeName);

	if (!m_dwProcessId) {
		return false;
	}

	m_hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, m_dwProcessId);

	if (!m_hProcess) {
		return false;
	}

	return DumpModList();
}

bool Process::AttachWindow(HWND window)
{
	Detach();

	GetWindowThreadProcessId(window, &m_dwProcessId);

	if (!m_dwProcessId) {
		return false;
	}

	m_hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, m_dwProcessId);

	if (!m_hProcess) {
		return false;
	}

	DumpModList();

	return true;
}

void Process::Detach() {
	if (m_hProcess) {
		CloseHandle(m_hProcess);
	}

	m_hProcess = NULL;
	m_dwProcessId = NULL;

	SAFE_DELETE_VECTOR(m_pModList);
}

bool Process::Read(DWORD_PTR dwAddress, LPVOID lpBuffer, DWORD_PTR dwSize) {
	SIZE_T Out = NULL;

	return (ReadProcessMemory(m_hProcess, (LPCVOID)dwAddress, lpBuffer, dwSize, &Out) == TRUE);
}

bool Process::Write(DWORD_PTR dwAddress, LPCVOID lpBuffer, DWORD_PTR dwSize) {
	SIZE_T Out = NULL;

	return (WriteProcessMemory(m_hProcess, (LPVOID)dwAddress, lpBuffer, dwSize, &Out) == TRUE);
}

Module* Process::GetModule(const std::string& ImageName) {
	if (m_pModList.empty()) {
		return nullptr;
	}

	for (auto& pMod : m_pModList) {
		if (ImageName.compare(pMod->GetName()) == 0) {
			return pMod;
		}
	}

	return nullptr;
}

HMODULE Process::LoadRemote(const std::string& ImageName) {
	if (m_pModList.empty()) {
		return NULL;
	}

	for (auto& pMod : m_pModList) {
		if (ImageName.compare(pMod->GetName()) == 0) {
			return LoadLibrary(pMod->GetPath().c_str());
		}
	}

	return NULL;
}

bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask) {
	for (; *pszMask; ++pszMask, ++pData, ++pMask) {
		if (*pszMask == 'x' && *pData != *pMask) {
			return false;
		}
	}

	return (*pszMask == NULL);
}

DWORD_PTR Process::Scan(DWORD_PTR dwStart, DWORD_PTR dwSize, const char* pSignature, const char* pMask) {
	BYTE* pRemote = new BYTE[dwSize];

	if (!Read(dwStart, pRemote, dwSize)) {
		return NULL;
	}

	for (DWORD_PTR dwIndex = 0; dwIndex < dwSize; dwIndex++) {
		if (DataCompare((const BYTE*)(pRemote + dwIndex), (const BYTE*)pSignature, pMask)) {
			return (dwStart + dwIndex);
		}
	}

	return NULL;
}

bool Process::DumpModList() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return false;
	}

	MODULEENTRY32 Entry = { NULL };

	Entry.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(hSnapshot, &Entry)) {
		CloseHandle(hSnapshot);
		return false;
	}

	Module* pMod = nullptr;

	do {
		char szPath[MAX_PATH] = { NULL };

		GetModuleFileNameEx(m_hProcess, Entry.hModule, szPath, MAX_PATH);

		pMod = new Module((DWORD_PTR)Entry.hModule, (DWORD_PTR)Entry.modBaseSize, Entry.szModule, szPath);
		m_pModList.push_back(pMod);
	} while (Module32Next(hSnapshot, &Entry));

	CloseHandle(hSnapshot);

	return !m_pModList.empty();
}

DWORD Process::GetProcessIdByName(const std::string& name) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	PROCESSENTRY32 Entry = { NULL };

	Entry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnapshot, &Entry)) {
		CloseHandle(hSnapshot);
		return NULL;
	}

	do {
		if (name.compare(Entry.szExeFile) == 0) {
			break;
		}
	} while (Process32Next(hSnapshot, &Entry));


	CloseHandle(hSnapshot);

	return Entry.th32ProcessID;
}