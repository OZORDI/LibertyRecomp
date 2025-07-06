#pragma once

#include <SWA.inl>

namespace SWA
{
    class CObjDashPanel
    {
    public:
        SWA_INSERT_PADDING(0xE8);
        be<float> m_FieldE8;
        be<float> m_Speed;
    };
}
