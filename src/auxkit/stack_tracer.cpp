#include "auxkit/stack_tracer.hpp"

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <dbghelp.h>
#include <cstdio>
#include <vector>

namespace utils
{
    namespace verborgen
    {
        class dbghelp_initializer
        {
        public:
            dbghelp_initializer()
            {
                SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
                SymInitialize(GetCurrentProcess(), nullptr, TRUE);
            }

            ~dbghelp_initializer()
            {
                SymCleanup(GetCurrentProcess());
            }
        };
    }
    static verborgen::dbghelp_initializer g_dbghelp_init;

    void stack_tracer::print(size_t max_frames) noexcept
    {
        HANDLE process = GetCurrentProcess();
        HANDLE thread = GetCurrentThread();

        std::vector<void*> stack(max_frames);
        USHORT captured = CaptureStackBackTrace(
            /* Frames to skip */ 0,
            /* Frames to capture */ static_cast<DWORD>(max_frames),
            stack.data(),
            nullptr
        );

        fprintf(stderr, "Stack trace (%u frames):\n", captured);

        // Allocate symbol buffer
        constexpr size_t kSymbolBufferSize = sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char);
        SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(malloc(kSymbolBufferSize));
        if (!symbol)
        {
            fprintf(stderr, "  [Error] Failed to allocate symbol info buffer.\n");
            return;
        }

        symbol->MaxNameLen = MAX_SYM_NAME;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        IMAGEHLP_LINE64 line;
        memset(&line, 0, sizeof(line));
        line.SizeOfStruct = sizeof(line);

        DWORD displacement = 0;

        for (USHORT i = 0; i < captured; ++i)
        {
            DWORD64 address = reinterpret_cast<DWORD64>(stack[i]);

            bool gotSymbol = SymFromAddr(process, address, nullptr, symbol);
            bool gotLine = SymGetLineFromAddr64(process, address, &displacement, &line);

            fprintf(stderr, "  #%02u: ", i);

            if (gotSymbol)
            {
                fprintf(stderr, "%s - ", symbol->Name);
            }
            else
            {
                fprintf(stderr, "0x%llX - ", address);
            }

            if (gotLine)
            {
                fprintf(stderr, "%s:%lu\n", line.FileName, line.LineNumber);
            }
            else
            {
                fprintf(stderr, "0x%llX\n", address);
            }
        }

        free(symbol);
    }
}


#elif defined(__linux__)

#error "Coming soon."

#else
#error "Unsupported Operating System."
#endif
