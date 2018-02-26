#pragma once

using namespace System;
using namespace System::Collections::ObjectModel;

namespace FableMod
{
namespace CLRCore
{

public ref class ProgressInterface abstract
{
public:

	virtual ~ProgressInterface();

	void Initialize();

	void Begin(int steps);

	void Update();

	void End();

protected:

	ref class Item
	{
	public:

		float m_Start;
		float m_End;
		float m_Step;
	};

	virtual void SetValue(float value) = 0;

	ProgressInterface();

	Collection<Item^>^ m_Items;
	float m_Current;
};

}
}