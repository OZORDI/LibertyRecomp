namespace SWA
{
    // TODO: Hedgehog::Base::TSynchronizedPtr<CGameDocument>
    inline CGameDocument* CGameDocument::GetInstance()
    {
        return *(xpointer<CGameDocument>*)MmGetHostAddress(0x83367900);
    }
}
