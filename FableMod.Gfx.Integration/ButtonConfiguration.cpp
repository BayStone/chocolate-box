#include "stdafx.h"
#include <windows.h>
#include "ButtonConfiguration.h"

using namespace System::Xml;
using namespace System::Reflection;
using namespace System::Text::RegularExpressions;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

/// Need this to properly order the buttons by priority.
ref class ButtonComparer : public IComparer<CfgButton^>
{
public:

	int GetModifiers(Buttons button)
	{
		int iCount = 0;

		if ((button & Buttons::Ctrl) == Buttons::Ctrl)
			++iCount;
		
		if ((button & Buttons::Alt) == Buttons::Alt)
			++iCount;
		
		if ((button & Buttons::Shift) == Buttons::Shift)
			++iCount;
		
		return iCount;
	}

	virtual int Compare(CfgButton^ a, CfgButton^ b) 
	{
		Buttons xb = a->Button;
		Buttons yb = b->Button;

		if (xb == yb)
			return 0;

		bool xbm = (xb & Buttons::Mouse) != Buttons::None;
		bool ybm = (yb & Buttons::Mouse) != Buttons::None;

		if (xbm && ybm)
		{
			bool xdbl = (xb & Buttons::Double) == Buttons::Double;
			bool ydbl = (yb & Buttons::Double) == Buttons::Double;

			if (xdbl && !ydbl)
				return -1;
			else if (ydbl && !xdbl)
				return 1;
			
			bool xkey = (xb & Buttons::Keys) != Buttons::None;
			bool ykey = (yb & Buttons::Keys) != Buttons::None;

			if (xkey && !ykey)
				return -1;
			else if (ykey && !xkey)
				return 1;
			
			int xm = GetModifiers(xb);
			int ym = GetModifiers(yb);

			if (xm > ym)
				return -1;
			else if (ym > xm)
				return 1;
		}
		else if (xbm && !ybm)
		{
			return -1;
		}
		else if (!xbm && ybm)
		{
			return 1;
		}
		else
		{
			int xm = GetModifiers(xb);
			int ym = GetModifiers(yb);

			if (xm > ym)
				return -1;
			else if (ym > xm)
				return 1;
		}

		return 0;
	}
};

ButtonConfiguration::ButtonConfiguration()
{
	m_Buttons = gcnew List<CfgButton^>();

	if (!m_VKeys)
	{
		m_VKeys = gcnew Dictionary<Buttons, int>();

		m_VKeys[Buttons::Left] = VK_LEFT;
		m_VKeys[Buttons::Up] = VK_UP;
		m_VKeys[Buttons::Right] = VK_RIGHT;
		m_VKeys[Buttons::Down] = VK_DOWN;
		m_VKeys[Buttons::A] = 'A';
		m_VKeys[Buttons::W] = 'W';
		m_VKeys[Buttons::S] = 'S';
		m_VKeys[Buttons::D] = 'D';
		m_VKeys[Buttons::Tab] = VK_TAB;
		m_VKeys[Buttons::Escape] = VK_ESCAPE;
		m_VKeys[Buttons::Space] = VK_SPACE;
		m_VKeys[Buttons::Delete] = VK_DELETE;
		m_VKeys[Buttons::Backspace] = VK_BACK;
		m_VKeys[Buttons::Enter] = VK_RETURN;
	}

	if (!m_Keys)
	{
		m_Keys = gcnew Dictionary<Buttons, Keys>();

		m_Keys[Buttons::Left] = Keys::Left;
		m_Keys[Buttons::Up] = Keys::Up;
		m_Keys[Buttons::Right] = Keys::Right;
		m_Keys[Buttons::Down] = Keys::Down;
		m_Keys[Buttons::A] = Keys::A;
		m_Keys[Buttons::W] = Keys::W;
		m_Keys[Buttons::S] = Keys::S;
		m_Keys[Buttons::D] = Keys::D;
		m_Keys[Buttons::Tab] = Keys::Tab;
		m_Keys[Buttons::Escape] = Keys::Escape;
		m_Keys[Buttons::Space] = Keys::Space;
		m_Keys[Buttons::Delete] = Keys::Delete;
		m_Keys[Buttons::Backspace] = Keys::Back;
		m_Keys[Buttons::Enter] = Keys::Enter;
		m_Keys[Buttons::NumPad0] = Keys::NumPad0;
		m_Keys[Buttons::NumPad1] = Keys::NumPad1;
		m_Keys[Buttons::NumPad2] = Keys::NumPad2;
		m_Keys[Buttons::NumPad3] = Keys::NumPad3;
		m_Keys[Buttons::NumPad4] = Keys::NumPad4;
		m_Keys[Buttons::NumPad5] = Keys::NumPad5;
		m_Keys[Buttons::NumPad6] = Keys::NumPad6;
		m_Keys[Buttons::NumPad7] = Keys::NumPad7;
		m_Keys[Buttons::NumPad8] = Keys::NumPad8;
		m_Keys[Buttons::NumPad9] = Keys::NumPad9;
	}
}

void ButtonConfiguration::Load(String^ fileName)
{
	XmlDocument^ doc = gcnew XmlDocument();

	doc->Load(fileName);

	XmlNode^ root = doc["buttonconfiguration"];

	if (root == nullptr)
	{
		throw gcnew System::Exception(
			"ButtonConfiguration::Load: invalid root");
	}

	XmlNode^ child = root->FirstChild;

	while (child)
	{
		if (child->Name == "button")
		{
			Add(child->Attributes["name"]->InnerText,
				child->Attributes["value"]->InnerText);
		}

		child = child->NextSibling;
	}

	doc = nullptr;
}

void ButtonConfiguration::Save(String^ fileName)
{
}

Buttons ButtonConfiguration::ToButton(String^ btnString)
{
	for each (FieldInfo^ fi in Buttons::typeid->GetFields())
	{
		if (fi->Name == btnString)
			return (Buttons)fi->GetValue(nullptr);
	}

	return Buttons::None;
}

void ButtonConfiguration::Add(String^ name, String^ btnString)
{
	array<__wchar_t>^ split = gcnew array<__wchar_t>(1);
	split[0] = '+';

	array<String^>^ buttons = btnString->Split(split);

	Buttons b = Buttons::None;

	for each (String^ btn in buttons)
	{
		b = b | ToButton(btn);
	}
	
	Add(name, b);
}

void ButtonConfiguration::Add(String^ name, Buttons button)
{
	//System::Diagnostics::Debug::Assert(button != Buttons::None);

	bool newButton = true;

	for each (CfgButton^ b in m_Buttons)
	{
		if (b->Name == name)
		{
			b->Button = button;
			newButton = false;
			break;
		}
	}

	if (newButton)
		m_Buttons->Add(gcnew CfgButton(name, button));

	// Sort the buttons according to priority.
	m_Buttons->Sort(gcnew ButtonComparer());

	/*
	Console::WriteLine("==ButtonConfiguration==");

	for each (CfgButton^ b in m_Buttons)
	{
		Console::WriteLine(" {0}: {1}", 
			b->Name, b->Button.ToString());
	}
	*/
}

void ButtonConfiguration::Enable(String^ name, bool enable)
{
	Regex^ regex = gcnew Regex(name);

	for each (CfgButton^ b in m_Buttons)
	{
		if (regex->IsMatch(b->Name))
		{
			b->Enabled = enable;
		}
	}
}

void ButtonConfiguration::Attach(Control^ c)
{
	c->MouseDown += gcnew MouseEventHandler(
		this, &ButtonConfiguration::Control_MouseDown);
	c->MouseUp += gcnew MouseEventHandler(
		this, &ButtonConfiguration::Control_MouseUp);

	c->KeyDown += gcnew KeyEventHandler(
		this, &ButtonConfiguration::Control_KeyDown);
	c->KeyUp += gcnew KeyEventHandler(
		this, &ButtonConfiguration::Control_KeyUp);
}

System::Void ButtonConfiguration::Control_MouseDown(Object^ sender, 
													MouseEventArgs^ e)
{
	//Console::WriteLine("ButtonConfiguration::MouseDown");

	for each (CfgButton^ b in m_Buttons)
	{
		if (b->Enabled &&
			!b->Pressed && 
			ModifiersOk(b->Button) &&
			MouseOk(b->Button, e))
		{
			b->Pressed = true;
			OnButtonDown(b->Name, e);
		}
	}
}

System::Void ButtonConfiguration::Control_MouseUp(Object^ sender, 
												  MouseEventArgs^ e)
{
	for each (CfgButton^ b in m_Buttons)
	{
		if (b->Enabled &&
			b->Pressed && 
			MouseOk(b->Button & ~Buttons::Double, e))
		{
			b->Pressed = false;			
			OnButtonUp(b->Name, e);
		}
	}
}

System::Void ButtonConfiguration::Control_MouseClick(Object^ sender, 
													 MouseEventArgs^ e)
{
	for each (CfgButton^ b in m_Buttons)
	{
		if (b->Enabled &&
			!b->Pressed && 
			ModifiersOk(b->Button) &&
			MouseOk(b->Button, e))
		{
			b->Pressed = true;
			OnButtonDown(b->Name, e);
			OnButtonUp(b->Name, e);
			b->Pressed = false;
		}
	}
}

System::Void ButtonConfiguration::Control_KeyDown(Object^ sender, 
												  KeyEventArgs^ e)
{	
	//Console::WriteLine("ButtonConfiguration::KeyDown");

	for each (CfgButton^ b in m_Buttons)
	{
		if (b->Enabled &&
			!b->Pressed &&
			ModifiersOk(b->Button) &&
			ButtonOk(b->Button, e))
		{
			//Console::WriteLine(" "+b->Name);
			b->Pressed = true;
			OnButtonDown(b->Name, nullptr);
			break;
		}
	}
}

System::Void ButtonConfiguration::Control_KeyUp(Object^ sender, 
												KeyEventArgs^ e)
{
	for each (CfgButton^ b in m_Buttons)
	{
		if (b->Enabled &&
			b->Pressed && 
			ModifiersOk(b->Button) &&
			ButtonOk(b->Button, e))
		{
			b->Pressed = false;			
			OnButtonUp(b->Name, nullptr);
		}
	}
}

bool ButtonConfiguration::ModifiersOk(Buttons button)
{
	if ((button & Buttons::ModifierKeys) != Buttons::None)
	{
		if ((button & Buttons::Ctrl) == Buttons::Ctrl)
		{
			if ((Control::ModifierKeys & Keys::Control) != Keys::Control)
				return false;
		}

		if ((button & Buttons::Alt) == Buttons::Alt)
		{
			if ((Control::ModifierKeys & Keys::Alt) != Keys::Alt)
				return false;
		}

		if ((button & Buttons::Shift) == Buttons::Shift)
		{
			if ((Control::ModifierKeys & Keys::Shift) != Keys::Shift)
				return false;
		}
	}
	else
	{
		// No modifiers needed for this button.
		
		if ((unsigned int)Control::ModifierKeys != 0)
			return false;
	}

	return true;
}

bool ButtonConfiguration::MouseOk(Buttons button,
								  MouseEventArgs^ mouse)
{
	if ((button & Buttons::Mouse) != Buttons::None)
	{
		if (!mouse)
			return false;

		if ((button & Buttons::Double) == Buttons::Double)
		{
			if (mouse->Clicks != 2)
				return false;
		}

		MouseButtons mb = mouse->Button;

		if ((button & Buttons::LButton) == Buttons::LButton)
		{
			if ((mb & MouseButtons::Left) != MouseButtons::Left)
				return false;
		}

		if ((button & Buttons::RButton) == Buttons::RButton)
		{
			if ((mb & MouseButtons::Right) != MouseButtons::Right)
				return false;
		}

		if ((button & Buttons::MButton) == Buttons::MButton)
		{
			if ((mb & MouseButtons::Middle) != MouseButtons::Middle)
				return false;
		}

		return true;
	}

	return false;
}

bool ButtonConfiguration::ButtonOk(Buttons button,
								   KeyEventArgs^ keys)
{
	if ((button & Buttons::NormalKeys) != Buttons::None)
	{
		if (!keys)
			return false;

		Buttons found = Buttons::None;

		for each (KeyValuePair<Buttons, Keys> kvp in m_Keys)
		{
			if ((button & kvp.Key) == kvp.Key)
			{
				if (keys->KeyCode == kvp.Value)
				{
					found = found | kvp.Key;
					break;
				}
			}
		}

		// Did we fill all the requirements? Remove the modifiers.
		if (found != (button & ~Buttons::ModifierKeys))
			return false;
	
		return true;
	}

	return false;
}

bool ButtonConfiguration::IsDown(String^ name)
{
	for each (CfgButton^ b in m_Buttons)
	{
		if (b->Enabled && b->Name == name)
		{
			int vk;

			if (m_VKeys->TryGetValue(
				(b->Button & ~Buttons::ModifierKeys), vk))
			{
				if (ModifiersOk(b->Button) &&
					(GetAsyncKeyState(vk) & 0x8000) != 0)
					return true;
			}
			else
			{
				if ((b->Button & ~Buttons::ModifierKeys) == Buttons::None)
				{
					return ModifiersOk(b->Button);
				}
			}

			return false;
		}
	}

	return false;
}

}
}
}