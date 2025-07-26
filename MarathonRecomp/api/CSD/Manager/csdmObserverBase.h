#pragma once

#include "Marathon.inl"

namespace Chao::CSD
{
    template<typename TObservee>
    class CObserverBase
    {
    public:
        struct Vftable
        {
            be<uint32_t> m_fpDtor;
        };

        xpointer<Vftable> m_pVftable;
        MARATHON_INSERT_PADDING(0x08);

        ~CObserverBase();
    };
}

#include "CSD/Manager/csdmObserverBase.inl"
