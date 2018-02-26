#include "TerrainVertex.h"

namespace FableMod
{
namespace Gfx
{

//---------------------------------------------------------------------------

TerrainVertex::TerrainVertex()
{
    m_apDependent[0] = 0;
    m_apDependent[1] = 0;
    m_bEnabled = false;
}

//----------------------------------------------------------------------------

void TerrainVertex::Enable()
{
    m_bEnabled = true;

    if (m_apDependent[0])
    {
        if (!m_apDependent[0]->GetEnabled())
            m_apDependent[0]->Enable();
    }
    
    if (m_apDependent[1])
    {
        if (!m_apDependent[1]->GetEnabled())
            m_apDependent[1]->Enable();
    }
}

//---------------------------------------------------------------------------

void TerrainVertex::Disable()
{
    m_bEnabled = false;
    
    if (m_apDependent[0])
    {
        if (m_apDependent[0]->GetEnabled())
            m_apDependent[0]->Disable();
    }

    if (m_apDependent[1])
    {
        if (m_apDependent[1]->GetEnabled())
            m_apDependent[1]->Disable();
    }
}

//---------------------------------------------------------------------------

}
}