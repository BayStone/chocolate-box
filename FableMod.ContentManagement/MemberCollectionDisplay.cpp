#include "StdAfx.h"
#include "MemberCollectionDisplay.h"

namespace FableMod
{
namespace ContentManagement
{

MemberCollectionDisplay::MemberCollectionDisplay(MemberCollection^ mc, 
												 ContentManager^ contentmgr)
{
	InitializeComponent();
	
	m_ContentMgr = contentmgr;
	m_Members = mc;
	
	SuspendLayout();

	for (int i = 0; i < mc->Count; ++i)
	{
		if (mc[i]->GetType() == Member::typeid)
		{
			MemberDisplay^ mem = 
				gcnew MemberDisplay((Member^)mc[i], m_ContentMgr);
		
			Controls->Add(mem);
		}
		else if (mc[i]->GetType() == ArrayMember::typeid)
		{
			ArrayMemberDisplay^ memarr = 
				gcnew ArrayMemberDisplay((ArrayMember^)mc[i], m_ContentMgr);
			
			memarr->SizeChanged += gcnew EventHandler(
				this, &MemberCollectionDisplay::MemberSizeChanged);
			
			Controls->Add(memarr);
		}
	}

	ResumeLayout();
	
	EditableElement = false;
}

void MemberCollectionDisplay::ApplyChanges()
{
	for (int i = 0; i < Controls->Count; ++i)
	{	
		if (Controls[i]->GetType() == MemberDisplay::typeid)
			((MemberDisplay^)Controls[i])->ApplyChanges();
		else if (Controls[i]->GetType() == ArrayMemberDisplay::typeid)
			((ArrayMemberDisplay^)Controls[i])->ApplyChanges();
	}
}

void MemberCollectionDisplay::DoLayout()
{
	SuspendLayout();
	
	int nexty = 0;
	int x = 3;
	
	if (EditableElement)
		x += btnDelete->Location.X+btnDelete->Size.Width;

	for (int i = 0; i < Controls->Count; ++i)
	{
		Control^ c;

		if (Controls[i]->GetType() == ArrayMemberDisplay::typeid)
		{
			ArrayMemberDisplay^ amd = (ArrayMemberDisplay^)Controls[i];
			
			amd->DoLayout();
			
			c = amd;
		}
		else if(Controls[i]->GetType() == MemberDisplay::typeid)
		{
			c = Controls[i];
		}

		if (c)
		{
			c->Location = Point(x, nexty);
			
			nexty += c->Size.Height-1;
		}
	}

	ResumeLayout();
}

}
}