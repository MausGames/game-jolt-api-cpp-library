////////////////////////////////////////////////////////////////////
//*--------------------------------------------------------------*//
//| Part of the Game Jolt API C++ Library (https://gamejolt.com) |//
//*--------------------------------------------------------------*//
//| Released into the public domain                              |//
//| More information available in the readme file                |//
//*--------------------------------------------------------------*//
////////////////////////////////////////////////////////////////////


// ****************************************************************
/* target configuration */
#if defined(__APPLE__)
    #include <TargetConditionals.h>
#endif

// compiler
#if defined(_MSC_VER)
    #define _GJ_MSVC_  (_MSC_VER)
#endif
#if defined(__GNUC__)
    #define _GJ_GCC_   ((__GNUC__)*10000 + (__GNUC_MINOR__)*100 + (__GNUC_PATCHLEVEL__)*1)
#endif
#if defined(__clang__)
    #define _GJ_CLANG_ ((__clang_major__)*10000 + (__clang_minor__)*100 + (__clang_patchlevel__)*1)
    #undef  _GJ_GCC_
#endif

// operating system
#if defined(_WIN32)
    #define _GJ_WINDOWS_
#endif
#if defined(__linux__)
    #define _GJ_LINUX_
#endif
#if defined(__APPLE__) && TARGET_OS_MAC
    #define _GJ_MACOS_
#endif
#if defined(__ANDROID__)
    #define _GJ_ANDROID_
    #undef  _GJ_LINUX_
#endif
#if defined(__APPLE__) && TARGET_OS_IPHONE
    #define _GJ_IOS_
    #undef  _GJ_MACOS_
#endif

// exception handling
#if defined(_CPPUNWIND) || defined(__EXCEPTIONS)
    #define _GJ_EXCEPTIONS_
#endif

// debug mode
#if defined(_DEBUG) || ((defined(_GJ_GCC_) || defined(_GJ_CLANG_)) && !defined(__OPTIMIZE__))
    #define _GJ_DEBUG_
#endif


// ****************************************************************
/* compiler definitions */
#pragma push_macro("noexcept")
#pragma push_macro("override")
#pragma push_macro("final")
#pragma push_macro("delete_func")

#undef noexcept
#undef override
#undef final
#undef delete_func

#if defined(_GJ_MSVC_)
    #if (_GJ_MSVC_) < 1900
        #define noexcept
    #endif
    #if (_GJ_MSVC_) < 1800
        #define delete_func
    #else
        #define delete_func = delete
    #endif
    #if (_GJ_MSVC_) < 1700
        #define final
    #endif
#else
    #if (_GJ_GCC_) < 40700
        #define override
        #define final
    #endif
    #define delete_func = delete
#endif

#if defined(_GJ_MSVC_)

    #pragma warning(push)

    // disable unwanted compiler warnings (with /Wall)
    #pragma warning(disable : 4061)   //!< enumerator not handled in switch
    #pragma warning(disable : 4100)   //!< unreferenced formal parameter
    #pragma warning(disable : 4127)   //!< constant conditional expression
    #pragma warning(disable : 4201)   //!< nameless struct or union
    #pragma warning(disable : 4242)   //!< implicit conversion to different precision #1
    #pragma warning(disable : 4244)   //!< implicit conversion to different precision #2
    #pragma warning(disable : 4266)   //!< virtual function not overridden
    #pragma warning(disable : 4267)   //!< implicit conversion of std::size_t
    #pragma warning(disable : 4365)   //!< implicit conversion between signed and unsigned
    #pragma warning(disable : 4371)   //!< layout of class may have changed from a previous version of the compiler
    #pragma warning(disable : 4514)   //!< unreferenced inline function removed
    #pragma warning(disable : 4548)   //!< expression before comma has no effect
    #pragma warning(disable : 4557)   //!< __assume contains effect
    #pragma warning(disable : 4571)   //!< semantic change of catch(...)
    #pragma warning(disable : 4577)   //!< noexcept used without exception handling
    #pragma warning(disable : 4623)   //!< default constructor implicitly deleted
    #pragma warning(disable : 4625)   //!< copy constructor implicitly deleted
    #pragma warning(disable : 4626)   //!< copy assignment operator implicitly deleted
    #pragma warning(disable : 4647)   //!< behavior change of std::is_pod
    #pragma warning(disable : 4668)   //!< preprocessor macro not defined
    #pragma warning(disable : 4710)   //!< function not inlined
    #pragma warning(disable : 4711)   //!< function automatically inlined
    #pragma warning(disable : 4774)   //!< format string not a string literal
    #pragma warning(disable : 4820)   //!< padding after data member
    #pragma warning(disable : 4987)   //!< nonstandard extension throw(...)
    #pragma warning(disable : 5026)   //!< move constructor implicitly deleted
    #pragma warning(disable : 5027)   //!< move assignment operator implicitly deleted
    #pragma warning(disable : 5031)   //!< detected warning(push) with no corresponding warning(pop)
    #pragma warning(disable : 5039)   //!< potentially throwing function passed to extern C function
    #pragma warning(disable : 5045)   //!< possible Spectre vulnerability

#endif

#if !defined(_GJ_EXCEPTIONS_)
    #define _HAS_EXCEPTIONS 0
#endif
#define _CRT_SECURE_NO_WARNINGS
#define _ALLOW_KEYWORD_MACROS
#define _ALLOW_RTCc_IN_STL