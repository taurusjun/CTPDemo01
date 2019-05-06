#include <iostream>
#include <sstream>
#include <string.h>
#include <cstdlib>
#include "stdio.h"
#include "ThostFtdcMdApi.h"

//// ----- simnow  -----
char* FrontAddress = "tcp://180.168.146.187:10010"; 
char* BrokerID = "9999";   
char* InvestorID = "142266";  
char* Password = "t0t0tity";   
//// -------------------------

//// ----- update by jw -----
// char* FrontAddress = "tcp://180.166.13.21:31807"; 
// char* BrokerID = "9099";   
// char* InvestorID = "9026789";  
// char* Password = "axiom2018";   
//// -------------------------

//// ----- origin -----
// char* FrontAddress = "tcp://ctp1-md7.citicsf.com:41213"; //上海电信二交易点
// // char* FrontAddress = "tcp://ctp1-md1.citicsf.com:41213";
// //char* FrontAddress = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";
// char* BrokerID = "6666";   // 中信期货的平台ID
// //char* BrokerID = "2030";
// char* InvestorID = "000023";   // 申请到的投资者账号
// char* Password = "88888888";   // 设置的投资者密码
//// -------------------------

////sem_t g_hEvent;

TThostFtdcBrokerIDType g_chBrokerID;
TThostFtdcUserIDType g_chUserID;

class CSimpleHandler : public CThostFtdcMdSpi
{
public:
  CSimpleHandler(CThostFtdcMdApi *pUserApi) : 
    m_pUserApi(pUserApi), requestID(0) {}
  ~CSimpleHandler() {}

  virtual void OnFrontConnected()
  {
    printf("OnFrontConnected!\n");
    CThostFtdcReqUserLoginField reqUserLogin;
    //printf("BrokerID:");
    //scanf("%s", &g_chBrokerID);
    strcpy(reqUserLogin.BrokerID, BrokerID);

    //printf("userid:");
    //scanf("%s", &g_chUserID);
    strcpy(reqUserLogin.UserID, InvestorID);

    //printf("password:");
    //scanf("%s", &reqUserLogin.Password);
    strcpy(reqUserLogin.Password, Password);

    int ret = m_pUserApi->ReqUserLogin(&reqUserLogin, requestID++);
  }

  virtual void OnFrontDisconnected(int nReason)
  {
    printf("OnFrontDisconnected.\n");
  }

  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
			      CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
  {
    printf("OnRspUserLogin:\n");
    printf("ErrorCode = [%d], ErrorMsg = [%s]\n", pRspInfo->ErrorID,
	   pRspInfo->ErrorMsg);
    printf("RequestID = [%d], Chain = [%d]\n", nRequestID, bIsLast);

    printf("SHFETime = %s\n", pRspUserLogin->SHFETime);
    printf("DCETime = %s\n", pRspUserLogin->DCETime);
    printf("CZCETime = %s\n", pRspUserLogin->CZCETime);
    printf("FFEXTime = %s\n", pRspUserLogin->FFEXTime);
    printf("INETime = %s\n", pRspUserLogin->INETime);

    if(pRspInfo->ErrorID != 0)
    {
	printf("Failed to login, errorcode = %d errormsg = %s  requestid = %d chain = %d",
	       pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
	exit(-1);
    }

    //// ----- update by jw -----
    // char* Instrument[] = {"sAUQ19"};  
    // int ret1 = m_pUserApi->SubscribeMarketData(Instrument, 1);
    //// -------------------------

    char* Instrument[] = {"al1906","cu1906"};  // 注意品种的大小写
    // char* Instrument[] = {"cu1608", "cu1606"};  // 注意品种的大小写
    int ret1 = m_pUserApi->SubscribeMarketData(Instrument, 2);
    //int ret2 = m_pUserApi->UnSubscribeMarketData(Instrument, 2);
    int ret3 = m_pUserApi->SubscribeForQuoteRsp(Instrument, 2);
    //int ret4 = m_pUserApi->UnSubscribeForQuoteRsp(Instrument, 2);
  }

  virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
  {
    printf("OnRspSubMarketData:\n");
    printf("ErrorCode = [%d], ErrorMsg = [%s]\n", 
	   pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    printf("RequestID = [%d], Chain = [%d]\n", nRequestID, bIsLast);

    printf("InstrumentID = %s\n", pSpecificInstrument->InstrumentID);
  }
  
  virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
  {
    printf("OnRspUnSubMarketData:\n");
  }

  virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
  {
    printf("OnRspSubForQuoteRsp:\n");
    printf("ErrorCode = [%d], ErrorMsg = [%s]\n", 
	   pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    printf("RequestID = [%d], Chain = [%d]\n", nRequestID, bIsLast);

    printf("InstrumentID = %s\n", pSpecificInstrument->InstrumentID);
  }

  virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
  {
    printf("OnRspUnSubForQuoteRsp:\n");
  }

  virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
  {
    printf("OnRtnForQuoteRsp:\n");
  }

  virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
  {
    printf("Info: Received market data: %s\n Info: Last price: %g\n Info: Update time: %s\n",
	   pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);

    ////sem_post(&g_hEvent);
  }

  virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
  {
    printf("OnRspError:\n");
    printf("ErrorCode = [%d], ErrorMsg[%s]\n", 
	   pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    printf("RequestID = [%d], Chain = [%d]\n", nRequestID, bIsLast);
  }

  virtual void OnHeartBeatWarning(int nTimeLapse)
  {
    printf("time out: %d\n", nTimeLapse);
  }

private:
  CThostFtdcMdApi *m_pUserApi;
  int requestID;
};

int main()
{
  ////sem_init(&g_hEvent, 0, 0);

  CThostFtdcMdApi* pUserApi = CThostFtdcMdApi::CreateFtdcMdApi(".conn", false);
  CSimpleHandler sh(pUserApi);
  pUserApi->RegisterSpi(&sh);

  //pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);
  //pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);

  pUserApi->RegisterFront(FrontAddress);
  pUserApi->Init();

  printf("date: %s\n", pUserApi->GetTradingDay());
  printf("vertion: %s\n", pUserApi->GetApiVersion());

  ////sem_wait(&g_hEvent);
  pUserApi->Join();
  pUserApi->Release();

  return 0;
}