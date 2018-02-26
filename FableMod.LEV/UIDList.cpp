#include "Stdafx.h"
#include "UIDList.h"

using namespace FableMod::CLRCore;

namespace FableMod
{
namespace LEV
{

UIDList::UIDList()
{
}

void UIDList::Load(BufferStream^ fileIn)
{
	unsigned __int32 NumUIDs = 0;
	
	fileIn->Read(&NumUIDs,sizeof(NumUIDs));

	m_UIDs.Clear();

	for (unsigned __int32 i = 0; i < NumUIDs; ++i)
	{
		UIDTYPE uid = 0;
		
		fileIn->Read(&uid,sizeof(uid));

		Add(uid+UID_BASE);
	}
}

void UIDList::Save(FileStream^ fileOut)
{
	unsigned __int32 uiNumUIDs = Count;
	
	FileControl::Write(fileOut,&uiNumUIDs,sizeof(uiNumUIDs));

	for (int i = 0; i < Count; ++i)
	{
		UIDTYPE uid = Items[i]-UID_BASE;
		
		FileControl::Write(fileOut,&uid,sizeof(uid));
	}
}

void UIDList::Add(UIDTYPE uid)
{
	if (m_UIDs.IndexOf(uid) >= 0)
		return;

	m_UIDs.Add(uid);
}

void UIDList::Copy(UIDList^ list)
{
	m_UIDs.Clear();
	
	Merge(list);
}

void UIDList::Merge(UIDList^ list)
{
	for (int i = 0; i < list->Count; ++i)
	{
		Add(list->Items[i]);
	}
}

int UIDList::Count::get()
{ 
	return m_UIDs.Count;
}

UIDTYPE UIDList::Items::get(int index)
{
	return m_UIDs[index];
}

}
}