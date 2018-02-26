#pragma once


using namespace System;
using namespace FableMod;

#include "ThingMap.h"
#include "GfxModel.h"
#include "GfxThingController.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class GfxThingInterface : public TNG::ThingInterface
{
public:

	GfxThingInterface(ThingMap^ map, TNG::Thing^ thing);

	virtual ~GfxThingInterface();

	!GfxThingInterface();

	bool Create(GfxThingController^ controller);

	virtual void Destroy() override;

	virtual void Update() override;

	virtual void UpdateThing() override;

	property FableMod::Gfx::Integration::ThingMap^ Map
	{
		FableMod::Gfx::Integration::ThingMap^ get() { return m_Map; }
	}

	property GfxModel^ Model
	{
		GfxModel^ get() { return m_Model; }
	}

	void GroundMode(bool value);

	void Show(bool value);

	property bool Highlight { void set(bool value); }

	property bool LockedInPlace { bool get(); }
	
	property bool Freezed 
	{ 
		bool get(); 
		void set(bool value);
	}
	
	FableMod::Gfx::Node* GetNode() 
	{ 
		if (m_pNode)
			return *m_pNode; 
		else
			return 0;
	}

	FableMod::Gfx::Node* GetOwnershipNode() 
	{ 
		if (m_pOwnershipNode)
			return *m_pOwnershipNode; 
		else
			return 0;
	}

internal:

	void UpdateOwnership();
	void UpdateInterface();

protected:

	static String^ HDMY_BUILDING = gcnew String("HDMY_CREATEBUILDING");
	static String^ HDMY_OBJECT = gcnew String("HDMY_CREATEOBJECT");

	bool CreateBuilding(GfxThingController^ controller);

	bool AddBuilding(
		ContentManagement::DefinitionType^ defType,
		Node* pNode);

	void AddHelpers(GfxModel^ model, Node* pNode);

	GfxModel^ GetGraphic(ContentManagement::DefinitionType^ defType);

	bool AddObject(
		ContentManagement::DefinitionType^ defType,
		Node* pNode);

	FableMod::Gfx::Integration::ThingMap^ m_Map;
	GfxModel^ m_Model;
	GfxThingController^ m_Controller;

	FableMod::Gfx::NodePtr* m_pNode;
	FableMod::Gfx::NodePtr* m_pOwnershipNode;
	FableMod::Gfx::NodePtr* m_pObject;
	FableMod::Gfx::MeshPtr* m_pSphereMesh;
};

}
}
}