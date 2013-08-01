#pragma once
#ifndef GJNETWORK_H
#define GJNETWORK_H


// ****************************************************************
/*! Main network interface class.\n
 *  Send requests, fetch data and download files with the cURL library.\n
 *  http://curl.haxx.se/  
 *  \brief Network Interface 
 *  \todo Recycle session handles */
class gjNetwork
{
private:
    // ****************************************************************
    /*! Callback interface class for non-blocking cURL sessions.\n
     *  \brief Callback Interface */
    class gjCall
    {
    protected:
        CURL* m_pSession;      //!< cURL session
        std::string m_sInfo;   //!< information about this callback


    public:
        gjCall(CURL* pSession, const std::string &sInfo) : m_pSession(pSession), m_sInfo(sInfo) {}
        virtual ~gjCall() {}

        /*! @name Finish Session */
        //!@{
        virtual void Finish(const bool &bOK) = 0;
        //!@}

        /*! @name Get Attributes */
        //!@{
        inline       CURL*        GetSession()const {return m_pSession;}   //!< \copybrief m_pSession
        inline const std::string& GetInfo()const    {return m_sInfo;}      //!< \copybrief m_sInfo
        /*! */ //!@}
    };

    // ****************************************************************
    /*! Template bridge for the callback interface class.\n
     *  T = type of output receiving object\n
     *  P = type of processing object\n
     *  D = output type\n
     *  \brief Template Bridge */
    template <typename T, typename P, typename D> class gjCallTemplate : public gjCall
    {
    protected:
        struct sOutput
        {
            T* m_pOutputObj;                                //!< object with output callback function
            void (T::*m_OutputCallback)(const D&, void*);   //!< callback function to receive the data object when finished
            void* m_pOutputData;                            //!< additional data for the output callback
        };


    protected:
        P* m_pProcessObj;                                             //!< object with processing callback function
        int (P::*m_ProcessCallback)(const std::string&, void*, D*);   //!< callback function to convert the response string to a data object
        void* m_pProcessData;                                         //!< additional data for the processing callback

        std::vector<sOutput> m_aOutput;                               //!< list of output structs
        

    public:
        gjCallTemplate(CURL* pSession, const std::string &sInfo, GJ_NETWORK_PROCESS);
        virtual ~gjCallTemplate() {m_aOutput.clear();}

        /*! @name Add Output */
        //!@{
        void AddOutput(GJ_NETWORK_OUTPUT(D));
        //!@}
    };


    // ****************************************************************
    /*! Request callback sub-interface class for non-blocking cURL sessions.\n
     *  \brief Request Callback Sub-Interface */
    template <typename T, typename P, typename D> class gjCallRequest : public gjCallTemplate<T,P,D>
    {
    private:
        std::string* m_psResponse;    //!< response string of the request
        curl_httppost* m_pPostList;   //!< data for a POST request


    public:
        gjCallRequest(std::string* psResponse, curl_httppost* pPostList, CURL* pSession, const std::string &sInfo, GJ_NETWORK_PROCESS)
        : gjCallTemplate<T,P,D>(pSession, sInfo, GJ_NETWORK_PROCESS_FW), m_psResponse(psResponse), m_pPostList(pPostList) {}

        /*! @name Finish Session */
        //!@{
        void Finish(const bool &bOK);
        //!@}
    };


    // ****************************************************************
    /*! Download callback sub-interface class for non-blocking cURL sessions.\n
     *  \brief Download Callback Sub-Interface */
    template <typename T, typename P, typename D> class gjCallDownload : public gjCallTemplate<T,P,D>
    {
    private:
        FILE* m_pFile;         //!< file handle
        std::string m_sPath;   //!< local path of the file


    public:
        gjCallDownload(FILE* pFile, const std::string &sPath, CURL* pSession, const std::string &sInfo, GJ_NETWORK_PROCESS)
        : gjCallTemplate<T,P,D>(pSession, sInfo, GJ_NETWORK_PROCESS_FW), m_pFile(pFile), m_sPath(sPath) {}

        /*! @name Finish Session */
        //!@{
        void Finish(const bool &bOK);
        //!@}
    };


private:    
    CURLM* m_pMultiHandle;           //!< handle for simultaneous cURL operations
    int m_iNumSessions;              //!< current number of active cURL sessions

    std::vector<gjCall*> m_apCall;   //!< list of callback objects
                               
    gjAPI* m_pAPI;                   //!< main interface access pointer
    

public:
    gjNetwork(gjAPI* pAPI);
    ~gjNetwork();

    /*! @name Update */
    //!@{
    /*! Update all active non-blocking cURL sessions.\n
     *  Execute associated callback functions when a session is finished.
     *  @return TRUE when active, FALSE when idle */
    bool Update();
    //!@}

    /*! @name Wait */
    //!@{
    /*! Finish all active sessions and return.
     *  @param iMaxWait Max waiting time in seconds (0 = until everything is finished) */
    void Wait(const unsigned int &iMaxWait);
    //!@}

    /*! @name Send Request */
    //!@{
    /*! Send a direct or non-blocking request to the API.\n
     *  Retrieve a response string when finished.
     *  \note   Creates a non-blocking session when output string is NULL\n
     *          Use "&POST<data>" at the end of the URL for a POST request
     *  @param  sURL Relative API request string
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_INVALID_INPUT</b> if URL string is empty\n
     *          <b>GJ_NETWORK_ERROR</b> if session cannot be established\n
     *          (see #GJ_ERROR) */
    template <typename T, typename P, typename D> int SendRequest(const std::string &sURL, std::string* psOutput, GJ_NETWORK_PROCESS, GJ_NETWORK_OUTPUT(D));
    //!@}

    /*! @name Download File */
    //!@{
    /*! Download a file direct or non-blocking from any URL.\n
     *  Retrieve the local path of the file when finished.
     *  \note   Creates a non-blocking session when output string is NULL
     *  @param  sURL    Full path of the remote file
     *  @param  sToFile Relative path of the download target
     *  @return <b>GJ_OK</b> on success\n
     *          <b>GJ_REQUEST_FAILED</b> if request was unsuccessful\n
     *          <b>GJ_INVALID_INPUT</b> if URL string or target file string is empty\n
     *          <b>GJ_NETWORK_ERROR</b> if session cannot be established\n
     *          <b>GJ_FILE_ERROR</b> if file cannot be written\n
     *          (see #GJ_ERROR) */
    template <typename T, typename P, typename D> int DownloadFile(const std::string &sURL, const std::string &sToFile, std::string* psOutput, GJ_NETWORK_PROCESS, GJ_NETWORK_OUTPUT(D));
    //!@}

    /*! @name Get Attributes */
    //!@{
    inline size_t GetNumSessions()const {return m_iNumSessions;}   //!< \copybrief m_iNumSessions
    /*! */ //!@}


private:
    /*! @name Disable Copy */
    //!@{
    gjNetwork(const gjNetwork& that) {}
    gjNetwork& operator = (const gjNetwork& that) {return *this;}
    //!@}

    /*! @name Management Functions */
    //!@{
    gjCall* __CheckCall(const std::string &sInfo);
    void    __KillCall(gjCall* pCall);
    //!@}
};


// ****************************************************************
/*! Writing Callbacks */
size_t write_to_string(char* ptr, size_t size, size_t count, std::string* stream);
size_t write_to_file(void* ptr, size_t size, size_t count, FILE* stream);


// template implementation can be found in gjNetwork.hpp


#endif /* GJNETWORK_H */