#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace FableMod
{
namespace TNG
{

public ref class UIDManager
{
public:

	static void Load(String^ directory);

	static void LoadFromFile(String^ fileName);

	static void WriteToFile(String^ fileName);

	static void Clear();

	static void Add(String^ uid);

	static void Remove(String^ uid);

	static bool Exists(String^ uid);

	static bool IsNormal(String^ uid);

	static String^ Generate();

	static bool IsEmpty();

private:

	UIDManager();

	static String^ Generate(int length, String^ start);

	static List<String^>^ m_UIDs = gcnew List<String^>(16384);
	static String^ BASE = "18446741";
	static int MAX_ITERATIONS = 65535;
	static int DEFAULT_LENGTH = 20;
	static Random^ m_Rnd = gcnew Random();
	static bool m_BinarySearch = true;
};

}
}
