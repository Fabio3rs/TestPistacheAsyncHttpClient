#pragma once

#include "../vendor/cppapiframework/src/WebInterface/CController.hpp"
#include "../vendor/cppapiframework/src/WebInterface/CPistacheEndpoint.hpp"
#include "../vendor/cppapiframework/src/WebInterface/WebApp.hpp"
#include "../vendor/cppapiframework/src/boot.hpp"
#include "../vendor/cppapiframework/src/jobhandler/JobsHandler.hpp"
#include "../vendor/cppapiframework/src/jobhandler/QueueableJob.hpp"
#include "../vendor/cppapiframework/src/utils/CConfig.hpp"
#include "../vendor/cppapiframework/src/utils/CLog.hpp"
#include "../vendor/cppapiframework/src/utils/RedisService.hpp"
#include "../vendor/cppapiframework/src/utils/Strutils.hpp"
#include "../vendor/cppapiframework/src/utils/Validator.hpp"
#include <pistache/client.h>
#include <csignal>
#include <cstdlib>
