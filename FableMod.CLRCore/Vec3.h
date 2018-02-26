#pragma once

using namespace System;
using namespace System::IO;

namespace FableMod
{
namespace CLRCore
{

public value struct Vec3
{
public:

	Vec3(float x, float y, float z);

	float x;	
	float y;	
	float z;	
};

}
}