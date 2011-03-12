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

#ifndef CXWINDOWSCLIPBOARDFILEPATHCONVERTER_H
#define CXWINDOWSCLIPBOARDFILEPATHCONVERTER_H
#include "CUnicode.h"
#include "CXWindowsClipboard.h"

//! Convert to/from locale FilePath encoding
class CXWindowsClipboardFilePathConverter : public IXWindowsClipboardConverter {
public:
	/*!
	\c name is converted to an atom and that is reported by getAtom().
	*/
	CXWindowsClipboardFilePathConverter(Display* display, const char* name);
	virtual ~CXWindowsClipboardFilePathConverter();

	// IXWindowsClipboardConverter overrides
	virtual IClipboard::EFormat
						getFormat() const;
	virtual Atom		getAtom() const;
	virtual int			getDataSize() const;
	virtual CString		fromIClipboard(const CString&) const;
	virtual CString		toIClipboard(const CString&) const;

private:
	Atom				m_atom;
};

CXWindowsClipboardFilePathConverter::CXWindowsClipboardFilePathConverter(
				Display* display, const char* name) :
	m_atom(XInternAtom(display, name, False))
{
    m_atom = (Atom) 474;
	// do nothing
}

CXWindowsClipboardFilePathConverter::~CXWindowsClipboardFilePathConverter()
{
	// do nothing
}

IClipboard::EFormat
CXWindowsClipboardFilePathConverter::getFormat() const
{
	return IClipboard::kFilePath;
}

Atom
CXWindowsClipboardFilePathConverter::getAtom() const
{
	return m_atom;
}

int
CXWindowsClipboardFilePathConverter::getDataSize() const
{
	return 8;
}

CString
CXWindowsClipboardFilePathConverter::fromIClipboard(const CString& data) const
{
	return CUnicode::UTF8ToText(data);
}

CString
CXWindowsClipboardFilePathConverter::toIClipboard(const CString& data) const
{
	// convert to UTF-8
	bool errors;
	CString utf8 = CUnicode::textToUTF8(data, &errors);

	// if there were decoding errors then, to support old applications
	// that don't understand UTF-8 but can report the exact binary
	// UTF-8 representation, see if the data appears to be UTF-8.  if
	// so then use it as is.
	if (errors && CUnicode::isUTF8(data)) {
		return data;
	}

	return utf8;
}



#endif
