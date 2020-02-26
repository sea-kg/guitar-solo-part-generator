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
    jsonResponse["result"] = nlohmann::json::array();

    // TODO redesign like a class filters
    nlohmann::json jsonMinFret;
    jsonMinFret["name"] = "min_fret";
    jsonMinFret["caption"] = "Min Fret";
    jsonMinFret["datatype"] = "list";
    jsonMinFret["values"] = nlohmann::json::array();
    for (int i = 0; i <= 24; i++) {
        nlohmann::json jsonValue;
        jsonValue["value"] = i;
        jsonValue["caption"] = "Fret #" + std::to_string(i);
        jsonMinFret["values"].push_back(jsonValue);
    }

    // TODO redesign like a class filters
    nlohmann::json jsonMaxFret;
    jsonMaxFret["name"] = "max_fret";
    jsonMaxFret["caption"] = "Max Fret";
    jsonMaxFret["datatype"] = "list";
    jsonMaxFret["values"] = nlohmann::json::array();
    for (int i = 24; i >= 0; i--) {
        nlohmann::json jsonValue;
        jsonValue["value"] = i;
        jsonValue["caption"] = "Fret #" + std::to_string(i);
        jsonMaxFret["values"].push_back(jsonValue);
    }

    jsonResponse["result"].push_back(jsonMinFret);
    jsonResponse["result"].push_back(jsonMaxFret);
    resp.ok().noCache().sendJson(jsonResponse);
    return true;
}