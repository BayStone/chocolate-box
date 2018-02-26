#include "Stdafx.h"
#include "Material.h"

using namespace System;
using namespace System::Globalization;
using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

Material::Material(MTRL* mat) :
m_Material(mat)
{
}

unsigned int Material::ID::get()
{
	return m_Material->ID;
}

void Material::ID::set(unsigned int id)
{
	m_Material->ID = id;
}

String^ Material::Name::get()
{
	return gcnew String(m_Material->Name);
}

void Material::Name::set(String^ name)
{
	delete[] m_Material->Name;
	m_Material->Name = new char[name->Length+1];
	IntPtr pname = Marshal::StringToHGlobalAnsi(name);
	strcpy(m_Material->Name, (char*)pname.ToPointer());
}

unsigned int Material::Padding::get()
{
	return m_Material->Padding;
}

unsigned int Material::BaseTextureID::get()
{
	return m_Material->BASE_Texture_ID;
}

void Material::BaseTextureID::set(unsigned int id)
{
	m_Material->BASE_Texture_ID = id;
}

unsigned int Material::BumpMapTextureID::get()
{
	return m_Material->BUMPMAP_Texture_ID;
}

void Material::BumpMapTextureID::set(unsigned int id)
{
	m_Material->BUMPMAP_Texture_ID = id;
}

unsigned int Material::ReflectionTextureID::get()
{
	return m_Material->REFLECT_Texture_ID;
}

void Material::ReflectionTextureID::set(unsigned int id)
{
	m_Material->REFLECT_Texture_ID = id;
}

unsigned int Material::AlphaMapTextureID::get()
{
	return m_Material->AlphaTextureID;
}

void Material::AlphaMapTextureID::set(unsigned int val)
{
	m_Material->AlphaTextureID = val;
}

unsigned int Material::TextureFlags::get()
{
	return m_Material->TextureFlags;
}

void Material::TextureFlags::set(unsigned int val)
{
	m_Material->TextureFlags = val;
}

unsigned int Material::GlowStrength::get()
{
	return m_Material->Glow_Strength;
}

void Material::GlowStrength::set(unsigned int val)
{
	m_Material->Glow_Strength = val;
}

unsigned char Material::Unknown2::get()
{
	return m_Material->Unknown2;
}

void Material::Unknown2::set(unsigned char val)
{
	m_Material->Unknown2 = val;
}

unsigned char Material::AlphaEnabled::get()
{
	return m_Material->Alpha_Enabled;
}

void Material::AlphaEnabled::set(unsigned char val)
{
	m_Material->Alpha_Enabled = val;
}

unsigned char Material::Unknown3::get()
{
	return m_Material->Unknown3;
}

void Material::Unknown3::set(unsigned char val)
{
	m_Material->Unknown3 = val;
}
unsigned short Material::Unknown4::get()
{
	return m_Material->Unknown4;
}

void Material::Unknown4::set(unsigned short val)
{
	m_Material->Unknown4 = val;
}

}
}
}