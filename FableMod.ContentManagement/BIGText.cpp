#include "Stdafx.h"
#include "BIGText.h"
#include <string.h>

using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace ContentManagement
{

	BIGText::BIGText()
{
	m_Identifier = new char[1];
	m_Identifier[0] = 0;

	m_Content = new wchar_t[1];
	m_Content[0] = 0;
}

BIGText::BIGText(AssetEntry^ ent)
{
	int offset = 0;

	if (ent->Length == 0)
		throw gcnew Exception("invalid entry");

	unsigned __int32 len = wcslen((wchar_t*)ent->GetData());
	m_Content = new wchar_t[len+1];
	wcscpy(m_Content, (wchar_t*)ent->GetData());
	
	len++;
	offset += len*sizeof(wchar_t);

	memcpy(&len, ent->GetData()+offset, 4);
	offset += 4;
	m_SoundFile = new char[len+1];
	memcpy(m_SoundFile, ent->GetData()+offset, len);
	offset += len;
	m_SoundFile[len] = 0;

	memcpy(&len, ent->GetData()+offset, 4);
	offset += 4;
	m_Speaker = new char[len+1];
	memcpy(m_Speaker, ent->GetData()+offset, len);
	offset += len;
	m_Speaker[len] = 0;

	memcpy(&len, ent->GetData()+offset, 4);
	offset += 4;
	m_Identifier = new char[len+1];
	memcpy(m_Identifier, ent->GetData()+offset, len);
	offset += len;
	m_Identifier[len] = 0;

	memcpy(&len, ent->GetData()+offset, 4);
	offset += 4;
	m_ModCount = len;

	m_Modifiers = new char*[m_ModCount];
	
	for (int i = 0; i < m_ModCount; ++i)
	{
		//skip four bytes, unknown data
		offset += 4;
		len = strlen(ent->GetData()+offset);
		len++;
		m_Modifiers[i] = new char[len];
		memcpy(m_Modifiers[i], ent->GetData()+offset, len);
		offset += len;
	}

}

BIGText::~BIGText()
{
	delete[] m_Content;
	delete[] m_SoundFile;
	delete[] m_Speaker;
	delete[] m_Identifier;

	for (int i = 0; i < m_ModCount; ++i)
		delete[] m_Modifiers[i];

	delete[] m_Modifiers;
}

String^ BIGText::Content::get()
{
	return Marshal::PtrToStringUni((IntPtr)m_Content)->Replace("\n", Environment::NewLine);
}

String^ BIGText::SoundFile::get()
{
	return Marshal::PtrToStringAnsi((IntPtr)m_SoundFile);
}

void BIGText::SoundFile::set(String^ file)
{
	IntPtr pfile = Marshal::StringToHGlobalAnsi(file);
	
	delete[] m_SoundFile;
	
	m_SoundFile = new char[file->Length + 1];
	
	strcpy(m_SoundFile, (char*)pfile.ToPointer());

	Marshal::FreeHGlobal(pfile);
}

String^ BIGText::Speaker::get()
{
	return Marshal::PtrToStringAnsi((IntPtr)m_Speaker);
}

void BIGText::Speaker::set(String^ speaker)
{
	IntPtr pspeaker = Marshal::StringToHGlobalAnsi(speaker);
	
	delete[] m_Speaker;
	
	m_Speaker = new char[speaker->Length + 1];
	
	strcpy(m_Speaker, (char*)pspeaker.ToPointer());

	Marshal::FreeHGlobal(pspeaker);
}

String^ BIGText::Identifier::get()
{
	return Marshal::PtrToStringAnsi((IntPtr)m_Identifier);
}

void BIGText::Identifier::set(String^ ident)
{
	IntPtr pident = Marshal::StringToHGlobalAnsi(ident);
	
	delete[] m_Identifier;
	
	m_Identifier = new char[ident->Length + 1];
	
	strcpy(m_Identifier, (char*)pident.ToPointer());

	Marshal::FreeHGlobal(pident);
}

array<String^>^ BIGText::Modifiers::get()
{
	array<String^>^ ret = gcnew array<String^>(m_ModCount);
	
	for(int i = 0; i < m_ModCount; ++i)
	{
		ret[i] = Marshal::PtrToStringAnsi((IntPtr)m_Modifiers[i]);
	}
	
	return ret;
}

void BIGText::Modifiers::set(array<String^>^ mod)
{
	for (int i = 0; i < m_ModCount; ++i)
		delete[] m_Modifiers[i];
	
	delete[] m_Modifiers;

	m_ModCount = mod->Length;
	m_Modifiers = new char*[m_ModCount];
	
	for (int i = 0; i < mod->Length; ++i)
	{
		IntPtr pmod = Marshal::StringToHGlobalAnsi(mod[i]);

		m_Modifiers[i] = new char[mod[i]->Length + 1];
		strcpy(m_Modifiers[i], (char*)pmod.ToPointer());

		Marshal::FreeHGlobal(pmod);
	}
}

void BIGText::Content::set(String^ cont)
{
	IntPtr pcont = Marshal::StringToHGlobalUni(cont->Replace("\r", ""));
	
	delete[] m_Content;
	
	m_Content = new wchar_t[cont->Length + 1];
	
	wcscpy(m_Content, (wchar_t*)pcont.ToPointer());

	Marshal::FreeHGlobal(pcont);
}

void BIGText::ApplyToEntry(AssetEntry^ entry)
{
	int clen = (wcslen(m_Content)+1)*sizeof(wchar_t);
	
	unsigned __int32 flen = strlen(m_SoundFile);
	unsigned __int32 slen = strlen(m_Speaker);
	unsigned __int32 ilen = strlen(m_Identifier);
	unsigned __int32 mlen = 4; // initial length just for modifier count
	
	for (int i = 0; i < m_ModCount; ++i)
	{
		mlen += strlen(m_Modifiers[i]) + 5;
	}

	char* dat = new char[clen + flen + slen + ilen + mlen + 12]; // 12 for length fields
	int offset = 0;

	memcpy(dat+offset, m_Content, clen);
	offset += clen;

	memcpy(dat+offset, &flen, sizeof(flen));
	offset += sizeof(flen);
	memcpy(dat+offset, m_SoundFile, flen);
	offset += flen;

	memcpy(dat+offset, &slen, sizeof(slen));
	offset += sizeof(slen);
	memcpy(dat+offset, m_Speaker, slen);
	offset += slen;

	memcpy(dat+offset, &ilen, sizeof(ilen));
	offset += sizeof(ilen);
	memcpy(dat+offset, m_Identifier, ilen);
	offset += ilen;

	unsigned __int32 localcount = m_ModCount;
	memcpy(dat+offset, &localcount, sizeof(localcount));
	offset += sizeof(localcount);

	for(int i = 0; i < m_ModCount; ++i)
	{
		memcpy(dat+offset, &i, sizeof(i));
		offset += sizeof(i);
		int len = strlen(m_Modifiers[i]);
		len++;
		memcpy(dat+offset, m_Modifiers[i], len);
		offset += len;
	}

	entry->SetData(dat, clen + flen + slen + ilen + mlen + 12);
	
	delete[] dat;
}

}
}