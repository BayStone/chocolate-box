#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{
	[FlagsAttribute]
	public enum class Buttons : System::Int64
	{
		None = 0,
		
		Ctrl = (1 << 1),
		Alt = (1 << 2),
		Shift = (1 << 3),
		Left = (1 << 4),
		Right = (1 << 5),
		Up = (1 << 6),
		Down = (1 << 7),
		A = (1 << 8),
		W = (1 << 9),
		S = (1 << 10),
		D = (1 << 11),
		Tab = (1 << 12),
		Escape = (1 << 13),
		Delete = (1 << 14),
		Space = (1 << 15),
		Backspace = (1 << 16),
		Enter = (1 << 17),
		NumPad0 = (1 << 18),
		NumPad1 = (1 << 19),
		NumPad2 = (1 << 20),
		NumPad3 = (1 << 21),
		NumPad4 = (1 << 22),
		NumPad5 = (1 << 23),
		NumPad6 = (1 << 24),
		NumPad7 = (1 << 25),
		NumPad8 = (1 << 26),
		NumPad9 = (1 << 27),

		LButton = ((System::Int64)1 << 30),
		MButton = ((System::Int64)1 << 31),
		RButton = ((System::Int64)1 << 32),
		
		Double = ((System::Int64)1 << 33),
		
		DLButton = LButton | Double,
		DMButton = MButton | Double,
		DRButton = RButton | Double,
		
		NumPadKeys =
			(NumPad0 | NumPad1 | NumPad2 | NumPad3 |
			 NumPad4 | NumPad5 | NumPad6 | NumPad7 |
			 NumPad8 | NumPad9),
		NormalKeys = 
			(Left | Right | Up | Down | 
			A | W | S | D | 
			Enter |
			Tab | Escape | Space | Backspace |
			NumPadKeys),
		ModifierKeys = (Shift | Alt | Ctrl),
		Keys = (NormalKeys | ModifierKeys),
		Mouse = (LButton | MButton | RButton | Double)
	};

	[FlagsAttribute]
	public enum class ButtonRequirement : unsigned int
	{
		Key,
		Mouse,
		Modifier
	};

	private ref class CfgButton
	{
	public:

		CfgButton(String^ name, Buttons button) :
			m_Name(name),
			m_Button(button),
			m_Pressed(false),
			m_Enabled(true)
			{
			}

		property String^ Name
		{
			String^ get() { return m_Name; }
		}

		property Buttons Button
		{
			Buttons get() { return m_Button; }
			void set(Buttons value) { m_Button = value; }
		}

		property bool Pressed
		{
			bool get() { return m_Pressed; }
			void set(bool value) { m_Pressed = value; }
		}

		property bool Enabled
		{
			bool get() { return m_Enabled; }
			void set(bool value) { m_Enabled = value; }
		}

	private:

		String^ m_Name;
		Buttons m_Button;
		bool m_Pressed;
		bool m_Enabled;
	};

	public delegate System::Void ButtonEventHandler(
		String^ buttonName, MouseEventArgs^ e);

	public ref class ButtonConfiguration
	{
	public:

		ButtonConfiguration();

		void Load(String^ fileName);

		void Save(String^ fileName);

		void Add(String^ name, Buttons button);

		void Add(String^ name, String^ btnString);

		void Enable(String^ name, bool enable);

		void Attach(Control^ c);

		bool IsDown(String^ name);

		event ButtonEventHandler^ OnButtonDown;

		event ButtonEventHandler^ OnButtonUp;

		static Buttons ToButton(String^ btnString);

	private:

		bool ModifiersOk(Buttons button);
		bool MouseOk(Buttons button, MouseEventArgs^ mouse);
		bool ButtonOk(Buttons button, KeyEventArgs^ keys);
		
		System::Void Control_MouseDown(
			Object^ sender, MouseEventArgs^ e);
		System::Void Control_MouseUp(
			Object^ sender, MouseEventArgs^ e);
		System::Void Control_MouseClick(
			Object^ sender, MouseEventArgs^ e);
		
		System::Void Control_KeyDown(
			Object^ sender, KeyEventArgs^ e);
		System::Void Control_KeyUp(
			Object^ sender, KeyEventArgs^ e);

		List<CfgButton^>^ m_Buttons;
		
		static Dictionary<Buttons, Keys>^ m_Keys;
		static Dictionary<Buttons, int>^ m_VKeys;
	};
}
}
}