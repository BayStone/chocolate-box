#include "StdAfx.h"
#include "MemberDisplay.h"
	
namespace FableMod
{
namespace ContentManagement
{

MemberDisplay::MemberDisplay(FableMod::ContentManagement::Member^ member, 
							 ContentManager^ contentmgr)
{
	InitializeComponent();
	
	m_ContentMgr = contentmgr;
	m_Member = member;
	
	labelName->Text = String::Format("{0} [{1}]",
		m_Member->Name, m_Member->Type.ToString());

	DoLinksTo();
	
	SuspendLayout();
	
	txtMemberValue->Left = labelName->Right+3;

	txtMemberValue->Text = m_Member->Value->ToString();
	
	if (m_Member->Type == MemberType::STRING)
		txtMemberValue->Width = 250;
	
	btnSelectEntry->Left = txtMemberValue->Right+3;
	
	if (!String::IsNullOrEmpty(m_Member->Comments))
	{
		ttComments->SetToolTip(labelName, m_Member->Comments);
	}

	ResumeLayout();
}

void MemberDisplay::ApplyChanges()
{
	m_Member->Value = txtMemberValue->Text;
}

void MemberDisplay::DoLinkLabel(LinkDestination dst, ContentObject^ o)
{
	if (Controls->Contains(lblLinksTo))
		Controls->Remove(lblLinksTo);

	if (!Controls->Contains(btnSelectEntry))
		Controls->Add(btnSelectEntry);

	LinkLabel^ lbllink = gcnew LinkLabel();
	lblLinksTo = lbllink;
	lblLinksTo->Tag = o->Object;
	lbllink->AutoSize = true;
	lbllink->Location = System::Drawing::Point(3, 29);
	lbllink->Name = L"lblLinksTo";
	lbllink->TabIndex = 2;
	lbllink->Text = 
		"Links to ("+
		dst.ToString()+
		"): "+
		o->Name;
		
	lbllink->LinkClicked += 
		gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(
			this, &MemberDisplay::lblLinksTo_LinkClicked);
	
	Controls->Add(lblLinksTo);
}

void MemberDisplay::DoNoLinkLabel(LinkDestination dst)
{
	if (Controls->Contains(lblLinksTo))
		Controls->Remove(lblLinksTo);

	if (!Controls->Contains(btnSelectEntry))
		Controls->Add(btnSelectEntry);
	
	lblLinksTo = gcnew Label();
	lblLinksTo->AutoSize = true;
	lblLinksTo->Location = System::Drawing::Point(3, 29);
	lblLinksTo->Name = L"lblLinksTo";
	lblLinksTo->TabIndex = 2;
	lblLinksTo->Text = 
		"Links to ("+
		m_Member->Link->To.ToString()+
		") : (Error Finding Entry)";
		
	Controls->Add(lblLinksTo);
}

void MemberDisplay::DoLinksTo()
{
	SuspendLayout();
	
	if (m_Member->Link)
	{
		if (m_ContentMgr)
		{
			ContentObject^ o = m_ContentMgr->FindEntry(
				m_Member->Link->To, m_Member->Value);

			if (o != nullptr)
			{
				DoLinkLabel(m_Member->Link->To, o);
			}
			else
				DoNoLinkLabel(m_Member->Link->To);
		
			ResumeLayout();

			return;
		}

		DoNoLinkLabel(m_Member->Link->To);
	}
	else
	{
		// No links.

		if (Controls->Contains(lblLinksTo))
			Controls->Remove(lblLinksTo);

		if (Controls->Contains(btnSelectEntry))
			Controls->Remove(btnSelectEntry);
	}
	
	ResumeLayout();
}

System::Void MemberDisplay::btnSelectEntry_Click(System::Object^  sender, System::EventArgs^  e) 
{
	Object^ ret = m_ContentMgr->SelectEntry(m_Member->Link, m_Member->Value);
	
	if (ret)
	{
		m_Member->Value = ret->ToString();
		
		DoLinksTo();

		txtMemberValue->Text = m_Member->Value->ToString();
	}

}

}
}