#ifndef PROCESS_H
#define PROCESS_H

#pragma once

#include "Module.h"

#define SAFE_DELETE_VECTOR(vec)	if (!vec.empty()) { \
									for (auto& slot : vec) { \
										delete slot; \
										slot = nullptr; \
									} \
								}

class Process {
public:
	Process();
	~Process();

	bool					Attach(const std::string& ExeName);
	bool                    AttachWindow(HWND window);
	void					Detach();

	bool					Read(DWORD_PTR dwAddress, LPVOID lpBuffer, DWORD_PTR dwSize);
	bool					Write(DWORD_PTR dwAddress, LPCVOID lpBuffer, DWORD_PTR dwSize);

	Module*					GetModule(const std::string& ImageName);
	HMODULE					LoadRemote(const std::string& ImageName);

	DWORD_PTR				Scan(DWORD_PTR dwStart, DWORD_PTR dwSize, const char* pSignature, const char* pMask);
	HANDLE					m_hProcess = NULL;

	template<typename T>
	T Read(DWORD_PTR dwAddress, const T& tDefault = T()) {
		T tRet;

		if (!Read(dwAddress, &tRet, sizeof(T))) {
			return tDefault;
		}

		return tRet;
	}

	template<typename T>
	bool Write(DWORD_PTR dwAddress, const T& tValue) {
		return Write(dwAddress, &tValue, sizeof(T));
	}

private:
	bool					DumpModList();
	DWORD					GetProcessIdByName(const std::string& name);

private:
	DWORD					m_dwProcessId = NULL;
	std::vector<Module*>	m_pModList;
};

extern Process*	process;
extern Module*	client;
extern Module*	engine;

#endif // PROCESS_H