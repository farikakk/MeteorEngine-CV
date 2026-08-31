/* Copyright 2020 - 2026, Hansson Software. All rights reserved. */

#ifdef MR_PLATFORM_WINDOWS
#include "HAL/Paths.h"
#include <HAL/HAL.h>
#include <Logging/Log.h>
#include <Types/StringView.h>

#include <HAL/WindowsPlatform.h>

namespace HAL
{
    namespace Paths
    {
        String GetExecutablePath()
        {
            wchar_t fixed[512] = {};
            char final[512] = {};
            HAL::ConvertToNarrow(final, GetModuleFileNameW(nullptr, fixed, 512), fixed);
                
            return final;
        }

        String GetEffectsPath()
        {
            return "";
        }

        void NormalizePath(String& inPath, const char to, const char from)
        {
            for (char* begin = inPath.Data(); *begin; begin++)
                if (*begin == from) *begin = to;
        }

        StringView GetExtension(const String& inPath)
        {
            const char* end = inPath.Chr() + inPath.Length();
            const char* begin = inPath.Chr() + inPath.Length();
            while (*--begin)
            {
                if (*begin == '.')
                {
                    return { begin, (u32)(end - begin) };
                }
            }

  
            return StringView();
        }
    }
}

#endif