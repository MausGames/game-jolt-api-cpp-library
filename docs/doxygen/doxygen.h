/*! \mainpage Main Page
 *
 * <img src="../gamejolt_cpp.png" align="left" title="(c) Game Jolt" alt="Game Jolt Logo" style="border-width: 1px; border-color: black; border-style: solid;" /><br clear="all">
 *
 * 1. \link introduction Introduction \endlink\n
 * 1.1. \link introduction_setup Setup \endlink\n
 * 1.2. \link introduction_listing File Listing \endlink\n
 * 1.3. \link introduction_update Update Notes \endlink\n
 * 2. \link features Features \endlink\n
 * 2.1. \link features_access Game Jolt API Access \endlink\n
 * <ul>
 *      <li>\link features_access_session Sessions \endlink\n
 *      <li>\link features_access_user Users \endlink\n
 *      <li>\link features_access_trophy Trophies \endlink\n
 *      <li>\link features_access_score Score Tables and Score Entries \endlink\n
 *      <li>\link features_access_datastore Data Store Items \endlink\n
 * </ul>
 * 2.2. \link features_transfer Non-Blocking Transfer \endlink\n
 * <ul>
 *      <li>\link features_transfer_now Now Request \endlink\n
 *      <li>\link features_transfer_call Call Request \endlink\n
 * </ul>
 * 2.3. \link features_caching Data Caching and Prefetching \endlink\n
 * <ul>
 *      <li>\link features_caching_offline Offline Caching \endlink\n
 * </ul>
 * 2.4. \link features_file File Download \endlink\n
 * 2.5. \link features_base64 Base64url Integration \endlink\n
 * 2.6. \link features_cross Cross-Platform Support \endlink\n
 * 3. \link credits Credits \endlink\n
 * 3.1. \link credits_libraries Additional Libraries \endlink\n
 * 3.2. \link credits_license Software License \endlink\n
 *
 * \section introduction   1. Introduction
 *
 * <a href="https://gamejolt.com">Game Jolt</a> is a host for free independently developed computer games.\n
 *
 * As a gamer you are able to play every game on this site for free, rate them with shiny stars, write awesome comments, get trophies and highscores and talk about stuff on the forum or the chat.\n
 * As a developer you can upload your game, add screenshots, embed videos, write news, receive feedback, eat pizza and do other great things.
 *
 * And if your game is written in C++, then this library is able to connect your game with Game Jolt.\n
 * (for other languages/engines/platforms take a look <a href="https://gamejolt.com/developers/achievements-new/">here</a>)
 *
 * \subsection introduction_setup   1.1. Setup
 *
 * The current library is available from <a href="https://gamejolt.com/games/other/game-jolt-api-c-library/15490/">Game Jolt</a> and contains all necessary files for Windows x86/x64, Linux x64 and Android. For OSX you may need to download the latest <a href="https://curl.haxx.se/">cURL library</a>.\n
 * You also need to create an account and upload your game to Game Jolt to retrieve a game ID and a private key.
 *
 * Currently there are no build scripts implemented, all source files are distributed raw. You simply need to add them to your project build (source folder), compile them with C++11 support,\n
 * and link with the cURL library (libraries folder, incl. include). Alternatively you can include the gjInline.h file.\n
 * <b>Supported compilers:</b> GCC/MinGW 4.7+, MSVC 10.0+, Clang 3.3+
 *
 * \subsection introduction_listing   1.2. File Listing
 *
 * - /<b>docs</b>/                    - documentation files (doxygen)
 *
 * - /<b>examples</b>/
 *   - /examples/<b>android</b>/      - required configuration properties for Android development
 *   - /examples/<b>cmake</b>/        - very basic CMake example <b>showing all relevant paths in a clean way</b> (Windows x86, Linux x64)
 *   - /examples/<b>codeblocks</b>/   - simple Code::Blocks example project (Windows x86, Linux x64)
 *   - /examples/<b>netbeans</b>/     - simple NetBeans example project (Windows x86, Linux x64)
 *   - /examples/<b>visualstudio</b>/ - simple Visual Studio 2010+ example project (Windows x86, Linux x64)
 *   - /examples/<b>libcurl.dll</b>   - pre-compiled cURL library for <b>Windows x86</b> (x64 version available in <b>/libraries/bin/windows/x64/</b>)
 *   - /examples/<b>main.cpp</b>      - example source file
 *
 * - /<b>libraries</b>/
 *   - /libraries/<b>bin</b>/         - pre-compiled cURL libraries (Windows x86/x64 incl. DLLs, Linux x64 incl. TAR-archive, Android)
 *   - /libraries/<b>include</b>/     - cURL include files
 *   - /libraries/<b>licenses</b>/    - license folder
 *
 * - /<b>source</b>/                  - all Game Jolt library source and header files
 *
 * \subsection introduction_update   1.3. Update Notes
 *
 * Current Update Notes are available <a href="update_notes.html" >here</a>.
 *
 * \section features   2. Features
 *
 * \subsection features_access   2.1. Game Jolt API Access
 * This library provides full access to the functions of the <a href="https://gamejolt.com/api/doc/game/">Game Jolt API</a>.\n
 * At the beginning you simply need to create an API-object in your game with the ID and private key from the achievements-tab of your game's dashboard.\n
 * After that you have to update the API-object in your main loop. This is very important when sending non-blocking requests.
 * \warning NEVER give your private key to anyone
 *
 * \code{.cpp}
 * #include "gjAPI.h"
 *
 * int main()
 * {
 *     // create API-object and get basic access to Game Jolt
 *     gjAPI API;
 *     API.Init(12345, "abcdefghijklmnopqrstuvwxyz012345");
 *
 *
 *     // quickly use some functions
 *     if(API.LoginNow(true, "UserName", "UserToken") == GJ_OK)
 *     {
 *         API.InterTrophy()->GetTrophy(1234)->AchieveCall();
 *         API.InterScore()->GetScoreTable(0)->AddScoreCall("123 Points", 123, "ExtraData");
 *     }
 *
 *
 *     // main loop
 *     while(true)
 *     {
 *         // update the API-object !
 *         API.Update();
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 * \subsubsection features_access_session   Sessions
 * (<a href="https://gamejolt.com/api/doc/game/sessions/">https://gamejolt.com/api/doc/game/sessions/</a>)\n
 * Currently a session only shows which games everyone plays at the moment on Game Jolt. This feature is still new and will be extended in the near future.\n
 * The library starts the session after logging in with an user and updates it autonomously, but you can set the session status manually to active or idle.\n
 * \note Regardless of the session, a user login is required for full access to the API
 *
 * \code{.cpp}
 * void Function(gjAPI& API)
 * {
 *     // login with specific user and start session
 *     API.LoginNow(true, "UserName", "UserToken");
 *
 *
 *     // or login with credentials file (quick play function) and don't start session
 *     API.LoginNow(false);
 *
 *
 *     // set session status to "idle"
 *     API.SetSessionActive(false);
 *
 *
 *     // logout with the current main user
 *     API.Logout();
 * }
 * \endcode
 *
 * \subsubsection features_access_user   Users
 * (<a href="https://gamejolt.com/api/doc/game/users/">https://gamejolt.com/api/doc/game/users/</a>)\n
 * Users and all their data can be fetched from Game Jolt by either knowing the user name or the user ID.\n
 * There is also an integration in the score class to easily fetch the associated user.
 *
 * \code{.cpp}
 * void Function(gjAPI& API, myClass& myObj)
 * {
 *     // direct access an user object and get last login string (may block if user is not cached)
 *     API.InterUser()->GetUser("CROS")->GetLastLoggedIn();
 *
 *
 *     // fetch an user now (same as GetUser, may block if user is not cached)
 *     gjUserPtr pUser;
 *     if(API.InterUser()->FetchUserNow("CROS", &pUser) == GJ_OK)
 *     {
 *         // and download his avatar now (may block if file is not cached)
 *         std::string sFilePath;
 *         pUser->DownloadAvatarNow("pictures/avatars", &sFilePath);
 *     }
 *
 *
 *     // fetch an user with a callback (does not block)
 *     API.InterUser()->FetchUserCall("CROS", &myObj, &myClass::ReceiveUser, NULL);
 * }
 * \endcode
 *
 * \subsubsection features_access_trophy   Trophies
 * (<a href="https://gamejolt.com/api/doc/game/trophies/">https://gamejolt.com/api/doc/game/trophies/</a>)\n
 * Trophies are achievements. You can create them on your game's dashboard, and include the appropriate triggers in your game.\n
 * Currently it's not possible by the API to retrieve trophy data without an user login.\n
 * Also you have to define the sorting, secret trophies (only display after achieved), and hidden trophies (never display) by yourself.\n
 *
 * \code{.cpp}
 * void Function(gjAPI& API, myClass& myObj)
 * {
 *     // direct access a trophy object and achieve it (may block if trophy is not cached)
 *     API.InterTrophy()->GetTrophy(1234)->AchieveCall();
 *
 *
 *     // fetch all achieved trophies now (may block if trophies are not cached)
 *     gjTrophyMap apTrophyMap;
 *     if(API.InterTrophy()->FetchTrophiesNow(GJ_TROPHY_ACHIEVED, &apTrophyMap) == GJ_OK)
 *     {
 *         // and download their thumbnails with a callback (does not block)
 *         for(auto it = apTrophyMap.begin(); it != apTrophyMap.end(); ++it)
 *         {
 *             gjTrophy* pTrophy = it->second;
 *             pTrophy->DownloadThumbnailCall("pictures/trophies", &myObj, &myClass::ReceiveThumbnail, pTrophy);
 *         }
 *     }
 *
 *
 *     // fetch all trophies with a callback (does not block)
 *     API.InterTrophy()->FetchTrophiesCall(GJ_TROPHY_ALL, &myObj, &myClass::ReceiveTrophies, NULL);
 *
 *
 *     // sort trophies
 *     const int iSort[] = {2542, 2545, 2546, 2543, 2547, 2544};
 *     API.InterTrophy()->SetSort(iSort, ARRAY_SIZE(iSort));
 *
 *
 *     // define secret trophies
 *     const int iSecret[] = {2546, 2547};
 *     API.InterTrophy()->SetSecret(iSecret, ARRAY_SIZE(iSecret));
 * }
 * \endcode
 *
 * \subsubsection features_access_score   Score Tables and Score Entries
 * (<a href="https://gamejolt.com/api/doc/game/scores/">https://gamejolt.com/api/doc/game/scores/</a>)\n
 * You can fetch all or specific score tables from the API and add new score entries to them or fetch existing score entries from them.\n
 * Score tables are also created and managed on the game's dashboard. There are several properties available, to customize their behavior.\n
 * Score entries are assigned to users or guests, and contain a score string, a sort value and optional extra data.
 *
 * \code{.cpp}
 * void Function(gjAPI& API, myClass& myObj)
 * {
 *     // direct access a score table object and add guest score (may block if score table is not cached)
 *     API.InterScore()->GetScoreTable(567)->AddGuestScoreCall("123 Points", 123, "", "GuestName");
 *
 *
 *     // fetch all score tables now (may block if score tables are not cached)
 *     gjScoreTableMap apScoreTableMap;
 *     if(API.InterScore()->FetchScoreTablesNow(&apScoreTableMap) == GJ_OK)
 *     {
 *         for(auto it = apScoreTableMap.begin(); it != apScoreTableMap.end(); ++it)
 *         {
 *             gjScoreTable* pScoreTable = it->second;
 *
 *             // and fetch the first 10 score entries of each score table now (blocks)
 *             gjScoreList apScoreList;
 *             if(pScoreTable->FetchScoresNow(false, 10, &apScoreList) == GJ_OK)
 *             {
 *                 // and fetch the associated users with a callback (does not block)
 *                 for(size_t i = 0; i < apScoreList.size(); ++i)
 *                 {
 *                     gjScore* pScore = apScoreList[i];
 *                     pScore->FetchUserCall(&myObj, &myClass::ReceiveUser, NULL);
 *                 }
 *             }
 *         }
 *     }
 *
 *
 *     // direct access the primary score table object and get his description (may block if score table is not cached)
 *     API.InterScore()->GetPrimaryTable()->GetDescription();
 * }
 * \endcode
 *
 * \subsubsection features_access_datastore   Data Store Items
 * (<a href="https://gamejolt.com/api/doc/game/data-store/">https://gamejolt.com/api/doc/game/data-store/</a>)\n
 * Data store items are cloud-based objects on the Game Jolt server, which are identified by a key and contain various data.\n
 * Those items are either globally stored or user specific stored (separate for each game) and can be created, modified and deleted.\n
 * This library provides a very easy and fast way to access them and allows to handle string and binary data with Base64 encoding.
 * \note The current max size of a data store item is ~1mb for string and ~768kb for binary data.
 *
 * \code{.cpp}
 * void Function(gjAPI& API)
 * {
 *     // direct access a global data store item and write some string data (the item is created if it doesn't exist)
 *     API.InterDataStoreGlobal()->GetDataItem("DataItemKey_1")->SetDataCall("some data");
 *
 *
 *     // direct access another global data store item and read binary data (blocks)
 *     float afData[10];
 *     API.InterDataStoreGlobal()->GetDataItem("DataItemKey_2")->GetDataBase64Now(afData, sizeof(afData));
 *
 *
 *     // get all existing user data store items (may block if user data store items are not cached)
 *     gjDataItemMap apDataItemMap;
 *     if(API.InterDataStoreUser()->FetchDataItemsNow(&apDataItemMap) == GJ_OK)
 *     {
 *         // and remove all user data store items (does not block)
 *         for(auto it = apDataItemMap.begin(); it != apDataItemMap.end(); ++it)
 *         {
 *             gjDataItem* pDataItem = it->second;
 *             pDataItem->RemoveCall();
 *         }
 *     }
 * }
 * \endcode
 *
 * \subsection features_transfer   2.2. Non-Blocking Transfer
 * This library uses the <a href="https://curl.haxx.se/">cURL library</a> for sending an retrieving data, which provides a simple interface to do this without blocking and freezing the application.\n
 * Therefore many request functions have two different sub-functions: <b>-Now</b> and <b>-Call</b>
 *
 * \subsubsection features_transfer_now   Now Request
 * Now-functions will block the application, if the requested data has to be fetched or downloaded,\n
 * but they have no drawback, when the data is already cached. The data is always forwarded to the target parameter.
 *
 * \code{.cpp}
 * void Function(gjAPI& API)
 * {
 *     // fetch an user now (may block if user is not cached)
 *     gjUserPtr pUser;
 *     API.InterUser()->FetchUserNow("CROS", &pUser);
 * }
 * \endcode
 *
 * \subsubsection features_transfer_call   Call Request
 * Call-functions use an object-related callback system. You forward an object which receives the data, a function pointer from his class as callback, and optional extra data as a void-pointer.\n
 * The implementation creates no additional thread, callbacks are executed immediately when the requested data is cached, or later in the update-function when the download is finished.\n
 * This method is especially useful for large requests and file downloads.
 * \note Some requests (where you don't fetch data) have also a call-function without callback,\n
 *       where you can simply execute them, without waiting for anything
 *
 * \code{.cpp}
 * // example class
 * class myClass
 * {
 * public:
 *     void ReceiveUser(const gjUserPtr& pUser, void* pExtraData);
 * };
 *
 *
 * void Function(gjAPI& API, myClass& myObj)
 * {
 *     // fetch an user with a callback (does not block)
 *     API.InterUser()->FetchUserCall("CROS", &myObj, &myClass::ReceiveUser, NULL);
 * }
 * \endcode
 *
 * \subsection features_caching   2.3. Data Caching and Prefetching
 * To increase the performance and reduce the network load, several data objects from the API are cached, because they won't change much, or only within the scope of the current main user.\n
 * Also some of the data is prefetched when creating the API-object or when logging in.
 *
 * <b>Cached are:</b> all users, all trophies, all score tables, user score entries, user data store items, downloaded files\n
 * <b>Not cached are:</b> non-user score entries, global data store items
 *
 * <b>Prefetched on creation are:</b> all score tables\n
 * <b>Prefetched on login are:</b> main user, all trophies, user data store items (only keys)\n
 *
 * \subsubsection features_caching_offline   Offline Caching
 * Currently only available for trophies, this feature saves retrieved Game Jolt data into a file to keep it after a logout, application restart or when offline.\n
 * This feature makes it possible to provide trophies even without an active Game Jolt login, and to ship it with the application for easier integration.\n
 * The local data is updated with the next successful login, the achieved status is not saved to prevent multi-user-conflicts.\n
 * Offline Caching is deactivated by default, but can be enabled in the gjAPI.h file.
 *
 * \subsection features_file   2.4. File Download
 * It's possible with the <a href="https://curl.haxx.se/">cURL library</a> to download files from any URL to a local folder and use them directly in your game (e.g. textures).\n
 * You simply need the URL of the file, a folder path where you want to save it, and an optional alternative file name.\n
 * The target folder(-hierarchy) is created if it does not exist. Also a direct download for user avatars and trophy thumbnails is already integrated.
 * \warning You need to overwrite the file name if it's not apparent from the URL
 *
 * \code{.cpp}
 * void Function(gjAPI& API, myClass& myObj)
 * {
 *     // download a texture now and use it with my object (may block if file is not cached)
 *     std::string sFilePath;
 *     API.InterFile()->DownloadFileNow("URL", "ToFolder/ToSubFolder", "FileNameOverwrite", &sFilePath);
 *     myObj.LoadTexture(sFilePath, NULL);
 *
 *
 *     // download a texture with a callback and use it with my object (does not block)
 *     API.InterFile()->DownloadFileCall("URL", "ToFolder", "FileNameOverwrite", &myObj, &myClass::LoadTexture, NULL);
 * }
 * \endcode
 *
 * \subsection features_base64   2.5. Base64url Integration
 * The library uses Base64url encoding and decoding with an URL and filename safe alphabet.\n
 * This is a method to convert binary data into readable string data, to use them in SQL-queries, URL-requests, or other situations where only text can be processed.\n
 * (more information is available <a href="https://en.wikipedia.org/wiki/Base64">here</a>)
 *
 * Dedicated Base64url-functions are available for data store items and the extra data of score entries.
 * \note You can use the base functions for own implementations:\n
 * \code{.cpp}
 *     int base64_encode(unsigned char *source, size_t sourcelen, char *target, size_t targetlen);
 *     size_t base64_decode(const char *source, unsigned char *target, size_t targetlen);
 * \endcode
 *
 * \code{.cpp}
 * void Function(gjAPI& API)
 * {
 *     // send binary data to a global data store item now (blocks)
 *     int aiData_1[10] = {0,1,2,3,4,5,6,7,8,9};
 *     API.InterDataStoreGlobal()->GetDataItem("DataItemKey_1")->SetDataBase64Now(aiData_1, sizeof(aiData_1));
 *
 *
 *     // fetch the first user score entry of a score table (may block if first score entry is not cached)
 *     gjScoreList apScoreList;
 *     if(API.InterScore()->GetScoreTable(567)->FetchScoresNow(true, 1, &apScoreList) == GJ_OK)
 *     {
 *         gjScore* pScore = apScoreList[0];
 *
 *         // get binary extra data of this score entry (does not block, only getter)
 *         int aiData_2[10];
 *         pScore->GetExtraDataBase64(aiData_2, sizeof(aiData_2));
 *     }
 * }
 * \endcode
 *
 * \subsection features_cross   2.6. Cross-Platform Support
 * <img src="../os_windows.png" align="left" alt="Windows Logo" /> <img src="../os_osx.png" align="left" alt="OSX Logo" /> <img src="../os_linux.png" align="left" alt="Linux Logo" /> <img src="../os_android.png" align="left" alt="Android Logo" /><br clear="all"><br clear="all">
 * This library is developed and tested with Windows, Linux, OSX and Android.\n
 * The cURL library for Windows x86/x64, Linux x64 and Android is enclosed to the project files.\n
 *
 * \section credits   3. Credits
 * <b>Special Thanks to:</b> <a href="https://twitter.com/theytookcros">David "CROS" DeCarmine</a>, 
 *                           <a href="https://twitter.com/erakkun">Joona "erakkun" Melartin</a>,
 *                           <a href="https://twitter.com/ashleygwinnell">Ashley Gwinnell</a>,
 *                           <a href="https://twitter.com/bassarisse">Bruno Assarisse</a>,
 *                           <a href="https://twitter.com/jani_m_n">Jani "JNyknn" Nykänen</a>,
 *                           <a href="https://gamejolt.com/profile/sasurai/117365/">Jorge Martínez "Sasurai" Vargas</a>\n
 * This is 3rd party software which is not directly affiliated with Game Jolt or Lucent Web Creative, LLC.
 *
 * \subsection credits_libraries   3.1. Additional Libraries
 * cURL Library (<a href="https://curl.haxx.se/">https://curl.haxx.se/</a>)\n
 * MD5 generator by Frank Thilo using the "RSA Data Security, Inc. MD5 Message-Digest Algorithm" (<a href="https://bzflag.org/">https://bzflag.org/</a>)\n
 * Base64 decoder (modified) by FreeCode-FreeCode (<a href="https://freecode-freecode.blogspot.co.at/2008/02/base64c.html">https://freecode-freecode.blogspot.co.at/2008/02/base64c.html</a>)
 *
 * \subsection credits_license   3.2. Software License
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or\n
 * distribute this software, either in source code form or as a compiled\n
 * binary, for any purpose, commercial or non-commercial, and by any\n
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors\n
 * of this software dedicate any and all copyright interest in the\n
 * software to the public domain. We make this dedication for the benefit\n
 * of the public at large and to the detriment of our heirs and\n
 * successors. We intend this dedication to be an overt act of\n
 * relinquishment in perpetuity of all present and future rights to this\n
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,\n
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.\n
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR\n
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,\n
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR\n
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*! \page update_notes Update Notes
 *
 * v1.1.0 (2017-06-25)
 * -------------------
 * <ul>
 *      <li>added Android support
 *      <li>added SSL support
 *      <li>added Windows x64 (MSVC) support
 * </ul>
 * v1.0.0 (2014-02-22)
 * -------------------
 * <ul>
 *      <li>implemented asynchronous login functions
 *      <li>implemented gjInline.h as alternate way to include the library
 *      <li>deactivated Offline Caching by default
 *      <li>updated the enclosed cURL libraries
 *      <li>fixed some minor bugs
 * </ul>
 * v0.3.0 (2013-11-18)
 * -------------------
 * <ul>
 *      <li>implemented trophy sorting
 *      <li>implemented handling of trophy attributes (secret, hidden)
 *      <li>implemented timestamp for logging purposes
 *      <li>improved some support-functions
 *      <li>improved performance
 *      <li>fixed cache-file creation without data
 *      <li>added MinGW and Clang support
 *      <li>added Code::Blocks and NetBeans example project
 *      <li>added OSX support
 * </ul>
 * v0.2.0 (2013-07-22)
 * -------------------
 * <ul>
 *      <li>new feature: \link features_caching_offline Offline Caching \endlink
 *      <li>implemented POST request for larger data uploads
 *      <li>implemented handling of identical requests
 *      <li>implemented connection and request timeouts
 *      <li>implemented empty error objects to prevent crashes
 *      <li>improved Linux compatibility
 *      <li>improved error handling and logging
 *      <li>improved data caching and prefetching
 *      <li>fixed possible memory leaks
 * </ul>
 * v0.1.0 (2013-06-21)
 * -------------------
 * <ul>
 *      <li>initial release
 * </ul>
 */
