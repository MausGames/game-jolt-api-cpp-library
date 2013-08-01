#include "gjAPI.h"

#include <sstream>
#include <iostream>


// ****************************************************************
/* constructor */
gjAPI::gjInterUser::gjInterUser(gjAPI* pAPI, gjNetwork* pNetwork)
: m_pAPI     (pAPI)
, m_pNetwork (pNetwork)
{
    // create NULL user for secure object handling
    gjData pNullData;
    pNullData["id"]         = "0";
    pNullData["username"]   = "NOT FOUND";
    pNullData["type"]       = "Guest";
    pNullData["avatar_url"] = GJ_API_AVATAR_DEFAULT;
    m_apUser[0] = new gjUser(pNullData, m_pAPI);

    // create guest user for secure object handling 
    gjData pGuestData;
    pGuestData["id"]         = "-1";
    pGuestData["username"]   = "Guest";
    pGuestData["type"]       = "Guest";
    pGuestData["avatar_url"] = GJ_API_AVATAR_DEFAULT;
    m_apUser[-1] = new gjUser(pGuestData, m_pAPI);
}


// ****************************************************************
/* destructor */
gjAPI::gjInterUser::~gjInterUser()
{
    // delete all users
    for(auto it = m_apUser.begin(); it != m_apUser.end(); ++it)
        SAFE_DELETE(it->second)

    // clear container
    m_apUser.clear();
}


// ****************************************************************
/* access user objects directly (may block) */
gjUser* gjAPI::gjInterUser::GetUser(const int &iID)
{
    gjUserPtr pOutput;

    if(this->__CheckCache(iID, &pOutput) == GJ_OK) return pOutput;
    if(this->FetchUserNow(iID, &pOutput) == GJ_OK) return pOutput;

    return m_apUser[0];
}

gjUser* gjAPI::gjInterUser::GetUser(const std::string &sName)
{
    gjUserPtr pOutput;

    if(this->__CheckCache(sName, &pOutput) == GJ_OK) return pOutput;
    if(this->FetchUserNow(sName, &pOutput) == GJ_OK) return pOutput;

    return m_apUser[0];
}


// ****************************************************************
/* access main user object directly (may block) */
gjUser* gjAPI::gjInterUser::GetMainUser()
{
    if(!m_pAPI->IsConnected()) return m_apUser[0];
    return this->GetUser(m_pAPI->GetUserName());
}


// ****************************************************************
/* delete all cached user objects */
void gjAPI::gjInterUser::ClearCache()
{
    // save NULL user and guest user
    gjUser* pNull  = m_apUser[0];  m_apUser.erase(0);
    gjUser* pGuest = m_apUser[-1]; m_apUser.erase(-1);

    // delete users
    for(auto it = m_apUser.begin(); it != m_apUser.end(); ++it)
        SAFE_DELETE(it->second)

    // clear container
    m_apUser.clear();
    m_apUser[0]  = pNull;
    m_apUser[-1] = pGuest;
}


// ****************************************************************
/* check for cached user objects */
int gjAPI::gjInterUser::__CheckCache(const int &iID, gjUserPtr* ppOutput)
{
    // retrieve cached user
    if(m_apUser.count(iID))
    {
        if(ppOutput) (*ppOutput) = m_apUser[iID];
        return GJ_OK;
    }
    return GJ_NO_DATA_FOUND;
}

int gjAPI::gjInterUser::__CheckCache(const std::string &sName, gjUserPtr* ppOutput)
{
    // retrieve cached user
    for(auto it = m_apUser.begin(); it != m_apUser.end(); ++it)
    {
        if(it->second->GetName() == sName)
        {
            if(ppOutput) (*ppOutput) = it->second;
            return GJ_OK;
        }
    }
    return GJ_NO_DATA_FOUND;
}


// ****************************************************************
/* process user data and cache user objects */
int gjAPI::gjInterUser::__Process(const std::string &sData, void* pAdd, gjUserPtr* ppOutput)
{
    // parse output
    gjDataList aaReturn;
    if(m_pAPI->ParseRequestKeypair(sData, &aaReturn) != GJ_OK)
    {
        m_pAPI->ErrorLogAdd("API Error: could not parse user");
        if(ppOutput) (*ppOutput) = m_apUser[0];
        return GJ_REQUEST_FAILED;
    }

    // create and cache user object
    gjUser* pNewUser = new gjUser(aaReturn[0], m_pAPI);
    const int iID = pNewUser->GetID();

    if(m_apUser.count(iID)) SAFE_DELETE(pNewUser)
    else m_apUser[iID] = pNewUser;

    if(ppOutput) (*ppOutput) = pNewUser;
    return pNewUser ? GJ_OK : GJ_NO_DATA_FOUND;
}


// ****************************************************************
/* constructor */
gjAPI::gjInterTrophy::gjInterTrophy(gjAPI* pAPI, gjNetwork* pNetwork)
: m_iCache   (0)
, m_pAPI     (pAPI)
, m_pNetwork (pNetwork)
{
    // create NULL trophy for secure object handling
    gjData pNullData;
    pNullData["id"]         = "0";
    pNullData["title"]      = "NOT FOUND";
    pNullData["difficulty"] = "Bronze";
    pNullData["image_url"]  = GJ_API_TROPHY_DEFAULT_1;
    m_apTrophy[0] = new gjTrophy(pNullData, m_pAPI);

    // retrieve offline-cached trophy data
    this->__LoadOffCache();
}


// ****************************************************************
/* destructor */
gjAPI::gjInterTrophy::~gjInterTrophy()
{
    // delete all trophies
    for(auto it = m_apTrophy.begin(); it != m_apTrophy.end(); ++it)
        SAFE_DELETE(it->second)

    // clear container
    m_apTrophy.clear();
}


// ****************************************************************
/* access trophy objects directly (may block) */
gjTrophy* gjAPI::gjInterTrophy::GetTrophy(const int &iID)
{
    if(!m_pAPI->IsConnected() && m_iCache == 0) return m_apTrophy[0];
    if(m_apTrophy.size() <= 1)
    {
        // wait for prefetching
        if(GJ_API_PREFETCH) m_pNetwork->Wait(2);
        if(m_apTrophy.size() <= 1)
        {
            gjTrophyMap papOutput;
            this->FetchTrophiesNow(0, &papOutput);
        }
    }
    return m_apTrophy.count(iID) ? m_apTrophy[iID] : m_apTrophy[0];
}


// ****************************************************************
/* delete all cached trophy objects */
void gjAPI::gjInterTrophy::ClearCache(const bool &bFull)
{
    const bool bRemoveAll = bFull || !GJ_API_OFFCACHE_TROPHY;

    if(bRemoveAll)
    {
        // save NULL trophy
        gjTrophy* pNull = m_apTrophy[0]; m_apTrophy.erase(0);

        // delete trophies
        for(auto it = m_apTrophy.begin(); it != m_apTrophy.end(); ++it)
            SAFE_DELETE(it->second)

        // clear container
        m_apTrophy.clear();
        m_apTrophy[0] = pNull;
    }

    // set cache status
    m_iCache = bRemoveAll ? 0 : 1;
}


// ****************************************************************
/* check for cached trophy objects */
int gjAPI::gjInterTrophy::__CheckCache(const int &iAchieved, gjTrophyMap* papOutput)
{
    // retrieve cached trophies
    if(m_apTrophy.size() > 1)
    {
        if(papOutput)
        {
            for(auto it = m_apTrophy.begin(); it != m_apTrophy.end(); ++it)
            {
                // check for achieved status
                if((iAchieved > 0 &&  it->second->IsAchieved()) ||
                   (iAchieved < 0 && !it->second->IsAchieved()) || !iAchieved)
                    if(it->first) (*papOutput)[it->first] = it->second;
            }
        }
        return GJ_OK;
    }
    return GJ_NO_DATA_FOUND;
}


// ****************************************************************
/* process trophy data and cache trophy objects */
int gjAPI::gjInterTrophy::__Process(const std::string &sData, void* pAdd, gjTrophyMap* papOutput)
{
    // parse output
    gjDataList aaReturn;
    if(m_pAPI->ParseRequestKeypair(sData, &aaReturn) != GJ_OK)
    {
        m_pAPI->ErrorLogAdd("API Error: could not parse trophies");
        return GJ_REQUEST_FAILED;
    }

    // offline-cache trophy data
    this->__SaveOffCache(sData);
    if(m_iCache == 0) m_iCache = 2;

    // create and cache trophy objects
    for(auto it = aaReturn.begin(); it != aaReturn.end(); ++it)
    {
        gjTrophy* pNewTrophy = new gjTrophy(*it, m_pAPI);
        const int iID = pNewTrophy->GetID();

        if(m_apTrophy.count(iID))
        {
            *m_apTrophy[iID] = *pNewTrophy;
            SAFE_DELETE(pNewTrophy)
        }
        else m_apTrophy[iID] = pNewTrophy;
    }

    return (this->__CheckCache((long)pAdd, papOutput) == GJ_OK) ? GJ_OK : GJ_NO_DATA_FOUND;
}


// ****************************************************************
/* save tropy data to a cache file */
void gjAPI::gjInterTrophy::__SaveOffCache(const std::string &sData)
{
    if(!GJ_API_OFFCACHE_TROPHY) return;
    if(m_iCache != 0) return;

    // open cache file
    FILE* pFile = fopen(GJ_API_OFFCACHE_NAME, "w");
    if(pFile)
    {
        // write data and close cache file
        fprintf(pFile, "[TROPHY]\n");
        fprintf(pFile, "%s", sData.c_str());
        fclose(pFile);
    }
}


// ****************************************************************
/* load trophy data from a cache file */
void gjAPI::gjInterTrophy::__LoadOffCache()
{
    if(!GJ_API_OFFCACHE_TROPHY) return;
    if(m_iCache != 0) return;

    // open cache file
    FILE* pFile = fopen(GJ_API_OFFCACHE_NAME, "r");
    if(pFile)
    {
        // read trophy header
        char acHeader[32];
        fscanf(pFile, "%31[^\n]%*c", acHeader);

        // read trophy data
        std::string sData;
        while(true)
        {
            char acLine[1024];
            fscanf(pFile, "%1023[^\n]%*c", acLine);
            if(feof(pFile)) break;

            if(strlen(acLine) > 1)
            {
                sData += acLine;
                sData += '\n';
            }
        }

        // close cache file
        fclose(pFile);
        
        if(!sData.empty())
        {
            // flag offline caching and load offline-cached trophies
            m_iCache = 1;
            this->__Process(sData, NULL, NULL);
        }
    }
}


// ****************************************************************
/* constructor */
gjAPI::gjInterScore::gjInterScore(gjAPI* pAPI, gjNetwork* pNetwork)
: m_pAPI     (pAPI)
, m_pNetwork (pNetwork)
{
    // create NULL score table for secure object handling
    gjData pNullData;
    pNullData["id"]   = "0";
    pNullData["name"] = "NOT FOUND";
    m_apScoreTable[0] = new gjScoreTable(pNullData, m_pAPI);
}


// ****************************************************************
/* destructor */
gjAPI::gjInterScore::~gjInterScore()
{
    // delete all score tables and scores entries
    for(auto it = m_apScoreTable.begin(); it != m_apScoreTable.end(); ++it)
        SAFE_DELETE(it->second)
    
    // clear container
    m_apScoreTable.clear();
}


// ****************************************************************   
/* access score table objects directly (may block) */
gjScoreTable* gjAPI::gjInterScore::GetScoreTable(const int &iID)
{
    if(m_apScoreTable.size() <= 1)
    {
        // wait for prefetching
        if(GJ_API_PREFETCH) m_pNetwork->Wait(2);
        if(m_apScoreTable.size() <= 1)
        {
            gjScoreTableMap papOutput;
            this->FetchScoreTablesNow(&papOutput);
        }
    }
    gjScoreTable* pPrimary = gjScoreTable::GetPrimary();
    return iID ? (m_apScoreTable.count(iID) ? m_apScoreTable[iID] : m_apScoreTable[0]) : (pPrimary ? pPrimary : m_apScoreTable[0]);
}


// ****************************************************************
/* delete all cached score table objects and score entries */
void gjAPI::gjInterScore::ClearCache()
{
    // save NULL score table
    gjScoreTable* pNull = m_apScoreTable[0]; m_apScoreTable.erase(0);

    // delete score tables and scores entries
    for(auto it = m_apScoreTable.begin(); it != m_apScoreTable.end(); ++it)
        SAFE_DELETE(it->second)
    
    // clear container
    m_apScoreTable.clear();
    m_apScoreTable[0] = pNull;
}


// ****************************************************************
/* check for cached score table objects */
int gjAPI::gjInterScore::__CheckCache(gjScoreTableMap* papOutput)
{
    // retrieve cached score tables
    if(m_apScoreTable.size() > 1)
    {
        if(papOutput)
        {
            for(auto it = m_apScoreTable.begin(); it != m_apScoreTable.end(); ++it)
                if(it->first) (*papOutput)[it->first] = it->second;
        }
        return GJ_OK;
    }
    return GJ_NO_DATA_FOUND;
}


// ****************************************************************
/* process score table data and cache score table objects */
int gjAPI::gjInterScore::__Process(const std::string &sData, void* pAdd, gjScoreTableMap* papOutput)
{
    // parse output
    gjDataList aaReturn;
    if(m_pAPI->ParseRequestKeypair(sData, &aaReturn) != GJ_OK)
    {
        m_pAPI->ErrorLogAdd("API Error: could not parse score tables");
        return GJ_REQUEST_FAILED;
    }

    // create and cache score tables
    for(auto it = aaReturn.begin(); it != aaReturn.end(); ++it)
    {
        gjScoreTable* pNewScoreTable = new gjScoreTable(*it, m_pAPI);
        const int iID = pNewScoreTable->GetID();

        if(m_apScoreTable.count(iID)) SAFE_DELETE(pNewScoreTable)
        else m_apScoreTable[iID] = pNewScoreTable;
    }

    return (this->__CheckCache(papOutput) == GJ_OK) ? GJ_OK : GJ_NO_DATA_FOUND;
}


// ****************************************************************
/* constructor */
gjAPI::gjInterDataStore::gjInterDataStore(const int &iType, gjAPI* pAPI, gjNetwork* pNetwork)
: m_iType    (iType)
, m_pAPI     (pAPI)
, m_pNetwork (pNetwork)
{
}


// ****************************************************************
/* destructor */
gjAPI::gjInterDataStore::~gjInterDataStore()
{
    this->ClearCache();
}


// ****************************************************************
/* create and access data store items directly */
gjDataItem* gjAPI::gjInterDataStore::GetDataItem(const std::string &sKey)
{
    // create new data store item
    if(!m_apDataItem.count(sKey))
    {
        gjData pDataItemData;
        pDataItemData["key"] = sKey;
        m_apDataItem[sKey] = new gjDataItem(pDataItemData,m_iType, m_pAPI);
    }

    return m_apDataItem.count(sKey) ? m_apDataItem[sKey] : NULL;
}


// ****************************************************************
/* delete all cached data store items */
void gjAPI::gjInterDataStore::ClearCache()
{
    // delete data store items
    for(auto it = m_apDataItem.begin(); it != m_apDataItem.end(); ++it)
        SAFE_DELETE(it->second)
    
    // clear container
    m_apDataItem.clear();
}


// ****************************************************************
/* check for cached data store items */
int gjAPI::gjInterDataStore::__CheckCache(gjDataItemMap* papOutput)
{
    // retrieve cached data store items
    if(!m_apDataItem.empty())
    {
        if(papOutput)
        {
            for(auto it = m_apDataItem.begin(); it != m_apDataItem.end(); ++it)
                (*papOutput)[it->first] = it->second;
        }
        return GJ_OK;
    }
    return GJ_NO_DATA_FOUND;
}


// ****************************************************************
/* process data store data and cache data store items */
int gjAPI::gjInterDataStore::__Process(const std::string &sData, void* pAdd, gjDataItemMap* papOutput)
{
    // parse output
    gjDataList aaReturn;
    if(m_pAPI->ParseRequestKeypair(sData, &aaReturn) != GJ_OK)
    {
        m_pAPI->ErrorLogAdd("API Error: could not parse data store items");
        return GJ_REQUEST_FAILED;
    }

    // create and cache data store items
    for(auto it = aaReturn.begin(); it != aaReturn.end(); ++it)
    {
        gjDataItem* pNewDataItem = new gjDataItem(*it, m_iType, m_pAPI);
        const std::string sKey = pNewDataItem->GetKey();

        if(m_apDataItem.count(sKey)) SAFE_DELETE(pNewDataItem)
        else m_apDataItem[sKey] = pNewDataItem;

        if(papOutput) (*papOutput)[sKey] = m_apDataItem[sKey];
    }

    return aaReturn.size() ? GJ_OK : GJ_NO_DATA_FOUND;
}


// ****************************************************************
/* constructor */
gjAPI::gjInterFile::gjInterFile(gjAPI* pAPI, gjNetwork* pNetwork)
: m_pAPI     (pAPI)
, m_pNetwork (pNetwork)
{
    // reserve some memory
    m_asFile.reserve(GJ_API_RESERVE_FILE);
}


// ****************************************************************
/* destructor */
gjAPI::gjInterFile::~gjInterFile()
{
    this->ClearCache();
}


// ****************************************************************
/* delete all cached file paths */
void gjAPI::gjInterFile::ClearCache()
{
    // clear container
    m_asFile.clear();
}


// ****************************************************************
/* check for cached files */
int gjAPI::gjInterFile::__CheckCache(const std::string &sPath)
{
    // compare cached file paths
    for(size_t i = 0; i < m_asFile.size(); ++i)
    {
        if(sPath == m_asFile[i])
            return GJ_OK;
    }
    return GJ_NO_DATA_FOUND;
}


// ****************************************************************
/* process downloaded file */
int gjAPI::gjInterFile::__Process(const std::string &sData, void* pAdd, std::string* psOutput)
{
    // save path of the file
    if(this->__CheckCache(sData) != GJ_OK) m_asFile.push_back(sData);
    if(psOutput) (*psOutput) = sData;

    return GJ_OK;
}


// ****************************************************************
/* constructor */
gjAPI::gjAPI(const int &iGameID, const std::string &sGamePrivateKey)
: m_iGameID          (iGameID)
, m_sGamePrivateKey  (sGamePrivateKey)
, m_sUserName        ("")
, m_sUserToken       ("")
, m_iNextPing        (0)
, m_bActive          (false)
, m_bConnected       (false)
, m_sProcUserName    ("")
, m_sProcUserToken   ("")
{
    // pre-process the game ID
    m_sProcGameID = this->UtilIntToString(m_iGameID);

    // create network object
    m_pNetwork = new gjNetwork(this);

    // create sub-interface objects
    m_pInterUser            = new gjInterUser(this, m_pNetwork);
    m_pInterTrophy          = new gjInterTrophy(this, m_pNetwork);
    m_pInterScore           = new gjInterScore(this, m_pNetwork); 
    m_pInterDataStoreGlobal = new gjInterDataStore(0, this, m_pNetwork);
    m_pInterDataStoreUser   = new gjInterDataStore(1, this, m_pNetwork);
    m_pInterFile            = new gjInterFile(this, m_pNetwork);

    // prefetch score tables
    if(GJ_API_PREFETCH) m_pInterScore->FetchScoreTablesCall(GJ_NETWORK_NULL_THIS(gjScoreTableMap));

    // init error log
    this->ErrorLogReset();
}


// ****************************************************************
/* destructor */
gjAPI::~gjAPI()
{
    // logout last user
    this->Logout();

    // delete network object
    SAFE_DELETE(m_pNetwork)

    // delete sub-interface objects
    SAFE_DELETE(m_pInterUser)
    SAFE_DELETE(m_pInterTrophy)
    SAFE_DELETE(m_pInterScore)
    SAFE_DELETE(m_pInterDataStoreGlobal)
    SAFE_DELETE(m_pInterDataStoreUser)
    SAFE_DELETE(m_pInterFile)

    // exit error log
    this->ErrorLogReset();
}


// ****************************************************************
/* main update function of the library */
void gjAPI::Update()
{
    // update network object
    m_pNetwork->Update();

    if(!this->IsConnected()) return;

    if(m_iNextPing)
    {
        // update ping for the user session
        const time_t iCurTime = time(NULL);
        if(iCurTime >= m_iNextPing)
        {
            m_iNextPing = iCurTime + GJ_API_PING_TIME;
            this->__PingSession(m_bActive);
        }
    }
}


// ****************************************************************
/* login with specific user */
int gjAPI::Login(const bool bSession, const std::string &sUserName, const std::string &sUserToken)
{
    if(this->IsConnected()) return GJ_INVALID_CALL;
    if(sUserName == "" || sUserToken == "")  return GJ_INVALID_INPUT;

    // authenticate user
    std::string sCheck;
    if(m_pNetwork->SendRequest("/users/auth/"
                               "?game_id="    + m_sProcGameID                     +
                               "&username="   + this->UtilEscapeString(sUserName) +
                               "&user_token=" + this->UtilEscapeString(sUserToken),
                               &sCheck, this, &gjAPI::Null, NULL, GJ_NETWORK_NULL_THIS(std::string))) return GJ_REQUEST_FAILED;

    // check for success
    gjDataList aaReturn;
    if(this->ParseRequestKeypair(sCheck, &aaReturn) != GJ_OK)
    {
        this->ErrorLogAdd("API Error: could not authenticate user <" + sUserName + ">");
        return GJ_REQUEST_FAILED;
    }
    
    // set main user data
    m_sUserName      = sUserName;
    m_sUserToken     = sUserToken;
    m_sProcUserName  = this->UtilEscapeString(m_sUserName);
    m_sProcUserToken = this->UtilEscapeString(m_sUserToken);
    
    // set connection
    m_bConnected = true;

    // open the user session
    if(bSession) this->__OpenSession();

    // prefetch user data
    if(GJ_API_PREFETCH)
    {
        m_pInterUser->FetchUserCall(0, GJ_NETWORK_NULL_THIS(gjUserPtr));
        m_pInterTrophy->FetchTrophiesCall(0, GJ_NETWORK_NULL_THIS(gjTrophyMap));
        m_pInterDataStoreUser->FetchDataItemsCall(GJ_NETWORK_NULL_THIS(gjDataItemMap));
    }

    return GJ_OK;
}

int gjAPI::Login(const bool bSession, std::string sCredPath)
{
    // open credentials file
    FILE* pFile = fopen(sCredPath.c_str(), "rb");
    if(!pFile) return GJ_FILE_ERROR;

    char acName[128], acToken[128];
    char* pcEnd;
    
    // get user name
    fscanf(pFile, "%127[^\n]%*c", acName);
    pcEnd = strchr(acName, 13);
    if(pcEnd) *pcEnd = '\0';

    // get user token
    fscanf(pFile, "%127[^\n]%*c", acToken);
    pcEnd = strchr(acToken, 13);
    if(pcEnd) *pcEnd = '\0';

    // close file and login
    fclose(pFile);
    return this->Login(bSession, acName, acToken);
}


// ****************************************************************
/* logout with specific user */
int gjAPI::Logout()
{
    if(!this->IsConnected()) return GJ_NOT_CONNECTED;

    // clear user specific data
    m_pInterTrophy->ClearCache(false);
    m_pInterDataStoreUser->ClearCache();

    // close the user session
    if(m_iNextPing) this->__CloseSession();

    // clear main user data
    m_sUserName      = "";
    m_sUserToken     = "";
    m_sProcUserName  = "";
    m_sProcUserToken = "";

    // clear connection
    m_bConnected = false;

    return GJ_OK;
}


// ****************************************************************   
/* parse a valid response string in keypair format */
int gjAPI::ParseRequestKeypair(const std::string &sInput, gjDataList* paaOutput)
{
    if(!paaOutput) return GJ_INVALID_INPUT;

    gjData aData;
    std::istringstream sStream(sInput);
    std::string sToken;

    // traverse input string
    while(std::getline(sStream, sToken))
    {
        if(sToken.back() == 13) sToken.pop_back(); // fixes problems between windows and linux

        // separate key and value
        const int iPos           = sToken.find(':');
        const std::string sKey   = sToken.substr(0, iPos);
        const std::string sValue = sToken.substr(iPos + 2, sToken.length() - iPos - 3);

        // next data block on same key
        if(aData.count(sKey))
        {
            paaOutput->push_back(aData);
            aData.clear();
        }

        // create key and save value
        aData[sKey] = sValue;
    }

    // insert last data block and check size
    if(!aData.empty()) paaOutput->push_back(aData);
    if(paaOutput->empty())
    {
        this->ErrorLogAdd("API Error: string parsing failed");
        return GJ_INVALID_INPUT;
    }

    // check for failed request
    if(paaOutput->front()["success"] != "true")
    {
        this->ErrorLogAdd("API Error: request was unsuccessful");
        this->ErrorLogAdd("API Error: " + paaOutput->front()["message"]);
        return GJ_REQUEST_FAILED;
    }

    return GJ_OK;
}


// ****************************************************************   
/* parse a valid response string in Dump format */
int gjAPI::ParseRequestDump(const std::string &sInput, std::string* psOutput)
{
    if(!psOutput) return GJ_INVALID_INPUT;

    // read status
    const std::string sStatus = sInput.substr(0, sInput.find_first_of(13));

    // read data
    *psOutput = sInput.substr(sStatus.length()+2);

    // check for failed request
    if(sStatus != "SUCCESS")
    {
        this->ErrorLogAdd("API Error: request was unsuccessful");
        this->ErrorLogAdd("API Error: " + *psOutput);
        return GJ_REQUEST_FAILED;
    }

    return GJ_OK;
}


// ****************************************************************
/* delete all cached objects */
void gjAPI::ClearCache()
{
    // clear cache of all sub-interface
    m_pInterUser->ClearCache();
    m_pInterTrophy->ClearCache(true);
    m_pInterScore->ClearCache();
    m_pInterDataStoreGlobal->ClearCache();
    m_pInterDataStoreUser->ClearCache();
    m_pInterFile->ClearCache(); 
}


// ****************************************************************
/* escape a string for proper url calling */
std::string gjAPI::UtilEscapeString(const std::string &sString)
{
    std::string sOutput = "";

    // traverse input string
    for(size_t i = 0; i < sString.length(); ++i)
    {
        // check the character type
        if
        (
            (48 <= sString[i] && sString[i] <=  57) || // 0-9
            (65 <= sString[i] && sString[i] <=  90) || // abc...xyz
            (97 <= sString[i] && sString[i] <= 122) || // ABC...XYZ
            (
                sString[i] == '~' || sString[i] == '.'  ||
                sString[i] == '-' || sString[i] == '_'
            )
        )
        {
            // add valid character
            sOutput += sString[i];
        }
        else
        {
            // convert character to hexadecimal value
            sOutput += "%" + UtilCharToHex(sString[i]); 
        }
    }

    return sOutput;
}


// ****************************************************************
/* convert a character into his hexadecimal value (C++98) */
std::string gjAPI::UtilCharToHex(const char &cChar)
{
    int iValue = (int)cChar;
    if(iValue < 0) iValue += 256;

    std::ostringstream sOut;
    sOut << std::hex << iValue;
    return sOut.str();
}


// ****************************************************************
/* simply convert an integer into a string (C++98) */
std::string gjAPI::UtilIntToString(const int &iInt)
{
    std::ostringstream sOut;
    sOut << iInt;
    return sOut.str();
}


// ****************************************************************
/* create a folder hierarchy */
void gjAPI::UtilCreateFolder(const std::string &sFolder)
{
    int iPos = 0;

    do
    {
        // get next subfolder
        iPos = sFolder.find('/', iPos+2);
        const std::string sSubFolder = sFolder.substr(0, iPos);
        
        // create subfolder
#ifdef _WIN32
        CreateDirectoryA(sSubFolder.c_str(), NULL);
#else
        mkdir(sSubFolder.c_str(), S_IRWXU);
#endif
    }
    while(iPos >= 0);
}


// ****************************************************************  
/* reset error log */
void gjAPI::ErrorLogReset()
{
    if(GJ_API_LOGFILE)
    {
        // remove error log file if empty
        if(m_asLog.empty())
            remove(GJ_API_LOGFILE_NAME);
    }

    // clear all log entries
    m_asLog.clear();
}


// ****************************************************************  
/* add error log entry */
void gjAPI::ErrorLogAdd(const std::string &sMsg)
{
    // add message
    m_asLog.push_back(sMsg);

    if(GJ_API_LOGFILE)
    {
        // add message to error log file
        FILE* pFile = fopen(GJ_API_LOGFILE_NAME, "a");
        if(pFile)
        {
            fprintf(pFile, "%s\n", sMsg.c_str());
            fclose(pFile);
        }
    }

#ifdef _DEBUG
    // print message to terminal
    std::cerr << "[!GJ] " << sMsg << std::endl;
#endif
}


// ****************************************************************
/* open the user session */
int gjAPI::__OpenSession()
{
    if(!this->IsConnected()) return GJ_NOT_CONNECTED;

    // send non-blocking open request
    if(m_pNetwork->SendRequest("/sessions/open/"
                               "?game_id="    + m_sProcGameID   +
                               "&username="   + m_sProcUserName +
                               "&user_token=" + m_sProcUserToken,
                               NULL, this, &gjAPI::Null, NULL, GJ_NETWORK_NULL_THIS(std::string))) return GJ_REQUEST_FAILED;

    // init session attributes
    m_iNextPing = time(NULL) + GJ_API_PING_TIME;
    m_bActive   = true;

    return GJ_OK;
}


// ****************************************************************
/* ping the user session */
int gjAPI::__PingSession(const bool &bActive)
{
    if(!this->IsConnected()) return GJ_NOT_CONNECTED;

    // use active status
    const std::string sActive = bActive ? "active" : "idle";

    // send non-blocking ping request
    if(m_pNetwork->SendRequest("/sessions/ping/"
                               "?game_id="    + m_sProcGameID    + 
                               "&username="   + m_sProcUserName  + 
                               "&user_token=" + m_sProcUserToken + 
                               "&status="     + sActive,
                               NULL, this, &gjAPI::Null, NULL, GJ_NETWORK_NULL_THIS(std::string))) return GJ_REQUEST_FAILED;

    return GJ_OK;
}


// ****************************************************************
/* close the user session */
int gjAPI::__CloseSession()
{
    if(!this->IsConnected()) return GJ_NOT_CONNECTED;

    // send non-blocking close request
    if(m_pNetwork->SendRequest("/sessions/close/"
                               "?game_id="    + m_sProcGameID   +
                               "&username="   + m_sProcUserName +
                               "&user_token=" + m_sProcUserToken,
                               NULL, this, &gjAPI::Null, NULL, GJ_NETWORK_NULL_THIS(std::string))) return GJ_REQUEST_FAILED;

    // clear session attributes
    m_iNextPing = 0;

    return GJ_OK;
}