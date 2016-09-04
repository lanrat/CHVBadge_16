/* String functions
 *
 * (c) Copyright 2005-2011, ITB CompuPhase
 * This file is provided as is (no warranties).
 */
#pragma library String

native strlen(const string[]) = -41;
native strpack(dest[], const source[], maxlength=sizeof dest) = -43;
native strunpack(dest[], const source[], maxlength=sizeof dest) = -44;
native strcopy(dest[], const source[], maxlength=sizeof dest) = -36;
native strcat(dest[], const source[], maxlength=sizeof dest) = -34;

native strmid(dest[], const source[], start=0, end=cellmax, maxlength=sizeof dest) = -42;
native bool: strins(string[], const substr[], index, maxlength=sizeof string) = -40;
native bool: strdel(string[], start, end) = -37;

native strcmp(const string1[], const string2[], bool:ignorecase=false, length=cellmax) = -35;
native strfind(const string[], const sub[], bool:ignorecase=false, index=0) = -38;

native strval(const string[], index=0) = -45;
native valstr(dest[], value, bool:pack=true) = -46;
native bool: ispacked(const string[]) = -32;

native strformat(dest[], size=sizeof dest, bool:pack=true, const format[], {Fixed,Float,_}:...) = -39;

native memcpy(dest[], const source[], index=0, numbytes, maxlength=sizeof dest) = -33;

stock bool: strequal(const string1[], const string2[], bool:ignorecase=false, length=cellmax)
    return strcmp(string1, string2, ignorecase, length) == 0;

