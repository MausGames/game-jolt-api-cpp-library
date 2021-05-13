////////////////////////////////////////////////////////////////////
//*--------------------------------------------------------------*//
//| Part of the Game Jolt API C++ Library (https://gamejolt.com) |//
//*--------------------------------------------------------------*//
//| Released into the public domain                              |//
//| More information available in the readme file                |//
//*--------------------------------------------------------------*//
////////////////////////////////////////////////////////////////////


// ****************************************************************
/* compiler definitions */
#pragma pop_macro("noexcept")
#pragma pop_macro("override")
#pragma pop_macro("final")
#pragma pop_macro("delete_func")

#if defined(_GJ_MSVC_)
    #pragma warning(pop)
#endif

#if !defined(_GJ_EXCEPTIONS_)
    #undef _HAS_EXCEPTIONS
#endif
#undef _CRT_SECURE_NO_WARNINGS
#undef _ALLOW_KEYWORD_MACROS
#undef _ALLOW_RTCc_IN_STL