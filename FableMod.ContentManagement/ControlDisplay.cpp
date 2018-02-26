#include "StdAfx.h"
#include "ControlDisplay.h"

namespace FableMod
{
namespace ContentManagement
{

ControlDisplay::ControlDisplay(FableMod::ContentManagement::Control^ control, 
							   ContentManager^ contentmgr)
{
	InitializeComponent();
	
	m_ContentMgr = contentmgr;
	m_Control = control;
	lblName->Text = control->ToString();

	MemberCollectionDisplay^ mcd = 
		gcnew MemberCollectionDisplay(m_Control->Members, m_ContentMgr);
	
	mcd->Location = Point(0, lblName->Location.Y + lblName->Size.Height + 3);
	
	Controls->Add(mcd);
}

void ControlDisplay::ApplyChanges()
{
	for (int i = 0; i < Controls->Count; ++i)
		if (Controls[i]->GetType() == MemberCollectionDisplay::typeid)
			((MemberCollectionDisplay^)Controls[i])->ApplyChanges();
}

}
}