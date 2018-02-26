#include "stdafx.h"
#include "Member.h"
#include "ArrayMemberDisplay.h"

namespace FableMod
{
namespace ContentManagement
{

ArrayMemberDisplay::ArrayMemberDisplay(ArrayMember^ arr, 
									   ContentManager^ contentmgr)
{
	InitializeComponent();
	
	m_ContentMgr = contentmgr;
	m_Array = arr;

	BuildLayout();
}

void ArrayMemberDisplay::BuildLayout()
{
	SuspendLayout();
	
	for (int i = 0; i < m_Array->Elements->Count; ++i)
	{
		MemberCollectionDisplay^ mcd = 
			gcnew MemberCollectionDisplay(m_Array->Elements[i], m_ContentMgr);
		
		mcd->RemoveElement += 
			gcnew RemoveElementEventHandler(this, 
				&ArrayMemberDisplay::OnRemoveElement);
		
		mcd->ElementUp += 
			gcnew MoveElementEventHandler(this, 
				&ArrayMemberDisplay::OnElementUp);
		
		mcd->ElementDown += 
			gcnew MoveElementEventHandler(this, 
				&ArrayMemberDisplay::OnElementDown);
		
		mcd->EditableElement = true;
		
		mcd->SizeChanged += gcnew EventHandler(
			this, &ArrayMemberDisplay::MemberSizeChanged);
		
		Controls->Add(mcd);	
	}
	
	if (m_Array->Comments)
	{
		if (m_Array->Comments != "")
		{
			ttComments->SetToolTip(
				lblName, 
				"Array "+m_Array->Name+" - "+m_Array->Comments);
		}
	}

	ResumeLayout();
	DoLayout();
}

System::Void ArrayMemberDisplay::btnAddNewElement_Click(System::Object^  sender, System::EventArgs^  e)
{
	MemberCollection^ element = m_Array->CreateElement();

	MemberCollectionDisplay^ mcd = 
		gcnew MemberCollectionDisplay(element, m_ContentMgr);
	
	mcd->EditableElement = true;
	
	mcd->RemoveElement += gcnew RemoveElementEventHandler(
		this, &ArrayMemberDisplay::OnRemoveElement);
	
	mcd->ElementUp += 
		gcnew MoveElementEventHandler(this, 
			&ArrayMemberDisplay::OnElementUp);
		
	mcd->ElementDown += 
		gcnew MoveElementEventHandler(this, 
			&ArrayMemberDisplay::OnElementDown);

	m_Array->Elements->Add(element);
	m_Array->UpdateCount();

	Controls->Add(mcd);
	
	DoLayout();
}

void ArrayMemberDisplay::DoLayout()
{
	lblName->Text = 
		m_Array->Name+"["+m_Array->Elements->Count.ToString()+"]";
	
	btnAddNewElement->Left = lblName->Right+3; 
	panelSort->Left = btnAddNewElement->Right+3;

	SuspendLayout();
	
	int nexty = 
		btnAddNewElement->Location.Y+btnAddNewElement->Size.Height+3;
	
	int width = 0;

	for (int i = 0; i < Controls->Count; ++i)
	{
		if (Controls[i]->GetType() == MemberCollectionDisplay::typeid)
		{
			MemberCollectionDisplay^ mcd = 
				(MemberCollectionDisplay^)Controls[i];
			
			mcd->Location = Point(0,nexty);

			if (mcd->Width > width)
				width = mcd->Width;

			nexty += mcd->Size.Height-1;
		}
	}

	for (int i = 0; i < Controls->Count; ++i)
	{
		if (Controls[i]->GetType() == MemberCollectionDisplay::typeid)
		{
			MemberCollectionDisplay^ mcd = 
				(MemberCollectionDisplay^)Controls[i];
			
			mcd->Width = width;
		}
	}

	ResumeLayout();
}

void ArrayMemberDisplay::ApplyChanges()
{
	for (int i = 0; i < Controls->Count; ++i)
	{
		if (Controls[i]->GetType() == MemberCollection::typeid)
		{
			MemberCollectionDisplay^ mcd = 
				(MemberCollectionDisplay^)Controls[i];
			
			mcd->ApplyChanges();
		}
	}
}

void ArrayMemberDisplay::OnRemoveElement(MemberCollectionDisplay^ element)
{
	m_Array->Elements->Remove(element->Members);
	m_Array->UpdateCount();

	Controls->Remove(element);
	DoLayout();
}

void ArrayMemberDisplay::OnElementUp(MemberCollectionDisplay^ element)
{
	int index = m_Array->Elements->IndexOf(element->Members);
	
	if (index > 0)
	{
		MemberCollection^ mc = m_Array->Elements[index-1];
		m_Array->Elements[index-1] = m_Array->Elements[index];		
		m_Array->Elements[index] = mc;
		
		int i = Controls->GetChildIndex(element);		
		Controls->SetChildIndex(element, i-1);
		i = Controls->GetChildIndex(element);
	}
	
	DoLayout();
}

void ArrayMemberDisplay::OnElementDown(MemberCollectionDisplay^ element)
{
	int index = m_Array->Elements->IndexOf(element->Members);
	
	if (index < m_Array->Elements->Count-1)
	{
		MemberCollection^ mc = m_Array->Elements[index+1];
		m_Array->Elements[index+1] = m_Array->Elements[index];		
		m_Array->Elements[index] = mc;
		
		int i = Controls->GetChildIndex(element);		
		Controls->SetChildIndex(element, i+1);
		i = Controls->GetChildIndex(element);
	}
	
	DoLayout();
}

void ArrayMemberDisplay::buttonSort_Click(System::Object^  sender, 
										  System::EventArgs^  e)
{
	int first = 0;

	for (int i = 0; i < Controls->Count; ++i)
	{
		if (Controls[i]->GetType() == MemberCollectionDisplay::typeid)
		{
			first = i;
			break;
		}
	}

	m_Array->Sort(int::Parse(textBoxElement->Text));
	
	SuspendLayout();

	int items = Controls->Count-first;

	for (int i = 0; i < items; ++i)
		Controls->RemoveAt(first);

	ResumeLayout();

	BuildLayout();
}

void ArrayMemberDisplay::MemberSizeChanged(Object^ sender, EventArgs^ e)
{
	DoLayout();
}

}
}