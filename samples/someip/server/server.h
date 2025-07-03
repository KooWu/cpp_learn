#pragma once

#include <CommonAPI/CommonAPI.hpp>
#include "v1/soa/sample/TestSrvIfStubDefault.hpp"

class TestSrvIfStubImpl : public v1::soa::sample::TestSrvIfStubDefault {
public:
    TestSrvIfStubImpl();
    virtual ~TestSrvIfStubImpl();
    void PlusMtd(const std::shared_ptr<CommonAPI::ClientId> _client, ::soa::sample::CustomTypes::PlusParamInfo _inData, PlusMtdReply_t _reply);
    void HelloMtd(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _inData);
};