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

#ifndef CMSWINDOWSCLIPBOARDFILEPATHCONVERTER_H
#define CMSWINDOWSCLIPBOARDFILEPATHCONVERTER_H

//#include "CMSWindowsClipboardAnyTextConverter.h"
#include "CMSWindowsClipboard.h"
#include <ShellAPI.h>

//! Convert to/from locale text encoding
class CMSWindowsClipboardFilePathConverter :
				public IMSWindowsClipboardConverter {
public:
	CMSWindowsClipboardFilePathConverter();
	virtual ~CMSWindowsClipboardFilePathConverter();

	// IMSWindowsClipboardConverter overrides
	virtual IClipboard::EFormat
						getFormat() const;
	virtual UINT		getWin32Format() const;
	virtual HANDLE		fromIClipboard(const CString&) const;
	virtual CString		toIClipboard(HANDLE) const;

	
};

#endif
