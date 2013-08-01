#include "gjAPI.h"


// ****************************************************************
/* constructor */
gjNetwork::gjNetwork(gjAPI* pAPI)
: m_iNumSessions (0)
, m_pAPI         (pAPI)
{
    // init cURL
    curl_global_init(CURL_GLOBAL_ALL);
    m_pMultiHandle = curl_multi_init();

    // reserve some memory
    m_apCall.reserve(GJ_API_RESERVE_CALL);
}


// ****************************************************************
/* destructor */
gjNetwork::~gjNetwork()
{
    // finish and remove open sessions
    this->Wait(2);
    while(!m_apCall.empty())
    {
        m_pAPI->ErrorLogAdd("Network Error: session had to be killed <" + m_apCall[0]->GetInfo() + ">");
        this->__KillCall(m_apCall[0]);
    }

    // clear memory
    m_apCall.clear();

    // exit cURL
    curl_multi_cleanup(m_pMultiHandle);
    curl_global_cleanup();
}


// ****************************************************************
/* update all active non-blocking cURL sessions */
bool gjNetwork::Update()
{
    if(m_apCall.empty()) return false;

    // update cURL
    curl_multi_perform(m_pMultiHandle, &m_iNumSessions);
  
    // manage active cURL sessions
    CURLMsg* pMsg;
    while((pMsg = curl_multi_info_read(m_pMultiHandle, &m_iNumSessions)))
    {
        // cURL session finished
        if(pMsg->msg == CURLMSG_DONE)
        {
            CURL* pSession = pMsg->easy_handle;
            const bool bOK = (pMsg->data.result == CURLE_OK) ? true : false;

            // search callback object and finish the operation
            for(size_t i = 0; i < m_apCall.size(); ++i)
            {
                if(m_apCall[i]->GetSession() == pSession)
                {
                    // check for errors
                    if(!bOK)
                    {
                        m_pAPI->ErrorLogAdd("Network Error: sending non-blocking request failed <" + m_apCall[i]->GetInfo() + ">");
                        m_pAPI->ErrorLogAdd("Network Error: " + std::string(curl_easy_strerror(pMsg->data.result)));
                    }

                    m_apCall[i]->Finish(bOK);
                    SAFE_DELETE(m_apCall[i])
                    m_apCall.erase(m_apCall.begin()+i);
                    break;
                }
            }

            // close cURL session
            curl_multi_remove_handle(m_pMultiHandle, pSession);
            curl_easy_cleanup(pSession);
        }
    }

    return true;
}


// ****************************************************************
/* finish all active sessions and return */
void gjNetwork::Wait(const unsigned int &iMaxWait)
{
    // get max waiting time (low precision)
    const time_t iMaxTime = time(NULL) + iMaxWait;

    // force network update
    while((iMaxTime >= time(NULL) || !iMaxWait) && this->Update()) {}
}


// ****************************************************************
/* check for existing callbacks on the list */
gjNetwork::gjCall* gjNetwork::__CheckCall(const std::string &sInfo)
{
    // search callback object and compare info string
    for(size_t i = 0; i < m_apCall.size(); ++i)
    {
        if(m_apCall[i]->GetInfo() == sInfo)
            return m_apCall[i];
    }

    return NULL;
}


// ****************************************************************
/* kill and remove session and callback object */
void gjNetwork::__KillCall(gjCall* pCall)
{
    // search callback object
    for(size_t i = 0; i < m_apCall.size(); ++i)
    {
        if(m_apCall[i] == pCall)
        {
            CURL* pSession = m_apCall[i]->GetSession();

            // delete callback object
            m_apCall[i]->Finish(false);
            SAFE_DELETE(m_apCall[i])
            m_apCall.erase(m_apCall.begin()+i);

            // close cURL session
            curl_multi_remove_handle(m_pMultiHandle, pSession);
            curl_easy_cleanup(pSession);

            return;
        }
    }
}


// ****************************************************************
/* callbacks for writing operations with cURL */
size_t write_to_string(char* ptr, size_t size, size_t count, std::string* stream)
{
    const size_t num = size*count;
    stream->append(ptr, 0, num);
    return num;
}

size_t write_to_file(void* ptr, size_t size, size_t count, FILE* stream)
{
    return fwrite(ptr, size, count, stream);
}