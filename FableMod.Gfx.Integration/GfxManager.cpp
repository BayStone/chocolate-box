#include "Stdafx.h"
#include "GfxManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ThemeManager.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

GfxManager::GfxManager()
{
}

void GfxManager::Initialize(
	System::Windows::Forms::Form^ form, int width, int height)
{
	try
	{
		FableMod::Gfx::Manager::Initialize(
			(HWND)form->Handle.ToPointer(), width, height);
	}
	catch (FableMod::Gfx::Exception& ex)
	{
		throw gcnew System::Exception(gcnew String(ex.GetMsg()));
	}

	m_ModelManager = gcnew ModelManager();
	m_TextureManager = gcnew TextureManager();
	m_ThemeManager = gcnew ThemeManager(m_TextureManager, m_ModelManager);
}

void GfxManager::SaveScreenToFile(System::String^ fileName)
{
	IDirect3DSurface9* pSurface;

	Gfx::Device* pDevice = Gfx::Manager::GetDevice();

	pDevice->Lock();

	if (FAILED(pDevice->GetD3DDevice()->GetBackBuffer(
		0,
		0,
		D3DBACKBUFFER_TYPE_MONO,
		&pSurface)))
	{
		pDevice->Unlock();
		throw gcnew System::Exception("Failed to create surface.");
	}

	IntPtr p = Marshal::StringToHGlobalUni(fileName);

	D3DXIMAGE_FILEFORMAT format = D3DXIFF_JPG;

	String^ ext = Path::GetExtension(fileName)->ToLower(); 
	
	if (ext == "png")
		format = D3DXIFF_PNG;
	else if (ext == "DDS")
		format = D3DXIFF_DDS;
	else if (ext == "BMP")
		format = D3DXIFF_BMP;

	if (FAILED(D3DXSaveSurfaceToFile(
		(TCHAR*)p.ToPointer(), 
		D3DXIFF_JPG,
		pSurface, 
		0,
		0)))
	{
		pDevice->Unlock();
		pSurface->Release();
		Marshal::FreeHGlobal(p);
		throw gcnew System::Exception("Failed to save surface.");
	}
	
	Marshal::FreeHGlobal(p);
	pSurface->Release();

	pDevice->Unlock();
}

void GfxManager::Destroy()
{
	if (m_TextureManager != nullptr)
	{
		delete m_TextureManager;
		m_TextureManager = nullptr;
	}

	if (m_ModelManager != nullptr)
	{
		delete m_ModelManager;
		m_ModelManager = nullptr;
	}

	if (m_ThemeManager != nullptr)
	{
		delete m_ThemeManager;
		m_ThemeManager = nullptr;
	}

	::FableMod::Gfx::Manager::Dispose();
}

void GfxManager::SetDirectory(System::String^ directory)
{
	if (!directory->EndsWith("\\"))
		directory += "\\";

	System::IntPtr ptr = Marshal::StringToHGlobalUni(directory);

	::FableMod::Gfx::Manager::SetDirectory((LPCTSTR)ptr.ToPointer());
	
	Marshal::FreeHGlobal(ptr);

	directory += "FableMod.Gfx.Log";

	ptr = Marshal::StringToHGlobalUni(directory);

	FM_LOG_OPEN((LPCTSTR)ptr.ToPointer());
		
	Marshal::FreeHGlobal(ptr);
}

TextureManager^ GfxManager::GetTextureManager()
{
	return m_TextureManager;
}

ModelManager^ GfxManager::GetModelManager()
{
	return m_ModelManager;
}

ThemeManager^ GfxManager::GetThemeManager()
{
	return m_ThemeManager;
}

}
}
}