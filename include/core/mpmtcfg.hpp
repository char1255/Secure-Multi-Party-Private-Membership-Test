#ifndef MPMTCFG_HPP
#define MPMTCFG_HPP

#define MPMT_VCB_STL     // Vector Computing Backend: C++STL
// #define MPMT_VCB_CUDA    // Vector Computing Backend: CUDA Thrust
// #define MPMT_VCB_XSIMD   // Vector Computing Backend: XSIMD

#define MPMT_VCBMARCO_COUNT (defined(MPMT_VCB_CUDA) + defined(MPMT_VCB_STL) + defined(MPMT_VCB_XSIMD))

#if (MPMT_VCBMARCO_COUNT) == 0
    #error "ERROR: No vector computing backend selected. Please define exactly one of MPMT_VCB_CUDA, MPMT_VCB_STL, or MPMT_VCB_XSIMD."
#elif (MPMT_VCBMARCO_COUNT) > 1
    #error "ERROR: Multiple vector computing backends defined. Only one backend can be enabled at a time."
#endif

static_assert(sizeof(size_t) == 8, "ERROR: 64-bit compilation required. Program requires 64-bit pointers (sizeof(size_t) != 8).");

#if defined(_WIN64)
    #define MPMT_OS_WIN

#elif defined(_WIN32)
    #error "Windows Target is 32-bit (MPMT_OS_WIN32), which is not supported. Please compile for x64."

#elif defined(__APPLE__) && defined(__MACH__)
    #include <TargetConditionals.h>
    
    #if defined(__x86_64__) || defined(__aarch64__)
        
        #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
            #define MPMT_OS_IOS
        #else
            #define MPMT_OS_MACOS
        #endif
        
    #else
        #error "Detected 32-bit Apple Platform (Unsupported)."
    #endif

#elif defined(__ANDROID__)
    #if defined(__x86_64__) || defined(__aarch64__)
        #define MPMT_OS_ANDROID
    #else
        #error "Android Target is 32-bit, which is not supported for 64-bit requirement."
    #endif

#elif defined(__linux__)
    #if defined(__x86_64__) || defined(__aarch64__)
        #define MPMT_OS_LINUX
    #else
        #error "Linux Target is 32-bit, which is not supported for 64-bit requirement."
    #endif

#else
    #error "Unsupported Operating System."
#endif


// 防御检查
#if (							\
	defined(MPMT_OS_WIN) 		\
	+ defined(MPMT_OS_MACOS) 	\
	+ defined(MPMT_OS_IOS) 		\
	+ defined(MPMT_OS_ANDROID) 	\
	+ defined(MPMT_OS_LINUX)	\
) != 1
    #error "Exactly one OS macro must be defined."
#endif


// 后续更新堆栈打印工具，并通过日志系统更新到日志内容中
#define MPMT_ASSERT(cond, msg)                                          \
		do {                                                            \
			if (!(cond)) {                                             	\
				fprintf(stderr,                                         \
					"\033[1;31m[ERROR]\033[0m\n"						\
					"  Condition : %s\n"                                \
					"  Message   : %s\n"                                \
					"  Function  : %s\n"                                \
					"  File      : %s\n"                                \
					"  Line      : %d\n",                               \
					#cond, msg, __func__, __FILE__, __LINE__);			\
				exit(1);                                                \
			}                                                           \
		} while(0)

// 后续更新日志系统，让WARN的内容更新到日志内容中
#define MPMT_WARN(cond, msg)                                           	\
        do {                                                            \
            if (!(cond)) {                                              \
                fprintf(stderr,                                         \
                    "\033[1;33m[WARN]\033[0m\n"                         \
                    "  Condition : %s\n"                                \
                    "  Message   : %s\n"                                \
                    "  Function  : %s\n"                                \
                    "  File      : %s\n"                                \
                    "  Line      : %d\n",                               \
                    #cond, msg, __func__, __FILE__, __LINE__);          \
            }                                                           \
        } while(0)
        
/** @namespace 项目命名空间。*/
namespace mpmt{
    
}

#endif // !MPMTCFG_HPP