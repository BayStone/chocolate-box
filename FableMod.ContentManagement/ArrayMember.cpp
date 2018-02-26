#include "Stdafx.h"
#include "Member.h"
#include "MemberCollection.h"
#include "ArrayMember.h"

using namespace System::Text;

namespace FableMod
{
namespace ContentManagement
{

ref class ElementSort : public System::Collections::IComparer
{
public:

	ElementSort(int element) : m_Element(element) {}

	virtual int Compare(Object^ x, Object^ y)
	{
		MemberCollection^ xc = (MemberCollection^)x;
		MemberCollection^ yc = (MemberCollection^)y;

		BaseMember^ xbm = xc[m_Element];
		BaseMember^ ybm = yc[m_Element];

		if (xbm->GetType() == Member::typeid)
		{
			Member^ xm = (Member^)xbm;
			Member^ ym = (Member^)ybm;

			if (xm->Type == MemberType::INT)
			{
				int xi = (int)xm->Value;
				int yi = (int)ym->Value;
				
				if (xi < yi)
					return -1;
				else if (xi == yi)
					return 0;
				if (xi > yi)
					return 1;
			}
			else if (xm->Type == MemberType::UINT)
			{
				UInt32 xi = (UInt32)xm->Value;
				UInt32 yi = (UInt32)ym->Value;
				
				if (xi < yi)
					return -1;
				else if (xi == yi)
					return 0;
				if (xi > yi)
					return 1;
			}
			else
				System::Diagnostics::Debug::Assert(false);
		}

		return -1;
	}

private:

	int m_Element;
};

ArrayMember::ArrayMember(String^ name, String^ comments, Member^ count) :
BaseMember(name, comments),
m_CountMember(count),
m_ElementMembers(gcnew MemberCollection()),
m_Elements(gcnew Collection<MemberCollection^>())
{
}

ArrayMember::ArrayMember(ArrayMember^ arr, MemberCollection^ mc) :
BaseMember(arr),
m_CountMember(mc->GetMemberByName(arr->ElementCount)),
m_ElementMembers(gcnew MemberCollection(arr->ElementMembers)),
m_Elements(gcnew Collection<MemberCollection^>())
{
}

ArrayMember::~ArrayMember()
{
	m_ElementMembers = nullptr;
	m_Elements = nullptr;
}

MemberCollection^ ArrayMember::CreateElement()
{
	return gcnew MemberCollection(m_ElementMembers);
}

void ArrayMember::FixLinks(
	LinkDestination link, 
	Object^ oldValue,
	Object^ newValue)
{
	for (int i = 0; i < Elements->Count; ++i)
	{
		for (int j = 0; j < Elements[i]->Count; ++j)
		{
			Elements[i][j]->FixLinks(link, oldValue, newValue);
		}
	}
}

void ArrayMember::Print(StringBuilder^ sb, String^ tab) 
{
	sb->AppendFormat("{0}Array: {1}[{2}]{3}",
		tab, Name, Elements->Count, Environment::NewLine);
	
	String^ newTab = tab+"    ";

	for (int i = 0; i < Elements->Count; ++i)
	{
		sb->AppendFormat("{0}  Element {1}{2}", 
			tab, i, Environment::NewLine);
		
		for (int j = 0; j < Elements[i]->Count; ++j)
		{
			sb->AppendFormat("{0}   Index {1}{2}", 
				tab, j, Environment::NewLine);
		
			Elements[i][j]->Print(sb, newTab);
		}
	}
}

String^ ArrayMember::ToString()
{
	StringBuilder^ sb = gcnew StringBuilder();

	sb->AppendFormat("Array: {0}[{1}]{2}",
		Name, Elements->Count, Environment::NewLine);
	
	for (int i = 0; i < Elements->Count; ++i)
	{
		sb->AppendFormat("    Element {0}{1}", i, Environment::NewLine);
		
		for (int j = 0; j < Elements[i]->Count; ++j)
		{
			sb->AppendFormat("        {0}{1}", 
				Elements[i][j]->ToString(), Environment::NewLine);
		}
	}

	return sb->ToString();
}

void ArrayMember::UpdateCount()
{
	m_CountMember->Value = m_Elements->Count.ToString();
}

int ArrayMember::ReadIn(char* data, int length)
{
	int offset = 0;
	
	unsigned int count = UInt32::Parse(m_CountMember->Value->ToString());
	
	m_Elements->Clear();
	
	for (int i = 0; i < count; ++i)
	{
		MemberCollection^ element = CreateElement();
		offset += element->ReadIn(data+offset, length-offset);

		m_Elements->Add(element);
	}

	return offset;
}

int ArrayMember::Write(char* data, int length)
{
	int offset = 0;

	for (int i = 0; i < m_Elements->Count; ++i)
	{
		offset += m_Elements[i]->Write(data+offset, length-offset);
	}
	
	return offset;
}

void ArrayMember::Sort(int sortElement)
{
	if (m_Elements->Count > 0)
	{
		array<MemberCollection^>^ ac = 
			gcnew array<MemberCollection^>(Elements->Count); 

		for (int i = 0; i < m_Elements->Count; ++i)
			ac[i] = m_Elements[i];

		Array::Sort(ac, gcnew ElementSort(sortElement));

		m_Elements->Clear();

		for (int i = 0; i < ac->Length; ++i)
			m_Elements->Add(ac[i]);
	}
}	

}
}