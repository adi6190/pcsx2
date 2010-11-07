/*  ZZ Open GL graphics plugin
 *  Copyright (c)2009-2010 zeydlitz@gmail.com, arcum42@gmail.com
 *  Based on Zerofrog's ZeroGS KOSMOS (c)2005-2008
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
 
// keyboard functions

#include "Util.h"
#include "GS.h"
#include "ZZoglShaders.h"
#include "Profile.h"
#include "GLWin.h"

extern int CurrentSavestate, g_GSMultiThreaded, g_nPixelShaderVer;
extern char *libraryName;
extern const unsigned char zgsversion;
extern unsigned char zgsrevision, zgsbuild, zgsminor;

extern u32 THR_KeyEvent; // value for passing out key events between threads
extern bool THR_bShift, SaveStateExists;

const char* s_aa[5] = { "AA none |", "AA 2x |", "AA 4x |", "AA 8x |", "AA 16x |" };
const char* pbilinear[] = { "off", "normal", "forced" };

extern void SetAA(int mode);

void ProcessBilinear()
{
	FUNCLOG
	char strtitle[256];
	
	if (g_nPixelShaderVer == SHADER_REDUCED)
	{
		conf.bilinear = 0;
		sprintf(strtitle, "reduced shaders don't support bilinear filtering");
	}
	else
	{
		conf.bilinear = (conf.bilinear + 1) % 3;
		sprintf(strtitle, "bilinear filtering - %s", pbilinear[conf.bilinear]);
	}
	ZZLog::WriteToScreen(strtitle);
	SaveConfig();
}

void ProcessInterlace()
{
	FUNCLOG

	char strtitle[256];
	conf.interlace++;

	if (conf.interlace > 2) conf.interlace = 0;

	if (conf.interlace < 2) 
		sprintf(strtitle, "interlace on - mode %d", conf.interlace);
	else 
		sprintf(strtitle, "interlace off");
		
	ZZLog::WriteToScreen(strtitle);
	SaveConfig();
}

void ProcessAASetting(bool reverse)
{
	FUNCLOG

	char strtitle[256];

	if (reverse)
		conf.decAA();
	else
		conf.incAA();
	
	sprintf(strtitle, "anti-aliasing - %s", s_aa[conf.aa]);
	SetAA(conf.aa);
	ZZLog::WriteToScreen(strtitle);

	SaveConfig();
}

void ProcessFPS()
{
	FUNCLOG
	g_bDisplayFPS ^= 1;
	ZZLog::Debug_Log("Toggled FPS.");
}

void ProcessWireFrame()
{
	FUNCLOG
	char strtitle[256];
	
	conf.zz_options.wireframe = !conf.zz_options.wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, (conf.wireframe()) ? GL_LINE : GL_FILL);
	sprintf(strtitle, "wireframe rendering - %s", (conf.wireframe()) ? "on" : "off");
	ZZLog::WriteToScreen(strtitle);
}

void ProcessHackSetting(bool reverse)
{
	FUNCLOG
	
	int hack = CurrentHack;

	if (reverse)
	{
		hack--;

		if (hack < 0) hack = HACK_NUMBER - 1;
	}
	else
	{
		hack++;

		if (hack >= HACK_NUMBER) hack = 0;
	}
	ChangeCurrentHack(hack);
		
	SaveConfig();
}

void ProcessSaveState()
{
	FUNCLOG
	char strtitle[256];
	sprintf(strtitle, "Saving in savestate %d", CurrentSavestate);
	SaveStateExists = true;
	if (CurrentHack != 0) DisplayHack(CurrentHack);
}

void OnFKey(int key, int shift)
{
	switch(key)
	{
		//case 1: 
		//	ProcessSaveState(); 
		//	break;
		case 5:
			if (shift)
				ProcessBilinear();
			else
				ProcessInterlace();
			break;
		case 6:
			if (shift)
				ProcessAASetting(true);
			else
				ProcessAASetting(false);
			break;
		case 7:
			if (!shift)
				ProcessFPS();
			else
				ProcessWireFrame();
			break;
		case 9:
			if (shift) 
				ProcessHackSetting(true);
			else
				ProcessHackSetting(false);
			break;
		default:
			break;
	}
}

void WriteAA()
{
	if (conf.aa != 0)
	{
		char strtitle[64];
		sprintf(strtitle, "anti-aliasing - %s", s_aa[conf.aa]);
		ZZLog::WriteToScreen(strtitle, 1000);
	}
}

void WriteBilinear()
{
	switch (conf.bilinear)
	{
		case 2:
			ZZLog::WriteToScreen("bilinear filtering - forced", 1000);
			break;

		case 1:
			ZZLog::WriteToScreen("bilinear filtering - normal", 1000);
			break;

		default:
			break;
	}
}

#ifdef _WIN32

extern void ChangeDeviceSize(int nNewWidth, int nNewHeight);

void ProcessEvents()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(msg));

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			switch (msg.message)
			{
				case WM_KEYDOWN :
					int my_KeyEvent = msg.wParam;
					bool my_bShift = !!(GetKeyState(VK_SHIFT) & 0x8000);

					switch (msg.wParam)
					{
						case VK_F5:
						case VK_F6:
						case VK_F7:
						case VK_F9:
							OnFKey(msg.wParam - VK_F1 + 1, my_bShift);
							break;

						case VK_ESCAPE:

							if (conf.fullscreen())
							{
								// destroy that msg
								conf.setFullscreen(false);
								ChangeDeviceSize(conf.width, conf.height);
								UpdateWindow(GShwnd);
								continue; // so that msg doesn't get sent
							}
							else
							{
								SendMessage(GShwnd, WM_DESTROY, 0, 0);
								return;
							}

							break;
					}

					break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			break;
		}
	}

	if ((GetKeyState(VK_MENU) & 0x8000) && (GetKeyState(VK_RETURN) & 0x8000))
	{
		conf.zz_options.fullscreen = !conf.zz_options.fullscreen;

		SetChangeDeviceSize(
			(conf.fullscreen()) ? 1280 : conf.width,
			(conf.fullscreen()) ? 960 : conf.height);
	}
}

#else // linux

void ProcessEvents()
{
	FUNCLOG

	// check resizing
	GLWin.ResizeCheck();

	if (THR_KeyEvent)     // This value was passed from GSKeyEvents which could be in another thread
	{
		int my_KeyEvent = THR_KeyEvent;
		bool my_bShift = THR_bShift;
		THR_KeyEvent = 0;

		switch (my_KeyEvent)
		{
			case XK_F5:
			case XK_F6:
			case XK_F7:
			case XK_F9:
				OnFKey(my_KeyEvent - XK_F1 + 1, my_bShift);
				break;
		}
	}
}

#endif // linux
