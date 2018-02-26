#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Member.h"
#include "ContentManager.h"

using namespace FableMod::BIG;
using namespace FableMod::BIN;

namespace FableMod
{
namespace ContentManagement
{

/// <summary>
/// Summary for MemberDisplay
/// </summary>
public ref class MemberDisplay : public System::Windows::Forms::UserControl
{
public:
	MemberDisplay(
		FableMod::ContentManagement::Member^ member, 
		ContentManager^ contentmgr);
	
	property FableMod::ContentManagement::Member^ Member
	{
		FableMod::ContentManagement::Member^ get() { return m_Member; }
	}
	
	void ApplyChanges();

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~MemberDisplay()
	{
		if (components)
		{
			delete components;
		}
	}
private: System::Windows::Forms::Label^  lblLinksTo;
private: System::Windows::Forms::TextBox^  txtMemberValue;

private:
	FableMod::ContentManagement::Member^		m_Member;
	ContentManager^		m_ContentMgr;
private: System::Windows::Forms::Button^  btnSelectEntry;
private: System::Windows::Forms::ToolTip^  ttComments;
private: System::Windows::Forms::Label^  labelName;

private: System::ComponentModel::IContainer^  components;
		 /// <summary>
	/// Required designer variable.
	/// </summary>


	void lblLinksTo_LinkClicked( Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e )
	{
		m_ContentMgr->ShowEntry(lblLinksTo->Tag, false);
	}

	void DoLinkLabel(LinkDestination dst, ContentObject^ o);

	void DoNoLinkLabel(LinkDestination dst);

	void DoLinksTo();

#pragma region Windows Form Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
		this->components = (gcnew System::ComponentModel::Container());
		this->txtMemberValue = (gcnew System::Windows::Forms::TextBox());
		this->lblLinksTo = (gcnew System::Windows::Forms::Label());
		this->btnSelectEntry = (gcnew System::Windows::Forms::Button());
		this->ttComments = (gcnew System::Windows::Forms::ToolTip(this->components));
		this->labelName = (gcnew System::Windows::Forms::Label());
		this->SuspendLayout();
		// 
		// txtMemberValue
		// 
		this->txtMemberValue->Location = System::Drawing::Point(131, 3);
		this->txtMemberValue->Name = L"txtMemberValue";
		this->txtMemberValue->Size = System::Drawing::Size(80, 20);
		this->txtMemberValue->TabIndex = 1;
		this->txtMemberValue->Validating += gcnew System::ComponentModel::CancelEventHandler(this, &MemberDisplay::txtMemberValue_Validating);
		// 
		// lblLinksTo
		// 
		this->lblLinksTo->AutoSize = true;
		this->lblLinksTo->Location = System::Drawing::Point(3, 29);
		this->lblLinksTo->Name = L"lblLinksTo";
		this->lblLinksTo->Size = System::Drawing::Size(137, 13);
		this->lblLinksTo->TabIndex = 2;
		this->lblLinksTo->Text = L"Links To: (Entry Not Found)";
		// 
		// btnSelectEntry
		// 
		this->btnSelectEntry->Location = System::Drawing::Point(217, 2);
		this->btnSelectEntry->Name = L"btnSelectEntry";
		this->btnSelectEntry->Size = System::Drawing::Size(25, 21);
		this->btnSelectEntry->TabIndex = 3;
		this->btnSelectEntry->Text = L"...";
		this->btnSelectEntry->UseVisualStyleBackColor = true;
		this->btnSelectEntry->Click += gcnew System::EventHandler(this, &MemberDisplay::btnSelectEntry_Click);
		// 
		// labelName
		// 
		this->labelName->AutoSize = true;
		this->labelName->Location = System::Drawing::Point(3, 6);
		this->labelName->MinimumSize = System::Drawing::Size(60, 0);
		this->labelName->Name = L"labelName";
		this->labelName->Size = System::Drawing::Size(97, 13);
		this->labelName->TabIndex = 4;
		this->labelName->Text = L"LongMemberName";
		// 
		// MemberDisplay
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->AutoSize = true;
		this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
		this->Controls->Add(this->labelName);
		this->Controls->Add(this->btnSelectEntry);
		this->Controls->Add(this->lblLinksTo);
		this->Controls->Add(this->txtMemberValue);
		this->Name = L"MemberDisplay";
		this->Size = System::Drawing::Size(245, 42);
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

private: 
	System::Void btnSelectEntry_Click(System::Object^  sender, System::EventArgs^  e);
	
private: 
	System::Void txtMemberValue_Validating(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) 
	{
		try 
		{
			m_Member->Value = txtMemberValue->Text;
			DoLinksTo();
		}
		catch (Exception^ exc)
		{
			MessageBox::Show(this, exc->Message, "Error Validating New Value");
		}
	}
};

}
}