#pragma once

#include <vector>
#include "xdbf.h"

struct Achievement
{
    uint16_t ID;
    std::string Name;
    std::string UnlockedDesc;
    std::string LockedDesc;
    const uint8_t* pImageBuffer;
    size_t ImageBufferSize;
    uint16_t Score;
};

struct XDBFBlock
{
    const uint8_t* pBuffer;
    size_t BufferSize;

    operator bool() const
    {
        return pBuffer;
    }
};

class XDBFWrapper
{
public:
    const uint8_t* pBuffer;
    size_t BufferSize;

    const uint8_t* pContent;

    const XDBFHeader* pHeader;
    const XDBFEntry* pEntries;
    const XDBFFreeSpaceEntry* pFiles;

    XDBFWrapper() : pBuffer(nullptr), BufferSize(0), pContent(nullptr), pHeader(nullptr), pEntries(nullptr), pFiles(nullptr) {}
    XDBFWrapper(const uint8_t* pBuffer, size_t bufferSize);
    XDBFBlock GetResource(EXDBFNamespace ns, uint64_t id) const;
    std::string GetString(EXDBFLanguage language, uint16_t id) const;
    std::vector<Achievement> GetAchievements(EXDBFLanguage language) const;
    Achievement GetAchievement(EXDBFLanguage language, uint16_t id) const;
};
