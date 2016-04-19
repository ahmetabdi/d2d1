#include "stdafx.h"
#include "Module.h"

Module::Module(DWORD_PTR ImageBase, DWORD_PTR ImageSize, const std::string& ImageName, const std::string& ImagePath)
	: m_ImageBase(ImageBase),
	m_ImageSize(ImageSize),
	m_ImageName(ImageName),
	m_ImagePath(ImagePath) {}

Module::~Module() {

}

DWORD_PTR Module::GetImage() const {
	return m_ImageBase;
}

DWORD_PTR Module::GetSize() const {
	return m_ImageSize;
}

const std::string& Module::GetName() const {
	return m_ImageName;
}

const std::string& Module::GetPath() const {
	return m_ImagePath;
}