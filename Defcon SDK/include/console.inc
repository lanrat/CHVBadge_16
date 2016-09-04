/* Console input/output functions
 *
 * (c) Copyright 1998-2011, ITB CompuPhase
 * This file is provided as is (no warranties).
 */
#pragma library Console

native print(const string[], foreground=-1, background=-1, highlight=-1) = -13;
native printf(const format[], {Float,Fixed,_}:...) = -14;
