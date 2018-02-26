#include "Stdafx.h"
#include "ThemeManager.h"

using namespace FableMod::ContentManagement;
using namespace FableMod::BIN;
using namespace System::Text;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

ThemeManager::ThemeManager(TextureManager^ textureManager,
						   ModelManager^ modelManager) :
m_Themes(gcnew Dictionary<String^, Theme^>()),
m_TextureManager(textureManager),
m_ModelManager(modelManager)
{
}

ThemeManager::~ThemeManager()
{
	Clear();

	m_Themes = nullptr;
	m_TextureManager = nullptr;
	m_ModelManager = nullptr;
}

void ThemeManager::Clear()
{
	m_Themes->Clear();
}

void ThemeManager::FindModel(Theme^ theme, unsigned __int32 entry)
{
	ContentManager^ mgr = ContentManager::Instance;
	
	ContentObject^ obj = mgr->FindEntry(
		LinkDestination::GameBINEntryID, entry);

	if (obj)
	{
		BINEntry^ entry = (BINEntry^)obj->Object;

		DefinitionType^ defType = mgr->Definitions->GetDefinition(
			entry->Definition);

		if (defType)
		{
			defType->ReadIn(entry);

			Control^ control = defType->FindControl(0xC549E650);

			if (control)
			{
				ArrayMember^ member1 = (ArrayMember^)control->Members[1];

				ArrayMember^ member2 = 
					(ArrayMember^)member1->Elements[0][5];

				Member^ member = (Member^)member2->Elements[0][1];

				UInt32 modelId = UInt32::Parse(member->Value->ToString());

				theme->Model = m_ModelManager->Get(modelId);
			}

			delete defType;
		}
	}

	mgr = nullptr;
}

Theme^ ThemeManager::Themes::get(String^ name)
{
	// Fast access.

	Theme^ theme = nullptr;

	if (m_Themes->TryGetValue(name, theme))
	{
		return theme;
	}

	ContentManager^ mgr = ContentManager::Instance;
	
	ContentObject^ obj = mgr->FindEntry(
		LinkDestination::GameBINEntryName, name);

	if (obj)
	{
		BINEntry^ entry = (BINEntry^)obj->Object;

		DefinitionType^ defType = mgr->Definitions->GetDefinition(
			entry->Definition);

		if (defType)
		{
			Theme^ theme = gcnew Theme();

			m_Themes[name] = theme;
			
			defType->ReadIn(entry);
			
			Control^ control = defType->FindControl(0x3F759331);

			if (control)
			{
				Member^ member = (Member^)control->Members[0];

				theme->Texture = m_TextureManager->Get(
					UInt32::Parse(member->Value->ToString()));
			}
			
			control = defType->FindControl(0xE86B72CC);

			if (control)
			{
				Member^ member = (Member^)control->Members[0];

				UInt32 id = UInt32::Parse(member->Value->ToString());

				if (id == 1799)
				{
					// Has local detail.
					FindModel(theme, id);
				}
			}
			
			delete defType;
			
			return theme;
		}
	}

	mgr = nullptr;

	return nullptr;
}

}
}
}