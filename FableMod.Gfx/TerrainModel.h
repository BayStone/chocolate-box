#pragma once

#include "D3DXModel.h"
#include "EditableTexture.h"
#include "Terrain.h"

namespace FableMod
{

namespace LEV
{
class LEVFile;
}

namespace Gfx
{

class GFX_API TerrainModel : public D3DXModel
{
public:

	TerrainModel(Terrain* pTerrain);
	
	~TerrainModel();

	virtual void Dispose();

	void Render(Mesh* pMesh, Camera* pCamera, bool bSimple = false);

	void Create(
		class FableMod::LEV::LEVFile* pLEVFile,
		int iX,
		int iY,
		int iBlockSize,
		float fCellSize);

	void GetLocation(
		unsigned int uiFace, 
		unsigned int& ruiX, 
		unsigned int& ruiY) const;

	void BuildHeightfield(
		const D3DXMATRIX* pWorldMatrix,
		const D3DXVECTOR2& vWorldBase, 
		EditableTexture* pTexture,
		unsigned int uiTexX,
		unsigned int uiTexY,
		FLOAT fCellSize);

	Terrain* GetTerrain() { return m_pTerrain; }

protected:

	static void ComputeBarycenter(
		const D3DXVECTOR2& vV0,
		const D3DXVECTOR2& vV1, 
		const D3DXVECTOR2& vV2, 
		const D3DXVECTOR2& vP,
		FLOAT afBary[3]);

	static bool InTriangle(
		const D3DXVECTOR2& vV0,
		const D3DXVECTOR2& vV1, 
		const D3DXVECTOR2& vV2, 
		const D3DXVECTOR2& vTest);
	
	unsigned int m_uiSize;
	Terrain* m_pTerrain;
};

}
}