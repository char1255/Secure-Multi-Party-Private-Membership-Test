#ifndef MPMTCFG_HPP
#define MPMTCFG_HPP

#define MPMT_VCB_STL     // Vector Computing Backend: C++STL
// #define MPMT_VCB_CUDA    // Vector Computing Backend: CUDA Thrust
// #define MPMT_VCB_XSIMD   // Vector Computing Backend: XSIMD

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