#pragma once
#ifndef GRAPHICSDEVICE_H
#define GRAPICSDEVICE_H

#include <d3d9.h>

class GameSprite {
public:
	GameSprite();
	~GameSprite();

	void Init(HWND hwnd, bool windowed);
	void Clear(D3DCOLOR color);
	void End();
	void Present();

	LPDIRECT3DDEVICE9 device;
private:
	LPDIRECT3D9 d3d;
};
#endif // !GRAPHICSDEVICE_H

