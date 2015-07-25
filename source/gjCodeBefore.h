///////////////////////////////////////////////////////////////////
//*-------------------------------------------------------------*//
//| Part of the Game Jolt API C++ Library (http://gamejolt.com) |//
//*-------------------------------------------------------------*//
//| Released under the zlib License                             |//
//| More information available in the readme file               |//
//*-------------------------------------------------------------*//
///////////////////////////////////////////////////////////////////

#if defined(_GJ_MSVC_)

    #pragma warning(push)

    // disable unwanted compiler warnings (with /W4)
    #pragma warning(disable : 4100)   //!< unreferenced formal parameter
    #pragma warning(disable : 4127)   //!< constant conditional expression
    #pragma warning(disable : 4201)   //!< nameless struct or union
    #pragma warning(disable : 4267)   //!< implicit conversion of std::size_t
    #pragma warning(disable : 4244)   //!< implicit conversion to smaller integer precision

    // enable additional compiler warnings (https://msdn.microsoft.com/library/23k5d385)
    #pragma warning(default : 4191 4264 4265 4287 4289 4296 4302 4311 4355 4388 4548 4555 4557 4738 4826 4837 4928 4946)

#endif