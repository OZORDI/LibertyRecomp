#include <api/SWA.h>
#include <locale/locale.h>
#include <ui/fader.h>
#include <ui/message_window.h>
#include <app.h>

static bool g_quitMessageOpen = false;
static bool g_quitMessageFaderBegun = false;
static int g_quitMessageResult = -1;

static bool ProcessQuitMessage()
{
    if (!g_quitMessageOpen)
        return false;

    if (g_quitMessageFaderBegun)
        return true;

    std::array<std::string, 2> options = { Localise("Common_Yes"), Localise("Common_No") };

    if (MessageWindow::Open(Localise("Title_Message_Quit"), &g_quitMessageResult, options, 1) == MSG_CLOSED)
    {
        switch (g_quitMessageResult)
        {
            case 0:
                Fader::FadeOut(1, []() { App::Exit(); });
                g_quitMessageFaderBegun = true;
                break;

            case 1:
                g_quitMessageOpen = false;
                g_quitMessageResult = -1;
                break;
        }
    }

    return true;
}

// SWA::CTitleStateIntro::Update
PPC_FUNC_IMPL(__imp__sub_82587E50);
PPC_FUNC(sub_82587E50)
{
    auto isAutoSaveWarningShown = *(bool*)g_memory.Translate(0x83367BC1);

    if (isAutoSaveWarningShown)
    {
        __imp__sub_82587E50(ctx, base);
    }
    else
    {
        auto pInputState = SWA::CInputState::GetInstance();

        if (pInputState && pInputState->GetPadState().IsTapped(SWA::eKeyState_B))
            g_quitMessageOpen = true;

        if (!ProcessQuitMessage())
            __imp__sub_82587E50(ctx, base);
    }
}
