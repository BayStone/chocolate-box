#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

ref class GfxView;
ref class GfxBaseModel;
ref class GfxModel;
ref class GfxModelLOD;

public ref class GfxController
{
public:

	GfxController();

	virtual ~GfxController();

	virtual void Destroy();

	virtual void ResetObjects();

	void AddView(GfxView^ view);

	void LoadObject(String^ fileName);

	void AddModel(GfxBaseModel^ model);

	void AddModel(GfxModelLOD^ modelLOD);

	void UpdateObjects();

	::FableMod::Gfx::Node* GetRoot() { return *m_pRoot; }

private:

	System::Collections::Generic::List<GfxView^>^ m_Views;

	::FableMod::Gfx::NodePtr* m_pRoot;
};

}
}
}