#include <cpu/guest_code.h>
#include <api/SWA.h>
#include <locale/locale.h>
#include <ui/fader.h>
#include <ui/message_window.h>
#include <ui/options_menu.h>
#include <user/paths.h>
#include <app.h>
#include <exports.h>

static bool g_installMessageFaderBegun = false;
static bool g_installMessageOpen = false;
static int g_installMessageResult = -1;

static bool ProcessInstallMessage()
{
    if (!g_installMessageOpen)
        return false;

    if (g_installMessageFaderBegun)
        return true;

    auto& str = g_isMissingDLC
        ? Localise("Installer_Message_TitleMissingDLC")
        : Localise("Installer_Message_Title");

    std::array<std::string, 2> options = { Localise("Common_Yes"), Localise("Common_No") };

    if (MessageWindow::Open(str, &g_installMessageResult, options, 1) == MSG_CLOSED)
    {
        switch (g_installMessageResult)
        {
            case 0:
                // TODO: replace ExitProcess with restart method using --install-dlc argument.
                Fader::FadeOut(1, []() { ExitProcess(0); });
                g_installMessageFaderBegun = true;
                break;

            case 1:
                g_installMessageOpen = false;
                g_installMessageResult = -1;
                break;
        }
    }

    return true;
}

// SWA::CTitleStateMenu::Update
PPC_FUNC_IMPL(__imp__sub_825882B8);
PPC_FUNC(sub_825882B8)
{
    auto pTitleState = (SWA::CTitleStateBase*)g_memory.Translate(ctx.r3.u32);
    auto pInputState = SWA::CInputState::GetInstance();
    auto& pPadState = pInputState->GetPadState();
    auto isAccepted = pPadState.IsTapped(SWA::eKeyState_A) || pPadState.IsTapped(SWA::eKeyState_Start);
    auto isOptionsIndex = pTitleState->m_pMember->m_pTitleMenu->m_CursorIndex == 2;
    auto isInstallIndex = pTitleState->m_pMember->m_pTitleMenu->m_CursorIndex == 3;

    if (!OptionsMenu::s_isVisible && isOptionsIndex)
    {
        if (isAccepted)
        {
            Game_PlaySound("sys_worldmap_window");
            Game_PlaySound("sys_worldmap_decide");

            OptionsMenu::Open();
        }
    }
    else if (isInstallIndex && isAccepted)
    {
        g_installMessageOpen = true;
    }

    if (!OptionsMenu::s_isVisible && !ProcessInstallMessage())
        __imp__sub_825882B8(ctx, base);

    if (isOptionsIndex)
    {
        if (OptionsMenu::CanClose() && pPadState.IsTapped(SWA::eKeyState_B))
        {
            Game_PlaySound("sys_worldmap_cansel");

            OptionsMenu::Close();
        }
    }
}

void TitleMenuRemoveStorageDeviceOptionMidAsmHook(PPCRegister& r11)
{
    r11.u32 = 0;
}

void TitleMenuAddInstallOptionMidAsmHook(PPCRegister& r3)
{
    r3.u32 = 1;
}
