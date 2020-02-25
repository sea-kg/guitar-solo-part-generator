#include <wsjcpp_core.h>
#include "http_handler_available_filters.h"
#include "solo_part_guitar.h"

// ----------------------------------------------------------------------

HttpHandlerAvailableFilters::HttpHandlerAvailableFilters(const SoloPartGuitarRules &rules)
: WSJCppLightWebHttpHandlerBase("available-filters") {
    TAG = "HttpHandlerAvailableFilters";
    m_rules = rules;
}

// ----------------------------------------------------------------------

bool HttpHandlerAvailableFilters::canHandle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();

    if (sRequestPath == "/api/v1/available-filters/" || sRequestPath == "/api/v1/available-filters") {
        return true;    
    }
    return false;
}

// ----------------------------------------------------------------------

bool HttpHandlerAvailableFilters::handle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    // WSJCppLog::warn(_tag, sRequestPath);
    
    WSJCppLightWebHttpResponse resp(pRequest->getSockFd());
    if (sRequestPath != "/api/v1/available-filters/" && sRequestPath != "/api/v1/available-filters") {
        return false;
    }

    nlohmann::json jsonResponse;
    // TODO
    resp.ok().noCache().sendJson(jsonResponse);
    return true;
}