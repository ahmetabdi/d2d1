#ifndef MODULE_H
#define MODULE_H

#pragma once

#include "d2d1.h"

class Module {
public:
	Module(DWORD_PTR ImageBase, DWORD_PTR ImageSize, const std::string& ImageName, const std::string& ImagePath);
	~Module();

	DWORD_PTR			GetImage() const;
	DWORD_PTR			GetSize() const;
	const std::string&	GetName() const;
	const std::string&	GetPath() const;

private:
	DWORD_PTR			m_ImageBase;
	DWORD_PTR			m_ImageSize;
	std::string			m_ImageName;
	std::string			m_ImagePath;
};

#endif // MODULE_H