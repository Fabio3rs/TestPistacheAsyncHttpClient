#include "webmgr.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <pistache/client.h>
#include <pistache/tcp.h>
#include <string>
#include <thread>
#include <utility>

void web::webmgr::run(int port) {
    using namespace webapp;
    app = std::make_unique<WebApp>(port, 125);

    {
        using namespace Pistache::Http::Experimental;
        Client::Options opt;
        client.init(opt.threads(125));
    }

    auto asyncRequestResponse = [this](Req /*req*/, Resp resp) {
        using namespace Pistache::Http;

        std::string address = "http://127.0.0.1:1234/another_address";
        auto resp_srv2 = client.get(address).send();

        auto responseheap =
            std::make_shared<ResponseWriter>(std::move(*resp.response));
        resp.response = nullptr;

        resp_srv2.then(
            [responseheap](Response srvresponse) mutable {
                auto response = std::move(responseheap);
                auto peerlocked = response->getPeer();

                if (!peerlocked) {
                    // Client disconnected
                    return;
                }
                // set mime type...
                response->send(srvresponse.code(), srvresponse.body());
            },
            [responseheap](std::exception_ptr exc) mutable {
                auto response = std::move(responseheap);
                auto peerlocked = response->getPeer();

                if (!peerlocked) {
                    // Client disconnected
                    return;
                }

                PrintException excPrinter;
                excPrinter(std::move(exc));

                // set mime type...
                response->send(Code::Internal_Server_Error, "{}");
            });
    };

    auto delayedResponse = [](Req /*req*/, Resp resp) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::string strresp;

        strresp.resize(8192 * 2, 'A');

        resp.send(Code::Ok, strresp);
    };

    using fn_t = RouterWrapper::respnotviaretfn_t;

    app->get("/verify_another_api", fn_t(asyncRequestResponse))
        .get("/another_address", (delayedResponse));

    app->startAsync();
}

// NOLINTNEXTLINE
void web::webmgr::stop() {
    if (app) {
        app->stop();
    }

    client.shutdown();
}
