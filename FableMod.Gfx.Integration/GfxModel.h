#pragma once

#include "GfxModelLOD.h"
#include "GfxBaseModel.h"

using namespace System;
using namespace System::Collections::Specialized;
using namespace FableMod::BIG;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

struct ModelSubHeader
{
	DWORD	Physics;
	float	floats[10];
	DWORD	LODCount;
	DWORD*	LODSizes;
	DWORD	padding;
	DWORD	TextureCount;
	DWORD*	TextureIDs;

	ModelSubHeader(char* data);

	void WriteToBuffer(char* data, unsigned int* length);

	~ModelSubHeader();
};

public ref class GfxModel : GfxBaseModel
{
public:

	GfxModel(AssetEntry^ entry);
	
	~GfxModel();
	
	!GfxModel();
	
	virtual void CompileToEntry(AssetEntry^ entry) override;
	  	
	virtual NameValueCollection^ CreateLinks() override;

   	virtual void ApplyLinks(NameValueCollection^ c) override;

	property unsigned int LODCount
	{
		unsigned int get();
	}
	
	property GfxModelLOD^ LODs[int]
	{
		GfxModelLOD^ get(int index);
		void set(int index, GfxModelLOD^ lod);
	}

	property UInt32 Physics
	{
		UInt32 get();
		void set(UInt32 value);
	}

protected:

	virtual FableMod::Gfx::Node* BuildModel() override;

private:

	array<GfxModelLOD^>^ m_LODs;
	ModelSubHeader*	m_SubHeader;
};

}
}
}