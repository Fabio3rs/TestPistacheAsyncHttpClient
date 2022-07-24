#pragma once

#include "../stdafx.hpp"
#include <memory>

namespace web {

class webmgr {

  public:
    std::unique_ptr<webapp::WebApp> app;

    Pistache::Http::Experimental::Client client;

    void run(int port);
    void stop();
};

} // namespace web
