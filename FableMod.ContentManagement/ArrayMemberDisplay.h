#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "ArrayMember.h"
#include "MemberCollectionDisplay.h"

namespace FableMod
{
namespace ContentManagement
{

ref class MemberCollectionDisplay;

/// <summary>
/// Summary for ArrayMemberDisplay
/// </summary>
public ref class ArrayMemberDisplay : public System::Windows::Forms::UserControl
{
public:
	ArrayMemberDisplay(ArrayMember^ arr, ContentManager^ contentmgr);
	void ApplyChanges();
	void DoLayout();
	
	property ArrayMember^ Array 
	{
		ArrayMember^ get() { return m_Array; }
	}

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ArrayMemberDisplay()
	{
		if (components)
		{
			delete components;
		}
	}
private: System::Windows::Forms::Label^  lblName;
protected: 

private:
	ArrayMember^	m_Array;
	ContentManager^ m_ContentMgr;
private: System::Windows::Forms::Button^  btnAddNewElement;
private: System::Windows::Forms::ToolTip^  ttComments;
private: System::Windows::Forms::Panel^  panelSort;
private: System::Windows::Forms::TextBox^  textBoxElement;
private: System::Windows::Forms::Button^  buttonSort;
private: System::ComponentModel::IContainer^  components;
		 /// <summary>
	/// Required designer variable.
	/// </summary>

	void BuildLayout();

	void OnRemoveElement(MemberCollectionDisplay^ element);
	void OnElementUp(MemberCollectionDisplay^ element);
	void OnElementDown(MemberCollectionDisplay^ element);

#pragma region Windows Form Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
		this->components = (gcnew System::ComponentModel::Container());
		this->lblName = (gcnew System::Windows::Forms::Label());
		this->btnAddNewElement = (gcnew System::Windows::Forms::Button());
		this->ttComments = (gcnew System::Windows::Forms::ToolTip(this->components));
		this->panelSort = (gcnew System::Windows::Forms::Panel());
		this->buttonSort = (gcnew System::Windows::Forms::Button());
		this->textBoxElement = (gcnew System::Windows::Forms::TextBox());
		this->panelSort->SuspendLayout();
		this->SuspendLayout();
		// 
		// lblName
		// 
		this->lblName->AutoSize = true;
		this->lblName->Location = System::Drawing::Point(3, 8);
		this->lblName->Margin = System::Windows::Forms::Padding(3);
		this->lblName->MaximumSize = System::Drawing::Size(0, 13);
		this->lblName->Name = L"lblName";
		this->lblName->Size = System::Drawing::Size(97, 13);
		this->lblName->TabIndex = 0;
		this->lblName->Text = L"ArrayMemberName";
		// 
		// btnAddNewElement
		// 
		this->btnAddNewElement->Location = System::Drawing::Point(157, 3);
		this->btnAddNewElement->Name = L"btnAddNewElement";
		this->btnAddNewElement->Size = System::Drawing::Size(41, 23);
		this->btnAddNewElement->TabIndex = 1;
		this->btnAddNewElement->Text = L"Add Element";
		this->btnAddNewElement->UseVisualStyleBackColor = true;
		this->btnAddNewElement->Click += gcnew System::EventHandler(this, &ArrayMemberDisplay::btnAddNewElement_Click);
		// 
		// panelSort
		// 
		this->panelSort->Controls->Add(this->textBoxElement);
		this->panelSort->Controls->Add(this->buttonSort);
		this->panelSort->Location = System::Drawing::Point(204, 0);
		this->panelSort->Name = L"panelSort";
		this->panelSort->Size = System::Drawing::Size(113, 29);
		this->panelSort->TabIndex = 2;
		// 
		// buttonSort
		// 
		this->buttonSort->Location = System::Drawing::Point(3, 3);
		this->buttonSort->Name = L"buttonSort";
		this->buttonSort->Size = System::Drawing::Size(54, 23);
		this->buttonSort->TabIndex = 0;
		this->buttonSort->Text = L"Sort";
		this->buttonSort->UseVisualStyleBackColor = true;
		this->buttonSort->Click += gcnew System::EventHandler(this, &ArrayMemberDisplay::buttonSort_Click);
		// 
		// textBoxElement
		// 
		this->textBoxElement->Location = System::Drawing::Point(63, 5);
		this->textBoxElement->Name = L"textBoxElement";
		this->textBoxElement->Size = System::Drawing::Size(44, 20);
		this->textBoxElement->TabIndex = 1;
		this->textBoxElement->Text = L"0";
		// 
		// ArrayMemberDisplay
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->AutoSize = true;
		this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
		this->Controls->Add(this->panelSort);
		this->Controls->Add(this->btnAddNewElement);
		this->Controls->Add(this->lblName);
		this->Name = L"ArrayMemberDisplay";
		this->Size = System::Drawing::Size(320, 32);
		this->panelSort->ResumeLayout(false);
		this->panelSort->PerformLayout();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion
private: 
	System::Void btnAddNewElement_Click(System::Object^  sender, System::EventArgs^  e) ;
	void MemberSizeChanged(Object^ sender, EventArgs^ e);
	void buttonSort_Click(System::Object^  sender, System::EventArgs^  e);
};

}
}