#pragma once
#ifndef BARDEFINITIONS_H
#include <Windows.h>

struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
    DWORD color;    // from the D3DFVF_DIFFUSE flag
};

CUSTOMVERTEX vertices[][4];

#endif