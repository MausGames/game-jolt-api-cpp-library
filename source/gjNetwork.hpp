#pragma once
#ifndef GJNETWORK_HPP
#define GJNETWORK_HPP


// ****************************************************************
/* constructor */
template <typename T, typename P, typename D> gjNetwork::gjCallTemplate<T,P,D>::gjCallTemplate(CURL* pSession, const std::string &sInfo, GJ_NETWORK_PROCESS)
: gjCall            (pSession, sInfo)
, m_pProcessObj     (pProcessObj)
, m_ProcessCallback (ProcessCallback)
, m_pProcessData    (pProcessData)
{
    // reserve some memory
    m_aOutput.reserve(GJ_API_RESERVE_CALL_OUTPUT);
}


// ****************************************************************
/* add output struct to the list */
template <typename T, typename P, typename D> void gjNetwork::gjCallTemplate<T,P,D>::AddOutput(GJ_NETWORK_OUTPUT(D))
{
    // create struct
    sOutput aOutput;
    aOutput.m_pOutputObj     = pOutputObj;
    aOutput.m_OutputCallback = OutputCallback;
    aOutput.m_pOutputData    = pOutputData;

    // add struct
    m_aOutput.push_back(aOutput);
}


// ****************************************************************
/* finish a request session */
template <typename T, typename P, typename D> void gjNetwork::gjCallRequest<T,P,D>::Finish(const bool &bOK)
{
    if(bOK)
    {
        D pProcessedOutput;

        // call the callbacks
        if(!(this->m_pProcessObj->*this->m_ProcessCallback)(*m_psResponse, this->m_pProcessData, &pProcessedOutput))
        {
            for(size_t i = 0; i < this->m_aOutput.size(); ++i)
                (this->m_aOutput[i].m_pOutputObj->*this->m_aOutput[i].m_OutputCallback)(pProcessedOutput, this->m_aOutput[i].m_pOutputData);
        }
    }

    // delete response string
    SAFE_DELETE(m_psResponse)

    // clear POST data
    if(m_pPostList) curl_formfree(m_pPostList);
}


// ****************************************************************
/* finish a download session */
template <typename T, typename P, typename D> void gjNetwork::gjCallDownload<T,P,D>::Finish(const bool &bOK)
{
    // close file handle
    fclose(m_pFile);

    if(bOK)
    {
        D pProcessedOutput;

        // call the callbacks
        if(!(this->m_pProcessObj->*this->m_ProcessCallback)(m_sPath, this->m_pProcessData, &pProcessedOutput))
        {
            for(size_t i = 0; i < this->m_aOutput.size(); ++i)
                (this->m_aOutput[i].m_pOutputObj->*this->m_aOutput[i].m_OutputCallback)(pProcessedOutput, this->m_aOutput[i].m_pOutputData);
        }
    }
}


// ****************************************************************
/* send direct or non-blocking request */
template <typename T, typename P, typename D>  int gjNetwork::SendRequest(const std::string &sURL, std::string* psOutput, GJ_NETWORK_PROCESS, GJ_NETWORK_OUTPUT(D))
{
    if(sURL == "") return GJ_INVALID_INPUT;

    const int iPostPos = sURL.find("&POST");
    const bool bPost   = (iPostPos >= 0) ? true : false;

    const bool bNow         = psOutput ? true : false;
    const std::string sInfo = bPost ? sURL.substr(0, iPostPos) : sURL;

    if(!bNow)
    {
        // check for existing session with same request string
        gjCall* pOldCall = this->__CheckCall(sInfo);
        if(pOldCall)
        {
            if(bPost)
            {
                // remove old session
                this->__KillCall(pOldCall);
            }
            else
            {
                // append new callback to old session
                gjCallRequest<T,P,D>* pOldCallRequest = (gjCallRequest<T,P,D>*)pOldCall;
                pOldCallRequest->AddOutput(GJ_NETWORK_OUTPUT_FW);
                return GJ_OK;
            }
        }
    }

    // open cURL session
    CURL* pSession = curl_easy_init();
    if(pSession)
    {
        std::string sRequest;

        curl_httppost* pPostList = NULL;
        curl_httppost* pEndList  = NULL;

        // create new response string for non-blocking session
        if(!bNow) psOutput = new std::string();

        // check for POST parameter
        if(bPost)
        {
            // set POST data
            curl_formadd(&pPostList, &pEndList,
                         CURLFORM_COPYNAME,     "data",
                         CURLFORM_COPYCONTENTS, sURL.substr(iPostPos+5).c_str(),
                         CURLFORM_END);

            // create a POST request
            sRequest = GJ_API_URL + sURL.substr(0, iPostPos);
            curl_easy_setopt(pSession, CURLOPT_POST,     1);
            curl_easy_setopt(pSession, CURLOPT_HTTPPOST, pPostList); 
        }
        else
        {
            // create a GET request
            sRequest = GJ_API_URL + sURL;
            curl_easy_setopt(pSession, CURLOPT_TIMEOUT, GJ_API_TIMEOUT_REQUEST);
        }

        // add MD5 signature
        sRequest += "&signature=" + md5(sRequest + m_pAPI->GetGamePrivateKey());

        // set all session parameters
        curl_easy_setopt(pSession, CURLOPT_URL,            sRequest.c_str());
        curl_easy_setopt(pSession, CURLOPT_WRITEFUNCTION,  write_to_string);
        curl_easy_setopt(pSession, CURLOPT_WRITEDATA,      psOutput);
        curl_easy_setopt(pSession, CURLOPT_CONNECTTIMEOUT, GJ_API_TIMEOUT_CONNECTION);

        if(bNow)
        {
            // perform request and close cURL session
            CURLcode res = curl_easy_perform(pSession);
            curl_easy_cleanup(pSession);

            // clear POST data
            if(pPostList) curl_formfree(pPostList);

            // check for errors
            if(res != CURLE_OK)
            {
                m_pAPI->ErrorLogAdd("Network Error: sending direct request failed <" + sInfo + ">");
                m_pAPI->ErrorLogAdd("Network Error: " + std::string(curl_easy_strerror(res)));
                return GJ_REQUEST_FAILED;
            }
        }
        else
        {   
            // append session to multi handle
            curl_multi_add_handle(m_pMultiHandle, pSession);
            curl_multi_perform(m_pMultiHandle, &m_iNumSessions);

            // create and save callback object
            gjCallRequest<T,P,D>* pCall = new gjCallRequest<T,P,D>(psOutput, pPostList, pSession, sInfo, GJ_NETWORK_PROCESS_FW);
            pCall->AddOutput(GJ_NETWORK_OUTPUT_FW);
            m_apCall.push_back(pCall);
        }
    }
    else
    {
        m_pAPI->ErrorLogAdd("Network Error: cannot establish curl session");
        return GJ_NETWORK_ERROR;
    }
    
    return GJ_OK;
}


// ****************************************************************
/* download file direct or non-blocking */
template <typename T, typename P, typename D>  int gjNetwork::DownloadFile(const std::string &sURL, const std::string &sToFile, std::string* psOutput, GJ_NETWORK_PROCESS, GJ_NETWORK_OUTPUT(D))
{
    if(sURL == "" || sToFile == "") return GJ_INVALID_INPUT;

    const bool bNow = psOutput ? true : false;
    const std::string sInfo = sURL;

    if(!bNow)
    {
        // check for existing session with same request string
        gjCall* pOldCall = this->__CheckCall(sInfo);
        if(pOldCall)
        {
            // append new callback to old session
            gjCallDownload<T,P,D>* pOldCallDownload = (gjCallDownload<T,P,D>*)pOldCall;
            pOldCallDownload->AddOutput(GJ_NETWORK_OUTPUT_FW);
            return GJ_OK;
        }
    }

    // open cURL session
    CURL* pSession = curl_easy_init();
    if(pSession)
    {        
        // open file
        FILE* pFile = fopen(sToFile.c_str(), "wb");
        if(pFile)
        {
            // set all session parameters
            curl_easy_setopt(pSession, CURLOPT_URL,            sURL.c_str());
            curl_easy_setopt(pSession, CURLOPT_WRITEFUNCTION,  write_to_file);
            curl_easy_setopt(pSession, CURLOPT_WRITEDATA,      pFile);
            curl_easy_setopt(pSession, CURLOPT_CONNECTTIMEOUT, GJ_API_TIMEOUT_CONNECTION);

            if(bNow)
            {
                // perform download and close cURL session
                CURLcode res = curl_easy_perform(pSession);
                curl_easy_cleanup(pSession);

                // close file
                fclose(pFile);

                // check for errors
                if(res != CURLE_OK)
                {
                    m_pAPI->ErrorLogAdd("Network Error: direct file download failed <" + sInfo + ">");
                    m_pAPI->ErrorLogAdd("Network Error: " + std::string(curl_easy_strerror(res)));
                    return GJ_REQUEST_FAILED;
                }

                (*psOutput) = sToFile;
            }
            else
            {
                // append session to multi handle
                curl_multi_add_handle(m_pMultiHandle, pSession);
                curl_multi_perform(m_pMultiHandle, &m_iNumSessions);

                // create and save callback object
                gjCallDownload<T,P,D>* pCall = new gjCallDownload<T,P,D>(pFile, sToFile, pSession, sInfo, GJ_NETWORK_PROCESS_FW);
                pCall->AddOutput(GJ_NETWORK_OUTPUT_FW);
                m_apCall.push_back(pCall);
            }
        }
        else
        {
            m_pAPI->ErrorLogAdd("File Error: cannot write file <" + sToFile + ">");
            curl_easy_cleanup(pSession);
            return GJ_FILE_ERROR;
        }
    }
    else
    {
        m_pAPI->ErrorLogAdd("Network Error: cannot establish curl session");
        return GJ_NETWORK_ERROR;
    }

    return GJ_OK;
}


#endif /* GJNETWORK_HPP */