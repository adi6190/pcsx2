/*  Pcsx2 - Pc Ps2 Emulator
 *  Copyright (C) 2002-2009  Pcsx2 Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#pragma once

#include <wx/wx.h>
#include <wx/image.h>

#include "wxHelpers.h"

#define FLAG_VU_ADD_SUB 0x1 //Tri-Ace - IDC_GAMEFIX2
#define FLAG_VU_CLIP 0x2 // Persona3/4  - IDC_GAMEFIX4
#define FLAG_FPU_Compare 0x4 // Digimon Rumble Arena - IDC_GAMEFIX3
#define FLAG_FPU_MUL 0x8 //Tales of Destiny - IDC_GAMEFIX5

namespace Dialogs
{
	class GameFixesDialog: public wxDialogWithHelpers
	{
	public:
		GameFixesDialog(wxWindow* parent, int id);

	protected:

	public:
		void FPUCompareHack_Click(wxCommandEvent &event);
		void FPUMultHack_Click(wxCommandEvent &event);
		void TriAce_Click(wxCommandEvent &event);
		void GodWar_Click(wxCommandEvent &event);
		void Ok_Click(wxCommandEvent &event);
		void Cancel_Click(wxCommandEvent &event);
	};
}

