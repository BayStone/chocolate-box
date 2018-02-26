#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Control.h"
#include "MemberCollectionDisplay.h"

namespace FableMod
{
namespace ContentManagement
{

/// <summary>
/// Summary for ControlDisplay
/// </summary>
public ref class ControlDisplay : public System::Windows::Forms::UserControl
{
public:
	ControlDisplay(
		FableMod::ContentManagement::Control^ control, 
		ContentManager^ contentmgr);
	
	void ApplyChanges();

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~ControlDisplay()
	{
		if (components)
		{
			delete components;
		}
	}
private: System::Windows::Forms::Label^  lblName;
	ContentManager^ m_ContentMgr;
protected: 

protected: 

private:
	FableMod::ContentManagement::Control^	m_Control;
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
		this->lblName = (gcnew System::Windows::Forms::Label());
		this->SuspendLayout();
		// 
		// lblName
		// 
		this->lblName->AutoSize = true;
		this->lblName->Location = System::Drawing::Point(3, 3);
		this->lblName->Margin = System::Windows::Forms::Padding(3);
		this->lblName->MaximumSize = System::Drawing::Size(190, 13);
		this->lblName->Name = L"lblName";
		this->lblName->Size = System::Drawing::Size(188, 13);
		this->lblName->TabIndex = 0;
		this->lblName->Text = L"FFFFFFFF - ControlNameXXXXXXXXX";
		// 
		// ControlDisplay
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->AutoSize = true;
		this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
		this->Controls->Add(this->lblName);
		this->Name = L"ControlDisplay";
		this->Size = System::Drawing::Size(194, 19);
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion
};

}
}