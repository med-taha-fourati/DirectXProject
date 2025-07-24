#include <Windows.h>
#include <d3d9.h>
#include "BarDefinitions.h"

CUSTOMVERTEX vertices[][4] =
{
    // x     y       z     rhw   color
    {{ 20.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 20.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 44.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 44.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }},

    {{ 64.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 64.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 88.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 88.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }},

    {{ 108.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 108.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 132.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 132.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }},

    {{ 152.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 152.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 176.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 176.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }},

    {{ 196.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 196.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 220.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 220.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }} ,

    {{ 240.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 240.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 264.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
    { 264.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }}
};