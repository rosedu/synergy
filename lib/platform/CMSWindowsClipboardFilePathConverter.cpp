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

#include "CMSWindowsClipboardFilePathConverter.h"
#include "CUnicode.h"
#include "CLog.h"


//
// CMSWindowsClipboardTextConverter
//

CMSWindowsClipboardFilePathConverter::CMSWindowsClipboardFilePathConverter()
{
	// do nothing
}

CMSWindowsClipboardFilePathConverter::~CMSWindowsClipboardFilePathConverter()
{
	// do nothing
}

IClipboard::EFormat
CMSWindowsClipboardFilePathConverter::getFormat() const
{
	return IClipboard::kFilePath;
}

UINT
CMSWindowsClipboardFilePathConverter::getWin32Format() const
{
	return CF_HDROP;
}

HANDLE
CMSWindowsClipboardFilePathConverter::fromIClipboard(const CString& data) const
{
	HGLOBAL    hgDrop;
	DROPFILES* pDrop;
	UINT uBuffSize = uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (data.size() + 1);
	hgDrop = GlobalAlloc ( GHND | GMEM_DDESHARE, uBuffSize );
	UINT pos= data.find(":");
    if ( NULL == hgDrop )
		return NULL;
	// convert and add nul terminator
	LOG((CLOG_INFO "recieved "" %s ""as a file path\n", data.c_str()));
	pDrop = (DROPFILES*) GlobalLock ( hgDrop );
	if ( NULL == pDrop )
    {
		GlobalFree ( hgDrop );
		return NULL;
    }
	pDrop->pFiles = sizeof(DROPFILES);
	TCHAR* pszBuff = (TCHAR*)(LPBYTE(pDrop)+sizeof(DROPFILES));
	//FIXME:Dirty hack of only appending C to it
	lstrcpy(pszBuff, CString("C:").append(data.substr(pos+1,data.size()-pos-1)).c_str());
	GlobalUnlock(hgDrop);
	LOG((CLOG_INFO "converted it to "" %s ""\n",CString("C:").append(data.substr(pos+1,data.size()-pos-1)).c_str()));
	return (HANDLE) hgDrop;
}

CString
CMSWindowsClipboardFilePathConverter::toIClipboard( HANDLE data) const
{
	// convert and truncate at first nul terminator
	/*
	CString dst          = CUnicode::textToUTF8(data);
	CString::size_type n = dst.find('\0');
	if (n != CString::npos) {
		dst.erase(n);
	}
	*/
	CString buff;
	buff.append(ARCH->getName());
	buff.append(":");

	LOG ((CLOG_INFO "Have entered in the function"));
	DROPFILES *stgm = (DROPFILES*)GlobalLock(data);
	UInt32 srcSize = (UInt32)GlobalSize(data);
	LOG ((CLOG_INFO "Data size is %d\n", srcSize));
	TCHAR szPath[MAX_PATH];
	
	UINT nFiles = DragQueryFile((HDROP)stgm, -1, NULL, 0);
	LOG ((CLOG_INFO "%d files found\n", (int)nFiles));
	for(int i = 0; i < nFiles; i++) {
		  DragQueryFile((HDROP)stgm, i, szPath, MAX_PATH);
		  LOG ((CLOG_INFO "File: %s\n", szPath));
		  if (i > 0)
			  buff.append(";");
		  buff.append(szPath);
	}
	//LOG ((CLOG_INFO "First file: %s", (char *)(a+a->pFiles) ));
	GlobalUnlock(data);
	return buff;
}
