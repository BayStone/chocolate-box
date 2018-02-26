#include "Stdafx.h"
#include "UIDManager.h"

using namespace System::Text;
using namespace System::IO;

namespace FableMod
{
namespace TNG
{

UIDManager::UIDManager()
{
}

void UIDManager::Load(String^ directory)
{
}

void UIDManager::LoadFromFile(String^ fileName)
{
	FileStream^ file = File::Open(
		fileName, 
		FileMode::Open, 
		FileAccess::ReadWrite,
		FileShare::Read);

	TextReader^ reader = gcnew StreamReader(file);

	String^ line = nullptr;

	m_UIDs->Clear();

	while ((line = reader->ReadLine()) != nullptr)
	{
		m_UIDs->Add(line);
	}

	// The file is always sorted.
	m_BinarySearch = true;

	reader->Close();
	file->Close();

	reader = nullptr;
	file = nullptr;
}

void UIDManager::WriteToFile(String^ fileName)
{
	if (IsEmpty())
		return;

	FileStream^ file = File::Create(fileName);

	TextWriter^ writer = gcnew StreamWriter(file);

	m_UIDs->Sort();
	m_BinarySearch = true;

	for (int i = 0; i < m_UIDs->Count; ++i)
	{
		writer->WriteLine(m_UIDs[i]);
	}

	writer->Close();
	file->Close();

	writer = nullptr;
	file = nullptr;
}

void UIDManager::Clear()
{
	m_UIDs->Clear();
}

void UIDManager::Add(String^ uid)
{
	if (IsNormal(uid) && !Exists(uid))
	{
		//Console::WriteLine("UIDManager::Add({0})", uid);
		m_UIDs->Add(uid);
		m_BinarySearch = false;
	}
}

void UIDManager::Remove(String^ uid)
{
	m_UIDs->Remove(uid);
	m_BinarySearch = false;
}

bool UIDManager::Exists(String^ uid)
{
	if (m_BinarySearch)
		return m_UIDs->BinarySearch(uid) >= 0;
	else
		return m_UIDs->IndexOf(uid) >= 0;
}

bool UIDManager::IsNormal(String^ uid)
{
	return 
		uid && 
		uid->Length >= DEFAULT_LENGTH &&
		uid->StartsWith(BASE);
}

String^ UIDManager::Generate(int length, String^ start)
{
	int len = 0;
	
	if (start)
		len = start->Length;

	if (length > len)
		len = length-len;
	else
		return start;

	StringBuilder^ sb = gcnew StringBuilder(start,len);

	for (int i = 0; i < len; ++i)
	{
		sb->Append(m_Rnd->Next(1,10));
	}

	return sb->ToString();
}

String^ UIDManager::Generate()
{
	String^ temp = nullptr;

	int i = 0;

	do
	{
		temp = Generate(DEFAULT_LENGTH+5, BASE);

		if (i++ == MAX_ITERATIONS)
		{
			throw gcnew System::InvalidOperationException(
				"FableMod::TNG::UIDManager: too many iterations");
		}
	}
	while (Exists(temp));

	Add(temp);
	
	return temp;
}

bool UIDManager::IsEmpty()
{
	return m_UIDs->Count == 0;
}

}
}