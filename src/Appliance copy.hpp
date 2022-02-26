// #pragma once

// #include <Arduino.h>
// #include <SPI.h>
// #include <arduino-timer.h>
// #include "RFM69_ATC.h"
// #include "RadioConfig.hpp"
// #include "MessageBuffer.hpp"

// class Appliance
// {
// public:
//     Appliance(uint8_t radioPairBtn, uint8_t statusLed_);
//     ~Appliance() = default;

// protected:


//     /** Initialization */
//     virtual void setupGPIO();
//     virtual void setupTimer();
//     void readConfig();

//     /** Events */
//     bool radioPairingEvent();

//     /** Handlers */
//     HandlerState radioPairingHandler();
//     HandlerState radioReceiveHandler();
//     HandlerState statusLedHandler();

//     /** Callbacks */
//     virtual void statusLedBlinkCallback(){};

//     // // Configuration
//     // virtual void readConfiguration();

//     // // Radio operations
//     // void radioSetup(const RadioConfigData &data);
//     // bool radioSend(const MessageBuffer *request, MessageBuffer *response, bool ack = true);
//     // bool radioPayloadToBuffer();
    
//     // virtual bool radioMessageProcess() = 0;
    
//     // bool isRadioPairBtnTriggered();
//     // RadioConfigData getRadioConfigForPair();

//     // /** Tasks */
//     // virtual TaskState radioPairTask();
//     // TaskState radioReceiveTask();

//     // /**
//     //  * @brief Function
//     //  * 
//     //  */
//     // void sendACKRepsonse();

// #if defined(__AVR__)
//     // Power saving operations
//     void deepSleepDelay(unsigned int delay);
//     void deepSleepForewerAndWakeInt(uint8_t pin, uint8_t mode);
// #endif

//     // Notifications
//     // virtual void statusLedBlink(int16_t speed) = 0;
//     // virtual void notifyPairStarted();
//     // virtual void notifyPairSuccess();
//     // virtual void notifyPairFailed();

// protected:
//     RFM69_ATC radio_;
//     RadioConfig radioConfig_;

//     MessageBuffer messageBuffer_;
//     const uint8_t RadioPairPin_;

//     CurrentJob currentJob_ = CurrentJob::Init;

//     Timer<1> timer_;
// };