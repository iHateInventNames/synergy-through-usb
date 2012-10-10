/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2012 Bolton Software Ltd.
 * Copyright (C) 2002 Chris Schoeneman
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

#ifndef CUSBDATALINK_H
#define CUSBDATALINK_H

#include "IDataTransfer.h"
#include "CStreamBuffer.h"
#include "CCondVar.h"
#include "CMutex.h"
#include "IArchUsbDataLink.h"
#include <libusb.h>

class CUSBDataLink : public IDataTransfer {
public:
	CUSBDataLink();
	~CUSBDataLink();

	// IDataTransfer overrides
	virtual void		connect(const CBaseAddress&);

	// ISocket overrides
	virtual void		bind(const CBaseAddress&);
	virtual void		close();
	virtual void*		getEventTarget() const;

	// IStream overrides
	virtual UInt32		read(void* buffer, UInt32 n);
	virtual void		write(const void* buffer, UInt32 n);
	virtual void		flush();
	virtual void		shutdownInput();
	virtual void		shutdownOutput();
	virtual bool		isReady() const;
	virtual UInt32		getSize() const;

private:
	void				initConnection(const CBaseAddress&);

	static void LIBUSB_CALL	readCallback(libusb_transfer *transfer);
	static void	LIBUSB_CALL writeCallback(libusb_transfer *transfer);

	void				sendEvent(CEvent::Type);

	void				onConnected();
	void				onInputShutdown();
	void				onOutputShutdown();
	void				onDisconnected();

private:
	USBDeviceHandle		m_device;
	USBDataLinkConfig	m_config;

	libusb_transfer*	m_transferRead;
	libusb_transfer*	m_transferWrite;

	CMutex				m_mutex;
	char				m_readBuffer[512];
	CStreamBuffer		m_inputBuffer;
	CStreamBuffer		m_outputBuffer;
	CCondVar<bool>		m_flushed;
	bool				m_connected;
	bool				m_readable;
	bool				m_writable;

	CMutex				m_waitAcceptMutex;
	bool				m_waitAccept;
	CCondVar<bool>		m_accepted;

};

#endif