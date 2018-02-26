#pragma once

using namespace System;
using namespace System::Collections::ObjectModel;
using namespace System::IO;
using namespace FableMod::CLRCore;
using namespace FableMod::BBB;

namespace FableMod
{
namespace STB
{
	
public value struct FloraPoint
{
	Vec3 Position;
	float Rotation;
	float Scale;
	Byte Type;
	UInt32 FloraIndex;
};

ref class Level;

public ref class FloraNode
{
public:

	FloraNode();

	virtual ~FloraNode();
	
	property int CellX
	{
		int get() { return m_CellX; }
	}

	property int CellY
	{
		int get() { return m_CellY; }
	}

	property int CellWidth
	{
		int get() { return m_CellWidth; }
	}

	property int CellHeight
	{
		int get() { return m_CellHeight; }
	}

	property Vec3 Position
	{
		Vec3 get() { return m_Position; }
	}

	property int ChildCount
	{
		int get() { return m_Children->Count; }
	}

	FloraNode^ GetChild(int index)
	{
		return m_Children[index];		
	}

	property int PointCount
	{
		int get() { return m_Points->Count; }
	}

	FloraPoint GetPoint(int index)
	{
		return m_Points[index];		
	}

	property int Depth
	{
		int get() { return m_Depth; }
	}

internal:

    virtual void Read(
		Level^ level, String^ depth, FileStream^ file, BBBEntry^ entry);
    
	virtual void ReadChildren(
		Level^ level, String^ depth, FileStream^ file, BBBEntry^ entry, UInt32 childCount);

	virtual void ReadPoints(
		Level^ level, String^ depth, FileStream^ file, BBBEntry^ entry);

	virtual void ReadFloraPoints(
		Level^ level, String^ depth, FileStream^ file);

	int m_CellX;
	int m_CellY;
	int m_CellWidth;
	int m_CellHeight;
	Vec3 m_Position;
	
private:
	
	int m_Depth;

	Collection<FloraNode^>^ m_Children;
	Collection<FloraPoint>^ m_Points;
};

}
}