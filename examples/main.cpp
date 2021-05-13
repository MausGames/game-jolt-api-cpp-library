#include <gjAPI.h>
#include <iostream>


// ****************************************************************
class TestTrophy
{
public:
    void InitTrophies(const gjTrophyList& apTrophies, void* pData)
    {
        // show finished trophy fetch
        std::cout << "[Trophy] Init Trophies" << std::endl;
        FOR_EACH(it, apTrophies)
        {
            gjTrophy* pTrophy = (*it);

            // download trophy thumbnails (parallel non-blocking download of all images)
            std::cout << "[Trophy]  Trophy <" << pTrophy->GetID() << ":" << pTrophy->GetSort() << "> \"" << pTrophy->GetTitle() << ":" << pTrophy->GetDescription() << "\"" << std::endl;
            pTrophy->DownloadThumbnailCall("pictures/trophies", this, &TestTrophy::InitTrophyImage, pTrophy);
        }
    }

    void InitTrophyImage(const std::string& sPath, void* pData)
    {
        gjTrophy* pTrophy = ((gjTrophy*)pData);

        // show finished thumbnail download
        std::cout << "[Trophy]   Trophy <" << pTrophy->GetID() << ":" << pTrophy->GetSort() << "> Thumbnail <" << sPath << ">" << std::endl;
    }
};


// ****************************************************************
class TestScoreAndUser
{
public:
    void InitScoreTable(const gjScoreTableMap& apScoreTables, void* pData)
    {
        // show finished score table fetch
        std::cout << "[Score] Init Score Tables"  << std::endl;
        FOR_EACH(it, apScoreTables)
        {
            gjScoreTable* pScoreTable = it->second;

            // get score entries of each score table (parallel and non-blocking)
            std::cout << "[Score]  Score Table <" << pScoreTable->GetID() << "> \"" << pScoreTable->GetTitle() << "\"" << std::endl;
            pScoreTable->FetchScoresCall(false, 10, this, &TestScoreAndUser::InitScore, pScoreTable);
        }
    }

    void InitScore(const gjScoreList& apScores, void* pData)
    {
        // show finished score entry fetch
        std::cout << "[Score]   Init Scores <" << ((gjScoreTable*)pData)->GetID() << ">" << std::endl;
        FOR_EACH(it, apScores)
        {
            gjScore* pScore = (*it);

            // get user of each specific score (parallel and non-blocking)
            std::cout << "[Score]    Score <" << pScore->GetUserName() << ":" << pScore->GetScore() << ">" << std::endl;
            pScore->FetchUserCall(this, &TestScoreAndUser::InitUser, pScore);
        }
    }

    void InitUser(const gjUserPtr& pUser, void* pData)
    {
        // show finished user fetch (guests have an own object with name "guest" and id -1 (currently all guests have the same))
        std::cout << "[User]     Init User <" << pUser->GetName() << ">" << std::endl;

        // download user avatars (parallel non-blocking download of all images)
        pUser->DownloadAvatarCall("pictures/avatars", this, &TestScoreAndUser::InitUserAvatar, pUser);
    }

    void InitUserAvatar(const std::string& sPath, void* pData)
    {
        // show finished avatar download
        std::cout << "[User]      User <" << ((gjUserPtr)pData)->GetName() << "> Avatar <" << sPath << ">" << std::endl;
    }
};


// ****************************************************************
#define TEST_DATA_ITEM_COUNT 1000
#define TEST_DATA_ITEM_SIZE  sizeof(int)*TEST_DATA_ITEM_COUNT
class TestDataItem
{
private:
    int* m_aiTestData;


public:
    TestDataItem()  {m_aiTestData = new int[TEST_DATA_ITEM_COUNT];}
    ~TestDataItem() {SAFE_DELETE_ARRAY(m_aiTestData)}

    void SetData(const gjDataItemPtr& pDataItem, void* pData)
    {
        // show finished Base64 data send
        std::cout << "[Data] Data Set <" << TEST_DATA_ITEM_SIZE << " bytes>" << std::endl;

        // retrieve data again
        pDataItem->GetDataBase64Call(m_aiTestData, TEST_DATA_ITEM_SIZE, this, &TestDataItem::GetData, pDataItem);
    }

    void GetData(const gjVoidPtr& pTestData, void* pData)
    {
        // show finished Base64 data retrieve
        std::cout << "[Data]  Data Get <" << TEST_DATA_ITEM_SIZE << " bytes>" << std::endl;

        // check for wrong data upload and download
        bool bOK = true;
        for(int i = 0; i < TEST_DATA_ITEM_COUNT; ++i)
        {
            if(m_aiTestData[i] != i)
            {
                std::cout << "[Data]   Error getting data" << std::endl;
                bOK = false;
                break;
            }
        }
        if(bOK) std::cout << "[Data]   Data successfully compared" << std::endl;

        // remove the data item again without callback, but stil non-blocking
        ((gjDataItemPtr)pData)->RemoveCall();
    }
};


// ****************************************************************
class TestLogin
{
private:
    TestTrophy       testTrophyObject;
    TestScoreAndUser testScoreAndUserObject;
    TestDataItem     testDataItemObject;


public:
    void FinishLogin(const int& iStatus, void* pData)
    {
        // check for succesfull login
        if(iStatus != GJ_OK) std::cout << "[Login] Login failed" << std::endl;
                        else std::cout << "[Login] Login successful" << std::endl;

        gjAPI* pAPI = ((gjAPI*)pData);

        // fetch IP
        pAPI->SendRequestCall("https://ipecho.net/plain", this, &TestLogin::GetIP, NULL);

        // test trophies
        pAPI->InterTrophy()->FetchTrophiesCall(0, &testTrophyObject, &TestTrophy::InitTrophies, NULL);

        // test scores and users
        pAPI->InterScore()->FetchScoreTablesCall(&testScoreAndUserObject, &TestScoreAndUser::InitScoreTable, NULL);

        // test data store
        int* aiTestData = new int[TEST_DATA_ITEM_COUNT];
        for(int i = 0; i < TEST_DATA_ITEM_COUNT; ++i)
            aiTestData[i] = i;

        pAPI->InterDataStoreGlobal()->GetDataItem("test_item")->SetDataBase64Call(aiTestData, TEST_DATA_ITEM_SIZE, &testDataItemObject, &TestDataItem::SetData, NULL);
        SAFE_DELETE_ARRAY(aiTestData)
    }

    void GetIP(const std::string& sIP, void* pData)
    {
         std::cout << "[Login]  Your IP is: " << sIP << std::endl;
    }
};


// ****************************************************************
int main()
{
    // data from the test-game
    gjAPI API;
    API.Init(15490, "0dbe90d7e46fbcaed459d434e5f835da");

    // simple examples
    /*
        if(API.LoginNow(true, "<UserName>", "<UserToken>"))
        {
            API.InterTrophy()->GetTrophy(1234)->AchieveCall();
            API.InterScore()->GetScoreTable(0)->AddScoreCall("123 Points", 123, "<ExtraData>");
        }
    */

    // set trophy attributes
    const int iSort[] = {2542, 2545, 2546, 2543, 2547, 2544};
    API.InterTrophy()->SetSort(iSort, ARRAY_SIZE(iSort));

    const int iSecret[] = {2546, 2547};
    API.InterTrophy()->SetSecret(iSecret, ARRAY_SIZE(iSecret));

    const int iHidden[] = {3105};
    API.InterTrophy()->SetHidden(iHidden, ARRAY_SIZE(iHidden));

    // login with user
    TestLogin testLoginObject;
    if(API.LoginCall(true, GJ_API_CRED, &testLoginObject, &TestLogin::FinishLogin, &API) != GJ_OK) // check for credentials file (QuickPlay)
    {
        API.LoginCall(true, "<UserName>", "<UserToken>", &testLoginObject, &TestLogin::FinishLogin, &API); // normal async login (forwarding the API object)
    }

    // main loop
    const time_t iMaxTime = time(NULL) + 5;
    while(iMaxTime >= time(NULL))
    {
        API.Update();
    }

    std::cout << "\nPress any key to continue ..." << std::endl;
    std::cin.get();

    return 0;
}
