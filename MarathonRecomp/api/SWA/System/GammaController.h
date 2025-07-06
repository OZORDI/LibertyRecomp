#pragma once

#include <SWA.inl>
#include <Hedgehog/Math/Vector.h>

namespace SWA
{
    class CGammaController : public Hedgehog::Base::CObject
    {
    public:
        SWA_INSERT_PADDING(0x10);
        uint8_t m_Field10;
        SWA_INSERT_PADDING(0x0F);
        Hedgehog::Math::CVector4 m_Gamma;
        Hedgehog::Math::CVector4 m_Field30;
    };

    SWA_ASSERT_OFFSETOF(CGammaController, m_Field10, 0x10);
    SWA_ASSERT_OFFSETOF(CGammaController, m_Gamma, 0x20);
    SWA_ASSERT_OFFSETOF(CGammaController, m_Field30, 0x30);
    SWA_ASSERT_SIZEOF(CGammaController, 0x40);
}
