/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2002 Chris Schoeneman, Nick Bolton, Sorin Sbarnea
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file COPYING that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "CXWindowsClipboardKdeFilePathConverter.h"
#include "CUnicode.h"
#include "CLog.h"


//
// CXWindowsClipboardKdeFilePathConverter
//

CXWindowsClipboardKdeFilePathConverter::CXWindowsClipboardKdeFilePathConverter(
				Display* display, const char* name) :
	m_atom(XInternAtom(display, name, False))
{
    LOG(( CLOG_INFO "CXWindowsClipboardKdeFilePathConverter::Constructor\nAtom: %i", m_atom));
	// do nothing
}

CXWindowsClipboardKdeFilePathConverter::~CXWindowsClipboardKdeFilePathConverter()
{
	// do nothing
}

IClipboard::EFormat
CXWindowsClipboardKdeFilePathConverter::getFormat() const
{
	return IClipboard::kFilePath;
}

Atom
CXWindowsClipboardKdeFilePathConverter::getAtom() const
{
	return m_atom;
}

int
CXWindowsClipboardKdeFilePathConverter::getDataSize() const
{
	return 8;
}

CString
CXWindowsClipboardKdeFilePathConverter::fromIClipboard(const CString& data) const
{
    LOG((CLOG_INFO "Converting  %s to uri/file-list filepath", data.c_str()));
    CString buffer;
    buffer.append("file://");
    buffer.append(data);
    for (size_t i = 0; i < buffer.size(); ++ i)
    	if (buffer[i] == '\n')
    		buffer.insert(i+1,"file://");
    LOG((CLOG_INFO "Converted string: %s", buffer.c_str()));
    return buffer;
}

CString
CXWindowsClipboardKdeFilePathConverter::toIClipboard(const CString& data) const
{
    CString buffer;
    LOG((CLOG_INFO "Converting %s to common filepath format", data.c_str()));
    buffer.append(ARCH->getName());
	buffer.append("\n");
	size_t p = 0;
	while (p != CString::npos)
	{

		size_t end = data.find("\n",p+1);
		if (end == CString::npos)
					end = data.size()-1;
		buffer.append(data.substr(p+7,end-p-6));
		p = data.find("file://",p+1);
		LOG ((CLOG_INFO "New p is: %d",p));
	}
    LOG((CLOG_INFO "Converted data: %s", buffer.c_str()));
    return buffer;
}


