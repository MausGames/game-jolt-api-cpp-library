#pragma once
#ifndef TROPHY_H
#define TROPHY_H


// ****************************************************************
/*! Trophy object class.\n
 *  http://gamejolt.com/api/doc/game/trophies/  
 *  \brief Trophy Object */
class gjTrophy
{
private:
    int m_iID;                     //!< ID of the trophy
    std::string m_sTitle;          //!< title/name of the trophy
    std::string m_sDescription;    //!< description text of the trophy

    std::string m_sDifficulty;     //!< difficulty of the trophy ("Bronze", "Silver", "Gold", "Platinum")
    int m_iDifficultyValue;        //!< difficulty as numerical value for easier access

    std::string m_sImageURL;       //!< URL of the thumbnail
    std::string m_sAchievedDate;   //!< time string when the trophy was achieved (e.g. 4 weeks ago)

    gjAPI* m_pAPI;                 //!< main interface access pointer


public:
    gjTrophy(const gjData &aTrophyData, gjAPI* pAPI);

    /*! @name Update Data Request */
    //!@{
    /*! Update data of this trophy through an API request.
     *  \pre    Login required
     *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_NOT_CONNECTED</b> if connection/login is missing\n
     *          (see #GJ_ERROR) */
                          inline int UpdateDataNow()                                {return this->__UpdateData(true, GJ_NETWORK_NULL_API(gjTrophyPtr));}
                          inline int UpdateDataCall()                               {return this->__UpdateData(false, GJ_NETWORK_NULL_API(gjTrophyPtr));}
    template <typename T> inline int UpdateDataCall(GJ_NETWORK_OUTPUT(gjTrophyPtr)) {return this->__UpdateData(false, GJ_NETWORK_OUTPUT_FW);}
    //!@}

    /*! @name Achieve Request */
    //!@{
    /*! Achieve this trophy through an API request.\n
     *  Does nothing, if trophy was already achieved.
     *  \pre    Login required
     *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_REQUEST_CANCELED</b> if trophy was already achieved\n
     *          <b>GJ_NOT_CONNECTED</b> if connection/login is missing\n
     *          (see #GJ_ERROR) */
                          inline int AchieveNow()                                {return this->__Achieve(true, GJ_NETWORK_NULL_API(gjTrophyPtr));}
                          inline int AchieveCall()                               {return this->__Achieve(false, GJ_NETWORK_NULL_API(gjTrophyPtr));}
    template <typename T> inline int AchieveCall(GJ_NETWORK_OUTPUT(gjTrophyPtr)) {return this->__Achieve(false, GJ_NETWORK_OUTPUT_FW);}
    //!@}

    /*! @name Download Thumbnail */
    //!@{
    /*! Download the thumbnail of this trophy.\n
     *  Receive the cached local path of the file.
     *  \note   <b>-Now</b> blocks, <b>-Call</b> uses non-blocking callbacks
     *  @param  sToFolder Relative path of the download target folder
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_INVALID_INPUT</b> if target folder string is empty\n
     *          (see #GJ_ERROR) */
                          inline int DownloadThumbnailNow(const std::string &sToFolder, std::string* psOutput)           {return m_pAPI->InterFile()->DownloadFileNow(m_sImageURL, sToFolder, "", psOutput);}
    template <typename T> inline int DownloadThumbnailCall(const std::string &sToFolder, GJ_NETWORK_OUTPUT(std::string)) {return m_pAPI->InterFile()->DownloadFileCall(m_sImageURL, sToFolder, "", GJ_NETWORK_OUTPUT_FW);}
    //!@}

    /*! @name Get Attributes */
    //!@{
    inline const int&         GetID()const              {return m_iID;}                //!< \copybrief m_iID
    inline const std::string& GetTitle()const           {return m_sTitle;}             //!< \copybrief m_sTitle
    inline const std::string& GetDescription()const     {return m_sDescription;}       //!< \copybrief m_sDescription
    inline const std::string& GetDifficulty()const      {return m_sDifficulty;}        //!< \copybrief m_sDifficulty
    inline const int&         GetDifficultyValue()const {return m_iDifficultyValue;}   //!< \copybrief m_iDifficultyValue
    inline const std::string& GetImageURL()const        {return m_sImageURL;}          //!< \copybrief m_sImageURL
    inline const std::string& GetAchievedDate()const    {return m_sAchievedDate;}      //!< \copybrief m_sAchievedDate
    /*! */ //!@}

    /*! @name Check Status */
    //!@{
    inline bool IsAchieved()const {return m_sAchievedDate.empty() ? false : true;}
    //!@}


private:
    /*! @name Disable Copy */
    //!@{
    gjTrophy(const gjTrophy& that);
    gjTrophy& operator = (const gjTrophy& that);
    friend class gjAPI;
    //!@}

    /*! @name Superior Request Functions */
    //!@{
    template <typename T> int __UpdateData(const bool &bNow, GJ_NETWORK_OUTPUT(gjTrophyPtr));
    template <typename T> int __Achieve(const bool &bNow, GJ_NETWORK_OUTPUT(gjTrophyPtr));
    //!@}

    /*! @name Callback Functions */
    //!@{
    int __UpdateDataCallback(const std::string &sData, void* pAdd, gjTrophyPtr* pOutput);
    int __AchieveCallback(const std::string &sData, void* pAdd, gjTrophyPtr* ppOutput);
    //!@}
};


// ****************************************************************
/* update data of this trophy */
template <typename T> int gjTrophy::__UpdateData(const bool &bNow, GJ_NETWORK_OUTPUT(gjTrophyPtr))
{
    if(!m_pAPI->IsConnected()) return GJ_NOT_CONNECTED;

    // send get trophies request
    std::string sResponse;
    if(m_pAPI->SendRequest("/trophies/"
                           "?game_id="    + m_pAPI->GetProcGameID()    + 
                           "&username="   + m_pAPI->GetProcUserName()  + 
                           "&user_token=" + m_pAPI->GetProcUserToken() +
                           "&trophy_id="  + m_pAPI->UtilIntToString(m_iID),
                           bNow ? &sResponse : NULL, this, &gjTrophy::__UpdateDataCallback, NULL, GJ_NETWORK_OUTPUT_FW)) return GJ_REQUEST_FAILED;

    if(bNow) return this->__UpdateDataCallback(sResponse, NULL, NULL);
    return GJ_OK;
}


// ****************************************************************
/* achieve this trophy */
template <typename T> int gjTrophy::__Achieve(const bool &bNow, GJ_NETWORK_OUTPUT(gjTrophyPtr))
{
    // cancel request if already achieved
    if(this->IsAchieved()) return GJ_REQUEST_CANCELED;

    // achieve offline-cached trophy
    if(!m_pAPI->IsConnected())
    {
        m_sAchievedDate = GJ_API_NOW;
        return GJ_NOT_CONNECTED;
    }

    // send achieve trophy request
    std::string sResponse;
    if(m_pAPI->SendRequest("/trophies/add-achieved/"
                           "?game_id="    + m_pAPI->GetProcGameID()    + 
                           "&username="   + m_pAPI->GetProcUserName()  + 
                           "&user_token=" + m_pAPI->GetProcUserToken() + 
                           "&trophy_id="  + m_pAPI->UtilIntToString(m_iID),
                           bNow ? &sResponse : NULL, this, &gjTrophy::__AchieveCallback, NULL, GJ_NETWORK_OUTPUT_FW)) return GJ_REQUEST_FAILED;

    if(bNow) return this->__AchieveCallback(sResponse, NULL, NULL);
    return GJ_OK;
}


#endif