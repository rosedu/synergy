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
    size_t pos = data.find(":");
    buffer.append(data.substr(pos+1, data.size()-pos-1));
    LOG((CLOG_INFO "Converted string: %s", buffer.c_str()));
    return buffer;
}

CString
CXWindowsClipboardKdeFilePathConverter::toIClipboard(const CString& data) const
{
    CString buffer;
    LOG((CLOG_INFO "Converting %s to common filepath format", data.c_str()));
    buffer.append(ARCH->getName());
	buffer+=":";
	buffer.append(data.substr(7,data.size()-7));
    LOG((CLOG_INFO "Converted data: %s", buffer.c_str()));
    return buffer;
}


