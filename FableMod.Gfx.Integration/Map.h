#pragma once

using namespace System;
using namespace FableMod;

#include "GfxController.h"
#include "ThemeManager.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class Map 
{
public:

	Map();

	virtual ~Map();

	void Create(
		GfxController^ controller,
		String^ name,
		int mapX, int mapY, 
		FableMod::LEV::LEVFile^ lev,
		FableMod::STB::Level^ stbLevel);

	bool ImportHeightfield(
		array<float>^ heights,
		int width,
		int height);		

	virtual void Destroy();

	void Highlight(bool bHighlight);

	bool IsOver(float x, float y, float z);

	property FableMod::LEV::LEVFile^ LEV 
	{  
		FableMod::LEV::LEVFile^ get() { return m_Lev; } 
	}
	
	property int MapX 
	{  
		int get() { return m_MapX; } 
	}

	property int MapY 
	{  
		int get() { return m_MapY; } 
	}

	property int MapWidth 
	{  
		int get() { return m_Lev->Width; } 
	}

	property int MapHeight
	{  
		int get() { return m_Lev->Height; } 
	}

	property float X
	{
		float get() 
		{ 
			if (!GetTerrain())
				return 0.0f;

			return GetTerrain()->GetWorldTranslation().x; 
		}
	}

	property float Y
	{
		float get() 
		{ 
			if (!GetTerrain())
				return 0.0f;

			return GetTerrain()->GetWorldTranslation().y; 
		}
	}

	property String^ Name
	{
		String^ get() { return m_Name; }
	}

	property bool Modified
	{
		virtual bool get();
	}

	FableMod::Gfx::Terrain* GetTerrain() 
	{ 
		if (m_pTerrain)
			return *m_pTerrain; 
		else
			return 0;
	}

	FableMod::Gfx::Node* GetSTBTerrain() 
	{ 
		if (m_pSTBTerrain)
			return *m_pSTBTerrain; 
		else
			return 0;
	}

	property FableMod::STB::Level^ STBLevel
	{  
		FableMod::STB::Level^ get() { return m_STBLevel; } 
	}
	
	void EnableFlora(bool enabled)
	{
		if (m_pSTBFlora)
		{
			if (!enabled)
			{
				(*m_pSTBFlora)->Flags() |= FLAGS_FORCE_CULLING;
			}
			else
			{
				(*m_pSTBFlora)->Flags() &= ~FLAGS_FORCE_CULLING;
			}
		}
	}

	void CreateWalkTexture();
	void FinishWalkTexture();

	void CreateNavTexture(String^ sectionName, int subset);
	void FinishNavTexture(String^ sectionName, int subset);

protected:

	void CreateTerrainTexture(GfxController^ controller);
	
	void DrawSprite(
		LPD3DXSPRITE pSprite, 
		LPDIRECT3DTEXTURE9 pTexture,
		unsigned char ucAlpha);

	void ApplyTheme(
		GfxController^ controller,
		LPD3DXSPRITE pSprite,
		Theme^ theme,
		D3DXMATRIX* pmatWorld,
		unsigned char ucStrength,
		float fWorldX,
		float fWorldY,
		bool bModel,
		int iCellX, 
		int iCellY);
	
	FableMod::LEV::LEVFile^ m_Lev;
	FableMod::STB::Level^ m_STBLevel;
	String^ m_Name;
	int m_MapX;
	int m_MapY;

private:

	void STBCreateFlora(GfxController^ controller);
	void STBAddFloraNode(
		GfxController^ controller,
		FableMod::STB::FloraNode^ node);
	void STBCreateTerrain(GfxController^ controller);

	Gfx::TerrainPtr* m_pTerrain;
	Gfx::NodePtr* m_pSTBTerrain;
	Gfx::NodePtr* m_pSTBFlora;
};

}
}
}