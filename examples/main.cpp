#include <gjAPI.h>
#include <iostream>


// ****************************************************************
class TestTrophy
{
public:
    void InitTrophies(const gjTrophyList &apTrophies, void* pData)
    {
        // show finished trophy fetch
        std::cout << "[Trophy] Init Trophies" << std::endl;
        for(auto it = apTrophies.begin(); it != apTrophies.end(); ++it)
        {
            gjTrophy* pTrophy = (*it);

            // download trophy thumbnails (parallel non-blocking download of all images)
            std::cout << "[Trophy]  Trophy <" << pTrophy->GetID() << ":" << pTrophy->GetSort() << "> \"" << pTrophy->GetTitle() << ":" << pTrophy->GetDescription() << "\"" << std::endl;
            pTrophy->DownloadThumbnailCall("pictures/trophies", this, &TestTrophy::InitTrophyImage, pTrophy);
        }
    }

    void InitTrophyImage(const std::string &sPath, void* pData)
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
    void InitScoreTable(const gjScoreTableMap &apScoreTables, void* pData)
    {
        // show finished score table fetch
        std::cout << "[Score] Init Score Tables"  << std::endl;
        for(auto it = apScoreTables.begin(); it != apScoreTables.end(); ++it)
        {
            gjScoreTable* pScoreTable = it->second;

            // get score entries of each score table (parallel and non-blocking)
            std::cout << "[Score]  Score Table <" << pScoreTable->GetID() << "> \"" << pScoreTable->GetTitle() << "\"" << std::endl;
            pScoreTable->FetchScoresCall(false, 10, this, &TestScoreAndUser::InitScore, pScoreTable);
        }
    }

    void InitScore(const gjScoreList &apScores, void* pData)
    {
        // show finished score entry fetch
        std::cout << "[Score]   Init Scores <" << ((gjScoreTable*)pData)->GetID() << ">" << std::endl;
        for(auto it = apScores.begin(); it != apScores.end(); ++it)
        {
            gjScore* pScore = (*it);

            // get user of each specific score (parallel and non-blocking)
            std::cout << "[Score]    Score <" << pScore->GetUserName() << ":" << pScore->GetScore() << ">" << std::endl;
            pScore->FetchUserCall(this, &TestScoreAndUser::InitUser, pScore);
        }
    }

    void InitUser(const gjUserPtr &pUser, void* pData)
    {
        // show finished user fetch (guests have an own object with name "guest" and id -1 (currently all guests have the same))
        std::cout << "[User]     Init User <" << pUser->GetName() << ">" << std::endl;

        // download user avatars (parallel non-blocking download of all images)
        pUser->DownloadAvatarCall("pictures/avatars", this, &TestScoreAndUser::InitUserAvatar, pUser);
    }

    void InitUserAvatar(const std::string &sPath, void* pData)
    {
        // show finished avatar download
        std::cout << "[User]      User <" << ((gjUserPtr)pData)->GetName() << "> Avatar <" << sPath << ">" << std::endl;
    }
};


// ****************************************************************
#define TEST_DATA_ITEM_SIZE 1000  // *4 (int)
class TestDataItem
{
private:
    int* m_aiTestData;


public:
    TestDataItem()  {m_aiTestData = new int[TEST_DATA_ITEM_SIZE];}
    ~TestDataItem() {SAFE_DELETE_ARRAY(m_aiTestData)}

    void SetData(const gjDataItemPtr &pDataItem, void* pData)
    {
        // show finished Base64 data send
        std::cout << "[Data] Data Set" << std::endl;

        // retrieve data again
        pDataItem->GetDataBase64Call(m_aiTestData, sizeof(int)*TEST_DATA_ITEM_SIZE, this, &TestDataItem::GetData, pDataItem);
    }

    void GetData(const gjVoidPtr &pTestData, void* pData)
    {
        // show finished Base64 data retrieve
        std::cout << "[Data]  Data Get" << std::endl;

        for(int i = 0; i < TEST_DATA_ITEM_SIZE; ++i) 
        {
            if(m_aiTestData[i] != i)
                std::cout << "[Data]   Error getting data" << std::endl;
        }

        // remove the data item again without callback, but stil non-blocking
        ((gjDataItemPtr)pData)->ClearCall();
    }
};


// ****************************************************************
int main()
{
    // data from the test-game
    gjAPI API(15490, "0dbe90d7e46fbcaed459d434e5f835da");

    // set trophy attributes
    const int iSort[] = {2542, 2545, 2546, 2543, 2547, 2544};
    API.InterTrophy()->SetSort(iSort, sizeof(iSort)/sizeof(int));

    const int iSecret[] = {2546, 2547};
    API.InterTrophy()->SetSecret(iSecret, sizeof(iSecret)/sizeof(int));

    const int iHidden[] = {3105};
    API.InterTrophy()->SetHidden(iHidden, sizeof(iHidden)/sizeof(int));

    // login with user
    if(API.Login(true) != GJ_OK)
        API.Login(true, "user_name", "user_token"); // should I ever forget my credentials here, get some trophies for me, thx

    // test trophies
    TestTrophy testTrophyObject;
    API.InterTrophy()->FetchTrophiesCall(0, &testTrophyObject, &TestTrophy::InitTrophies, NULL);

    // test scores and users
    TestScoreAndUser testScoreAndUserObject;
    API.InterScore()->FetchScoreTablesCall(&testScoreAndUserObject, &TestScoreAndUser::InitScoreTable, NULL);

    // test data store
    int* aiTestData = new int[TEST_DATA_ITEM_SIZE];
    for(int i = 0; i < TEST_DATA_ITEM_SIZE; ++i)
        aiTestData[i] = i;

    TestDataItem testDataItemObject;
    API.InterDataStoreGlobal()->GetDataItem("test_item")->SetDataBase64Call(aiTestData, sizeof(int)*TEST_DATA_ITEM_SIZE, &testDataItemObject, &TestDataItem::SetData, NULL);

    // main loop
    const time_t iMaxTime = time(NULL) + 3;
    while(iMaxTime >= time(NULL))
    {
        API.Update();
    }

    std::cout << "\nPress any key to continue ..." << std::endl;
    std::cin.get();

    SAFE_DELETE_ARRAY(aiTestData)
    return 0;
}