#include "Stdafx.h"
#include "ProgressInterface.h"

namespace FableMod
{
namespace CLRCore
{

ProgressInterface::ProgressInterface() :
m_Items(gcnew Collection<Item^>())
{
}

ProgressInterface::~ProgressInterface()
{
	for (int i = 0; i < m_Items->Count; ++i)
		delete m_Items[i];
	
	m_Items->Clear();

	delete m_Items;
}

void ProgressInterface::Initialize()
{
	for (int i = 0; i < m_Items->Count; ++i)
		delete m_Items[i];
	
	m_Items->Clear();

	m_Current = 0.0f;

	SetValue(m_Current);
}

void ProgressInterface::Begin(int steps)
{
	//Console::WriteLine("ProgressInterface::Begin({0})", steps);

	if (m_Items->Count == 0)
	{
		Item^ item = gcnew Item();

		item->m_Start = 0.0f;
		item->m_End = 1.0f;
		item->m_Step = 1.0f/(float)steps;
	
		//Console::WriteLine("step: {0}", item->m_Step);

		m_Items->Add(item);
	}
	else
	{
		float step = m_Items[m_Items->Count-1]->m_Step;

		Item^ item = gcnew Item();

		item->m_Start = m_Current;
		item->m_End = m_Current+step;
		item->m_Step = step/(float)steps;

		m_Items->Add(item);
	
		//Console::WriteLine("step2: {0}", item->m_Step.ToString("N5"));
	}
}

void ProgressInterface::End()
{
	//Console::WriteLine("ProgressInterface::End()");

	if (m_Items->Count > 0)
	{
		m_Current = m_Items[m_Items->Count-1]->m_End;
		delete m_Items[m_Items->Count-1];
		m_Items->RemoveAt(m_Items->Count-1);
	}
	else
	{
		m_Current = 1.0f;
	}

	SetValue(m_Current);
}

void ProgressInterface::Update()
{
	m_Current += m_Items[m_Items->Count-1]->m_Step;
	
	//Console::WriteLine("upd: {0}", m_Current.ToString("N5"));

	if (m_Current > m_Items[m_Items->Count-1]->m_End)
		m_Current = m_Items[m_Items->Count-1]->m_End;
	
	/*
	Begin(1);
	End();
	*/
	SetValue(m_Current);
}

}
}