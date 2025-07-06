#pragma once

#include <SWA.inl>

namespace SWA
{
    class CTitleStateMenu : public CTitleStateBase
    {
    public:
        class CTitleStateMenuContext
        {
        public:
            SWA_INSERT_PADDING(0x1E8);
            xpointer<CTitleMenu> m_pTitleMenu;
        };
    };
}
