#ifndef MPMTCFG_HPP
#define MPMTCFG_HPP

#define MPMT_VCB_STL     // Vector Computing Backend: C++STL
#define MPMT_VCB_CUDA    // Vector Computing Backend: CUDA Thrust
#define MPMT_VCB_XSIMD   // Vector Computing Backend: XSIMD

#if defined(_WIN32) || defined(_WIN64)          // Windows
    #define MPMT_OS_WIN

#elif defined(__APPLE__) && defined(__MACH__)   // macOS / iOS
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        #define MPMT_OS_IOS
    #else
        #define MPMT_OS_MACOS
    #endif

#elif defined(__ANDROID__)                       // Android
    #define MPMT_OS_ANDROID

#elif defined(__linux__)                         // Linux
    #define MPMT_OS_LINUX

#else
    #error "Unsupported Operating System."
#endif

#if (defined(MPMT_OS_WIN) + defined(MPMT_OS_MACOS) + defined(MPMT_OS_IOS) + defined(MPMT_OS_ANDROID) + defined(MPMT_OS_LINUX)) != 1
    #error "Exactly one OS macro must be defined."
#endif


#define MPMT_ASSERT(cond, msg)                                          \
		do {                                                            \
			if (!(cond)) {                                             \
				fprintf(stderr,                                         \
					"\033[1;31m[错误]\033[0m\n"						    \
					"  Condition : %s\n"                                \
					"  Message   : %s\n"                                \
					"  Function  : %s\n"                                \
					"  File      : %s\n"                                \
					"  Line      : %d\n",                               \
					#cond, msg, __func__, __FILE__, __LINE__);			\
				exit(1);                                                \
			}                                                           \
		} while(0)

        
/** @namespace 项目命名空间。*/
namespace mpmt{
    
}

#endif // !MPMTCFG_HPP