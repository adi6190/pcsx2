/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2009  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef newVif
#include "newVif_BlockBuffer.h"
#include "newVif_HashBucket.h"
#include "x86emitter/x86emitter.h"
using namespace x86Emitter;
extern void mVUmergeRegs(int dest, int src, int xyzw, bool modXYZW = 0);
extern void  nVifGen(int usn, int mask, int curCycle);
extern void _nVifUnpack (int idx, u8 *data, u32 size);
extern void  dVifUnpack (int idx, u8 *data, u32 size);
extern void  dVifInit   (int idx);

typedef u32  (__fastcall *nVifCall)(void*, void*);
typedef void (__fastcall *nVifrecCall)(uptr dest, uptr src);

static __pagealigned u8 nVifUpkExec[__pagesize*4];
static __aligned16 nVifCall nVifUpk[(2*2*16)  *4]; // ([USN][Masking][Unpack Type]) [curCycle]
static __aligned16 u32 nVifMask[3][4][4] = {0};	   // [MaskNumber][CycleNumber][Vector]

#define _1mb (0x100000)
#define	_v0 0
#define	_v1 0x55
#define	_v2 0xaa
#define	_v3 0xff
#define aMax(x, y) std::max(x,y)
#define aMin(x, y) std::min(x,y)
#define _f __forceinline
#define xmmCol0 xmm2
#define xmmCol1 xmm3
#define xmmCol2 xmm4
#define xmmCol3 xmm5
#define xmmRow  xmm6
#define xmmTemp xmm7

struct nVifBlock { // Ordered for Hashing
	u8   num;	   // Num  Field
	u8   upkType;  // Unpack Type [usn*1:mask*1:upk*4]
	u8   mode;	   // Mode Field
	u8	 scl;	   // Start Cycle
	u8   cl;	   // CL   Field
	u8   wl;	   // WL   Field
	u32  mask;	   // Mask Field
	u8*  startPtr; // Start Ptr of RecGen Code
};

#define _hSize 0x4000 // [usn*1:mask*1:upk*4:num*8] hash...
#define _cmpS  (sizeof(nVifBlock) - sizeof(uptr))
#define _tParams nVifBlock, _hSize, _cmpS
struct nVifStruct {
	u32						idx;		// VIF0 or VIF1
	vifStruct*				vif;		// Vif Struct ptr
	VIFregisters*			vifRegs;	// Vif Regs   ptr
	VURegs*					VU;			// VU  Regs   ptr
	u8*						vuMemEnd;   // End of VU Memory
	u32						vuMemLimit; // Use for fast AND
	u8*						recPtr;		// Cur Pos to recompile to
	u8*						recEnd;		// End of Rec Cache
	BlockBuffer*			vifCache;	// Block Buffer
	HashBucket<_tParams>*	vifBlocks;	// Vif Blocks
	nVifBlock*				vifBlock;	// Current Vif Block Ptr
};

// Contents of this table are doubled up for doMask(false) and doMask(true) lookups.
// (note: currently unused, I'm using gsize in the interp tables instead since it
//  seems to be faster for now, which may change when nVif isn't reliant on interpreted
//  unpackers anymore --air)
static const u32 nVifT[32] = { 
	4, // S-32
	2, // S-16
	1, // S-8
	0, // ----
	8, // V2-32
	4, // V2-16
	2, // V2-8
	0, // ----
	12,// V3-32
	6, // V3-16
	3, // V3-8
	0, // ----
	16,// V4-32
	8, // V4-16
	4, // V4-8
	2, // V4-5

	// Second verse, same as the first!

	4, // S-32
	2, // S-16
	1, // S-8
	0, // ----
	8, // V2-32
	4, // V2-16
	2, // V2-8
	0, // ----
	12,// V3-32
	6, // V3-16
	3, // V3-8
	0, // ----
	16,// V4-32
	8, // V4-16
	4, // V4-8
	2, // V4-5
};

#define  useOldUnpack  0 // Use code in newVif_OldUnpack.inl
#define  newVifDynaRec 1 // Use code in newVif_Dynarec.inl
#include "newVif_OldUnpack.inl"
#include "newVif_Unpack.inl"
#include "newVif_UnpackGen.inl"

#include "newVif_Tables.inl"
#include "newVif_Dynarec.inl"

#endif