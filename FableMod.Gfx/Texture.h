#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "GfxConf.h"
#include "RootObject.h"

namespace FableMod
{
namespace Gfx
{

/// Texture control class. Used with 3D meshes.
class GFX_API Texture : public RootObject
{
public:

	Texture();

	virtual ~Texture();

	void Create(
		unsigned int uiWidth, 
		unsigned int uiHeight,
		unsigned int uiMipLevels = 1,
		bool bRenderTarget = false,
		bool bAlpha = false);

	void Load(LPCTSTR pFilename);

	virtual void Dispose();

	bool AlphaRequired();
	
	unsigned int GetWidth(unsigned int uiLevel = 0) const;

	unsigned int GetHeight(unsigned int uiLevel = 0) const;

	D3DFORMAT GetFormat(unsigned int uiLevel = 0) const;

	void SetD3DTexture(LPDIRECT3DTEXTURE9 pTexture);

	LPDIRECT3DTEXTURE9 GetD3DTexture() const { return m_pTexture; }

	void SetAlphaRequired(bool bRequired);

protected:

	LPDIRECT3DTEXTURE9 m_pTexture;
	unsigned char m_ucAlphaRequired;
};

GFX_DECLARE_SMARTPOINTER(Texture);

}
}