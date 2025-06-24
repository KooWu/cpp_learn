#define FDB_LOG_TAG "HELLO_SERVER"
#include <fdbus/fdbus.h>
#include "test.pb.h"
#include <fdbus/CFdbProtoMsgBuilder.h>
#include <fdbus/cJSON/cJSON.h>

using namespace ipc::fdbus;
// FDBUS提供的功能线程
static CBaseWorker main_worker;

static const int METHOD_ID = 1;

class HelloServer : public CBaseServer
{
public:
    HelloServer(const char*name, CBaseWorker* work = 0)
            : CBaseServer(name, work)
    {
            // Empty
    }


protected:
    void onOnline(const CFdbOnlineInfo &info)
    {
        std::cout << "connected to the client" << std::endl;
    }

    void onOffline(const CFdbOnlineInfo &info)
    {
        std::cout << "disconnected from client" << std::endl;
    }

    /* called when client calls invoke() */
    void onInvoke(CBaseJob::Ptr &msg_ref)
    {
        auto msg = castToMessage<CBaseMessage *>(msg_ref);
        static int32_t elapse_time = 0;
        switch (msg->code())
        {
            case METHOD_ID:
                {
                HelloWorld client;
                CFdbProtoMsgParser parser(client);
                if (!msg->deserialize(parser))
                {
                    return;
                }

                std::cout << "Client name " << client.name() << std::endl;
                std::cout << "Client age " << client.age() << std::endl;
                /* fill in protocol buffer and reply to client */
                HelloWorld server;
                server.set_name("Linduo");
                server.set_age(24);
                CFdbProtoMsgBuilder builder(server);
                msg->reply(msg_ref, builder);
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

    /* create servers and bind the address: svc://service_name */
    for (int i = 1; i < argc; ++i)
    {
        std::string server_name = argv[i];
        std::string url(FDB_URL_SVC);
        url += server_name;
        server_name += "_server";
        auto server = new HelloServer(server_name.c_str(), worker_ptr);
        server->enableWatchdog(true);
        server->enableUDP(true);
        server->setExportableLevel(FDB_EXPORTABLE_SITE);
        server->bind(url.c_str());
    }

    // 将主线程变为WorkThread
    CBaseWorker background_worker;
    background_worker.start(FDB_WORKER_EXE_IN_PLACE);
}
