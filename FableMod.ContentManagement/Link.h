#pragma once

using namespace System;
using namespace System::IO;

namespace FableMod
{
namespace ContentManagement
{

public enum class LinkDestination
{
	ModelID,
	ModelName,
	AnimationID,
	NamesBINOffset,
	NamesBINEnum,
	NamesBINName,
	GameBINEntryID,
	GameBINEntryName,
	FrontEndBINEntryID,
	ScriptBINEntryID,
	EffectID,
	MainTextureID,
	MainTextureName,
	GUITextureID,
	FrontEndTextureID,
	ParticleID,
	TextID,
	TextGroupID,
	ShaderID,
    BIGSymbolName,
};

public ref class Link
{
public:
	
	Link(LinkDestination to) :
	m_To(to)
	{
	}
	
	Link(LinkDestination to, String^ restriction) :
	m_To(to),
	m_Restriction(restriction)
	{
	}
	
	property LinkDestination To
	{
		LinkDestination get() { return m_To; }
	}
	
	property String^ Restriction
	{
		String^ get() { return m_Restriction; }
	}

private:
	
	LinkDestination	m_To;
	String^	m_Restriction;
};

}
}