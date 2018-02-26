#include "Stdafx.h"
#include "GfxThingInterface.h"
#include "GfxManager.h"
#include "ModelManager.h"
#include "Defs.h"

//#define USE_CUSTOM_LOD

using namespace System::Runtime::InteropServices;
using namespace FableMod::BIN;
using namespace FableMod::ContentManagement;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

GfxThingInterface::GfxThingInterface(
	FableMod::Gfx::Integration::ThingMap^ map, TNG::Thing^ thing) :
TNG::ThingInterface(thing),
m_Map(map),
m_pNode(0),
m_pOwnershipNode(0),
m_pObject(0),
m_pSphereMesh(0)
{
}

GfxThingInterface::~GfxThingInterface()
{
	this->!GfxThingInterface();
}

GfxThingInterface::!GfxThingInterface()
{
	Destroy();
}

void GfxThingInterface::Destroy()
{	
	if (GetNode())
		GetNode()->Detach();
	
	GFX_SAFE_DELETE(m_pNode);
	GFX_SAFE_DELETE(m_pOwnershipNode);
	GFX_SAFE_DELETE(m_pObject);
	GFX_SAFE_DELETE(m_pSphereMesh);

	m_Model = nullptr;
	m_Thing = nullptr;
	m_Map = nullptr;
	m_Controller = nullptr;
}

GfxModel^ GfxThingInterface::GetGraphic(
	ContentManagement::DefinitionType^ defType)
{
	if (!defType)
		return nullptr;

	Control^ control = defType->FindControl(0xC8636B2E);

	if (control != nullptr && control->Members->Count == 5)
	{
		Member^ member = (Member^)control->Members[1];

		return GfxManager::GetModelManager()->Get(
			UInt32::Parse(member->Value->ToString()));
	}

	return nullptr;
}

bool GfxThingInterface::AddObject(
	ContentManagement::DefinitionType^ defType,
	Node* pNode)
{
	GfxModel^ model = GetGraphic(defType);

	if (model)
	{
		pNode->AttachChild(model->GetGfx());
		model = nullptr;
		return true;
	}

	return false;
}

bool GfxThingInterface::AddBuilding(
	ContentManagement::DefinitionType^ defType,
	Node* pNode)
{
	if (!defType)
		return false;

	ArrayMember^ cdefs = defType->CDefs;

	if (cdefs->Elements->Count < 6)
		return false;

	int iMeshDefID = (int)((Member^)cdefs->Elements[5][1])->Value;

	ContentObject^ obj = ContentManager::Instance->FindEntry(
		LinkDestination::GameBINEntryID, iMeshDefID);

	GfxModel^ extModel = nullptr;
	GfxModel^ intModel = nullptr;

	if (obj != nullptr)
	{
		BINEntry^ binEntry = (BINEntry^)obj->Object;

		DefinitionType^ defType = 
			ContentManager::Instance->Definitions->GetDefinition(
				binEntry->Definition);

		if (defType == nullptr)
		{
			binEntry = nullptr;
			obj = nullptr;
			return false;
		}

		defType->ReadIn(binEntry);

		Control^ control = defType->FindControl(0x1CBDC0C);

		if (control != nullptr)
		{
			ArrayMember^ arr = (ArrayMember^)control->Members[1];

			Member^ intMember = (Member^)arr->Elements[0][1];
			Member^ extMember = (Member^)arr->Elements[1][1];

			intModel = GfxManager::GetModelManager()->Get(
				UInt32::Parse(intMember->Value->ToString()));

			extModel = GfxManager::GetModelManager()->Get(
				UInt32::Parse(extMember->Value->ToString()));
		
			intMember = nullptr;
			extMember = nullptr;
			arr = nullptr;
			control = nullptr;
		}

		obj = nullptr;
		binEntry = nullptr;
	}

	if (!extModel || !intModel)
		return false;

	pNode->AttachChild(extModel->GetGfx());

	Spatial* pInterior = intModel->GetGfx();

	// Visible only when camera inside worldbound.
	pInterior->Flags() |= FLAGS_CAMERA_INSIDE_WORLDBOUND;

	pNode->AttachChild(pInterior);
		
	AddHelpers(extModel, pNode);
	AddHelpers(intModel, pNode);

	return true;
}

void GfxThingInterface::AddHelpers(GfxModel^ model, Node* pNode)
{
	CompiledModel* pModel = model->LODs[0]->m_CompiledModel;

	System::Diagnostics::Debug::Assert(pModel != 0);

	ContentManager^ mgr = ContentManager::Instance;

	char* pszTmp = new char[1024];

	D3DXMATRIX matFix;
	D3DXMatrixRotationZ(&matFix, D3DXToRadian(180));		
	
	D3DXQUATERNION quatFix,quatRot,quatR;
	D3DXQuaternionRotationMatrix(&quatFix, &matFix);
	
	D3DXMATRIX matTransform;

	for (int i = 0; i < pModel->NumberHDMYs; ++i)
	{
		int nameIndex = pModel->FindHDMYString(pModel->HDMYs[i].HDMYCRC);
		
		strcpy(pszTmp, "HDMY_");

		if (nameIndex > -1)
			strcat(pszTmp, pModel->HDMYIndex[nameIndex]);
		else
			strcat(pszTmp, "NAME_NOT_FOUND");

		String^ name = gcnew String(pszTmp);

		//Console::WriteLine("Helper: \"{0}\"", name);
		//Console::WriteLine(" Parent: {0}", pModel->HDMYs[i].Parent);

		if (name->StartsWith(HDMY_BUILDING))
		{
			String^ objectName = name->Substring(HDMY_BUILDING->Length+1);

			Console::WriteLine("Building Helper: \"{0}\"", objectName);

			Node* pHelperNode = new FableMod::Gfx::Node();

			pModel->HDMYs[i].GetMatrix(&matTransform);

			//D3DXMatrixMultiply(&matTransform,&matTransform,&matInv);
			
			// Fix the transformation matrix.
			// Shouldn't really even be here, but don't blame me.

			D3DXVECTOR3 vPos(
				matTransform._41,
				matTransform._42,
				matTransform._43);

			matTransform._41 = 0.0f;
			matTransform._42 = 0.0f;
			matTransform._43 = 0.0f;

			D3DXVec3TransformCoord(&vPos, &vPos, &matFix);

			pHelperNode->SetTranslation(vPos);

			D3DXQuaternionRotationMatrix(&quatRot, &matTransform);

			quatR = quatFix*quatRot;
			
			D3DXMatrixRotationQuaternion(&matTransform, &quatR);
			
			pHelperNode->SetRotation(matTransform);
			
			DefinitionType^ defType =
				mgr->FindObjectDefinitionType(objectName);
			
			if (AddBuilding(defType, pHelperNode))
			{
				pNode->AttachChild(pHelperNode);
			}
			else
			{
				if (AddObject(defType, pHelperNode))
				{
					pNode->AttachChild(pHelperNode);
				}
				else
				{
					delete pHelperNode;
					Console::WriteLine(" Helper building ignored.");
				}
			}
		}
		else if (name->StartsWith(HDMY_OBJECT))
		{
			String^ objectName = name->Substring(HDMY_OBJECT->Length+1);

			Console::WriteLine("Object Helper: \"{0}\"", objectName);

			Node* pHelperNode = new FableMod::Gfx::Node();

			D3DXMATRIX matTransform;

			pModel->HDMYs[i].GetMatrix(&matTransform);

			// Fix the transformation matrix.
			// Shouldn't really even be here, but don't blame me.

			D3DXVECTOR3 vPos(
				matTransform._41,
				matTransform._42,
				matTransform._43);

			Console::WriteLine("pos: {0},{1},{2}",
				vPos.x,vPos.y,vPos.z);
			
			D3DXVec3TransformCoord(&vPos, &vPos, &matFix);

			pHelperNode->SetTranslation(vPos);

			matTransform._41 = 0.0f;
			matTransform._42 = 0.0f;
			matTransform._43 = 0.0f;
			
			D3DXQuaternionRotationMatrix(&quatRot, &matTransform);

			quatR = quatFix*quatRot;
			
			D3DXMatrixRotationQuaternion(&matTransform, &quatR);
			
			pHelperNode->SetRotation(matTransform);

			if (AddObject(
				mgr->FindObjectDefinitionType(objectName), pHelperNode))
			{
				pNode->AttachChild(pHelperNode);
			}
			else
			{
				delete pHelperNode;
				Console::WriteLine(" Helper object ignored.");
			}
		}
	}

	delete[] pszTmp;
}

bool GfxThingInterface::CreateBuilding(GfxThingController^ controller)
{
	if (Thing->BinDefinitionType != nullptr)
	{
		m_pObject = new FableMod::Gfx::NodePtr(
			new FableMod::Gfx::ImpostorNode());
		
		Node* pNode = (Node*)(Spatial*)(*m_pObject);

		if (!AddBuilding(Thing->BinDefinitionType, pNode))
		{
			delete m_pObject;
			m_pObject = 0;
			return false;
		}
		
		// Create the thing base node.
		m_pNode = new FableMod::Gfx::NodePtr(new FableMod::Gfx::Node());

		System::IntPtr ptr = 
			Marshal::StringToHGlobalUni(Thing->UID);

		(*m_pObject)->SetName((LPCTSTR)ptr.ToPointer());
		
		GetNode()->SetName((LPCTSTR)ptr.ToPointer());

		Marshal::FreeHGlobal(ptr);

		GetNode()->ID() = Spatial::GetNextID();

		GetNode()->AttachChild(*m_pObject);
		
		// This is for the ownership stuff.
		m_pOwnershipNode = new FableMod::Gfx::NodePtr(
			new FableMod::Gfx::Node());

		GetNode()->AttachChild(*m_pOwnershipNode);
		
		GetNode()->Update();

		Thing->Interface = this;

		controller->AddThing(GetNode()->ID(), Thing);
		
		return true;
	}

	return false;
}

bool GfxThingInterface::Create(GfxThingController^ controller)
{
	m_Controller = controller;

	String^ thingDefType = Thing->DefinitionType;
	
	bool bGraphic = false;

	if (!String::IsNullOrEmpty(Thing->GraphicOverride))
	{
		// Object has a graphic override.
		m_Model = GfxManager::GetModelManager()->Get(Thing->GraphicOverride);
	}
	else
	{
		if (Thing->Name == "Building")
		{
			if (CreateBuilding(controller))
				return true;
			else
				bGraphic = true;
		}
		else
		{
			bGraphic = true;
		}
	}

	if (bGraphic)
	{
		// Get the model entry from the bin and load it.
		m_Model = GetGraphic(Thing->BinDefinitionType);
	}

	if (m_Model != nullptr)
	{
		// Normal model mode.

		if (Thing->Name == "Building")
		{
			// Use impostors with buildings.

			m_pObject = new FableMod::Gfx::NodePtr(
				new FableMod::Gfx::ImpostorNode());
			
			((Node*)(Spatial*)(*m_pObject))->AttachChild(
				m_Model->GetGfx());

		}
		else
		{
			m_pObject = new FableMod::Gfx::NodePtr(m_Model->GetGfx());
		}

		// m_pNode is m_pObject's parent, and a hook
		// to which children can be attached.
		// This way we get the freedom to change the 
		// mesh object, but not the children.
		// E.g. House with furniture.

		m_pNode = new FableMod::Gfx::NodePtr(new FableMod::Gfx::Node());
		
		AddHelpers(m_Model, (Node*)(Spatial*)(*m_pObject));
		
		// Do we need a radius for this object...

		if (thingDefType == "MARKER_INFO_DISPLAY" ||
			thingDefType == "MARKER_LIGHT" ||
			thingDefType == "MARKER_CREATURE_GENERATOR" ||
			thingDefType == "REGION_EXIT_POINT")
		{
			m_pSphereMesh = new FableMod::Gfx::MeshPtr(new Mesh());

			D3DXModel* pModel = new D3DXModel();

			pModel->BuildSphere(1.0f, D3DCOLOR_ARGB(32, 200, 200, 150));

			(*m_pSphereMesh)->SetModel(pModel);

			(*m_pNode)->AttachChild(*m_pSphereMesh);
		
            (*m_pSphereMesh)->Flags() |= 
				FLAGS_EXCLUDE_PICK | 
				FLAGS_FORCE_CULLING |
				FLAGS_EXCLUDE_BOUND_MERGE;
		}
		
		if (Thing->Name == "Marker")
		{
			// Needed for teleporter stuff.
			(*m_pNode)->Flags() |= FLAGS_EXCLUDE_BOUND_MERGE;
		}

		System::IntPtr ptr = 
			Marshal::StringToHGlobalUni(Thing->UID);

		(*m_pObject)->SetName((LPCTSTR)ptr.ToPointer());
		(*m_pNode)->SetName((LPCTSTR)ptr.ToPointer());

		Marshal::FreeHGlobal(ptr);

		// Set a unique idenfitier.
		// UIDs may not be unique across a region.
		// We use the ID to connect the spatial object
		// to the thing.
		(*m_pNode)->ID() = Spatial::GetNextID();

		(*m_pNode)->AttachChild(*m_pObject);

		m_pOwnershipNode = new FableMod::Gfx::NodePtr(
			new FableMod::Gfx::Node());

		GetNode()->AttachChild(*m_pOwnershipNode);

		(*m_pNode)->Update();

		Thing->Interface = this;

		controller->AddThing((*m_pNode)->ID(), Thing);

		return true;
	}

	Console::WriteLine("No model for {0}", Thing->DefinitionType);

	return false;
}

void GfxThingInterface::UpdateOwnership()
{
	FableMod::TNG::CTCBlock^ block = Thing->CTCs["CTCOwnedEntity"];

	if (block)
	{
		String^ uid = block->Variables["OwnerUID"]->StringValue;

		FableMod::TNG::Thing^ ownerThing = nullptr;

		if (uid != "0")
			ownerThing = Map->TNG->FindThing(uid);
		
		if (ownerThing)
		{
			GfxThingInterface^ ownerInterface = 
				(GfxThingInterface^)ownerThing->Interface;

			if (ownerInterface != nullptr && 
				ownerInterface->GetOwnershipNode())
			{
				// Detach this.
				SpatialPtr spSaved(GetNode());
				GetNode()->Detach();
				ownerInterface->GetOwnershipNode()->AttachChild(GetNode());
			}
			else
			{
				Console::WriteLine("Strange owner for {0} {1}",
					Thing->DefinitionType,
					Thing->UID);
			}
		}
		else
		{
			// No owner.
			
			if (GetNode()->Parent() != m_Controller->GetRoot())
			{			
				// Had some owner, remove it from that and
				// add it to the level base.

				Console::WriteLine("Remove {0}:{1} as owned",
					Thing->DefinitionType, Thing->UID);

				SpatialPtr spSaved(GetNode());
				GetNode()->Detach();
				m_Controller->GetRoot()->AttachChild(GetNode());
			}
		}
	}
}

void GfxThingInterface::UpdateInterface()
{
	// This seems to be the correct world size.
	// Notice we apply it to the object, not to the node.
	float fScale = 0.01f;

	try
	{
		fScale *= (float)Thing->Variables["ObjectScale"]->Value;
	}
	catch (System::Exception^)
	{
	}

	(*m_pObject)->SetScale(fScale);

	const D3DXVECTOR3 vMapLocation(Map->X, Map->Y, 0);

	FableMod::TNG::CTCBlock^ block = nullptr;
	
	if (Thing->Name == "AICreature")
		block = Thing->CTCs["CTCPhysicsNavigator"];
	else
		block = Thing->CTCs["CTCPhysicsStandard"];

	if (block != nullptr)
	{
		try
		{
			FableMod::TNG::Variable^ x = block->Variables["PositionX"];
			FableMod::TNG::Variable^ y = block->Variables["PositionY"];
			FableMod::TNG::Variable^ z = block->Variables["PositionZ"];
	
			const D3DXVECTOR3 vAdd(
				(float)x->Value, (float)y->Value, (float)z->Value);

			const D3DXVECTOR3 vForward(
				(float)block->Variables["RHSetForwardX"]->Value,
				(float)block->Variables["RHSetForwardY"]->Value,
				(float)block->Variables["RHSetForwardZ"]->Value);
			
			const D3DXVECTOR3 vUp(
				(float)block->Variables["RHSetUpX"]->Value,
				(float)block->Variables["RHSetUpY"]->Value,
				(float)block->Variables["RHSetUpZ"]->Value);

			GetNode()->SetWorldTranslation(vMapLocation+vAdd);

			GetNode()->SetWorldDirection(vForward, vUp);
		}
		catch (System::Exception^ ex)
		{
			Console::WriteLine("GfxThingInterface: {0}", ex->ToString());
		}
	}
	
	if (m_pSphereMesh)
	{
		// Special cases... things that have a radius associated with them.

		try
		{
			String^ thingDefType = Thing->DefinitionType;
		
			float fRadius = 1.0f;

			if (thingDefType == "MARKER_INFO_DISPLAY")
			{
				FableMod::TNG::CTCBlock^ block = Thing->CTCs["CTCInfoDisplay"];
			
				fRadius = (float)block->Variables["Radius"]->Value;
			}
			else if (thingDefType == "MARKER_CREATURE_GENERATOR")
			{
				FableMod::TNG::CTCBlock^ block = 
					Thing->CTCs["CTCCreatureGenerator"];
			
				fRadius = (float)block->Variables["GenerationRadius"]->Value;
			}
			else if (thingDefType == "REGION_EXIT_POINT")
			{
				FableMod::TNG::CTCBlock^ block = Thing->CTCs["CTCDRegionExit"];
			
				fRadius = (float)block->Variables["Radius"]->Value;
			}
			else if (thingDefType == "MARKER_LIGHT")
			{
				FableMod::TNG::CTCBlock^ block = Thing->CTCs["CTCLight"];

				fRadius = (float)block->Variables["OuterRadius"]->Value;
			}

			(*m_pSphereMesh)->SetScale(fRadius);
		}
		catch (System::Exception^ ex)
		{
		}
	}

	Spatial::ApplyTreeFlags(*m_pObject, 
		false, FLAGS_EXCLUDE_PICK, false, MRF_AS_STONE | MRF_WIREFRAME);

	if (Freezed)
	{
		Spatial::ApplyTreeFlags(
			*m_pObject, true, FLAGS_EXCLUDE_PICK, true, MRF_WIREFRAME);
	}
	else if (LockedInPlace)
	{
		// Apply locked flags.
		Spatial::ApplyTreeFlags(*m_pObject, true, 0, true, MRF_AS_STONE);
	}

	GetNode()->Update();
}

void GfxThingInterface::Update()
{
	UpdateOwnership();
	UpdateInterface();
}

bool GfxThingInterface::LockedInPlace::get()
{
	FableMod::TNG::CTCBlock^ block = Thing->CTCs["CTCEditor"];
	
	if (block != nullptr)
	{
		try
		{
			return (bool)block->Variables["LockedInPlace"]->Value;
		}
		catch (System::Exception^)
		{
		}
	}
	
	return false;
}

bool GfxThingInterface::Freezed::get()
{
	FableMod::TNG::CTCBlock^ block = Thing->CTCs["CTCEditor"];
	
	if (block != nullptr)
	{
		try
		{
			return (bool)block->Variables["Freezed"]->Value;
		}
		catch (System::Exception^)
		{
		}
	}
	
	return false;
}

void GfxThingInterface::Freezed::set(bool value)
{
	FableMod::TNG::CTCBlock^ block = Thing->CTCs["CTCEditor"];
	
	if (block != nullptr)
	{
		try
		{
			// Causes an object update.
			block->Variables["Freezed"]->Value = value;
		}
		catch (System::Exception^)
		{
		}
	}
}

void GfxThingInterface::UpdateThing()
{
	// Change the thing.
	
	Thing->LockUpdate();

	FableMod::TNG::CTCBlock^ block = nullptr;
	
	if (Thing->Name == "AICreature")
		block = Thing->CTCs["CTCPhysicsNavigator"];
	else
		block = Thing->CTCs["CTCPhysicsStandard"];

	if (block != nullptr)
	{
		block->Variables["PositionX"]->Value =
			(*m_pNode)->GetWorldTranslation().x-Map->X;
		block->Variables["PositionY"]->Value =
			(*m_pNode)->GetWorldTranslation().y-Map->Y;
		block->Variables["PositionZ"]->Value =
			(*m_pNode)->GetWorldTranslation().z;

		block->Variables["RHSetForwardX"]->Value =
			(*m_pNode)->GetWorldForward().x;
		block->Variables["RHSetForwardY"]->Value =
			(*m_pNode)->GetWorldForward().y;
		block->Variables["RHSetForwardZ"]->Value =
			(*m_pNode)->GetWorldForward().z;

		block->Variables["RHSetUpX"]->Value =
			(*m_pNode)->GetWorldUp().x;
		block->Variables["RHSetUpY"]->Value =
			(*m_pNode)->GetWorldUp().y;
		block->Variables["RHSetUpZ"]->Value =
			(*m_pNode)->GetWorldUp().z;
	}

	Thing->UnlockUpdate();
}

void GfxThingInterface::Highlight::set(bool value)
{
	if (value)
		Spatial::ApplyTreeFlags(*m_pNode, true, 0, true, MRF_HIGHLIGHT);
	else	
		Spatial::ApplyTreeFlags(*m_pNode, true, 0, false, MRF_HIGHLIGHT);

	if (value)
		(*m_pObject)->Flags() |= FLAGS_DISABLE_SIMPLIFICATION;
	else
		(*m_pObject)->Flags() &= ~FLAGS_DISABLE_SIMPLIFICATION;

	if (m_pSphereMesh)
	{
		if (value)
			(*m_pSphereMesh)->Flags() &= ~FLAGS_FORCE_CULLING;
		else
			(*m_pSphereMesh)->Flags() |= FLAGS_FORCE_CULLING;

		(*m_pSphereMesh)->RenderFlags() &= ~MRF_HIGHLIGHT;
	}
}

void GfxThingInterface::Show(bool value)
{
	if (!value)
		GetNode()->Flags() |= FLAGS_FORCE_CULLING;
	else
		GetNode()->Flags() &= ~FLAGS_FORCE_CULLING;

	if (m_pSphereMesh)
	{
		// Also hide the sphere mesh (area effect), if there.

		if (!value)
			(*m_pSphereMesh)->Flags() &= ~FLAGS_FORCE_CULLING;
		else
			(*m_pSphereMesh)->Flags() |= FLAGS_FORCE_CULLING;
	}
}

void GfxThingInterface::GroundMode(bool value)
{
	if (value)
	{
		(*m_pObject)->Flags() |= FLAGS_DISABLE_SIMPLIFICATION;

		Spatial::ApplyTreeFlags(GetNode(), 
			true, FLAGS_EXCLUDE_PICK, true, MRF_AS_STONE);
		
		GetNode()->SetAlpha(GFXI_GROUNDMODE_ALPHA);
	}
	else
	{
		(*m_pObject)->Flags() &= ~FLAGS_DISABLE_SIMPLIFICATION;

		GetNode()->SetAlpha(1.0f);

		if (m_pSphereMesh)	
		{
			(*m_pSphereMesh)->Flags() |= 
				FLAGS_EXCLUDE_PICK | 
				FLAGS_EXCLUDE_BOUND_MERGE;
		}

		Spatial::ApplyTreeFlags(GetNode(), 
			false, FLAGS_EXCLUDE_PICK, false, MRF_AS_STONE);
		
		if (Freezed)
		{
			Spatial::ApplyTreeFlags(
				GetNode(), true, FLAGS_EXCLUDE_PICK, true, MRF_WIREFRAME);
		}
		else if (LockedInPlace)
		{
			// Apply locked flags.
			Spatial::ApplyTreeFlags(GetNode(), true, 0, true, MRF_AS_STONE);
		}
	}
}

}
}
}