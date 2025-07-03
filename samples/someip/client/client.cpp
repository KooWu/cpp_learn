#include <iostream>
#include <cstdint>
#include <thread>
#include <chrono>

#include <CommonAPI/CommonAPI.hpp>
#include "v1/soa/sample/TestSrvIfProxy.hpp"

using namespace v1::soa::sample;

int32_t main(int argc, char **argv) {
    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();
    if (runtime == nullptr) {
        std::cout << "get runtime failed" << std::endl;
        return -1;
    }

    std::shared_ptr<v1::soa::sample::TestSrvIfProxy<>> proxy = runtime->buildProxy<TestSrvIfProxy>("local", "TestSrvIf.server_app");
    if (proxy == nullptr) {
        std::cout << "build proxy failed" << std::endl;
        return -1;
    }
    std::cout << "build proxy successful" << std::endl;

    while (!proxy->isAvailable()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "find service successful" << std::endl;
    ::soa::sample::CustomTypes::PlusParamInfo _inData;
    _inData.setX(10);
    _inData.setY(35);
    CommonAPI::CallStatus callStatus;
    int32_t outData;
    CommonAPI::CallInfo info(-1);
    proxy->PlusMtd(_inData, callStatus, outData, &info);
    if (callStatus != CommonAPI::CallStatus::SUCCESS) {
        std::cout << "PlusMtd failed" << std::endl;
    }
    std::cout << "PlusMtd success outData: " << outData << std::endl;
    std::string helloData("hello, my name is yxw");
    proxy->HelloMtd(helloData, callStatus);
    if (callStatus != CommonAPI::CallStatus::SUCCESS) {
        std::cout << "PlusMtd failed" << std::endl;
    }
    std::cout << "HelloMtd success" << std::endl;

    int8_t status = 0;
    proxy->getStatusFldAttribute().getValue(callStatus, status, &info);
    if (callStatus != CommonAPI::CallStatus::SUCCESS) {
        std::cout << "proxy->getStatusFldAttribute().getValue failed" << std::endl;
    }
    std::cout << "getStatusFldAttribute get status: " << (int32_t)status << std::endl;
    status = 10;
    int8_t outStatus;
    proxy->getStatusFldAttribute().setValue(status, callStatus, outStatus, &info);
    if (callStatus != CommonAPI::CallStatus::SUCCESS) {
        std::cout << "getStatusFldAttribute().setValue failed" << std::endl;
    }
    std::cout << "getStatusFldAttribute set status = 10: " << (int32_t)outStatus << std::endl;
    proxy->getTimeEvtEvent().subscribe([](const ::soa::sample::CustomTypes::TimeInfo &data) {
        std::cout << "recv event time info" << std::endl;
        std::cout << "timestamp: " << data.getTimestamp() << std::endl;
        std::cout << "seq: " << data.getSeq() << std::endl;
    });

    proxy->getStatusFldAttribute().getChangedEvent().subscribe([](const int8_t &status) {
        std::cout << "recv fld status info: " <<  (int32_t)status << std::endl;
    });

    proxy->getStatusFldAttribute().getChangedEvent().subscribe([](const int8_t &status) {
        std::cout << "recv11 fld status info: " <<  (int32_t)status << std::endl;
    });

    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}