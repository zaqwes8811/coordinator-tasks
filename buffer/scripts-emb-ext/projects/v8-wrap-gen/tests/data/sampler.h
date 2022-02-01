/**
  "Copyright [year] <Copyright Owner>"

  dict :
    mwf - messege or warning or failure
    frw - ���
  @TODO: <igor.a.lugansky@gmail.com> [optionally include text about more work to be done]
*/
#ifndef CC_HEADERS_WEB_SERVICE_SAMPLER_H_
#define CC_HEADERS_WEB_SERVICE_SAMPLER_H_

// C++
#include <map>
#include <vector>
#include <deque>
#include <string>

// Other
#include <app-server-win/global_abstractions.h>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

// Inner reuse
#include "applications-server-cross/dal/transports/abstract_channel.h"

// App
#ifdef FILTER_SIGNAL_GLITCHES
#include <src-back-end/fsm/filters.h>
#endif 
#include "in-memory-storages/linux_version/abstract_in_memory_storage.h"
#include "app-server-win/top_headers/app_types.h"
#include "app-server-win/top_headers/app_constants.h"

#define foreach_ BOOST_FOREACH
#define foreach_r_ BOOST_REVERSE_FOREACH

using boost::weak_ptr;
using boost::shared_ptr;

namespace tmitter_web_service {
class InMemoryStorageImpl : public in_memory_storages::InMemoryStorage {
 public: 
  InMemoryStorageImpl() {}
  ~InMemoryStorageImpl() {}

  void ParseCfgSystem(::channels::Slot* localCopyBuffer);
  void parsePkgAndChangeState(::channels::Slot* localCopyBuffer, int bufferLength);
  //@EmmitPart
  virtual std::vector<::channels::Slot> emitAllParams();
  virtual std::vector<::channels::Slot> emitConfiguration();

public:
  int lengthAllParams___;
#ifdef _DEBUG
  int counterDebug___;
#endif

  std::vector<std::string> stringMsgsQuerySTL;  
  // LLevel
  std::vector<int> LLTypeCodesQuerySTL;  
  // HLevel
  std::vector<int> HLTypeCodesQuerySTL;
   // �� ��� ��

  // ���� ��� ���. �� ��� ����������� ���� �� ����
  std::string typeControl_; 

  bool failsReseted_;
 
  bool printPABLock_[kMaxPABs];
  bool printPABLockEvent_[kMaxPABs];
  bool printPABNoInPower_[kMaxPABs];
  bool printPABNoInPowerEvent_[kMaxPABs];
  bool printPABUnlock_[kMaxPABs];
  bool printPABInPowerOk_[kMaxPABs];
  
  // number SNMP events in Queue
  int currentQueryIndex_;    
  //int currentItemInQuery_;
  // maximum number SNMP events in Queue
  static const int kMaxSNMPQueue = kQuerySize;  
  // !Danger! ��, ��� ��������� ��� � ����� ������������
  // array of codes SNMP events
  uchar snmpEventsQuery_[kQuerySize];  
  
  // Msgs
  // string array for message
  std::string stringMsgsQuery_[kQuerySize];  
  // LLevel
  // Fail Ok or Warnig Code for Queue
  uchar typesCodesInQuery_[kQuerySize];  
  // HLevel
  // flag of Event (Fail Ok or Warnig)
  uchar typeMsgsQuery_[kQuerySize];  
  

  // ���������� �� ��������� ��������
  bool powerOkTTgr_;
  bool needPrintFullPower_;
  bool needPrintOkPower_;

  // ���������� �� ��������� ��������
  bool FRWOkTTgr_;
  bool needPrintFullFRW_;
  bool needPrintOkFRW_;

  /* analog */
  // trigger Vtv signal on off
  uchar vtvSigOnOffTgrD_[kMaxExciters];     
  uchar vtvSigOnOffTgrQ_[kMaxExciters];
 
  // trigger overmodulation
  uchar vtvOverModTgrD_[kMaxExciters];
  uchar vtvOverModTgrQ_[kMaxExciters];
 
  // haneda's code 
  uchar ctrlModeTgrVar_;
  uchar typeLoadTgrVar_;
  uchar tmitterLockTgrVar___;
  uchar tmitterOnTgrVar___;
  uchar powHalfModeTgrVar_;
  uchar istreamTgrVar_;
  uchar outSynTgrVar_;
  uchar statusTmitterTgrVar_;
  //
  // 1-digital 0 - analog 
  uchar type_tm_digital_;  
  // 1 - norm 0 - not norm  
  uchar ibWork_;        
  // Vtv 1 - lock 0 -unlock  
  uchar exciterLock_;   
  // Power Amplifier // 1 - lock 0 -unlock
  uchar ibPowAmpLock_;     
  
  // Vtv 1 = 1 or ETV2 = 2
  uchar idx_oned_etv_;     
   // Vtv 1 = 1 or ETV2 = 2
  uchar idx_oned_etv_q_;    
  // radiation mode 1 -reserve 0 - main
  uchar ibRadioMode_;      
  
  // Transmitter 1 - ready 0 -not ready
  uchar transmitterReady_;

  //
  //
  // 1 - fail
  uchar failNet_;   
  // 1 -fail
  uchar failBallast_;  
  // FRW (or KBV in russion)1 - fail
  uchar failFRW_;      
  // 1 - fail  
  uchar failCoolling_; 
  // power amplifier block (10..1) for bits 9..0, 1 - fail
  uint ibPAB;  
     // bits 1..0 for Vtv2..Vtv1, 1 - fail  
  uchar ibVtv; 
  // VTV modulator status// 1 - fail
  uchar ibVtvModStatus[kMaxExciters]; 
  // VTV US status // 1 - fail  
  uchar ibVtvUsStatus[kMaxExciters];  
  
  // PAB
  // 1 - invalid < Pmin
  uchar ibPABOutPowerStatus[kMaxPABs];
  // 1..0;  1 - VT 2, 0 - VT 1  
  uchar ibPreVT12status[kMaxPABs];
  // temperature status  // 1 -fail  
  uchar ibPreTemStaus[kMaxPABs]; 
  // 1 -fail
  uchar ibPreMIP[kMaxPABs]; 
  // 1..0;  1 - VT 2, 0 - VT 1
  uchar ibPaVT12status[kMaxPABs][kMaxTAsPerPAB];
  // 1 -fail  
  uchar ibPaTemStaus[kMaxPABs][kMaxTAsPerPAB]; 
  // 1 -fail
  uchar ibPaMIP[kMaxPABs][kMaxTAsPerPAB]; 
  

  uchar ibBCVstatus[kMaxBCLs];
  // R1 status
  uchar bcl_R1[kMaxBCLs]; 
  // Temperature R1 status
  int bcl_TR1[kMaxBCLs]; 
  uchar bcl_R2[kMaxBCLs];
  uchar bcl_TR2[kMaxBCLs];
  uchar bcl_R3[kMaxBCLs];
  uchar bcl_TR3[kMaxBCLs];
  uchar bcl_R4[kMaxBCLs];
  uchar bcl_TR4[kMaxBCLs];
  uchar bcl_R5[kMaxBCLs];
  uchar bcl_TR5[kMaxBCLs];
  uchar bcl_R6[kMaxBCLs];
  uchar bcl_TR6[kMaxBCLs];
  uchar bcl_R7[kMaxBCLs];
  uchar bcl_TR7[kMaxBCLs];
  uchar bcl_R8[kMaxBCLs];
  uchar bcl_TR8[kMaxBCLs];

  uchar ibBDstatus[kMaxDBs];

  uint failsTotalD_;    // total fails code
  uint failsTotalQ_;     // last value of fail code


  uint failsTotalQQ_;    // before last value
 
  uchar ibRS485;    // 1 - fail
  uchar ibI2C;      // 1 - fail

  uint channalValuePacked_;
  int iChannel; // Transmitter// Channel number
  int iChannelSetup;
  uint ibPower;
  int realPowerRepresent___; // Transmitter// Channel power
  uint ibValueFRW;
  int FRWValue___; // Transmitter  // real FRW * 100 = iFRW
  uchar ibSoundLock;    // Transmitter // Sound lock
  uchar ibSU1St;    // SU1 (or US1  see protocol) 1 - fail
  uchar ibSU2St;
  uint failBallast_Power;
  int iBallastPower;
  
  int transmitterAddress___;    // Transmitter// Number
  int iMaxTemre;    // Transmitter // maximum tempreture
  int iMaxStatus;    // Transmitter temperature status
  int new_max_temperature_;
  int iNewMaxStatus;
  int connecton_state___;  // connection status
  int snmp_connecton_status___;  // snmp connection status
    // SNMP status, 0 - stop, 1 - successful, 2 - not receive
 
  static const uchar  kSendCmd_ = 0;
  static const uchar  kBoundUpdate_ = 1;
  static const uchar  kSendSnmp_ = 2;
  uint current_ctrl_cmd___;  // current control command (send coomand of transmitter)
  int transmitter_number_to_set_;
  char cVtvIP[kMaxExciters][30];
  uint uiPort[kMaxExciters];
  char cExt[30];    // external IP
   
  char* cSNMP;
  int* iSNMPeriod;
  
  uchar fixedAlrmRecordIndex_;                  // index in array
  bool hasMsgForSnmp_;   // 
 
  int statusRecordIndex_;
  static const int kReserveSize = 1024;
  // 
  // Msgs Query

  bool newFailOccure_; // New Fail now
  uchar currentMWFCode_; // current Fail Warnig code
  uchar currentAliveFaWa___;
  bool FRWOk_;
  bool temperatureWasOk_;
  int counterFailsAndWarns_; // counter for fails
  int iUsedTime;
  int iUsedReserv;
  int lengthCfgAnswer; // length of config answer  

  // cfg
  int nominalPower___;
  uint numExcitersPack_;     // Number VTV
  int excitersTotal_;
  uint ibNumPAB;     // Number PAB
  uint PABTotal_;
  uint ibNumPAinPAB;   // Number power amplifier in PAB
  uint terminal_amps_per_block___;  // ��� ����� ���������
  uint ibNumBCV;     // Number BCV
  uint current_nominal_power;
  uint total_bcl_;
  uint ibNUMDB;     // Number BD
  uint DBTotal_;
  uint ibSizeIBMod; // size of information block for VTV modulator
  uint sizeBlockModParams_;
  uint ibSizeIBPAPAB;  // size of inform block for power amplif block
  uint sizeBlockTerminalAmpParams_;
  uint ibSizeIBPrAPAB;    // size of information block for preamplifier
  uint sizeBlockPreampParams_;  // size of inform block preamplif
  uint ibSizeIBBCV;    // size of information block for BCV
  uint sizeBlockBCNParams_;
  uint ibEventStringSize; 
  uint sizeEventsString_;
  uint ibFailStringSize;
  uint sizeFailsString_;

  uint ibSizeDB;
  uint sizeBlockDBParams_;

  uint transmitterID___;      // Transmitter Name
  uint exciterType_;              // VTV type
  uint countReservedTransmitters_;    // Number reserved tranmitters

  // VTV  Main
  uchar ibVtvCntrlMode[kMaxExciters];    // Control mode // 1 - dist
  uchar ibVtvRadioMode[kMaxExciters];    // radiation mode // 1 - reserv
  
  uchar etv_digital_[kMaxExciters];    // analod = 0 / digital = 1
  
  uchar ibUsVtvLock[kMaxExciters];      // US VTV // 1 - lock
  uchar ibModVtvLock[kMaxExciters];  // Modulator Lock // 1 - lock
  uchar ibOwnVtvLock[kMaxExciters];  // VTV lock // 1 - lock
  uchar ibVtvReady[kMaxExciters];      // VTV Ready // 1 - ready
  uchar ibVtvWork[kMaxExciters];      // VTV work// 1 - norm
 
  // bits 1..0
  uchar ibVtvStatus[kMaxExciters];      
  uchar exciterIstream_[kMaxExciters];  // VTV input stream// 1 - fail
  uchar connection_state_etv[kMaxExciters];
  uchar ibVtvOutSync[kMaxExciters];      // VTV out synchronization// 1 - fail

  uchar exciterIstreamError_[kMaxExciters];    // uchar
  
  static const uchar bNoConnecion = M1;
  uchar ibVtvErrMod[kMaxExciters];      // uchar
  uint ibVtvOutPower[kMaxExciters];    // vtv output power
  int iVtvOutPower[kMaxExciters];

  // analog
  uchar ibVtvSKSStatus[kMaxExciters];    // S K S
  uchar ibVtvSChMStatus[kMaxExciters];    // S Ch M
  uchar ibVtvSPChStatus[kMaxExciters];    // S P ch

  uchar ibVtvVideoLock[kMaxExciters];    // video lock unlock
  uchar ibVtvPowerStatus[kMaxExciters]; 
  uchar ibVtvSound[kMaxExciters];      // sound

  uchar ibVtvChanNumAnalog[kMaxExciters];
  int iVtvChanNumAnalog[kMaxExciters];

  // VTV  Mod
  uchar exciterASI12TgrVar_[kMaxExciters]; //for triggers haneda's code
  uchar ibVtvNet[kMaxExciters];      // Net power // bits 2..0
  uchar ibVtvDVBTOnOf[kMaxExciters];  // DVB-H on off// bits 1..0 -
  uchar ibVtvMIPOnOf[kMaxExciters];    // MIP on off// bits 1..0 -
  uchar ibVtvQAM[kMaxExciters];      // bits 2..0 -
  uchar ibVtvCodRate[kMaxExciters];    // bits 3..0 -
  uchar ibVtvGI[kMaxExciters];      // guard interval// bits 3..0 -
  uchar ibVtvHierar[kMaxExciters];    // bits 3..0 -
  uchar ibVtvCarNum[kMaxExciters];    // carriers number// bits 2..0 -
  uint ibVtvChannel[kMaxExciters];    // channel// uchar
  int iVtvChannel[kMaxExciters];   
  uchar ibVtvFreq[kMaxExciters][5]; // frequency// 
  int iVtvFreq[kMaxExciters];
  uint ibVtvOutLevel[kMaxExciters]; // 
  int iVtvOutLevel[kMaxExciters]; // level*10
  uint ibVtvTransmNum[kMaxExciters];    // transmitter number
  int iVtvTransmNum[kMaxExciters]; // 
  uint ibVtvIDCellNum[kMaxExciters];    // ID Cell Number
  int iVtvIDCellNum[kMaxExciters]; // 
  uint ibVtvAdDelay[kMaxExciters];    // additional delay
  int iVtvAdDelay[kMaxExciters]; // 
  uchar ibVtvPrecLineNum[kMaxExciters];    // preccorection line number
  int iVtvPrecLineNum[kMaxExciters]; // 
  uchar ibVtvPrecLOnOff[kMaxExciters];    // 3..0
  uchar ibVtvPrecNonLOnOff[kMaxExciters];  // 3..0
  uchar ibVtvTestMode[kMaxExciters]; // 3..0
  uchar ibVTVNoconnet[kMaxExciters]; // 1 - no connect

  // PAB main
  // haneda's code
  uchar PABOnOffTgrVar_[kMaxPABs];
  uchar PABLockTgrVar_[kMaxPABs];

  uchar pab_connection_state_[kMaxPABs];
  uchar ibPABAnlgDigit[kMaxPABs];    // 1 - dig
  uchar ibPABInPowerStatus[kMaxPABs];  // 1 - invalid
  uchar ibPABStatus[kMaxPABs];      // 1 - fail
  uchar ibPABFRWStatus[kMaxPABs];    // 1 - fail

  uint ibPABInPow[kMaxPABs];      // Input power// uchar
  int iPABInPow[kMaxPABs];
  uint ibPABOutPow[kMaxPABs]; // /PAB output power/2 uchar
  int iPABOutPow[kMaxPABs];
  uint ibPABFRW[kMaxPABs]; // FRW value// uchar
  int iPABFRW[kMaxPABs]; // FRW * 100

  // PAB  pre amplif
  uchar ibPreAGC[kMaxPABs]; // auto gain control// 1 -fail
  uchar ibPrePlus15[kMaxPABs]; // +15// 1 -fail
  uint ibPreCurVT1[kMaxPABs]; // current value VT1
  int iPreCurVT1[kMaxPABs]; // cut*10
  uint ibPreCurVT2[kMaxPABs]; // current value VT2
  int iPreCurVT2[kMaxPABs]; // cur*10
  uint ibPreTemValue[kMaxPABs]; // temperature value
  int iPreTemValue[kMaxPABs];

  uint ibPreMIPVoltage[kMaxPABs];
  int iPreMIPVoltage[kMaxPABs];    // vol *10
  uint ibPreAttenVoltage[kMaxPABs]; // attenuator
  int iPreAttenVoltage[kMaxPABs];    // Voltage*10
  uint ibPrePhaseVoltage[kMaxPABs];    // phase setting Voltage
  int iPreAPhaseVoltage[kMaxPABs];    // Voltage*100
  uint ibPreReferVoltage[kMaxPABs];    // reference voltage
  int iPreReferVoltage[kMaxPABs];    // Voltage*10

  // PAB  power amplif - ��������� �������
  uint ibPaCurVT1[kMaxPABs][kMaxTAsPerPAB]; // current value VT1
  int iPaCurVT1[kMaxPABs][kMaxTAsPerPAB]; // cut*10
  uint ibPaCurVT2[kMaxPABs][kMaxTAsPerPAB]; // current value VT2
  int iPaCurVT2[kMaxPABs][kMaxTAsPerPAB]; // cur*10
  uint ibPaTemValue[kMaxPABs][kMaxTAsPerPAB];
  int iPaTemValue[kMaxPABs][kMaxTAsPerPAB];

  uint ibPaMIPVoltage[kMaxPABs][kMaxTAsPerPAB];
  int iPaMIPVoltage[kMaxPABs][kMaxTAsPerPAB]; // vol *10
 
  // BCV kMaxBCLs
  uchar sum_bcl_[kMaxBCLs];
  uchar ibBCVTrOnOf[kMaxBCLs];
  uchar ibBCVTrLock[kMaxBCLs];
  uchar bcl_a_or_d_WTF[kMaxBCLs];
  uchar ibBCVPowerStat[kMaxBCLs];

  //temps, haneda's code
  int iBCV_R1PowTemp[kMaxBCLs];
  int iBCV_R2PowTemp[kMaxBCLs];
  int iBCV_R3PowTemp[kMaxBCLs];
  int iBCV_R4PowTemp[kMaxBCLs];
  int iBCV_R5PowTemp[kMaxBCLs];
  int iBCV_R6PowTemp[kMaxBCLs];
  int iBCV_R7PowTemp[kMaxBCLs];
  int iBCV_R8PowTemp[kMaxBCLs];

  int iBCV_R1Tem[kMaxBCLs];    // R1 temperature velue
  int iBCV_R1Pow[kMaxBCLs];    // R1 power velue
  int iBCV_R2Tem[kMaxBCLs];
  int iBCV_R2Pow[kMaxBCLs];
  int iBCV_R3Tem[kMaxBCLs];
  int iBCV_R3Pow[kMaxBCLs];
  int iBCV_R4Tem[kMaxBCLs];
  int iBCV_R4Pow[kMaxBCLs];
  int iBCV_R5Tem[kMaxBCLs];
  int iBCV_R5Pow[kMaxBCLs];
  int iBCV_R6Tem[kMaxBCLs];
  int iBCV_R6Pow[kMaxBCLs];
  int iBCV_R7Tem[kMaxBCLs];
  int iBCV_R7Pow[kMaxBCLs];
  int iBCV_R8Tem[kMaxBCLs];
  int iBCV_R8Pow[kMaxBCLs];

  // DB kMaxDBs
  uchar sum_about_db_[kMaxDBs];
  uchar ibBDTrOnOf[kMaxDBs];
  uchar ibBDTrLock[kMaxDBs];
  uchar ibBDAnlgDigit[kMaxDBs];
  uchar ibBDChannel[kMaxDBs];
  uchar ibBDFactor[kMaxDBs];
 
  int sound_power_[kMaxDBs];
  
  //haneda's code
  int sound_power_Temp[kMaxDBs];
  int iBDOutPowerTemp[kMaxDBs];

  int iBDOutPower[kMaxDBs];
  uchar iBDFRW[kMaxDBs];

 protected:
  //@EmitterPart
  std::vector<::channels::Slot> vectorSetNull(std::vector<::channels::Slot>);

  //@Deprecated
  std::vector<std::vector<std::vector<::channels::Slot> > > emitMain();

  //@Deprecated
  std::vector<std::vector<::channels::Slot>> emitMainParamsETV__dep(
      std::vector<std::vector<::channels::Slot>>, 
      int number);

  std::vector<::channels::Slot> emitMainParamsETV_(int number);
  std::vector<::channels::Slot> emitGlobalMainParams_();
  std::vector<::channels::Slot> emitModulatorParam(int number);
  std::vector<::channels::Slot> emitMainBum(int number);
  std::vector<::channels::Slot> emitPreampBum(int number);
  std::vector<::channels::Slot> emitVTV(int number);
  std::vector<::channels::Slot> emitAmpPowBum(int number, int terminalAmpsPerBlockIndex);
  std::vector<::channels::Slot> emitParamsComplexLoad_(int number);
  std::vector<::channels::Slot> emitParamsDetector_(int number);

  //@ParserPart
  void ParseMainStateTmitter(const ::channels::Slot* ptrSourceArray);
  void ParseMainParamsExciter(const int number, const ::channels::Slot* ptrSourceArray);
  void ParseModParams(int number, const ::channels::Slot* ptrSourceArray);
  void ParseMainParamsPAB(int number, const ::channels::Slot* ptrSourceArray);
  void ParsePreAmpParams(int number, const ::channels::Slot* ptrSourceArray);
  void ParseTerminalAmpliferParams(
      int number, int n, 
      const ::channels::Slot* ptrSourceArray);
  void ParseParamsBCL(int number, const ::channels::Slot* ptrSourceArray);
  void ParseParamsDB(int number, const ::channels::Slot* ptrSourceArray);
  
  //DANGER_ANALOG_NO_FOUND
  void ProcessExciterAnalog(uchar number, const ::channels::Slot* ptrSourceArray); 
};
}

#endif  // CC_HEADERS_WEB_SERVICE_SAMPLER_H_


