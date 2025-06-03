#pragma once
#ifndef SPRITELOAD_H
#define SPRITELOAD_H
#include <d3d11.h>
#include <string>

class SpriteLoader {
public:
	SpriteLoader();
	SpriteLoader(float x, float y);
	~SpriteLoader();

	bool InitSprite(LPDIRECT3DDEVICE9 device, std::string file, int width, int height);
	bool Initted();
	virtual void Update(float GameTime);
	virtual void Draw(float GameTime);
	
private:
	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;

	D3DXVECTOR3 pos;
	D3DXCOLOR color;
	bool initted;
};
#endif // !SPRITELOAD_H

