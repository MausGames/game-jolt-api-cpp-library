////////////////////////////////////////////////////////////////////////////////////////////
//*--------------------------------------------------------------------------------------*//
//|   ______    ______    __    __    ______          __    ______    __        ______   |//
//|  /\  ___\  /\  __ \  /\ "-./  \  /\  ___\        /\ \  /\  __ \  /\ \      /\__  _\  |//
//|  \ \ \__ \ \ \  __ \ \ \ \-./\ \ \ \  __\       _\_\ \ \ \ \/\ \ \ \ \____ \/_/\ \/  |//
//|   \ \_____\ \ \_\ \_\ \ \_\ \ \_\ \ \_____\    /\_____\ \ \_____\ \ \_____\   \ \_\  |//
//|    \/_____/  \/_/\/_/  \/_/  \/_/  \/_____/    \/_____/  \/_____/  \/_____/    \/_/  |//
//|                                                                                      |//
//*--------------------------------------------------------------------------------------*//
////////////////////////////////////////////////////////////////////////////////////////////
//*--------------------------------------------------------------------------------------*//
//| Game Jolt API C++ Library v0.8a (http://gamejolt.com)                                |//
//*--------------------------------------------------------------------------------------*//
//| Author: Martin Mauersics                                                             |//
//| Special Thanks to: David "CROS" DeCarmine, Joona "erakko" Melartin, Ashley Gwinnell  |//
//*--------------------------------------------------------------------------------------*//
//| Copyright (c) 2013 Martin Mauersics                                                  |//
//|                                                                                      |//
//| This software is provided 'as-is', without any express or implied                    |//
//| warranty. In no event will the authors be held liable for any damages                |//
//| arising from the use of this software.                                               |//
//|                                                                                      |//
//| Permission is granted to anyone to use this software for any purpose,                |//
//| including commercial applications, and to alter it and redistribute it               |//
//| freely, subject to the following restrictions:                                       |//
//|                                                                                      |//
//|   1. The origin of this software must not be misrepresented; you must not            |//
//|   claim that you wrote the original software. If you use this software               |//
//|   in a product, an acknowledgment in the product documentation would be              |//
//|   appreciated but is not required.                                                   |//
//|                                                                                      |//
//|   2. Altered source versions must be plainly marked as such, and must not be         |//
//|   misrepresented as being the original software.                                     |//
//|                                                                                      |//
//|   3. This notice may not be removed or altered from any source                       |//
//|   distribution.                                                                      |//
//|                                                                                      |//
//|   4. This software may only be used within the terms of Game Jolt.                   |//
//|   (http://gamejolt.com/terms/)                                                       |//
//*--------------------------------------------------------------------------------------*//
////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#ifndef GJAPI_H
#define GJAPI_H


/* --- configuration --- */
#define GJ_API_URL                 "http://gamejolt.com/api/game/v1"
#define GJ_API_AVATAR_DEFAULT      "http://gamejolt.com/img/no-avatar-1.png"
#define GJ_API_AVATAR_FORMAT       ".png"
#define GJ_API_TROPHY_DEFAULT_1    "http://gamejolt.com/img/trophy-bronze-1.jpg"
#define GJ_API_TROPHY_DEFAULT_2    "http://gamejolt.com/img/trophy-silver-1.jpg"
#define GJ_API_TROPHY_DEFAULT_3    "http://gamejolt.com/img/trophy-gold-1.jpg"
#define GJ_API_TROPHY_DEFAULT_4    "http://gamejolt.com/img/trophy-platinum-1.jpg"
#define GJ_API_PING_TIME           30
#define GJ_API_CRED                "gjapi-credentials.txt"
#define GJ_API_NOW                 "now"
#define GJ_API_RESERVE_CALL        16
#define GJ_API_RESERVE_CALL_OUTPUT 4
#define GJ_API_RESERVE_SCORE       128
#define GJ_API_RESERVE_FILE        64
#define GJ_API_TIMEOUT_CONNECTION  5
#define GJ_API_TIMEOUT_REQUEST     15
#define GJ_API_LOGFILE             true
#define GJ_API_LOGFILE_NAME        "gjapi_log.txt"
#define GJ_API_PREFETCH            true
#define GJ_API_OFFCACHE_USER       true // currently unused
#define GJ_API_OFFCACHE_TROPHY     true
#define GJ_API_OFFCACHE_SCORE      true // currently unused
#define GJ_API_OFFCACHE_DATASTORE  true // currently unused
#define GJ_API_OFFCACHE_FILE       true // currently unused
#define GJ_API_OFFCACHE_NAME       "gjapi_cache.dat"
/* --- configuration --- */


/*! @param pOutputObj     output receiving object of <b>class T</b>
 *  @param OutputCallback callback function from <b>class T</b> with a specific return <b>type x</b>
 *  @param pOutputData    additional data which will be forwarded to the callback function
 *
 *  <b>Code Example</b>
 *  \code{.cpp}
 *  void Function(myClass &myObj)
 *  {
 *      // fetch an user with a callback (does not block)
 *      API.InterUser()->FetchUserCall("CROS", &myObj, &myClass::ReceiveUser, NULL);
 *  }
 *  \endcode */
#define GJ_NETWORK_OUTPUT(x)    T* pOutputObj, void (T::*OutputCallback)(const x&, void*), void* pOutputData
#define GJ_NETWORK_PROCESS      P* pProcessObj, int (P::*ProcessCallback)(const std::string&, void*, D*), void* pProcessData

#define GJ_NETWORK_OUTPUT_FW    pOutputObj, OutputCallback, pOutputData
#define GJ_NETWORK_PROCESS_FW   pProcessObj, ProcessCallback, pProcessData

#define GJ_NETWORK_NULL_THIS(d) this, &gjAPI::Null<d>, NULL
#define GJ_NETWORK_NULL_API(d)  m_pAPI, &gjAPI::Null<d>, NULL

#define SAFE_DELETE(p)          {if(p) {delete   (p); (p)=NULL;}}
#define SAFE_DELETE_ARRAY(p)    {if(p) {delete[] (p); (p)=NULL;}}
#define SAFE_MAP_GET(o,s)       ((o).count(s) ? (o).at(s) : "")

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <map>
#include <vector>
#ifndef _WIN32
    #include <string.h>
    #include <sys/stat.h>
#endif

#include "MD5.h"
#include "Base64.h"
#include "curl/curl.h"

#include <stdlib.h>

#undef GetUserName

class gjAPI;
class gjUser;
class gjTrophy;
class gjScoreTable;
class gjScore;
class gjDataItem;

typedef std::map<std::string, std::string> gjData;
typedef std::vector<gjData>                gjDataList;
typedef void*                              gjVoidPtr;
typedef gjUser*                            gjUserPtr;
typedef std::map<int, gjTrophy*>           gjTrophyMap;
typedef gjTrophy*                          gjTrophyPtr;
typedef std::map<int, gjScoreTable*>       gjScoreTableMap;
typedef std::vector<gjScore*>              gjScoreList;
typedef gjScore*                           gjScorePtr;
typedef std::map<std::string, gjDataItem*> gjDataItemMap;
typedef gjDataItem*                        gjDataItemPtr;

enum GJ_ERROR
{
    GJ_OK               = 0x00,   //!< everything is fine
    GJ_INVALID_CALL     = 0x01,   //!< function cannot be called
    GJ_INVALID_INPUT    = 0x02,   //!< function parameters are invalid
    GJ_REQUEST_FAILED   = 0x04,   //!< request to the API failed
    GJ_REQUEST_CANCELED = 0x08,   //!< request was canceled because of redundancy
    GJ_NOT_CONNECTED    = 0x10,   //!< no main user connected (login first)
    GJ_NO_DATA_FOUND    = 0x20,   //!< no data was found
    GJ_NETWORK_ERROR    = 0x40,   //!< error sending or receiving data, or failed to establish a connection
    GJ_FILE_ERROR       = 0x80    //!< error on opening, writing or finding a file
};

enum GJ_SORT_DIRECTION
{
    GJ_SORT_UNDEF =  0,   //!< undefined sorting
    GJ_SORT_DESC  =  1,   //!< descending sorting (3, 2, 1)
    GJ_SORT_ASC   = -1    //!< ascending sorting (1, 2, 3)
};

enum GJ_TROPHY_TYPE
{
    GJ_TROPHY_ALL          =  0,   //!< all trophies
    GJ_TROPHY_ACHIEVED     =  1,   //!< only achieved trophies
    GJ_TROPHY_NOT_ACHIEVED = -1    //!< only unachieved trophies
};

#include "gjNetwork.h"   // other header files are post-included


// ****************************************************************
/*! Main interface class of the library to connect with the Game Jolt API.\n
 *  Manages sessions, users, trophies, scores, data items and downloaded files.\n
 *  http://gamejolt.com/api/doc/game/
 *  \brief Main Interface 
 *  \todo Maybe switch all function parameters from std::string to char*\n
 *        Implement event loop system as third way to do requests\n
 *        Improve offline caching */
class gjAPI
{
private:
    // ****************************************************************
    /*! Sub-Interface class for user operations.\n
     *  http://gamejolt.com/api/doc/game/users/
     *  \brief User Sub-Interface */
    class gjInterUser
    {
    private:
        std::map<int, gjUser*> m_apUser;   //!< cached user objects

        gjAPI* m_pAPI;                     //!< main interface access pointer
        gjNetwork* m_pNetwork;             //!< network access pointer


    public:
        gjInterUser(gjAPI* pAPI, gjNetwork* pNetwork);
        ~gjInterUser();

        /*! @name Direct Access */
        //!@{
        /*! Get direct access to the user objects.\n
         *  This function may block to cache the specific user.
         *  \pre    Login maybe required
         *  @param  iID   Unique ID of an user (0 = current main user, Login required)
         *  @param  sName Unique name of an user
         *  @return Pointer to specific user or empty object (ID == 0) on error */
        gjUser* GetUser(const int &iID);
        gjUser* GetUser(const std::string &sName);
        gjUser* GetMainUser();
        //!@}

        /*! @name Fetch User Request */
        //!@{
        /*! Fetch and cache a specific user through an API request.
         *  \pre    Login maybe required
         *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
         *  @param  iID   Unique ID of an user (0 = current main user, Login required)
         *  @param  sName Unique name of an user
         *  @return <b>GJ_OK</b> on success\n
         *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
         *          <b>GJ_INVALID_INPUT</b> if name string is empty\n
         *          (see #GJ_ERROR) */
                              inline int FetchUserNow(const int &iID, gjUserPtr* ppOutput)                     {if(!ppOutput) return GJ_INVALID_INPUT; return this->__FetchUser(iID, ppOutput, GJ_NETWORK_NULL_API(gjUserPtr));}
                              inline int FetchUserNow(const std::string &sName, gjUserPtr* ppOutput)           {if(!ppOutput) return GJ_INVALID_INPUT; return this->__FetchUser(sName, ppOutput, GJ_NETWORK_NULL_API(gjUserPtr));}
        template <typename T> inline int FetchUserCall(const int &iID, GJ_NETWORK_OUTPUT(gjUserPtr))           {return this->__FetchUser(iID, NULL, GJ_NETWORK_OUTPUT_FW);}
        template <typename T> inline int FetchUserCall(const std::string &sName, GJ_NETWORK_OUTPUT(gjUserPtr)) {return this->__FetchUser(sName, NULL, GJ_NETWORK_OUTPUT_FW);}
        //!@}

        /*! @name Clear Cache */
        //!@{
        /*! Delete all cached user objects.
         *  \warning All external pointers will be invalid */
        void ClearCache();
        //!@}


    private:
        /*! @name Superior Request Functions */
        //!@{
        template <typename T> int __FetchUser(const int &iID, gjUserPtr* ppOutput, GJ_NETWORK_OUTPUT(gjUserPtr));
        template <typename T> int __FetchUser(const std::string &sName, gjUserPtr* ppOutput, GJ_NETWORK_OUTPUT(gjUserPtr));
        //!@}

        /*! @name Management Functions */
        //!@{
        int __CheckCache(const int &iID, gjUserPtr* ppOutput);
        int __CheckCache(const std::string &sName, gjUserPtr* ppOutput);
        int __Process(const std::string &sData, void* pAdd, gjUserPtr* ppOutput);
        //!@}
    };


    // ****************************************************************
    /*! Sub-Interface class for trophy operations.\n
     *  http://gamejolt.com/api/doc/game/trophies/
     *  \brief Trophy Sub-Interface
     *  \todo Implement sort function and alternative list return instead of a map\n
     *        Implement secret and hidden trophy handling*/
    class gjInterTrophy
    {
    private:
        std::map<int, gjTrophy*> m_apTrophy;   //!< cached trophy objects
        int m_iCache;                          //!< cache status (0 = empty, 1 = offline, 2 = online)

        gjAPI* m_pAPI;                         //!< main interface access pointer
        gjNetwork* m_pNetwork;                 //!< network access pointer


    public:
        gjInterTrophy(gjAPI* pAPI, gjNetwork* pNetwork);
        ~gjInterTrophy();

        /*! @name Direct Access */
        //!@{
        /*! Get direct access to the trophy objects.\n
         *  This function may block to cache all trophies.
         *  \pre    Login required
         *  @param  iID Unique ID of a trophy
         *  @return Pointer to specific trophy or empty object (ID == 0) on error */
        gjTrophy* GetTrophy(const int &iID);
        //!@}

        /*! @name Fetch Trophies Request */
        //!@{
        /*! Fetch and cache all trophies through an API request.
         *  \pre    Login required
         *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
         *  @param  iAchieved Status of the returned trophies (see #GJ_TROPHY_TYPE)
         *  @return <b>GJ_OK</b> on success\n
         *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
         *          <b>GJ_NOT_CONNECTED</b> if connection/login is missing\n
         *          <b>GJ_NO_DATA_FOUND</b> if no trophies where found\n
         *          (see #GJ_ERROR) */
                              inline int FetchTrophiesNow(const long &iAchieved, gjTrophyMap* papOutput)          {if(!papOutput) return GJ_INVALID_INPUT; return this->__FetchTrophies(iAchieved, papOutput, GJ_NETWORK_NULL_API(gjTrophyMap));}
        template <typename T> inline int FetchTrophiesCall(const long &iAchieved, GJ_NETWORK_OUTPUT(gjTrophyMap)) {return this->__FetchTrophies(iAchieved, NULL, GJ_NETWORK_OUTPUT_FW);}
        //!@}

        /*! @name Clear Cache */
        //!@{
        /*! Delete all cached trophy objects.
         *  \warning All external pointers will be invalid */
        void ClearCache(const bool &bFull);
        //!@}


    public:
        /*! @name Superior Request Functions */
        //!@{
        template <typename T> int __FetchTrophies(const long &iAchieved, gjTrophyMap* papOutput, GJ_NETWORK_OUTPUT(gjTrophyMap));
        //!@}

        /*! @name Management Functions */
        //!@{
        int __CheckCache(const int &iAchieved, gjTrophyMap* papOutput);
        int __Process(const std::string &sData, void* pAdd, gjTrophyMap* papOutput);
        //!@}

        /*! @name Offline Cache Functions */
        //!@{
        void __SaveOffCache(const std::string &sData);
        void __LoadOffCache();
        //!@}
    };


    // ****************************************************************
    /*! Sub-Interface class for score operations.\n
     *  http://gamejolt.com/api/doc/game/scores/
     *  \brief Score Sub-Interface */
    class gjInterScore
    {
    private:
        std::map<int, gjScoreTable*> m_apScoreTable;   //!< cached score table objects with semi-cached score entries

        gjAPI* m_pAPI;                                 //!< main interface access pointer
        gjNetwork* m_pNetwork;                         //!< network access pointer


    public:
        gjInterScore(gjAPI* pAPI, gjNetwork* pNetwork);
        ~gjInterScore();

        /*! @name Direct Access */
        //!@{
        /*! Get direct access to score table objects.\n
         *  This function may block to cache all score tables.
         *  @param  iID Unique ID of a score table (0 = primary score table)
         *  @return Pointer to specific score table or empty object (ID == 0) on error */
               gjScoreTable* GetScoreTable(const int &iID);
        inline gjScoreTable* GetPrimaryTable() {return this->GetScoreTable(0);}
        //!@}

        /*! @name Fetch Score Tables Request */
        //!@{
        /*! Fetch and cache all score tables through an API request.
         *  \bug    The API returns already deleted score tables
         *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
         *  @return <b>GJ_OK</b> on success\n
         *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
         *          <b>GJ_NO_DATA_FOUND</b> if no score tables where found\n
         *          (see #GJ_ERROR) */
                              inline int FetchScoreTablesNow(gjScoreTableMap* papOutput)          {if(!papOutput) return GJ_INVALID_INPUT; return this->__FetchScoreTables(papOutput, GJ_NETWORK_NULL_API(gjScoreTableMap));}
        template <typename T> inline int FetchScoreTablesCall(GJ_NETWORK_OUTPUT(gjScoreTableMap)) {return this->__FetchScoreTables(NULL, GJ_NETWORK_OUTPUT_FW);}
        //!@}

        /*! @name Clear Cache */
        //!@{
        /*! Delete all cached score table objects and score entries.
         *  \warning All external pointers will be invalid */
        void ClearCache();
        //!@}


    private:
        /*! @name Superior Request Functions */
        //!@{
        template <typename T> int __FetchScoreTables(gjScoreTableMap* papOutput, GJ_NETWORK_OUTPUT(gjScoreTableMap));
        //!@}

        /*! @name Management Functions */
        //!@{
        int __CheckCache(gjScoreTableMap* papOutput);
        int __Process(const std::string &sData, void* pAdd, gjScoreTableMap* papOutput);
        //!@}
    };


    // ****************************************************************
    /*! Sub-Interface class for data store operations.\n
     *  http://gamejolt.com/api/doc/game/data-store/
     *  \brief Data Store Sub-Interface */
    class gjInterDataStore
    {
    private:
        std::map<std::string, gjDataItem*> m_apDataItem;   //!< semi-cached data store items

        int m_iType;                                       //!< type of this interface (0 = global, 1 = user)

        gjAPI* m_pAPI;                                     //!< main interface access pointer
        gjNetwork* m_pNetwork;                             //!< network access pointer


    public:
        gjInterDataStore(const int &iType, gjAPI* pAPI, gjNetwork* pNetwork);
        ~gjInterDataStore();

        /*! @name Direct Access */
        //!@{
        /*! Get direct access to data store items.\n
         *  This function creates a new data store item, if the key does not exist.\n
         *  To get all existing items, use \link FetchDataItemsNow FetchDataItems\endlink.
         *  \pre    Login maybe required
         *  @param  sKey Unique key of a data store item
         *  @return Pointer to specific data store item or NULL on error */
        gjDataItem* GetDataItem(const std::string &sKey);
        //!@}

        /*! @name Fetch Data Items Request */
        //!@{
        /*! Fetch and semi-cache all data store items through an API request.
         *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
         *  @return <b>GJ_OK</b> on success\n
         *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
         *          <b>GJ_NOT_CONNECTED</b> if connection/login is missing\n
         *          <b>GJ_NO_DATA_FOUND</b> if no data items where found\n
         *          (see #GJ_ERROR) */
                              inline int FetchDataItemsNow(gjDataItemMap* papOutput)          {if(!papOutput) return GJ_INVALID_INPUT; return this->__FetchDataItems(papOutput, GJ_NETWORK_NULL_API(gjDataItemMap));}
        template <typename T> inline int FetchDataItemsCall(GJ_NETWORK_OUTPUT(gjDataItemMap)) {return this->__FetchDataItems(NULL, GJ_NETWORK_OUTPUT_FW);}
        //!@}

        /*! @name Clear Cache */
        //!@{
        /*! Delete all cached data store items.
         *  \warning All external pointers will be invalid */
        void ClearCache();
        //!@}

        /*! @name Get Attributes */
        //!@{
        inline const int& GetType()const {return m_iType;}   //!< \copybrief m_iType
        /*! */ //!@}


    private:
        /*! @name Superior Request Functions */
        //!@{
        template <typename T> int __FetchDataItems(gjDataItemMap* papOutput, GJ_NETWORK_OUTPUT(gjDataItemMap));
        //!@}

        /*! @name Management Functions */
        //!@{
        int __CheckCache(gjDataItemMap* papOutput);
        int __Process(const std::string &sData, void* pAdd, gjDataItemMap* papOutput);
        //!@}

        /*! @name Callback Functions */
        //!@{
        int __AddDataItemCallback(const std::string &sData, void* pAdd, gjTrophyPtr* pOutput);
        int __RemoveDataItemCallback(const std::string &sData, void* pAdd, gjTrophyPtr* ppOutput);
        //!@}
    };


    // ****************************************************************
    /*! Sub-Interface class for file downloads.\n
     *  \brief File Download Sub-Interface */
    class gjInterFile
    {
    private:
        std::vector<std::string> m_asFile;   //!< cached file paths

        gjAPI* m_pAPI;                       //!< main interface access pointer
        gjNetwork* m_pNetwork;               //!< network access pointer


    public:
        gjInterFile(gjAPI* pAPI, gjNetwork* pNetwork);
        ~gjInterFile();

        /*! @name Download File */
        /*! Download a file from any URL.\n
         *  Retrieve the local path of the file when finished.
         *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
         *  \warning You need to overwrite the file name if it's not apparent from the URL
         *  @param  sURL               Full path of the remote file
         *  @param  sToFolder          Relative path of the download target folder
         *  @param  sFileNameOverwrite Custom target file name (mandatory, if file name is not apparent from the URL)
         *  @return <b>GJ_OK</b> on success\n
         *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
         *          <b>GJ_INVALID_INPUT</b> if URL string or target folder string is empty\n
         *          (see #GJ_ERROR) */
                              inline int DownloadFileNow(const std::string &sURL, const std::string &sToFolder, const std::string &sFileNameOverwrite, std::string* psOutput)           {if(!psOutput) return GJ_INVALID_INPUT; return this->__DownloadFile(sURL, sToFolder, sFileNameOverwrite, psOutput, GJ_NETWORK_NULL_API(std::string));}
        template <typename T> inline int DownloadFileCall(const std::string &sURL, const std::string &sToFolder, const std::string &sFileNameOverwrite, GJ_NETWORK_OUTPUT(std::string)) {return this->__DownloadFile(sURL, sToFolder, sFileNameOverwrite, NULL, GJ_NETWORK_OUTPUT_FW);}

        /*! @name Clear Cache */
        //!@{
        /*! Delete all cached file paths. */
        void ClearCache();
        //!@}


    private:
        /*! @name Superior Request Functions */
        //!@{
        template <typename T> int __DownloadFile(const std::string &sURL, const std::string &sToFolder, const std::string &sFileNameOverwrite, std::string* psOutput, GJ_NETWORK_OUTPUT(std::string));
        //!@}

        /*! @name Management Functions */
        //!@{
        int __CheckCache(const std::string &sPath);
        int __Process(const std::string &sData, void* pAdd, std::string* psOutput);
        //!@}
    };


private:
    int m_iGameID;                               //!< ID to identify the game
    std::string m_sGamePrivateKey;               //!< private key to generate MD5 signature
                                                 
    std::string m_sUserName;                     //!< name of the main user
    std::string m_sUserToken;                    //!< token of the main user
                                                 
    time_t m_iNextPing;                          //!< next ping for the user session
    bool m_bActive;                              //!< current status for the user session
    bool m_bConnected;                           //!< current connection status

    gjInterUser* m_pInterUser;                   //!< user Sub-Interface object
    gjInterTrophy* m_pInterTrophy;               //!< trophy Sub-Interface object
    gjInterScore* m_pInterScore;                 //!< score Sub-Interface object
    gjInterDataStore* m_pInterDataStoreGlobal;   //!< global data store Sub-Interface object
    gjInterDataStore* m_pInterDataStoreUser;     //!< user data store Sub-Interface object
    gjInterFile* m_pInterFile;                   //!< file download Sub-Interface object
    
    gjNetwork* m_pNetwork;                       //!< network object

    std::string m_sProcGameID;                   //!< already processed/converted game ID
    std::string m_sProcUserName;                 //!< already processed/escaped user name
    std::string m_sProcUserToken;                //!< already processed/escaped user token
                                                 
    std::vector<std::string> m_asLog;            //!< error log


public:
    gjAPI(const int &iGameID, const std::string &sGamePrivateKey);
    ~gjAPI();

    /*! @name Update */
    //!@{
    /*! Main update function of the library.
     *  \brief Must be executed in the main loop of the application
     *  \note Must be executed in the main loop of the application */
    void Update();
    //!@}

    /*! @name Login User*/
    //!@{
    /*! Login with a specific user.\n
     *  Authenticate user and establish an user session through the API.\n
     *  Prefetch the user object, trophies and user releated data store items.
     *  \todo   Only blocking version available at the moment
     *  @param  bSession   Establish an user session
     *  @param  sUserName  Login name of the user
     *  @param  sUserToken Token for that user
     *  @param  sCredPath  Relative path to the credentials file of the quick play function
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_INVALID_CALL</b> if already connected\n
     *          <b>GJ_INVALID_INPUT</b> if user name or user token is missing\n
     *          <b>GJ_FILE_ERROR</b> if credentials file was not found\n
     *          (see #GJ_ERROR) */
    int Login(const bool bSession, const std::string &sUserName, const std::string &sUserToken);
    int Login(const bool bSession, std::string sCredPath = GJ_API_CRED);
    //!@}

    /*! @name Logout User */
    //!@{
    /*! Logout with the current main user.\n
     *  \warning External pointers to trophies and user data store items will be invalid
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_NOT_CONNECTED</b> if connection/login is missing\n
     *          (see #GJ_ERROR) */
    int Logout();
    //!@}

    /*! @name Sub-Interface Access */
    //!@{
    inline gjInterUser*      InterUser()const            {return m_pInterUser;}
    inline gjInterTrophy*    InterTrophy()const          {return m_pInterTrophy;}
    inline gjInterScore*     InterScore()const           {return m_pInterScore;}
    inline gjInterDataStore* InterDataStoreGlobal()const {return m_pInterDataStoreGlobal;}
    inline gjInterDataStore* InterDataStoreUser()const   {return m_pInterDataStoreUser;}
    inline gjInterFile*      InterFile()const            {return m_pInterFile;}
    //!@}

    /*! @name Send Custom Request */
    //!@{
    /*! Send a custom request to the API.\n
     *  Retrieve a response string when finished.
     *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks\n
     *          Use "&POST<data>" at the end of the URL for a POST request
     *  @param  sURL Relative API request string
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_NETWORK_ERROR</b> if session cannot be established\n
     *          (see #GJ_ERROR) */
                                                  inline int SendRequestNow(const std::string &sURL, std::string* psOutput)                                        {return m_pNetwork->SendRequest(sURL, psOutput, this, &gjAPI::Null, NULL, GJ_NETWORK_NULL_THIS(std::string));} 
    template <typename T, typename P, typename D> inline int SendRequestCall(const std::string &sURL, GJ_NETWORK_PROCESS, GJ_NETWORK_OUTPUT(D))                    {return m_pNetwork->SendRequest(sURL, NULL, GJ_NETWORK_PROCESS_FW, GJ_NETWORK_OUTPUT_FW);} 
    template <typename T, typename P, typename D> inline int SendRequest(const std::string &sURL, std::string* psOutput, GJ_NETWORK_PROCESS, GJ_NETWORK_OUTPUT(D)) {return m_pNetwork->SendRequest(sURL, psOutput, GJ_NETWORK_PROCESS_FW, GJ_NETWORK_OUTPUT_FW);}
    //!@}

    /*! @name Parse Request */
    //!@{
    /*! Parse a valid response string from the API.\n
     *  Retrieve a list of data objects or a single string.
     *  @param  sInput Valid response string
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if the parsed request was unsuccessful\n
     *          <b>GJ_INVALID_INPUT</b> if the string parsing failed\n
     *          (see #GJ_ERROR) */
    int ParseRequestKeypair(const std::string &sInput, gjDataList* paaOutput);
    int ParseRequestDump(const std::string &sInput, std::string* psOutput);
    //!@}

    /*! @name Clear Cache */
    //!@{
    /*! Delete all cached objects.
     *  \warning All external pointers will be invalid \n
     *           Try to avoid using this function */
    void ClearCache();
    //!@}

    /*! @name Utility Functions */
    //!@{
    std::string UtilEscapeString(const std::string &sString);
    std::string UtilCharToHex(const char &cChar);
    std::string UtilIntToString(const int &iInt);
    void        UtilCreateFolder(const std::string &sFolder);
    //!@}

    /*! @name Error Log */
    //!@{
                 void                      ErrorLogReset();
                 void                      ErrorLogAdd(const std::string &sMsg);
    inline const std::vector<std::string>& ErrorLogGet()const {return m_asLog;}
    //!@}

    /*! @name Set Attributes */
    //!@{
    inline void SetSession(const bool &bActive) {m_bActive = bActive;}   //!< \copybrief m_bActive
    /*! */ //!@}

    /*! @name Get Attributes */
    //!@{
    inline const int&         GetGameID()const         {return m_iGameID;}           //!< \copybrief m_iGameID
    inline const std::string& GetGamePrivateKey()const {return m_sGamePrivateKey;}   //!< \copybrief m_sGamePrivateKey
    inline const std::string& GetUserName()const       {return m_sUserName;}         //!< \copybrief m_sUserName
    inline const std::string& GetUserToken()const      {return m_sUserToken;}        //!< \copybrief m_sUserToken
    /*! */ //!@}

    /*! @name Get Processed Attributes */
    //!@{
    inline const std::string& GetProcGameID()const    {return m_sProcGameID;}      //!< \copybrief m_sProcGameID
    inline const std::string& GetProcUserName()const  {return m_sProcUserName;}    //!< \copybrief m_sProcUserName
    inline const std::string& GetProcUserToken()const {return m_sProcUserToken;}   //!< \copybrief m_sProcUserToken
    /*! */ //!@}

    /*! @name Check Status */
    //!@{
    inline const bool& IsActive()const    {return m_bActive;}
    inline const bool& IsConnected()const {return m_bConnected;}
    //!@}

    /*! @name Callback Placeholder */
    //!@{
                          inline int  Null(const std::string &sData, void* pAdd, std::string* psOutput) {if(psOutput) (*psOutput) = sData; return GJ_OK;}
    template <typename D> inline void Null(const D &pObject, void* pData)                               {}
    //!@}


private:
    /*! @name Disable Copy */
    //!@{
    gjAPI(const gjAPI& that) {}
    gjAPI& operator = (const gjAPI& that) {return *this;}
    //!@}

    /*! @name Session Functions */
    //!@{
    int __OpenSession();
    int __PingSession(const bool &bActive);
    int __CloseSession();
    //!@}
};


// ****************************************************************
/* fetch and cache a specific user with user ID */
template <typename T> int gjAPI::gjInterUser::__FetchUser(const int &iID, gjUserPtr* ppOutput, GJ_NETWORK_OUTPUT(gjUserPtr))
{
    const bool bNow = ppOutput ? true : false;

    // fetch current main user
    if(!iID) return this->__FetchUser(m_pAPI->GetUserName(), ppOutput, GJ_NETWORK_OUTPUT_FW);

    // check for cached user
    gjUserPtr pCache = NULL;
    if(this->__CheckCache(iID, &pCache) == GJ_OK)
    {
        if(bNow) (*ppOutput) = pCache;
        else (pOutputObj->*OutputCallback)(pCache, pOutputData);
        return GJ_OK;
    }

    // send get user request
    std::string sResponse;
    if(m_pNetwork->SendRequest("/users/"
                               "?game_id=" + m_pAPI->GetProcGameID() + 
                               "&user_id=" + m_pAPI->UtilIntToString(iID),
                               bNow ? &sResponse : NULL, this, &gjAPI::gjInterUser::__Process, NULL, GJ_NETWORK_OUTPUT_FW)) return GJ_REQUEST_FAILED;

    if(bNow) return this->__Process(sResponse, NULL, ppOutput);
    return GJ_OK;
}


// ****************************************************************
/* fetch and cache a specific user with user name */
template <typename T> int gjAPI::gjInterUser::__FetchUser(const std::string &sName, gjUserPtr* ppOutput, GJ_NETWORK_OUTPUT(gjUserPtr))
{
    if(sName == "") return GJ_INVALID_INPUT;

    const bool bNow = ppOutput ? true : false;

    // check for cached user
    gjUserPtr pCache = NULL;
    if(this->__CheckCache(sName, &pCache) == GJ_OK)
    {
        if(bNow) (*ppOutput) = pCache;
        else (pOutputObj->*OutputCallback)(pCache, pOutputData);
        return GJ_OK;
    }

    // send get user request
    std::string sResponse;
    if(m_pNetwork->SendRequest("/users/"
                               "?game_id="  + m_pAPI->GetProcGameID() + 
                               "&username=" + m_pAPI->UtilEscapeString(sName),
                               bNow ? &sResponse : NULL, this, &gjAPI::gjInterUser::__Process, NULL, GJ_NETWORK_OUTPUT_FW)) return GJ_REQUEST_FAILED;

    if(bNow) return this->__Process(sResponse, NULL, ppOutput);
    return GJ_OK;
}


// ****************************************************************
/* fetch and cache all trophies */
template <typename T> int gjAPI::gjInterTrophy::__FetchTrophies(const long &iAchieved, gjTrophyMap* papOutput, GJ_NETWORK_OUTPUT(gjTrophyMap))
{
    if(!m_pAPI->IsConnected() && m_iCache == 0) return GJ_NOT_CONNECTED;

    const bool bNow = papOutput ? true : false;

    // handle offline-cached trophies
    if(m_pAPI->IsConnected() && m_iCache != 2)
        m_iCache = 0;

    // wait for prefetching
    if(bNow && GJ_API_PREFETCH)
    {
        if(m_apTrophy.size() <= 1)
            m_pNetwork->Wait(1);
    }

    if(m_iCache != 0)
    {
        // check for cached trophies
        gjTrophyMap apCache;
        if(this->__CheckCache(iAchieved, &apCache) == GJ_OK)
        {
            if(bNow) (*papOutput) = apCache;
            else (pOutputObj->*OutputCallback)(apCache, pOutputData);
            return GJ_OK;
        }
    }

    // send get trophies request
    std::string sResponse;
    if(m_pNetwork->SendRequest("/trophies/"
                               "?game_id="    + m_pAPI->GetProcGameID()   + 
                               "&username="   + m_pAPI->GetProcUserName() + 
                               "&user_token=" + m_pAPI->GetProcUserToken(),
                               bNow ? &sResponse : NULL, this, &gjAPI::gjInterTrophy::__Process, (void*)iAchieved, GJ_NETWORK_OUTPUT_FW)) return GJ_REQUEST_FAILED;

    if(bNow) return this->__Process(sResponse, (void*)iAchieved, papOutput);
    return GJ_OK;
}


// ****************************************************************
/* fetch and cache all score tables */
template <typename T> int gjAPI::gjInterScore::__FetchScoreTables(gjScoreTableMap* papOutput, GJ_NETWORK_OUTPUT(gjScoreTableMap))
{
    const bool bNow = papOutput ? true : false;

    // wait for prefetching
    if(bNow && GJ_API_PREFETCH)
    {
        if(m_apScoreTable.size() <= 1)
            m_pNetwork->Wait(1);
    }

    // check for cached score tables
    gjScoreTableMap apCache;
    if(this->__CheckCache(&apCache) == GJ_OK)
    {
        if(bNow) (*papOutput) = apCache;
        else (pOutputObj->*OutputCallback)(apCache, pOutputData);
        return GJ_OK;
    }

    // send get score tables request
    std::string sResponse;
    if(m_pNetwork->SendRequest("/scores/tables/"
                               "?game_id=" + m_pAPI->GetProcGameID(),
                               bNow ? &sResponse : NULL, this, &gjAPI::gjInterScore::__Process, NULL, GJ_NETWORK_OUTPUT_FW)) return GJ_REQUEST_FAILED;

    if(bNow) return this->__Process(sResponse, NULL, papOutput);
    return GJ_OK;
}


// ****************************************************************
/* fetch and semi-cache all data store items */
template <typename T> int gjAPI::gjInterDataStore::__FetchDataItems(gjDataItemMap* papOutput, GJ_NETWORK_OUTPUT(gjDataItemMap))
{
    if(!m_pAPI->IsConnected() && m_iType) return GJ_NOT_CONNECTED;

    const bool bNow = papOutput ? true : false;

    if(m_iType)
    {
        // wait for prefetching
        if(bNow && GJ_API_PREFETCH)
        {
            if(m_apDataItem.empty())
                m_pNetwork->Wait(1);
        }

        // check for cached data store items
        gjDataItemMap apCache;
        if(this->__CheckCache(&apCache) == GJ_OK)
        {
            if(bNow) (*papOutput) = apCache;
            else (pOutputObj->*OutputCallback)(apCache, pOutputData);
            return GJ_OK;
        }
    }

    // access user or global data store items
    const std::string sUserData = m_iType ?
                                  "&username="   + m_pAPI->GetProcUserName()  +
                                  "&user_token=" + m_pAPI->GetProcUserToken() :
                                  "";

    // send get data store item keys request
    std::string sResponse;
    if(m_pNetwork->SendRequest("/data-store/get-keys/"
                               "?game_id=" + m_pAPI->GetProcGameID() +
                               sUserData, bNow ? &sResponse : NULL, this, &gjAPI::gjInterDataStore::__Process, NULL, GJ_NETWORK_OUTPUT_FW)) return GJ_REQUEST_FAILED;

    if(bNow) return this->__Process(sResponse, NULL, papOutput);
    return GJ_OK;
}


// ****************************************************************   
/* download a file from any URL */
template <typename T> int gjAPI::gjInterFile::__DownloadFile(const std::string &sURL, const std::string &sToFolder, const std::string &sFileNameOverwrite, std::string* psOutput, GJ_NETWORK_OUTPUT(std::string))
{
    if(sURL == "" || sToFolder == "") return GJ_INVALID_INPUT;

    const bool bNow = psOutput ? true : false;

    // create output path
    const std::string sFileName = (sFileNameOverwrite == "") ? sURL.substr(sURL.rfind('/')+1) : sFileNameOverwrite;
    const std::string sToFile = sToFolder + "/" + sFileName;

    // check for cached file
    if(this->__CheckCache(sToFile) == GJ_OK)
    {
        if(bNow) (*psOutput) = sToFile;
        else (pOutputObj->*OutputCallback)(sToFile, pOutputData);
        return GJ_OK;
    }

    // create folder
    m_pAPI->UtilCreateFolder(sToFolder);

    // download file
    if(m_pNetwork->DownloadFile(sURL, sToFile, psOutput, this, &gjAPI::gjInterFile::__Process, NULL, GJ_NETWORK_OUTPUT_FW)) return GJ_REQUEST_FAILED;

    if(bNow) this->__Process(*psOutput, NULL, NULL);
    return GJ_OK;
}


// ****************************************************************   
/* post-include because of generic dependencies */
#include "gjNetwork.hpp"
#include "gjUser.h"
#include "gjTrophy.h"
#include "gjScore.h"
#include "gjDataItem.h"


#endif /* GJAPI_H */