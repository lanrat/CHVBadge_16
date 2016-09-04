/* Core functions
 *
 * (c) Copyright 1998-2011, ITB CompuPhase
 * This file is provided as is (no warranties).
 */
#pragma library Core

native heapspace() = -4;

native funcidx(const name[]) = -5;

native numargs() = -1;
native getarg(arg, index=0) = -2;
native setarg(arg, index=0, value) = -3;

native tolower(c) = -7;
native toupper(c) = -8;
native swapchars(c) = -6;

native random(max) = -12;

native min(value1, value2) = -9;
native max(value1, value2) = -10;
native clamp(value, min=cellmin, max=cellmax) = -11;

