#define FDB_LOG_TAG "HELLO_CLIENT"
#include <fdbus/fdbus.h>
#include <thread>
#include "test.pb.h"
#include <fdbus/CFdbProtoMsgBuilder.h>
#include <fdbus/cJSON/cJSON.h>
#include <fdbus/CFdbCJsonMsgBuilder.h>

// 引用命名空间
// 正式开发代码不建议
using namespace ipc::fdbus;
// FDBUS提供的功能线程
static CBaseWorker main_worker;
static const int METHOD_ID = 1;

class HelloClient : public CBaseClient
{
public:
	HelloClient(const char*name, CBaseWorker* work = 0)
		: CBaseClient(name, work)
	{
		// Empty
	}

	void callServerSync()
	{
		// ProtoBuffer
        HelloWorld my;
        my.set_name("Adver");
        // 序列化
        CFdbProtoMsgBuilder builder(my);
        // 同步调用
        CBaseJob::Ptr ref(new CBaseMessage(1));
        invoke(ref, builder);

        // 解析Server返回的内容
        auto msg = castToMessage<CBaseMessage *>(ref);
        if (msg->isStatus()) {
        	// Error;
        	return;
        }

        // 反序列化
        HelloWorld server;
        CFdbProtoMsgParser parser(server);
        bool result = msg->deserialize(parser);
        if (result) {
    		std::cout << "My Name:" << std::endl;
        	std::cout << server.name() << std::endl;
            std::cout << "My age:" << std::endl;
            std::cout << server.age() << std::endl;
        }
	}


	void callServerAsync()
	{
		// ProtoBuffer
        HelloWorld my;
        my.set_name("Adver");
        my.set_age(34);
        // 序列化
        CFdbProtoMsgBuilder builder(my);
        // 异步调用
        invoke(METHOD_ID, builder);
	}

protected:
    void onOnline(const CFdbOnlineInfo &info)
    {
    	std::cout << "connected to the server" << std::endl;
        callServerAsync();
    }

     void onOffline(const CFdbOnlineInfo &info)
     {
     	std::cout << "disconnected from server" << std::endl;
     }

	void onReply(CBaseJob::Ptr &msg_ref)
	{
        auto msg = castToMessage<CBaseMessage *>(msg_ref);

     	switch (msg->code())
        {
            case METHOD_ID:
	            {
	                if (msg->isStatus())
	                {
	      				std::cout << "Error" << std::endl;
	                    if (msg->isError())
	                    {
	                    	return;
	                    }
	                    return;
	                }

	                HelloWorld server;
	                CFdbProtoMsgParser parser(server);
	                if (msg->deserialize(parser))
	                {
	           			std::cout << "Server name " << server.name() << std::endl;
                        std::cout << "Server age " << server.age() << std::endl;
	                }

                    // callServerAsync();
	            }
	            break;
            default:
            	break;
        }
	}
};


int main(int argc, char*argv[])
{
    /* start fdbus context thread */
    FDB_CONTEXT->start();
    fdbLogAppendLineEnd(true);
    FDB_CONTEXT->registerNsWatchdogListener([](const tNsWatchdogList &dropped_list)
    {
        for (auto it = dropped_list.begin(); it != dropped_list.end(); ++it)
        {
        	printf("Error!!! Endpoint drops - name: %s, pid: %d\n",
        		it->mClientName.c_str(), it->mPid);
        }
    });

	CBaseWorker *worker_ptr = &main_worker;
    /* start worker thread */
    worker_ptr->start();

    for (int i = 1; i < argc; ++i)
    {
        std::string server_name = argv[i];
        std::string url(FDB_URL_SVC);
        url += server_name;
        server_name += "_client";
        auto client = new HelloClient(server_name.c_str(), worker_ptr);
        
        client->enableReconnect(true);
        client->enableUDP(true);
        client->enableTimeStamp(true);
        client->connect(url.c_str());
    }

    // 将主线程变为WorkThread
    CBaseWorker background_worker;
    background_worker.start(FDB_WORKER_EXE_IN_PLACE);
}
