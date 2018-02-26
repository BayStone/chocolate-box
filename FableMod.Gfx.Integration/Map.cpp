#include "Stdafx.h"
#include "Map.h"
#include "GfxManager.h"
#include "GfxTexture.h"
#include "ThemeManager.h"
#include "Defs.h"

using namespace System::Runtime::InteropServices;
using namespace FableMod::LEV;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

Map::Map() :
m_pTerrain(0)
{
}

Map::~Map()
{
	Destroy();
}

void Map::Destroy()
{
	GFX_SAFE_DELETE(m_pTerrain);
	GFX_SAFE_DELETE(m_pSTBTerrain);
	
	m_Name = nullptr;
	
	if (m_Lev)
	{
		m_Lev->Destroy();
		m_Lev = nullptr;
	}
}

#define D3DCOLOR_EXTRACT(_c, _r, _g, _b) \
{ \
	_r = ((_c) >> 16) & 0xFF; \
	_g = ((_c) >> 8) & 0xFF; \
	_b = (_c) & 0xFF; \
}

static D3DCOLOR Combine(D3DCOLOR Color1, 
					D3DCOLOR Color2,
					D3DCOLOR Color3,
					int iStrength1,
					int iStrength2,
					int iStrength3)
{
	int iR1,iG1,iB1;
	int iR2,iG2,iB2;
	int iR3,iG3,iB3;

	D3DCOLOR_EXTRACT(Color1, iR1, iG1, iB1);
	D3DCOLOR_EXTRACT(Color2, iR2, iG2, iB2);
	D3DCOLOR_EXTRACT(Color3, iR3, iG3, iB3);

	const float fStrength1 = (float)iStrength1/255.0f;
	const float fStrength2 = (float)iStrength2/255.0f;
	const float fStrength3 = (float)iStrength3/255.0f;

	iR1 = (int)(iR1*fStrength1);
	iG1 = (int)(iG1*fStrength1);
	iB1 = (int)(iB1*fStrength1);

	iR2 = (int)(iR2*fStrength2);
	iG2 = (int)(iG2*fStrength2);
	iB2 = (int)(iB2*fStrength2);

	iR3 = (int)(iR3*fStrength3);
	iG3 = (int)(iG3*fStrength3);
	iB3 = (int)(iB3*fStrength3);

	return D3DCOLOR_ARGB(
		255, iR1+iR2+iR3, iG1+iG2+iG3, iB1+iB2+iB3);
}

void Map::Create(GfxController^ controller,
				 String^ name,
				 int mapX, int mapY, 
				 FableMod::LEV::LEVFile^ lev,
				 FableMod::STB::Level^ stbLevel)
{
	m_Name = name;
	m_Lev = lev;
	m_STBLevel = stbLevel;

	m_MapX = mapX;
	m_MapY = mapY;

	if (!m_Lev)
		return;

	m_pTerrain = new TerrainPtr(new Terrain());
	
	System::IntPtr ptr = Marshal::StringToHGlobalUni(name);
	GetTerrain()->SetName((LPCTSTR)ptr.ToPointer());
	Marshal::FreeHGlobal(ptr);

	FLOAT* pfHeightfield = new FLOAT[(lev->Width+1)*(lev->Height+1)];

	for (int i = 0; i <= lev->Width; ++i)
	{
		for (int j = 0; j <= lev->Height; ++j)
		{
			pfHeightfield[j*(lev->Width+1)+i] = lev->Heights[i,j];
		}
	}

	GetTerrain()->Create(pfHeightfield, lev->Width, lev->Height);

	delete[] pfHeightfield;

	controller->GetRoot()->AttachChild(GetTerrain());

	const float fSpacing = 
		FableMod::Gfx::Settings::GetInstance()->GetFloat(
			_T("Terrain"),_T("Spacing"),1.0f);

	const D3DXVECTOR3 vMapLocation(mapX*fSpacing, mapY*fSpacing, 0);

	GetTerrain()->SetTranslation(vMapLocation);

	GetTerrain()->Update();

	CreateTerrainTexture(controller);

	if (m_STBLevel != nullptr)
	{
		bool bSTBTerrain = 
			FableMod::Gfx::Settings::GetInstance()->GetInt(
			_T("STB"),_T("CreateTerrain"), 1) != 0;

		bool bSTBFlora = 
			FableMod::Gfx::Settings::GetInstance()->GetInt(
			_T("STB"),_T("CreateFlora"), 1) != 0;

		if (bSTBTerrain)
			STBCreateTerrain(controller);
		
		if (bSTBFlora)
			STBCreateFlora(controller);
	}
}

bool Map::ImportHeightfield(array<float>^ heights, int width, int height)
{
	FLOAT* heightfield = new FLOAT[(m_Lev->Width+1)*(m_Lev->Height+1)];

	for (int i = 0; i <= m_Lev->Width; ++i)
	{
		for (int j = 0; j <= m_Lev->Height; ++j)
		{
			heightfield[j*(m_Lev->Width+1)+i] = heights[j*(width+1)+i];
		}
	}

	GetTerrain()->Create(heightfield, m_Lev->Width, m_Lev->Height);

	delete[] heightfield;

	return true;
}

void Map::STBCreateTerrain(GfxController^ controller)
{
	EditableTexturePtr spEmptyTexture = new EditableTexture();
	
	spEmptyTexture->Create(
		16,
		16,
		1,
		false,
		false);

	spEmptyTexture->Lock();
			
	for (int i = 0; i < spEmptyTexture->GetWidth(); ++i)
	{
		for (int j = 0; j < spEmptyTexture->GetHeight(); ++j)
		{
			spEmptyTexture->DrawRawPixel(i,j, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	
	spEmptyTexture->Unlock();

	m_pSTBTerrain = new NodePtr(new Node());

	GetSTBTerrain()->SetName(GetTerrain()->GetName());
	
	int maxVertexCount = 512;

	D3DXVECTOR3* pVertices = new D3DXVECTOR3[maxVertexCount];
	D3DXVECTOR3* pNormals = new D3DXVECTOR3[maxVertexCount];
	D3DXVECTOR2* pUVs = new D3DXVECTOR2[maxVertexCount];
	
	UInt32* pTriangles = new UInt32[2048];
			
	for (int y = 0; y < m_STBLevel->BlockCount; ++y)
	{
		FableMod::STB::HeightBlock^ block = m_STBLevel->GetBlock(y);

		NodePtr spBlock = new ImpostorNode();
			
		float zOffset = 0.0f;
		float bias = 0.005f;
			
		D3DXVECTOR3 center(
			block->Center.x, block->Center.y, block->Center.z+zOffset);
			
		for (int m = 0; m < block->MeshCount; ++m)
		{
			FableMod::STB::HeightMesh^ mesh = block->GetMesh(m);
	
			if (mesh->VertexCount == 0 || mesh->FaceCount <= 2)
			{
				// Bullshit mesh.
				continue;
			}

			System::Diagnostics::Debug::Assert(mesh->VertexCount < maxVertexCount);

			int index = 0;
			int triangleCount = 0;

			for (int i = 0; i < mesh->FaceCount-2; ++i)
			{
				UInt32 a, b, c;

				if (i % 2 == 0)
				{
					a = mesh->GetFace(i+2);
					b = mesh->GetFace(i+1);
					c = mesh->GetFace(i+0);
				}
				else
				{
					a = mesh->GetFace(i+0);
					b = mesh->GetFace(i+1);
					c = mesh->GetFace(i+2);
				}

				if (a == b || a == c || b == c)
				{
					// Degenerate.
					continue;
				}

				++triangleCount;
					
				pTriangles[index++] = a;
				pTriangles[index++] = b;
				pTriangles[index++] = c;
			}
				
			if (triangleCount == 0)
			{
				// No triangles.
				continue;
			}
			
			for (int v = 0; v < mesh->VertexCount; ++v)
			{
				FableMod::STB::HeightVertex vert(mesh->GetVertex(v));

				D3DXVECTOR3 vtx(
					vert.Position.x, 
					vert.Position.y, 
					vert.Position.z+zOffset);
						
				pVertices[v] = vtx-center;
				
				pNormals[v] = 
					D3DXVECTOR3(vert.Normal.x, vert.Normal.y, vert.Normal.z);
					
				pUVs[v] = D3DXVECTOR2(vert.U, vert.V);
			}

			STBTerrainModelPtr spTerrainModel = new STBTerrainModel();
			
			spTerrainModel->Create(
				pVertices,
				pNormals,
				pUVs,
				mesh->VertexCount,
				pTriangles,
				triangleCount);
				
			MeshPtr spTerrainMesh = new Mesh();

			spTerrainMesh->SetModel(spTerrainModel);
				
			spTerrainMesh->RenderFlags() |= MRF_WIREFRAME_OVERLAY;
				
			GfxTexture^ texture = 
				GfxManager::GetTextureManager()->Get(mesh->Texture);

			if (texture != nullptr)
			{
				spTerrainModel->SetTexture(texture->GetBaseTexture());
			}
			else
			{
				spTerrainModel->SetTexture(spEmptyTexture);
			}

			spBlock->AttachChild(spTerrainMesh);

			zOffset += bias;
		}
			
		spBlock->SetTranslation(center);

		GetSTBTerrain()->AttachChild(spBlock);
	}

	controller->GetRoot()->AttachChild(GetSTBTerrain());
	
	delete[] pTriangles;
			
	delete[] pVertices;
	delete[] pNormals;
	delete[] pUVs;
}

void Map::FinishWalkTexture()
{
	EditableTexturePtr spTexture = GetTerrain()->GetTexture(1);

	if (spTexture)
	{
		spTexture->Lock();
			
		for (int i = 0; i < m_Lev->Width; ++i)
		{
			for (int j = 0; j < m_Lev->Height; ++j)
			{
				m_Lev->Walks[i, j] = 
					(spTexture->GetRawPixel(i, j) == GFXI_COLOR_WALK);
			}
		}
		
		spTexture->Unlock();

		GetTerrain()->SetTexture(1, 0);
	}
}

void Map::CreateWalkTexture()
{
	EditableTexturePtr spTexture = new EditableTexture();

	spTexture->Create(
		GetTerrain()->GetTextureSize(),
		GetTerrain()->GetTextureSize(),
		1,
		false,
		false);

	GetTerrain()->SetTexture(1, spTexture);
	
	GetTerrain()->SetTextureSpacing(1, D3DXVECTOR2(
		GetTerrain()->GetWidth(),
		GetTerrain()->GetHeight()),
		spTexture);
	
	spTexture->Lock();
			
	for (int i = 0; i < m_Lev->Width; ++i)
	{
		for (int j = 0; j < m_Lev->Height; ++j)
		{
			if (m_Lev->Walks[i,j])
				spTexture->DrawRawPixel(i,j, GFXI_COLOR_WALK);
			else
				spTexture->DrawRawPixel(i,j, GFXI_COLOR_EMPTY);
		}
	}
	
	spTexture->Unlock();
}

void Map::FinishNavTexture(String^ sectionName, int subset)
{
	NavSection^ section = LEV->Sections[sectionName];

	if (section)
	{
		Grid^ grid = section->GetGrid(subset);

		if (grid)
		{
			EditableTexturePtr spTexture = GetTerrain()->GetTexture(1);

			if (spTexture)
			{
				spTexture->Lock();
			
				for (int i = 0; i < grid->Width; ++i)
				{
					for (int j = 0; j < grid->Height; ++j)
					{
						GridCell^ cell = 
							grid->GetCellAt((grid->Width-1)-i, j);

						D3DCOLOR color = spTexture->GetRawPixel(i, j);

						unsigned char v = 0;

						if (color == GFXI_COLOR_NAVIGATION)
						{
							v = (unsigned char)CellMode::Navigation;
						}
						else if (color == GFXI_COLOR_DYNAMIC)
						{
							v = (unsigned char)CellMode::Dynamic;
						}
						
						cell->Value = v;
					}
				}

				spTexture->Unlock();

				GetTerrain()->SetTexture(1, 0);
			}
		}
	}
}

void Map::CreateNavTexture(String^ sectionName, int subset)
{
	Console::WriteLine("Map::CreateNavTexture({0}, {1})",
		sectionName, subset);

	NavSection^ section = m_Lev->Sections[sectionName];

	if (section)
	{
		Grid^ grid = section->GetGrid(subset);

		if (grid)
		{
			EditableTexturePtr spTexture = new EditableTexture();

			unsigned int uiSize = GetTerrain()->GetTextureSize(2);

			spTexture->Create(
				uiSize,
				uiSize,
				1,
				false,
				true);

			GetTerrain()->SetTexture(1, spTexture);
		
			GetTerrain()->SetTextureSpacing(1, D3DXVECTOR2(
				GetTerrain()->GetWidth()*2,
				GetTerrain()->GetHeight()*2),
				spTexture);
			
			spTexture->Lock();
		
			for (int i = 0; i < grid->Width; ++i)
			{
				for (int j = 0; j < grid->Height; ++j)
				{
					GridCell^ cell = grid->GetCellAt(
						(grid->Width-1)-i, j);

					D3DCOLOR color = GFXI_COLOR_EMPTY;

					unsigned char v = cell->Value;

					if (v & (unsigned char)CellMode::Magic)
					{
						color = GFXI_COLOR_MAGIC;
					}
					else if (v & (unsigned char)CellMode::Navigation)
					{
						if (v & (unsigned char)CellMode::Special1)
							color = GFXI_COLOR_NAVIGATION_S1;
						else if (v & (unsigned char)CellMode::Special2)
							color = GFXI_COLOR_NAVIGATION_S2;
						else
							color = GFXI_COLOR_NAVIGATION;
					}
					else if (v & (unsigned char)CellMode::Dynamic)
					{
						if (v & (unsigned char)CellMode::Special1)
							color = GFXI_COLOR_DYNAMIC_S1;
						else if (v & (unsigned char)CellMode::Special2)
							color = GFXI_COLOR_DYNAMIC_S2;
						else
							color = GFXI_COLOR_DYNAMIC;
					}
					else if (v > 0)
					{
						color = GFXI_COLOR_UNKNOWN;
					}

					spTexture->DrawRawPixel(i,j,color);
				}
			}

			spTexture->Unlock();
		}
	}
}

void Map::DrawSprite(
	LPD3DXSPRITE pSprite, 
	LPDIRECT3DTEXTURE9 pTexture,
	unsigned char ucAlpha)
{
	pSprite->Draw(
		pTexture,
		NULL,
		NULL,
		&D3DXVECTOR3(0,0,0),
		D3DCOLOR_ARGB(ucAlpha,255,255,255));
}

void Map::ApplyTheme(GfxController^ controller,
					 LPD3DXSPRITE pSprite,
					 Theme^ theme,
					 D3DXMATRIX* pmatWorld,
					 unsigned char ucStrength,
					 float fWorldX,
					 float fWorldY,
					 bool bModel,
					 int iCellX,
					 int iCellY)

{
	if (!theme)
		return;

	if (theme->Texture)
	{
		LPDIRECT3DTEXTURE9 pTexture = theme->Texture->GetBaseD3DTexture();

		if (pTexture)
		{
			D3DSURFACE_DESC Desc;
		
			pTexture->GetLevelDesc(0, &Desc);

			const float fMultiplier = Terrain::GetTextureMultiplier();

			pmatWorld->_11 = fMultiplier/(FLOAT)Desc.Width;
			pmatWorld->_22 = fMultiplier/(FLOAT)Desc.Height;
			
			pSprite->SetTransform(pmatWorld);

			DrawSprite(pSprite, pTexture, ucStrength);
		}
	}

	/*
	if (theme->Model && 
		bModel && 
		(iCellX % 8) == 0 &&
		(iCellY % 8) == 0)
	{
		FLOAT fHeight;

		if (TerrainManager::GetInstance()->GetHeight(
			D3DXVECTOR2(fWorldX, fWorldY), &fHeight))
		{
			NodePtr spNode = new Node();

			spNode->SetScale(0.01f);

			spNode->AttachChild(theme->Model->GetSpatial());

			controller->GetRoot()->AttachChild(spNode);

			spNode->SetWorldTranslation(
				D3DXVECTOR3(fWorldX, fWorldY, fHeight));
		}
	}
	*/
}

void Map::CreateTerrainTexture(GfxController^ controller)
{
	// Apply the ground textures to the terrain texture.

	EditableTexturePtr spTexture = GetTerrain()->GetTexture();

	LPD3DXRENDERTOSURFACE pRenderToSurface = 0;

	GFX_TRYD3D(D3DXCreateRenderToSurface(
		Manager::GetDevice()->GetD3DDevice(),
		spTexture->GetWidth(),
		spTexture->GetHeight(),
		spTexture->GetFormat(),
		FALSE,
		D3DFMT_D24S8,
		&pRenderToSurface));

	LPD3DXSPRITE pSprite = 0;

	GFX_TRYD3D(D3DXCreateSprite(
		Manager::GetDevice()->GetD3DDevice(),
		&pSprite));

	LPDIRECT3DSURFACE9 pSurface = 0;

	GFX_TRYD3D(spTexture->GetD3DTexture()->GetSurfaceLevel(0, &pSurface));

	D3DVIEWPORT9 Viewport;

	Viewport.X = 0;
	Viewport.Y = 0;
	Viewport.Width = spTexture->GetWidth();
	Viewport.Height = spTexture->GetHeight();
	Viewport.MinZ = 0.0f;
	Viewport.MaxZ = 1.0f;

	GFX_TRYD3D(pRenderToSurface->BeginScene(pSurface, &Viewport)); 

	GFX_TRYD3D(pSprite->Begin(D3DXSPRITE_ALPHABLEND));

	D3DXMATRIX matWorld;

	D3DXMatrixIdentity(&matWorld);

	const float fMultiplier = Terrain::GetTextureMultiplier();

	ThemeManager^ themeMgr = GfxManager::GetThemeManager();

	const float fSpacing = GetTerrain()->GetSpacing();
	
	float fTopRightX = GetTerrain()->GetWorldTranslation().x;
	
	for (int i = 0; i < m_Lev->Width; ++i)
	{
		float fTopRightY = GetTerrain()->GetWorldTranslation().y;
		
		for (int j = 0; j < m_Lev->Height; ++j)
		{
			matWorld._41 = i*fMultiplier;
			matWorld._42 = j*fMultiplier;
				
			unsigned char i0 = m_Lev->GroundThemeIndices[i,j,0];
			unsigned char i1 = m_Lev->GroundThemeIndices[i,j,1];
			unsigned char i2 = m_Lev->GroundThemeIndices[i,j,2];
		
			unsigned char s0 = m_Lev->GroundThemeStrengths[i,j,0];
			unsigned char s1 = m_Lev->GroundThemeStrengths[i,j,1];
			unsigned char s2 = m_Lev->GroundThemeStrengths[i,j,2];

			float fX = fTopRightX+fSpacing/2;
			float fY = fTopRightY+fSpacing/2;

			Theme^ theme = themeMgr->Themes[m_Lev->GroundThemeNames[i0]];
			
			ApplyTheme(controller, pSprite, theme, &matWorld, 
				255, fX, fY, false, i, j);
			
			theme = nullptr;
			
			/*
			theme = themeMgr->Themes[m_Lev->GroundThemeNames[i1]];
			
			ApplyTheme(controller, pSprite, theme, &matWorld, 
				s1, fX, fY, false, i, j);
			
			theme = themeMgr->Themes[m_Lev->GroundThemeNames[i2]];

			ApplyTheme(controller, pSprite, theme, &matWorld, 
				s2, fX, fY, false, i, j);

			theme = nullptr;
			*/

			fTopRightY += fSpacing;
		}
			
		fTopRightX += fSpacing;
	}

	GFX_TRYD3D(pSprite->End());

	GFX_TRYD3D(pRenderToSurface->EndScene(D3DX_FILTER_LINEAR));

	GFX_SAFE_RELEASE(pSurface);
	GFX_SAFE_RELEASE(pSprite);
	GFX_SAFE_RELEASE(pRenderToSurface);
}

bool Map::IsOver(float x, float y, float z)
{
	return GetTerrain()->IsOver(D3DXVECTOR3(x,y,z));
}

void Map::Highlight(bool bHighlight)
{
	if (bHighlight)
		GetTerrain()->RenderFlags() |= MRF_WIREFRAME_OVERLAY;
	else
		GetTerrain()->RenderFlags() &= ~MRF_WIREFRAME_OVERLAY;
}

bool Map::Modified::get()
{
	return m_Lev->Modified;
}

void Map::STBAddFloraNode(
	GfxController^ controller,
	FableMod::STB::FloraNode^ node)
{
	Console::WriteLine("Adding flora {0} object(s)...", node->PointCount);

	for (int i = 0; i < node->PointCount; ++i)
	{
		FableMod::STB::FloraPoint fp = node->GetPoint(i);

		if (fp.FloraIndex >= m_STBLevel->FloraCount)
		{
			Console::WriteLine("!!!!!!! Invalid flora {0}.", fp.FloraIndex);
			continue;
		}
		
		FableMod::STB::Flora^ flora = m_STBLevel->GetFlora(fp.FloraIndex);

		GfxModel^ model = 
			GfxManager::GetModelManager()->Get(flora->GraphicsID[0]);
			
		if (model == nullptr)
		{
			Console::WriteLine("!!!!!!! Invalid flora model {0}.", flora->GraphicsID[0]);
			continue;
		}

		FableMod::CLRCore::Vec3 p = fp.Position;

		D3DXVECTOR3 v(
			p.x,//+flora->Offset.x, 
			p.y,//+flora->Offset.y, 
			p.z);
			
		NodePtr spNode = new Node();

		spNode->AttachChild(model->GetGfx());

		Spatial::ApplyTreeFlags(spNode, 
			true, FLAGS_EXCLUDE_PICK, true, MRF_FLORA);

		spNode->SetScale(/*(fp.Scale/100.0f)**/ 0.01f);

		spNode->SetTranslation(D3DXVECTOR3(v.x, v.y, v.z));
		spNode->SetRotation(D3DXVECTOR3(0, 0.0f, fp.Rotation));
				
		(*m_pSTBFlora)->AttachChild(spNode);
	}

	for (int i = 0; i < node->ChildCount; ++i)
	{
		STBAddFloraNode(controller, node->GetChild(i));
	}
}

void Map::STBCreateFlora(GfxController^ controller)
{
	if (!m_pSTBFlora)
	{
		m_pSTBFlora = new NodePtr(new Node());
		controller->GetRoot()->AttachChild(*m_pSTBFlora);
	}
	
	(*m_pSTBFlora)->RemoveChildren();

	STBAddFloraNode(controller, m_STBLevel->RootFloraNode);
}


}
}
}