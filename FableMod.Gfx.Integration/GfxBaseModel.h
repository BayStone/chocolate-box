#pragma once

#include "ExtendedFrame.h"

using namespace System;
using namespace FableMod::BIG;
using namespace FableMod::ContentManagement;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class GfxBaseModel : AssetLinker
{
public:

	GfxBaseModel(AssetEntry^ entry);
	
	virtual ~GfxBaseModel();
	
	virtual void ExportX(String^ fileName);
	
	virtual void CompileToEntry(AssetEntry^ entry) {}

	virtual FableMod::Gfx::Node* GetGfx();

	static FableMod::Gfx::Node* BuildGfx(
		D3DXEXTENDEDFRAME* pFrame, FableMod::Gfx::Node* pParent);

	static void ExportToX(String^ fileName, D3DXEXTENDEDFRAME* pRoot);
	
protected:

	virtual FableMod::Gfx::Node* BuildModel();

	D3DXEXTENDEDFRAME* m_pRoot;
	FableMod::Gfx::NodePtr* m_pNode;

private:
};

}
}
}