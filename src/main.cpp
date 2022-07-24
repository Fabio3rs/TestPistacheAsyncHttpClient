#include "stdafx.hpp"
#include "web/webmgr.hpp"
#include <string>
#include <unistd.h>

namespace {
void shutdownSignalHandler(int signal);
void waitUserCommand();

web::webmgr app;
} // namespace

int main() {
    auto &config = CConfig::config();
    config.load_from_envp(environ);

    CLog::initSingleton({{}, &std::cout});
    signal(SIGTERM, shutdownSignalHandler);
    signal(SIGPIPE, shutdownSignalHandler);

    app.run(std::stoi(config.at("PORT", "9000")));

    waitUserCommand();
}

namespace {
std::atomic<bool> keepRunning{true};
std::mutex commandMutex;

void shutdownApp();

void shutdownSignalHandler(int signal) {
    std::cout << "signal   " << signal << std::endl;
    CLog::log().multiRegister("Signal received %0", signal);

    if (signal != SIGPIPE) {
        shutdownApp();
    }
}

void shutdownApp() {
    std::lock_guard<std::mutex> lck(commandMutex);

    auto &log = CLog::log();

    log << "Starting signal sequence";
    keepRunning = false;

    app.stop();

    log << "std::cin will be closed to stop command reading";
    log.multiRegister("close(0) -> %0", close(0));
}

void waitUserCommand() {
    std::cout << "Type stop to exit\n";
    std::string str;
    while (keepRunning) {
        str.clear();
        std::cin >> str;

        if (std::cin.fail() || str.empty()) {
            if (!keepRunning) {
                return;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        std::lock_guard<std::mutex> lck(commandMutex);

        if (str == "stop") {
            std::cout << "Exiting..." << std::endl;
            app.stop();
            break;
        }
    }
}
} // namespace
