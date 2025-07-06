namespace SWA
{
    inline bool SPadState::IsDown(const EKeyState in_Keys) const
    {
        return (DownState & in_Keys) == in_Keys;
    }

    inline bool SPadState::IsUp(const EKeyState in_Keys) const
    {
        return (UpState & in_Keys) == in_Keys;
    }

    inline bool SPadState::IsTapped(const EKeyState in_Keys) const
    {
        return (TappedState & in_Keys) == in_Keys;
    }

    inline bool SPadState::IsReleased(const EKeyState in_Keys) const
    {
        return (ReleasedState & in_Keys) == in_Keys;
    }
}