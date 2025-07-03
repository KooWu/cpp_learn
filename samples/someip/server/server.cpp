#include "server.h"
#include <iostream>
#include <cstdint>
#include <thread>
#include <chrono>

using namespace v1::soa::sample;

TestSrvIfStubImpl::TestSrvIfStubImpl() {}
TestSrvIfStubImpl::~TestSrvIfStubImpl() {}

void TestSrvIfStubImpl::PlusMtd(const std::shared_ptr<CommonAPI::ClientId> _client, ::soa::sample::CustomTypes::PlusParamInfo _inData, PlusMtdReply_t _reply) {
    (void)_client;
    std::cout << "server handle PlusMtd" << std::endl;
    int32_t outData = _inData.getX() + _inData.getY();
    _reply(outData);
}

void TestSrvIfStubImpl::HelloMtd(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _inData) {
    (void)_client;
    (void)_inData;
    std::cout << "server handle HelloMtd" << std::endl;
    std::cout << _inData << std::endl;
}


int32_t main(int argc, char **argv) {
    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();
    if (runtime == nullptr) {
        std::cout << "get runtime failed" << std::endl;
        return -1;
    }

    std::shared_ptr<TestSrvIfStubImpl> srv = std::make_shared<TestSrvIfStubImpl>();
    if (srv == nullptr) {
        std::cout << "make srv failed" << std::endl;
        return -1;
    }

    bool isOk = runtime->registerService("local", "TestSrvIf.server_app", srv);
    if (!isOk) {
        std::cout << "register srv failed" << std::endl;
        return -1;
    }
    std::cout << "register srv successful" << std::endl;

    ::soa::sample::CustomTypes::TimeInfo data;
    data.setSeq(0);
    data.setTimestamp(0);
    int8_t status = 18;
    srv->setStatusFldAttribute(status);
    int64_t i = 0;
    while (1) {
        i++;
        status++;
        data.setSeq(i);
        srv->fireTimeEvtEvent(data);
        srv->setStatusFldAttribute(status);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}