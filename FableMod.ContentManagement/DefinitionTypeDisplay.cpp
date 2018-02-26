#include "StdAfx.h"
#include "DefinitionTypeDisplay.h"

namespace FableMod
{
namespace ContentManagement
{

DefinitionTypeDisplay::DefinitionTypeDisplay(DefinitionType^ definition, ContentManager^ contentmgr)
{
	InitializeComponent();
	
	m_ContentMgr = contentmgr;
	m_Definition = definition;
	
	TreeNode^ defnode = gcnew TreeNode(definition->Name);
	
	defnode->Tag = definition;
	
	if (m_Definition->HasCDefListing)
	{
		TreeNode^ node = gcnew TreeNode(m_Definition->CDefs->Name);
		node->Tag = m_Definition->CDefs;
		defnode->Nodes->Add(node);
	}
	
	bool idiotMode = !DefinitionDB::DeveloperModeEnabled();

	for (int i = 0; i < definition->Controls->Count; ++i)
	{
		FableMod::ContentManagement::Control^ control = 
			definition->Controls[i];
		
		TreeNode^ node = gcnew TreeNode(control->ToString());
		
		node->Tag = control;
		
		for (int i = 0; i < control->Members->Count; ++i)
		{
			BaseMember^ member = control->Members[i];
			
			if (idiotMode && member->Name == "unknown")
				continue;
			
			TreeNode^ memberNode = gcnew TreeNode(member->Name);
			memberNode->Tag = member;
		
			node->Nodes->Add(memberNode);
		}
		
		if (node->Nodes->Count > 0)
			defnode->Nodes->Add(node);
	}
	
	tvMembers->Nodes->Add(defnode);
}

void DefinitionTypeDisplay::ApplyChanges()
{
	for (int i = 0; i < Controls->Count; ++i)
	{	
		if (Controls[i]->GetType() == ControlDisplay::typeid)
			((ControlDisplay^)Controls[i])->ApplyChanges();
		else if (Controls[i]->GetType() == ArrayMemberDisplay::typeid)
			((ArrayMemberDisplay^)Controls[i])->ApplyChanges();
	}
}
	
void DefinitionTypeDisplay::DoLayout()
{
	int nexty = 0;
	
	SuspendLayout();
	
	for (int i = 0; i < pnlMemberDisplay->Controls->Count; ++i)
	{
		if (pnlMemberDisplay->Controls[i]->GetType() == 
			ControlDisplay::typeid)
		{
			ControlDisplay^ cd = (ControlDisplay^)
				pnlMemberDisplay->Controls[i];
			
			cd->Location = Point(0, nexty);
			
			nexty += cd->Size.Height;
		}
		else if (pnlMemberDisplay->Controls[i]->GetType() == 
				ArrayMemberDisplay::typeid)
		{
			ArrayMemberDisplay^ amd = (ArrayMemberDisplay^)
				pnlMemberDisplay->Controls[i];
			
			amd->Location = Point(0, nexty);
			
			nexty += amd->Size.Height;
		}
	}
	
	ResumeLayout();
}
	
}
}