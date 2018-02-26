#pragma once

#include "D3DModel.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API ArrowheadModel : public D3DModel
{
public:

	ArrowheadModel();

	virtual ~ArrowheadModel();

	void Create();

	virtual void Render(Mesh* pMesh, Camera* pCamera, bool bSimple);
	
	void SetColor(D3DCOLOR Color);

protected:

	D3DMATERIAL9 m_Material;
};

GFX_DECLARE_SMARTPOINTER(ArrowheadModel);

}
}