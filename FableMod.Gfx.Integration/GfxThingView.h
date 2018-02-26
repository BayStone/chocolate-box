#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "GfxView.h"
#include "GfxThingController.h"
#include "ThingMap.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration 
{
	public delegate void ThingSelectedHandler(
		array<FableMod::TNG::Thing^>^ things);
	
	public delegate void ThingCreateHandler(
		float x, float y, float z,
		float nx, float ny, float nz);

	public delegate void ThingPickHandler(FableMod::TNG::Thing^ thing);

	public enum class EditorMode
	{
		Normal,
		Walk,
		Player,
		Create,
		Navigation,
		Pick,
	};

	public enum class NavigationEditMode
	{
		Navigation,
		Dynamic,
	};

	/// <summary>
	/// Summary for GfxThingView
	/// </summary>
	public ref class GfxThingView : public FableMod::Gfx::Integration::GfxView
	{
	public:
		
		GfxThingView(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		property Collection<ThingMap^>^ Maps
		{
			Collection<ThingMap^>^ get()
			{
				return m_Maps;
			}
			
			void set(Collection<ThingMap^>^ maps)
			{
				m_Maps = maps;
			}
		}
		
		property GfxThingController^ ThingController
		{
			GfxThingController^ get() 
			{ 
				return (GfxThingController^)m_Controller; 
			}
		}

		property EditorMode Mode
		{
			EditorMode get() { return m_EditorMode; }
			void set(EditorMode value);
		}

		property String^ EditSection
		{
			String^ get() { return m_EditSection; }
			void set(String^ value) 
			{ 
				SaveGroundChanges();

				m_EditSection = value; 

				if (Mode == EditorMode::Navigation)
					ToNavMode();
			}
		}

		property int EditSubset
		{
			int get() { return m_iEditSubset; }
			void set(int value) 
			{ 
				SaveGroundChanges();

				m_iEditSubset = value; 

				if (Mode == EditorMode::Navigation)
					ToNavMode();
			}
		}

		property ThingMap^ EditMap
		{
			ThingMap^ get() { return m_EditMap; }

			void set(ThingMap^ value) 
			{ 
				SaveGroundChanges();
				
				m_EditMap = value; 
				
				if (Mode == EditorMode::Navigation)
					ToNavMode();
				else if (Mode == EditorMode::Walk)
					ToWalkMode();
				else
					ToNormalMode();
			}
		}

		property int BrushSize
		{
			int get() { return m_iBrushSize; }
			void set(int value) { m_iBrushSize = value; }
		}

		property NavigationEditMode NavEditMode
		{
			NavigationEditMode get() { return m_NavEditMode; }
			void set(NavigationEditMode value) { m_NavEditMode = value; }
		}

		property bool STBDebugFlora
		{
			bool get() { return m_STBDebugFlora; }
			void set(bool value) { m_STBDebugFlora = value; }
		}
			
		property bool STBDebugBlocks
		{
			bool get() { return m_STBDebugBlocks; }
			void set(bool value) { m_STBDebugBlocks = value; }
		}
		
		property bool DirectionAxes
		{
			bool get() { return m_bDirectionAxes; }
			void set(bool value) { m_bDirectionAxes = value; }
		}

		property bool IsDoing
		{
			bool get() { return m_bMoving || m_bRotating; }
		}

		event ThingSelectedHandler^ ThingSelected;
		event ThingCreateHandler^ ThingCreated;
		event ThingPickHandler^ ThingPicked;
		
		virtual void Destroy() override;
		virtual void OnResetObjects() override;
		virtual void DefaultButtonConfiguration() override;

		void SaveChanges();

		void ResetNavSections();

		void FindByUID(String^ uid);

		void UpdateAll();

		void Reset();

		void ClearSelection();

	internal:

		virtual void Initialize() override;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~GfxThingView()
		{
			if (components)
			{
				delete components;
			}
		}

		virtual void RenderInterface() override;
		virtual void CameraHeartbeat() override;

		void STBRenderDebugFlora();
		void STBRenderFloraNode(::FableMod::STB::FloraNode^ node);
		void STBRenderDebugBlocks();
		void ShowSTBTerrain(bool show);

		void DrawAxes();
		void SetupAxes();
		bool AxesOn();
		void SetupPlane(FableMod::Gfx::Mesh* pArrowMesh);
		void UpdateMovePoint();
		float GetPlaneDistance(
			const D3DXVECTOR3* pvOrigin,
			const D3DXVECTOR3* pvDirection,
			Plane* pPlane);
		void StopMove();
		void StopRotation();
		void UpdateSelectedThings();
		void SelectDuplicates();
		void SetSelectionHighlight(bool on);
		void ToWalkMode();
		void ToNormalMode();
		void ToNavMode();
		void ToGroundMode();
		void SaveGroundChanges();
		void CallThingSelected();
		array<FableMod::TNG::Thing^>^ GetSelectedThings();
		void ApplyOwnership(FableMod::TNG::Thing^ parent);
		void RemoveOwnership(FableMod::TNG::Thing^ parent);
		virtual void OnRender() override;

		Collection<ThingMap^>^ m_Maps;
		String^ m_EditSection;
		int m_iEditSubset;
		ThingMap^ m_EditMap;

		FableMod::Gfx::SelectionBox* m_pSelection;
		FableMod::Gfx::MeshPtr* m_pXArrowMesh;
		FableMod::Gfx::MeshPtr* m_pYArrowMesh;
		FableMod::Gfx::MeshPtr* m_pZArrowMesh;
		FableMod::Gfx::Mesh* m_pArrow;
		D3DXVECTOR3* m_pvMoveDiff;
		D3DXVECTOR3* m_pvMovePoint;
		D3DXVECTOR2* m_pvMousePoint;
		bool m_bMoving;
		bool m_bRotating;
		FableMod::Gfx::NodePtr* m_pAxes;
		FableMod::Gfx::Plane* m_pPlane;
		EditorMode m_EditorMode;
		bool m_bDirectionAxes;
		FableMod::Gfx::EditableTexture* m_pWorkTexture;
		FableMod::Gfx::Mesh* GetMeshAtMouse();
		FableMod::Gfx::Mesh* GetArrowAtMouse();
		int m_iX, m_iY;
		int m_iBrushSize;
		NavigationEditMode m_NavEditMode;
		FLOAT m_fPlayerHeight;
		FLOAT m_fPlayerFOV;
		FLOAT m_fSavedFOV;
		D3DCOLOR m_MapColor;
		int m_iDrawState;
		bool m_STBDebugFlora;
		bool m_STBDebugBlocks;

		FableMod::Gfx::Node* GetAxes() { return *m_pAxes; }

		bool CheckLockedInPlace(bool begin);

		virtual void BtnCfg_OnButtonDown(
			System::String^ button, 
			System::Windows::Forms::MouseEventArgs^ e) override;
		virtual void BtnCfg_OnButtonUp(
			System::String^ button, 
			System::Windows::Forms::MouseEventArgs^ e) override;

		bool ButtonDownNormal(String^ button);
		bool MouseMoveNormal(System::Windows::Forms::MouseEventArgs^  e);
		void SelectThing(bool add);

		virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^  e) override;
		virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^  e) override;
		virtual void OnMouseUp(System::Windows::Forms::MouseEventArgs^  e) override;
		virtual void OnMouseDoubleClick(System::Windows::Forms::MouseEventArgs^  e) override;
		virtual void OnKeyUp(System::Windows::Forms::KeyEventArgs^ e) override;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		}
#pragma endregion
	};

}
}
}