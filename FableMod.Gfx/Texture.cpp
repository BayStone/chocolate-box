#include "stdafx.h"
#include "Texture.h"
#include "Manager.h"
#include <d3dx9.h>
#include <cassert>
#include <tchar.h>
#include "Exception.h"
#include "Helpers.h"
#include "LogFile.h"

namespace FableMod
{
namespace Gfx
{

Texture::Texture() :
m_pTexture(0),
m_ucAlphaRequired(0)
{
}

Texture::~Texture()
{
	FM_DBG_LOG1(_T("Texture(%s)::Destructor\n"), GetName());

	Dispose();
}

void Texture::Dispose()
{
	GFX_SAFE_RELEASE(m_pTexture);

	//m_ucAlphaRequired = 0;

	RootObject::Dispose();
}

void Texture::Create(
	unsigned int uiWidth, 
	unsigned int uiHeight,
	unsigned int uiMipLevels,
	bool bRenderTarget,
	bool bAlpha)
{
	Dispose();

	if (FAILED(D3DXCreateTexture(
		FableMod::Gfx::Manager::GetDevice()->GetD3DDevice(), 
		uiWidth,
		uiHeight,
		uiMipLevels,
		(bRenderTarget ? D3DUSAGE_RENDERTARGET : 0),
		(bAlpha ? D3DFMT_A8R8G8B8 : D3DFMT_X8R8G8B8),
		(bRenderTarget ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED),
		&m_pTexture)))
	{
		throw Exception(_T("FABLEMOD_GFX_EXCEPTION_TEXTURE_CREATE"));
	}
}

void Texture::Load(LPCTSTR pFilename)
{
	Dispose();

	if (FAILED(D3DXCreateTextureFromFile(
		FableMod::Gfx::Manager::GetDevice()->GetD3DDevice(), 
		pFilename, 
		&m_pTexture)))
	{
		throw Exception(_T("FABLEMOD_GFX_EXCEPTION_TEXTURE_LOAD"));
	}

	SetName(pFilename);
}

void Texture::SetD3DTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
	m_pTexture->AddRef();
}

void Texture::SetAlphaRequired(bool bRequired)
{
	m_ucAlphaRequired = bRequired ? 2 : 1;
}

bool Texture::AlphaRequired()
{
	if (m_ucAlphaRequired == 0)
	{
		D3DFORMAT fmt = GetFormat();

		if (fmt == D3DFMT_A8R8G8B8 ||
			fmt == D3DFMT_A4R4G4B4 ||
			fmt == D3DFMT_A1R5G5B5)
			m_ucAlphaRequired = 2;
		else
			m_ucAlphaRequired = 1;
	}
	
	return m_ucAlphaRequired == 2;
}

unsigned int Texture::GetWidth(unsigned int uiLevel) const
{
	D3DSURFACE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	GFX_TRYD3D(m_pTexture->GetLevelDesc(uiLevel, &desc));
	return desc.Width;
}

unsigned int Texture::GetHeight(unsigned int uiLevel) const
{
	D3DSURFACE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	GFX_TRYD3D(m_pTexture->GetLevelDesc(uiLevel, &desc));
	return desc.Height;
}

D3DFORMAT Texture::GetFormat(unsigned int uiLevel) const
{
	D3DSURFACE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	GFX_TRYD3D(m_pTexture->GetLevelDesc(uiLevel, &desc));
	return desc.Format;
}

}
}