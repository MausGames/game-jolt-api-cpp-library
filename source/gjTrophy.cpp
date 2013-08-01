#include "gjAPI.h"


// ****************************************************************
/* constructor */
gjTrophy::gjTrophy(const gjData &aTrophyData, gjAPI* pAPI)
: m_iID           (atoi(SAFE_MAP_GET(aTrophyData, "id").c_str()))
, m_sTitle        (SAFE_MAP_GET(aTrophyData, "title"))
, m_sDescription  (SAFE_MAP_GET(aTrophyData, "description"))
, m_sDifficulty   (SAFE_MAP_GET(aTrophyData, "difficulty"))
, m_sImageURL     (SAFE_MAP_GET(aTrophyData, "image_url"))
, m_pAPI          (pAPI)
{
    // set difficulty value for easier access
         if(m_sDifficulty ==   "Bronze") m_iDifficultyValue = 1;
    else if(m_sDifficulty ==   "Silver") m_iDifficultyValue = 2;
    else if(m_sDifficulty ==     "Gold") m_iDifficultyValue = 3;
    else if(m_sDifficulty == "Platinum") m_iDifficultyValue = 4;
    else                                 m_iDifficultyValue = 0;

    // set the achieved date
    const std::string sAchievedDate = SAFE_MAP_GET(aTrophyData, "achieved");
    m_sAchievedDate = (sAchievedDate == "false" || sAchievedDate == "" || !m_pAPI->IsConnected()) ? "" : sAchievedDate;
}


// ****************************************************************
/* copy-constructor and copy-assignment */
gjTrophy::gjTrophy(const gjTrophy& that)
: m_iID              (that.m_iID)
, m_sTitle           (that.m_sTitle)
, m_sDescription     (that.m_sDescription)
, m_sDifficulty      (that.m_sDifficulty)
, m_iDifficultyValue (that.m_iDifficultyValue)
, m_sImageURL        (that.m_sImageURL)
, m_sAchievedDate    (that.m_sAchievedDate)
, m_pAPI             (that.m_pAPI)
{
}

gjTrophy& gjTrophy::operator = (const gjTrophy& that)
{
    m_iID              = that.m_iID; 
    m_sTitle           = that.m_sTitle; 
    m_sDescription     = that.m_sDescription; 
    m_sDifficulty      = that.m_sDifficulty; 
    m_iDifficultyValue = that.m_iDifficultyValue; 
    m_sImageURL        = that.m_sImageURL; 
    m_sAchievedDate    = that.m_sAchievedDate; 
    m_pAPI             = that.m_pAPI;

    return *this;
}


// ****************************************************************  
// callback for updating the data */
int gjTrophy::__UpdateDataCallback(const std::string &sData, void* pAdd, gjTrophyPtr* pOutput)
{
    // parse output
    gjDataList aaReturn;
    if(m_pAPI->ParseRequestKeypair(sData, &aaReturn) != GJ_OK)
    {
        m_pAPI->ErrorLogAdd("API Error: could not parse trophy");
        return GJ_REQUEST_FAILED;
    }

    // copy new trophy over old trophy
    gjTrophy NewTrophy(aaReturn[0], m_pAPI);
    *this = NewTrophy;

    if(pOutput) (*pOutput) = this;
    return GJ_OK;
}


// ****************************************************************  
// callback for achieving the trophy */
int gjTrophy::__AchieveCallback(const std::string &sData, void* pAdd, gjTrophyPtr* ppOutput)
{
    // parse output (only check status)
    gjDataList aaReturn;
    if(m_pAPI->ParseRequestKeypair(sData, &aaReturn) != GJ_OK)
    {
        m_pAPI->ErrorLogAdd("API Error: could not parse trophy");
        return GJ_REQUEST_FAILED;
    }

    // set time string
    m_sAchievedDate = GJ_API_NOW;

    if(ppOutput) (*ppOutput) = this; 
    return GJ_OK;
}