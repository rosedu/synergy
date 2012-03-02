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

#include "CPrimaryClient.h"
#include "CScreen.h"
#include "CClipboard.h"
#include "CLog.h"
#include "CServerApp.h"
#include <cstring>
//
// CPrimaryClient
//

CPrimaryClient::CPrimaryClient(const CString& name, CScreen* screen) :
	CBaseClientProxy(name),
	m_screen(screen),
	m_fakeInputCount(0)
{
	// all clipboards are clean
	for (UInt32 i = 0; i < kClipboardEnd; ++i) {
		m_clipboardDirty[i] = false;
	}
}

CPrimaryClient::~CPrimaryClient()
{
	// do nothing
}

void
CPrimaryClient::reconfigure(UInt32 activeSides)
{
	m_screen->reconfigure(activeSides);
}

UInt32
CPrimaryClient::registerHotKey(KeyID key, KeyModifierMask mask)
{
	return m_screen->registerHotKey(key, mask);
}

void
CPrimaryClient::unregisterHotKey(UInt32 id)
{
	m_screen->unregisterHotKey(id);
}

void
CPrimaryClient::fakeInputBegin()
{
	if (++m_fakeInputCount == 1) {
		m_screen->fakeInputBegin();
	}
}

void
CPrimaryClient::fakeInputEnd()
{
	if (--m_fakeInputCount == 0) {
		m_screen->fakeInputEnd();
	}
}

SInt32
CPrimaryClient::getJumpZoneSize() const
{
	return m_screen->getJumpZoneSize();
}

void
CPrimaryClient::getCursorCenter(SInt32& x, SInt32& y) const
{
	m_screen->getCursorCenter(x, y);
}

KeyModifierMask
CPrimaryClient::getToggleMask() const
{
	return m_screen->pollActiveModifiers();
}

bool
CPrimaryClient::isLockedToScreen() const
{
	return m_screen->isLockedToScreen();
}

void*
CPrimaryClient::getEventTarget() const
{
	return m_screen->getEventTarget();
}

bool
CPrimaryClient::getClipboard(ClipboardID id, IClipboard* clipboard) const
{
	return m_screen->getClipboard(id, clipboard);
}

void
CPrimaryClient::getShape(SInt32& x, SInt32& y,
				SInt32& width, SInt32& height) const
{
	m_screen->getShape(x, y, width, height);
}

void
CPrimaryClient::getCursorPos(SInt32& x, SInt32& y) const
{
	m_screen->getCursorPos(x, y);
}

void
CPrimaryClient::enable()
{
	m_screen->enable();
}

void
CPrimaryClient::disable()
{
	m_screen->disable();
}

void
CPrimaryClient::enter(SInt32 xAbs, SInt32 yAbs,
				UInt32 seqNum, KeyModifierMask mask, bool screensaver)
{
	m_screen->setSequenceNumber(seqNum);
	if (!screensaver) {
		m_screen->warpCursor(xAbs, yAbs);
	}
	m_screen->enter(mask);
}

bool
CPrimaryClient::leave()
{
	return m_screen->leave();
}
#include <iostream>
typedef std::map<CString,  CString> CScreenMounts;
void
CPrimaryClient::setClipboard(ClipboardID id, const IClipboard* clipboard)
{
	LOG((CLOG_INFO "xCprimaryClient::setClipboard call. Name: %s", getName().c_str()));
	// ignore if this clipboard is already clean
	if (m_clipboardDirty[id]) {
		// this clipboard is now clean
		m_clipboardDirty[id] = false;
	CClipboard Clipboard;
	Clipboard.copy(&Clipboard,clipboard); 
	Clipboard.open(0);
			CString content, new_content;
			if(Clipboard.has(IClipboard::kFilePath)) 
			{
				CString prefix, source;
				content = Clipboard.get(IClipboard::kFilePath);
				size_t pos = content.find("\n");
				source = content.substr(0,pos);
				//content = content.substr(pos+1, content.size());
				CScreenMounts *map = ((CServerApp*) &ARCH->app())->args().m_config->getMounts(source, getName());
				LOG((CLOG_INFO "X_PAS1 setClipboard: %s %s",source.c_str(), content.c_str()));
				
				if (map!=NULL && !map->empty())
				{
					while (pos < content.size())
					{
						++pos;
						CString line = content.substr(pos, content.find("\n", pos)-pos+1);
						pos = content.find("\n", pos);
						LOG ((CLOG_INFO "X_PAS2 The line is: %s\n", line.c_str()));
						for( CScreenMounts::iterator it = map->begin(); it != map->end(); it++)
						{
							int p = line.find(it->first);
							
							if( p != std::string::npos)
							{
								line = it->second + line.substr(p + it->first.size() );
								
								break;
							}
						}
						LOG ((CLOG_INFO "it changed to: %s\n", line.c_str()));
						new_content.append(line);
					}
					Clipboard.add(IClipboard::kFilePath, new_content);
					LOG((CLOG_INFO "X_PAS3 setClipboard: %s %s",source.c_str(), Clipboard.get(IClipboard::kFilePath).c_str()));
				}
				LOG((CLOG_INFO "X_PAS4 Changed2 clipboard: %s", new_content.c_str()));
			}		
			Clipboard.close();
		// set clipboard
		m_screen->setClipboard(id, &Clipboard);
	}
}

void
CPrimaryClient::grabClipboard(ClipboardID id)
{
	// grab clipboard
	m_screen->grabClipboard(id);

	// clipboard is dirty (because someone else owns it now)
	m_clipboardDirty[id] = true;
}

void
CPrimaryClient::setClipboardDirty(ClipboardID id, bool dirty)
{
	m_clipboardDirty[id] = dirty;
}

void
CPrimaryClient::keyDown(KeyID key, KeyModifierMask mask, KeyButton button)
{
	if (m_fakeInputCount > 0) {
// XXX -- don't forward keystrokes to primary screen for now
		(void)key;
		(void)mask;
		(void)button;
//		m_screen->keyDown(key, mask, button);
	}
}

void
CPrimaryClient::keyRepeat(KeyID, KeyModifierMask, SInt32, KeyButton)
{
	// ignore
}

void
CPrimaryClient::keyUp(KeyID key, KeyModifierMask mask, KeyButton button)
{
	if (m_fakeInputCount > 0) {
// XXX -- don't forward keystrokes to primary screen for now
		(void)key;
		(void)mask;
		(void)button;
//		m_screen->keyUp(key, mask, button);
	}
}

void
CPrimaryClient::mouseDown(ButtonID)
{
	// ignore
}

void
CPrimaryClient::mouseUp(ButtonID)
{
	// ignore
}

void
CPrimaryClient::mouseMove(SInt32 x, SInt32 y)
{
	m_screen->warpCursor(x, y);
}

void
CPrimaryClient::mouseRelativeMove(SInt32, SInt32)
{
	// ignore
}

void
CPrimaryClient::mouseWheel(SInt32, SInt32)
{
	// ignore
}

void
CPrimaryClient::screensaver(bool)
{
	// ignore
}

void
CPrimaryClient::resetOptions()
{
	m_screen->resetOptions();
}

void
CPrimaryClient::setOptions(const COptionsList& options)
{
	m_screen->setOptions(options);
}
