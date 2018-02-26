#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "DefinitionType.h"
#include "ControlDisplay.h"

namespace FableMod
{
namespace ContentManagement
{

/// <summary>
/// Summary for DefinitionTypeDisplay
/// </summary>
public ref class DefinitionTypeDisplay : public System::Windows::Forms::UserControl
{
public:
	DefinitionTypeDisplay(DefinitionType^ definition, ContentManager^ contentmgr);
	
	void ApplyChanges();
	
	void DoLayout();

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~DefinitionTypeDisplay()
	{
		if (components)
		{
			delete components;
		}
	}

private:
	ContentManager^ m_ContentMgr;
	DefinitionType^ m_Definition;
	Control^		m_Display;
private: System::Windows::Forms::TreeView^  tvMembers;
private: System::Windows::Forms::Splitter^  splitter1;
private: System::Windows::Forms::Panel^  pnlMemberDisplay;

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
		this->tvMembers = (gcnew System::Windows::Forms::TreeView());
		this->splitter1 = (gcnew System::Windows::Forms::Splitter());
		this->pnlMemberDisplay = (gcnew System::Windows::Forms::Panel());
		this->SuspendLayout();
		// 
		// tvMembers
		// 
		this->tvMembers->Dock = System::Windows::Forms::DockStyle::Left;
		this->tvMembers->HideSelection = false;
		this->tvMembers->Location = System::Drawing::Point(0, 0);
		this->tvMembers->Name = L"tvMembers";
		this->tvMembers->Size = System::Drawing::Size(200, 449);
		this->tvMembers->TabIndex = 0;
		this->tvMembers->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &DefinitionTypeDisplay::tvMembers_AfterSelect);
		// 
		// splitter1
		// 
		this->splitter1->Location = System::Drawing::Point(200, 0);
		this->splitter1->Name = L"splitter1";
		this->splitter1->Size = System::Drawing::Size(5, 449);
		this->splitter1->TabIndex = 1;
		this->splitter1->TabStop = false;
		// 
		// pnlMemberDisplay
		// 
		this->pnlMemberDisplay->AutoScroll = true;
		this->pnlMemberDisplay->Dock = System::Windows::Forms::DockStyle::Fill;
		this->pnlMemberDisplay->Location = System::Drawing::Point(205, 0);
		this->pnlMemberDisplay->Name = L"pnlMemberDisplay";
		this->pnlMemberDisplay->Size = System::Drawing::Size(348, 449);
		this->pnlMemberDisplay->TabIndex = 2;
		this->pnlMemberDisplay->TabStop = true;
		this->pnlMemberDisplay->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &DefinitionTypeDisplay::pnlMemberDisplay_MouseClick);
		// 
		// DefinitionTypeDisplay
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->AutoSize = true;
		this->Controls->Add(this->pnlMemberDisplay);
		this->Controls->Add(this->splitter1);
		this->Controls->Add(this->tvMembers);
		this->Name = L"DefinitionTypeDisplay";
		this->Size = System::Drawing::Size(553, 449);
		this->ResumeLayout(false);

	}
#pragma endregion
private:
	void MemberSizeChanged(Object^ sender, EventArgs^ e)
	{
		DoLayout();
	}
private: 
	System::Void tvMembers_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
	{
		pnlMemberDisplay->Controls->Clear();
		
		if (e->Node)
		{
			Object^ tag = e->Node->Tag;

			if (tag)
			{
				Type^ type = tag->GetType();

				if (type == Member::typeid)
				{
					MemberDisplay^ member = gcnew MemberDisplay(
						(Member^)tag, m_ContentMgr);
					m_Display = member;
					pnlMemberDisplay->Controls->Add(member);
				}
				else if (type == ArrayMember::typeid)
				{
					ArrayMemberDisplay^ arr = gcnew ArrayMemberDisplay(
						(ArrayMember^)tag, m_ContentMgr);
					m_Display = arr;
					pnlMemberDisplay->Controls->Add(arr);
				}
				else if (type == 
						FableMod::ContentManagement::Control::typeid)
				{
					ControlDisplay^ cont = gcnew ControlDisplay(
						(FableMod::ContentManagement::Control^)tag, 
						m_ContentMgr);
					
					m_Display = cont;
					pnlMemberDisplay->Controls->Add(cont);
				}
				else if (type == DefinitionType::typeid)
				{
					pnlMemberDisplay->SuspendLayout();

					if (m_Definition->HasCDefListing)
					{
						ArrayMemberDisplay^ amd = gcnew ArrayMemberDisplay(
							m_Definition->CDefs, m_ContentMgr);
						
						amd->SizeChanged += gcnew EventHandler(
							this, &DefinitionTypeDisplay::MemberSizeChanged);
						
						pnlMemberDisplay->Controls->Add(amd);
					}
					for(int i=0;i<m_Definition->Controls->Count;i++)
					{
						ControlDisplay^ cd = gcnew ControlDisplay(
							m_Definition->Controls[i], m_ContentMgr);
						
						cd->SizeChanged += gcnew EventHandler(this, 
							&DefinitionTypeDisplay::MemberSizeChanged);
						
						pnlMemberDisplay->Controls->Add(cd);
					}

					pnlMemberDisplay->ResumeLayout();
					
					DoLayout();

					// For mouse wheel scroll.
					pnlMemberDisplay->Focus();
				}
			}
		}
	}
private: System::Void pnlMemberDisplay_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 pnlMemberDisplay->Focus();
		 }
};

}
}