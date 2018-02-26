#pragma once

namespace FableMod
{
namespace Gfx
{

class Node;

namespace Integration
{

ref class TextureManager;
ref class ModelManager;
ref class ThemeManager;

public ref class GfxManager
{
public:

	static void Initialize(System::Windows::Forms::Form^ form, 
						   int width, int height);

	static void SaveScreenToFile(System::String^ fileName);

	static void Destroy();

	static void SetDirectory(System::String^ directory);

	static TextureManager^ GetTextureManager();

	static ModelManager^ GetModelManager();

	static ThemeManager^ GetThemeManager();

private:

	GfxManager();

	static TextureManager^ m_TextureManager;
	static ModelManager^ m_ModelManager;
	static ThemeManager^ m_ThemeManager;
	
	static ::FableMod::Gfx::Node* m_pRoot;
};

}
}
}