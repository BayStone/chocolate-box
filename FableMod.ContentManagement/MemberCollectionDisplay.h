#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "MemberCollection.h"
#include "MemberDisplay.h"
#include "ArrayMemberDisplay.h"

namespace FableMod
{
namespace ContentManagement
{

ref class ArrayMemberDisplay;
ref class MemberCollectionDisplay;

public delegate void RemoveElementEventHandler(MemberCollectionDisplay^ sender);
public delegate void MoveElementEventHandler(MemberCollectionDisplay^ sender);

/// <summary>
/// Summary for MemberCollectionDisplay
/// </summary>
public ref class MemberCollectionDisplay : public System::Windows::Forms::UserControl
{
public:
	MemberCollectionDisplay(MemberCollection^ mc, ContentManager^ contentmgr);

	event RemoveElementEventHandler^ RemoveElement;
	event MoveElementEventHandler^ ElementUp;
	event MoveElementEventHandler^ ElementDown;

	property bool EditableElement
	{
		bool get() 
		{ 
			return btnDelete->Visible; 
		}

		void set(bool value) 
		{ 
			if (btnDelete->Visible != value)
			{
				btnDelete->Visible = value; 
				buttonUp->Visible = value; 
				buttonDown->Visible = value;
				DoLayout();
			}
		}
	}
	
	property MemberCollection^ Members
	{
		MemberCollection^ get() { return m_Members; }
	}

	void ApplyChanges();
	void DoLayout();

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~MemberCollectionDisplay()
	{
		if (components)
		{
			delete components;
		}
	}

private:
	//EventHandler^		m_Delete;
	MemberCollection^	m_Members;
	ContentManager^ m_ContentMgr;
private: System::Windows::Forms::Button^  btnDelete;
private: System::Windows::Forms::Button^  buttonUp;
private: System::Windows::Forms::Button^  buttonDown;


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
		this->btnDelete = (gcnew System::Windows::Forms::Button());
		this->buttonUp = (gcnew System::Windows::Forms::Button());
		this->buttonDown = (gcnew System::Windows::Forms::Button());
		this->SuspendLayout();
		// 
		// btnDelete
		// 
		this->btnDelete->Location = System::Drawing::Point(3, 3);
		this->btnDelete->Name = L"btnDelete";
		this->btnDelete->Size = System::Drawing::Size(44, 23);
		this->btnDelete->TabIndex = 0;
		this->btnDelete->Text = L"Del";
		this->btnDelete->UseVisualStyleBackColor = true;
		this->btnDelete->Click += gcnew System::EventHandler(this, &MemberCollectionDisplay::btnDelete_Click);
		// 
		// buttonUp
		// 
		this->buttonUp->Location = System::Drawing::Point(3, 32);
		this->buttonUp->Name = L"buttonUp";
		this->buttonUp->Size = System::Drawing::Size(44, 23);
		this->buttonUp->TabIndex = 1;
		this->buttonUp->Text = L"Up";
		this->buttonUp->UseVisualStyleBackColor = true;
		this->buttonUp->Click += gcnew System::EventHandler(this, &MemberCollectionDisplay::buttonUp_Click);
		// 
		// buttonDown
		// 
		this->buttonDown->Location = System::Drawing::Point(3, 61);
		this->buttonDown->Name = L"buttonDown";
		this->buttonDown->Size = System::Drawing::Size(44, 23);
		this->buttonDown->TabIndex = 2;
		this->buttonDown->Text = L"Down";
		this->buttonDown->UseVisualStyleBackColor = true;
		this->buttonDown->Click += gcnew System::EventHandler(this, &MemberCollectionDisplay::buttonDown_Click);
		// 
		// MemberCollectionDisplay
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->AutoSize = true;
		this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
		this->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->Controls->Add(this->buttonDown);
		this->Controls->Add(this->buttonUp);
		this->Controls->Add(this->btnDelete);
		this->Name = L"MemberCollectionDisplay";
		this->Size = System::Drawing::Size(50, 87);
		this->ResumeLayout(false);

	}
#pragma endregion
private: 
	System::Void btnDelete_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		RemoveElement(this);
	}
	void MemberSizeChanged(System::Object^  sender, System::EventArgs^  e)
	{
		DoLayout();
	}
private: System::Void buttonUp_Click(System::Object^  sender, System::EventArgs^  e) {
			ElementUp(this);
		 }
private: System::Void buttonDown_Click(System::Object^  sender, System::EventArgs^  e) {
			ElementDown(this);
		 }
};

}
}