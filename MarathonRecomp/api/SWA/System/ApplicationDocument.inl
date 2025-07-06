namespace SWA
{
    // TODO: Hedgehog::Base::TSynchronizedPtr<CApplicationDocument>
    inline CApplicationDocument* CApplicationDocument::GetInstance()
    {
        return *(xpointer<CApplicationDocument>*)MmGetHostAddress(0x833678A0);
    }
}
