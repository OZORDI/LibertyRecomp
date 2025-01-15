#pragma once

#include <SWA.inl>

namespace SWA
{
    class CGameParameter // : public Hedgehog::Universe::CMessageActor
    {
    public:
        struct SSaveData;
        struct SStageParameter;

        SWA_INSERT_PADDING(0x94);
        xpointer<SSaveData> m_pSaveData;
        xpointer<SStageParameter> m_pStageParameter;
    };
}
