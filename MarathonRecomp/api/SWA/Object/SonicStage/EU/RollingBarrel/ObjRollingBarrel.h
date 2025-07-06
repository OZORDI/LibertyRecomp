#pragma once

#include <SWA.inl>

namespace SWA
{
    class CObjRollingBarrel
    {
    public:
        class CElement
        {
        public:
            SWA_INSERT_PADDING(0xEC);
            xpointer<void> m_FieldEC;
            SWA_INSERT_PADDING(0x20);
            be<float> m_Roll;
        };

        SWA_INSERT_PADDING(0xE8);
        be<float> m_GenerationTime;
        SWA_INSERT_PADDING(0x09);
        bool m_FieldF5;
        SWA_INSERT_PADDING(0x0A);
        be<float> m_RandomRange;
        be<float> m_Field104;
    };

    SWA_ASSERT_OFFSETOF(CObjRollingBarrel::CElement, m_FieldEC, 0xEC);
    SWA_ASSERT_OFFSETOF(CObjRollingBarrel::CElement, m_Roll, 0x110);

    SWA_ASSERT_OFFSETOF(CObjRollingBarrel, m_GenerationTime, 0xE8);
    SWA_ASSERT_OFFSETOF(CObjRollingBarrel, m_FieldF5, 0xF5);
    SWA_ASSERT_OFFSETOF(CObjRollingBarrel, m_RandomRange, 0x100);
    SWA_ASSERT_OFFSETOF(CObjRollingBarrel, m_Field104, 0x104);
}
