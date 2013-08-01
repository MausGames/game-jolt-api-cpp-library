#pragma once
#ifndef GJSCORE_H
#define GJSCORE_H


// ****************************************************************
/*! Score table object class.\n
 *  http://gamejolt.com/api/doc/game/scores/  
 *  \brief Score Table Object*/
class gjScoreTable
{
private:
    int m_iID;                          //!< ID of the score table
    std::string m_sTitle;               //!< title/name of the score table
    std::string m_sDescription;         //!< description text of the score table
                                        
    int m_iSortDir;                     //!< sort direction of the score table (see #GJ_SORT_DIRECTION)
                                        
    bool m_bPrimary;                    //!< primary status
    static gjScoreTable* s_pPrimary;    //!< pointer to the primary score table
                                        
    std::vector<gjScore*> m_apScore;    //!< semi-cached score entries
    std::vector<gjScore*> m_apVerify;   //!< verification list with temporary score entries (helper)

    gjAPI* m_pAPI;                      //!< main interface access pointer


public:
    gjScoreTable(const gjData &aScoreTableData, gjAPI* pAPI);
    ~gjScoreTable();

    /*! @name Fetch Scores Request */
    //!@{
    /*! Fetch and semi-cache specific score entries of this score table through an API request.
     *  \pre    Login maybe required
     *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
     *  @param  bOnlyUser Fetch only score entries from the current main user (Login required)
     *  @param  iLimit    Max number of fetched score entries
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_NOT_CONNECTED</b> if connection/login is missing\n
     *          <b>GJ_NO_DATA_FOUND</b> if no scores where found\n
     *          (see #GJ_ERROR) */
                          inline int FetchScoresNow(const bool bOnlyUser, const int &iLimit, gjScoreList* papOutput)          {if(!papOutput) return GJ_INVALID_INPUT; return this->__FetchScores(bOnlyUser, iLimit, papOutput, GJ_NETWORK_NULL_API(gjScoreList));}
    template <typename T> inline int FetchScoresCall(const bool bOnlyUser, const int &iLimit, GJ_NETWORK_OUTPUT(gjScoreList)) {return this->__FetchScores(bOnlyUser, iLimit, NULL, GJ_NETWORK_OUTPUT_FW);}
    //!@}
    
    /*! @name Add Score Request */
    //!@{
    /*! Add a new score entry to this score table through an API request.\n
     *  May does nothing, if score entry was already added (compares name and sort).\n
     *  May does nothing, if main user has a better highscore.
     *  \pre    Login maybe required
     *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
     *  @param  sScore     Score string
     *  @param  iSort      Numerical sort value of the score
     *  @param  sExtraData Extra data associated with the score
     *  @param  sGuestName Name of a guest user (leave empty to use the current main user, Login required)
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_REQUEST_CANCELED</b> if score entry was already added or if main user has a better highscore\n
     *          <b>GJ_INVALID_INPUT</b> if score string is empty or sort value is zero\n
     *          <b>GJ_NOT_CONNECTED</b> if connection/login is missing\n
     *          (see #GJ_ERROR) */
                          inline int AddScoreNow(const std::string &sScore, const int &iSort, const std::string &sExtraData, const std::string &sGuestName)                                 {return this->__AddScore(sScore, iSort, sExtraData, sGuestName, true, GJ_NETWORK_NULL_API(gjScorePtr));}
                          inline int AddScoreCall(const std::string &sScore, const int &iSort, const std::string &sExtraData, const std::string &sGuestName)                                {return this->__AddScore(sScore, iSort, sExtraData, sGuestName, false, GJ_NETWORK_NULL_API(gjScorePtr));}
    template <typename T> inline int AddScoreCall(const std::string &sScore, const int &iSort, const std::string &sExtraData, const std::string &sGuestName, GJ_NETWORK_OUTPUT(gjScorePtr)) {return this->__AddScore(sScore, iSort, sExtraData, sGuestName, false, GJ_NETWORK_OUTPUT_FW);}
    //!@}

    /*! @name Add Score Request Base64 */
    //!@{
    /*! Like \link AddScoreNow AddScore\endlink\n
     *  Allows to send extra data in binary form.
     *  \pre    Login maybe required
     *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
     *  @param  sScore     Score string
     *  @param  iSort      Numerical sort value of the score
     *  @param  pExtraData Extra data in binary form associated with the score
     *  @param  iExtraSize Size of the extra data
     *  @param  sGuestName Name of a guest user (leave empty to use the current main user, Login required)
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_REQUEST_CANCELED</b> if score entry was already added or if main user has a better highscore\n
     *          <b>GJ_INVALID_INPUT</b> if score string is empty or sort value is zero\n
     *          <b>GJ_NOT_CONNECTED</b> if connection/login is missing\n
     *          (see #GJ_ERROR) */
                          inline int AddScoreBase64Now(const std::string &sScore, const int &iSort, void* pExtraData, const size_t &iExtraSize, const std::string &sGuestName)                                 {return this->__AddScoreBase64(sScore, iSort, pExtraData, iExtraSize, sGuestName, true, GJ_NETWORK_NULL_API(gjScorePtr));}
                          inline int AddScoreBase64Call(const std::string &sScore, const int &iSort, void* pExtraData, const size_t &iExtraSize, const std::string &sGuestName)                                {return this->__AddScoreBase64(sScore, iSort, pExtraData, iExtraSize, sGuestName, false, GJ_NETWORK_NULL_API(gjScorePtr));}
    template <typename T> inline int AddScoreBase64Call(const std::string &sScore, const int &iSort, void* pExtraData, const size_t &iExtraSize, const std::string &sGuestName, GJ_NETWORK_OUTPUT(gjScorePtr)) {return this->__AddScoreBase64(sScore, iSort, pExtraData, iExtraSize, sGuestName, false, GJ_NETWORK_OUTPUT_FW);}
    //!@}

    /*! @name Get Attributes */
    //!@{
    inline const int&         GetID()const            {return m_iID;}            //!< \copybrief m_iID
    inline const std::string& GetTitle()const         {return m_sTitle;}         //!< \copybrief m_sTitle
    inline const std::string& GetDescription()const   {return m_sDescription;}   //!< \copybrief m_sDescription
    inline const int&         GetSortDirection()const {return m_iSortDir;}       //!< \copybrief m_iSortDir
    /*! */ //!@}

    /*! @name Check Status */
    //!@{
    inline bool IsPrimary()const {return m_bPrimary;}
    //!@}

    /*! @name Get Static Attributes */
    //!@{
    static inline gjScoreTable* GetPrimary() {return s_pPrimary;}   //!< \copybrief s_pPrimary
    /*! */ //!@}


private:
    /*! @name Disable Copy */
    //!@{
    gjScoreTable(const gjScoreTable& that) {}
    gjScoreTable& operator = (const gjScoreTable& that) {return *this;}
    //!@}

    /*! @name Superior Request Functions */
    //!@{
    template <typename T> int __FetchScores(const bool bOnlyUser, const int &iLimit, gjScoreList* papOutput, GJ_NETWORK_OUTPUT(gjScoreList));
    template <typename T> int __AddScore(const std::string &sScore, const int &iSort, const std::string &sExtraData, const std::string &sGuestName, const bool &bNow, GJ_NETWORK_OUTPUT(gjScorePtr));
    template <typename T> int __AddScoreBase64(const std::string &sScore, const int &iSort, void* pExtraData, const size_t &iExtraSize, const std::string &sGuestName, const bool &bNow, GJ_NETWORK_OUTPUT(gjScorePtr));
    //!@}

    /*! @name Management Functions */
    //!@{
    int __CheckCache(const bool bOnlyUser, const int &iLimit, gjScoreList* papOutput);
    int __Process(const std::string &sData, void* pAdd, gjScoreList* papOutput);
    //!@}

    /*! @name Callback Functions */
    //!@{
    int __AddScoreCallback(const std::string &sData, void* pAdd, gjScorePtr* pOutput);
    //!@}
};


// ****************************************************************
/*! Score entry class.\n
 *  http://gamejolt.com/api/doc/game/scores/ 
 *  \brief Score Entry Object */
class gjScore
{
private:
    std::string m_sScore;          //!< score string
    int m_iSort;                   //!< numerical sort value of the score
    std::string m_sExtraData;      //!< extra data associated with the score
                                   
    int m_iUserID;                 //!< ID of the user, 0 if guest
    std::string m_sUserName;       //!< display name of the user or guest
                                   
    std::string m_sDate;           //!< time string when the score was logged (e.g. 4 weeks ago)

    gjScoreTable* m_pScoreTable;   //!< associated score table

    gjAPI* m_pAPI;                 //!< main interface access pointer


public:
    gjScore(const gjData &aScoreData, gjScoreTable* pScoreTable, gjAPI* pAPI);

    /*! @name Fetch User Request */
    //!@{
    /*! Fetch and cache the associated user through an API request.
     *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_NOT_CONNECTED</b> if connection/login is missing\n
     *          (see #GJ_ERROR) */
                          inline int FetchUserNow(gjUserPtr* ppOutput)           {return m_pAPI->InterUser()->FetchUserNow(this->IsGuest() ? -1 : m_iUserID, ppOutput);}
    template <typename T> inline int FetchUserCall(GJ_NETWORK_OUTPUT(gjUserPtr)) {return m_pAPI->InterUser()->FetchUserCall(this->IsGuest() ? -1 : m_iUserID, GJ_NETWORK_OUTPUT_FW);}
    //!@}

    /*! @name Get Attributes */
    //!@{
    inline const std::string&  GetScore()const      {return m_sScore;}        //!< \copybrief m_sScore
    inline const int&          GetSort()const       {return m_iSort;}         //!< \copybrief m_iSort
    inline const std::string&  GetExtraData()const  {return m_sExtraData;}    //!< \copybrief m_sExtraData
    inline const int&          GetUserID()const     {return m_iUserID;}       //!< \copybrief m_iUserID
    inline const std::string&  GetUserName()const   {return m_sUserName;}     //!< \copybrief m_sUserName
    inline const std::string&  GetDate()const       {return m_sDate;}         //!< \copybrief m_sDate
    inline const gjScoreTable* GetScoreTable()const {return m_pScoreTable;}   //!< \copybrief m_pScoreTable
    /*! */ //!@}

    /*! @name Get Attributes Base64 */
    //!@{
    inline int GetExtraDataBase64(void* pTarget, const size_t &iSize)const {if(!pTarget || iSize <= 0) return GJ_INVALID_INPUT; base64_decode(m_sExtraData.c_str(), (unsigned char*)pTarget, iSize); return GJ_OK;}   //!< \copybrief m_sExtraData
    /*! */ //!@}

    /*! @name Check Status */
    //!@{
    inline bool IsGuest()const {return m_iUserID ? false : true;}
    //!@}


private:
    /*! @name Disable Copy */
    //!@{
    gjScore(const gjScore& that) {}
    gjScore& operator = (const gjScore& that) {return *this;}
    //!@}
};


// ****************************************************************
/*! @name Sorting Callbacks */
//!@{
bool SortAscending(const gjScore* i, const gjScore* j);
bool SortDescending(const gjScore* i, const gjScore* j);
//!@}


// ****************************************************************
/* fetch and semi-cache specific score entries of this score table */
template <typename T> int gjScoreTable::__FetchScores(const bool bOnlyUser, const int &iLimit, gjScoreList* papOutput, GJ_NETWORK_OUTPUT(gjScoreList))
{
    if(!m_pAPI->IsConnected() && bOnlyUser) return GJ_NOT_CONNECTED;
    if(iLimit <= 0) return GJ_INVALID_INPUT;

    const bool bNow = papOutput ? true : false;

    if(bOnlyUser && m_iSortDir)
    {
        // check for cached score entries
        gjScoreList apCache;
        if(this->__CheckCache(bOnlyUser, iLimit, &apCache) == GJ_OK)
        {
            if(bNow) (*papOutput) = apCache;
            else (pOutputObj->*OutputCallback)(apCache, pOutputData);
            return GJ_OK;
        }
    }

    // only scores from the main user or all scores
    const std::string sUserData = bOnlyUser ?
                                  "&username="   + m_pAPI->GetProcUserName()  +
                                  "&user_token=" + m_pAPI->GetProcUserToken() :
                                  "";

    // send get score request
    std::string sResponse;
    if(m_pAPI->SendRequest("/scores/"
                           "?game_id="  + m_pAPI->GetProcGameID()         + 
                           "&table_id=" + m_pAPI->UtilIntToString(m_iID)  +
                           "&limit="    + m_pAPI->UtilIntToString(iLimit) + 
                           sUserData, bNow ? &sResponse : NULL, this, &gjScoreTable::__Process, NULL, GJ_NETWORK_OUTPUT_FW)) return GJ_REQUEST_FAILED;

    if(bNow) return this->__Process(sResponse, NULL, papOutput);
    return GJ_OK;
}


// ****************************************************************
/* add score entry to this score table */
template <typename T> int gjScoreTable::__AddScore(const std::string &sScore, const int &iSort, const std::string &sExtraData, const std::string &sGuestName, const bool &bNow, GJ_NETWORK_OUTPUT(gjScorePtr))
{
    if(sScore == "" || iSort == 0) return GJ_INVALID_INPUT;

    const bool bGuest = (sGuestName == "") ? true : false;
    if(!m_pAPI->IsConnected() && !bGuest) return GJ_NOT_CONNECTED;
    
    if(m_iSortDir && !bGuest)
    {
        // check for cached best score and cancel request
        gjScoreList apBestScore;
        if(this->__CheckCache(true, 1, &apBestScore) == GJ_OK)
        {
            if(((m_iSortDir == GJ_SORT_DESC) && (iSort <= apBestScore[0]->GetSort())) ||
               ((m_iSortDir == GJ_SORT_ASC)  && (iSort >= apBestScore[0]->GetSort())))
                return GJ_REQUEST_CANCELED;
        }
    }

    // check if specific score entry is already available and cancel request
    const std::string sUserName = bGuest ? sGuestName : m_pAPI->GetUserName();
    for(size_t i = 0; i < m_apScore.size(); ++i)
    {
        gjScore* pOldScore = m_apScore[i];

        if(pOldScore->GetUserName() == sUserName &&
           pOldScore->GetSort()     == iSort)
        {
            // specific score entry already available
            return GJ_REQUEST_CANCELED;
        }
    }

    // create temporary score entry
    gjData asScoreData;
    if(!bGuest)
    {
        // fetch cached main user
        gjUserPtr pMainUser;
        const int iError = m_pAPI->InterUser()->FetchUserNow(0, &pMainUser);
        if(iError) return iError;

        asScoreData["user_id"] = m_pAPI->UtilIntToString(pMainUser->GetID());
        asScoreData["user"]    = pMainUser->GetName();
    }
    asScoreData["score"]      = sScore;
    asScoreData["sort"]       = m_pAPI->UtilIntToString(iSort);
    asScoreData["extra_data"] = sExtraData;
    asScoreData["stored"]     = GJ_API_NOW;
    asScoreData["guest"]      = sGuestName;

    // add score entry to verification list
    gjScore* pVerify = new gjScore(asScoreData, this, m_pAPI);
    m_apVerify.push_back(pVerify);

    // use user data or guest name
    const std::string sUserData = bGuest ?
                                  "&username="   + m_pAPI->GetProcUserName()  +
                                  "&user_token=" + m_pAPI->GetProcUserToken() :
                                  "&guest="      + m_pAPI->UtilEscapeString(sGuestName);

    // send add score request
    std::string sResponse;
    if(m_pAPI->SendRequest("/scores/add/"
                           "?game_id="    + m_pAPI->GetProcGameID()              +
                           "&table_id="   + m_pAPI->UtilIntToString(m_iID)       +
                           "&score="      + m_pAPI->UtilEscapeString(sScore)     + 
                           "&sort="       + m_pAPI->UtilIntToString(iSort)       + 
                           "&extra_data=" + m_pAPI->UtilEscapeString(sExtraData) +
                           sUserData, bNow ? &sResponse : NULL, this, &gjScoreTable::__AddScoreCallback, pVerify, GJ_NETWORK_OUTPUT_FW)) return GJ_REQUEST_FAILED;

    if(bNow) return this->__AddScoreCallback(sResponse, pVerify, NULL);
    return GJ_OK;
}


// ****************************************************************
/* add score entry to this score table with Base64 extra data */
template <typename T> int gjScoreTable::__AddScoreBase64(const std::string &sScore, const int &iSort, void* pExtraData, const size_t &iExtraSize, const std::string &sGuestName, const bool &bNow, GJ_NETWORK_OUTPUT(gjScorePtr))
{
    if(!pExtraData || iExtraSize <= 0) return GJ_INVALID_INPUT;

    const size_t iNeed = base64_needed(iExtraSize);

    // convert binary data to Base64 string
    char* pcBase64 = new char[iNeed];
    base64_encode((unsigned char*)pExtraData, iExtraSize, pcBase64, iNeed);
    
    // execute add score function with Base64 string
    const int iReturn = this->__AddScore(sScore, iSort, pcBase64, sGuestName, bNow, GJ_NETWORK_OUTPUT_FW);
    SAFE_DELETE_ARRAY(pcBase64)

    return iReturn; 
}


#endif /* GJSCORE_H */