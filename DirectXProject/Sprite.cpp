#include "SpriteLoader.h"
SpriteLoader::SpriteLoader() {
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	color = D3DCOLOR_ARGB(255, 255, 255, 255);
	initted = false;

 }

SpriteLoader::SpriteLoader(float x, float y) {
	pos.x = x;
	pos.y = y;
	pos.z = 0;
	color = D3DCOLOR_ARGB(255, 255, 255, 255);
	initted = false;
}

bool SpriteLoader::InitSprite(LPDIRECT3DDEVICE9 dev, std::string file, int width, int height) {
	if (!SUCCEEDED(D3DXCreateTextureFromFileExA(dev, file.c_str(), width, height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tex))) {
		std::string s = "Error while loading image 0x01, Rquasted image : " + file;
		MessageBoxA(NULL, s.c_str(), NULL, NULL);
		return false;
	}

	if (!SUCCEEDED(D3DXCreateSprite(dev, &sprite))) {
		MessageBoxA(NULL, "Error while loading image 0x02", NULL, NULL);
		initted = false;
		return true;
	}
}

bool SpriteLoader::Initted() {
	return initted;
}

void SpriteLoader::Update(float gameTime) {
	// Update logic here
}

void SpriteLoader::Draw(float gameTime) {
	if (sprite && tex) {
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		sprite->Draw(tex, NULL, NULL, &pos, color);
		sprite->End();
	}


	SpriteLoader::~SpriteLoader(); {
		if (sprite) {
			sprite->Release();
			sprite = 0;
		}

		if (tex) {
			tex->Release();
			tex = 0;
		}

	}
}
	
