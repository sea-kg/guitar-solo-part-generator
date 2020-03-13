#include <wsjcpp_core.h>
#include "http_handler_movement_rules.h"

// ----------------------------------------------------------------------

HttpHandlerMovementRules::HttpHandlerMovementRules(GuitarSoloPartGeneratorMovementRules  *pMovementRules) 
: WSJCppLightWebHttpHandlerBase("movement-rules") {
    TAG = "HttpHandlerMovementRules";
    m_pMovementRules = pMovementRules;
}

// ----------------------------------------------------------------------

bool HttpHandlerMovementRules::canHandle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();

    if (sRequestPath == "/api/v1/movement-rules/" || sRequestPath == "/api/v1/movement-rules") {
        return true;    
    }
    return false;
}

// ----------------------------------------------------------------------

bool HttpHandlerMovementRules::handle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    // WSJCppLog::warn(_tag, sRequestPath);
    
    WSJCppLightWebHttpResponse resp(pRequest->getSockFd());
    if (sRequestPath != "/api/v1/movement-rules/" && sRequestPath != "/api/v1/movement-rules") {
        return false;
    }

    nlohmann::json jsonResponse;
    nlohmann::json jsonResult = nlohmann::json::array();
    std::vector<std::string> vPredefinedRules = m_pMovementRules->getPredefinedRules();
    for (int i = 0; i < vPredefinedRules.size(); i++) {
        jsonResult.push_back(vPredefinedRules[i]);
    }
    jsonResponse["result"] = jsonResult;
    resp.ok().noCache().sendJson(jsonResponse);
    return true;
}