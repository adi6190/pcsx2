/* 
 *	Copyright (C) 2007-2009 Gabest
 *	http://www.gabest.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#pragma once

#include "GSVector.h"

class GSTexture
{
public:
	GSVector2 m_scale;

	struct GSMap {uint8* bits; int pitch;};

	enum {None, RenderTarget, DepthStencil, Texture, Offscreen};

public:
	GSTexture() : m_scale(1, 1) {}
	virtual ~GSTexture() {}

	virtual operator bool() {ASSERT(0); return false;}

	virtual int GetType() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual int GetFormat() const = 0;

	virtual bool Update(const GSVector4i& r, const void* data, int pitch) = 0;
	virtual bool Map(GSMap& m, const GSVector4i* r = NULL) = 0;
	virtual void Unmap() = 0;
	virtual bool Save(const string& fn, bool dds = false) = 0;

	GSVector2i GetSize() const {return GSVector2i(GetWidth(), GetHeight());}
};
