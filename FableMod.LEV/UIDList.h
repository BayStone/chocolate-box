#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace LEV
{

typedef unsigned __int64 UIDTYPE;

/// UID list is used for dynamic nodes.
/// @see DynamicNode
public ref class UIDList
{
public:

	UIDList();

	void Add(UIDTYPE uod);

	void Copy(UIDList^ list);

	void Merge(UIDList^ list);

	void Load(BufferStream^ fileIn);

	void Save(FileStream^ fileOut);

	property int Count { int get(); }

	property UIDTYPE Items[int] { unsigned __int64 get(int index); }

protected:

	static UIDTYPE UID_BASE = 18446741874686296064;

	List<UIDTYPE> m_UIDs;
};

}
}
