#include <wsjcpp_core.h>
#include "http_handler_available_filters.h"

// ----------------------------------------------------------------------

HttpHandlerAvailableFilters::HttpHandlerAvailableFilters(
    const std::vector<GuitarSoloPartGenerateFilterBase *> &vFilters
) : WsjcppLightWebHttpHandlerBase("available-filters") {
    TAG = "HttpHandlerAvailableFilters";
    for (int i = 0; i < vFilters.size(); i++) {
        m_vFilters.push_back(vFilters[i]);
    }
}

// ----------------------------------------------------------------------

bool HttpHandlerAvailableFilters::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();

    if (sRequestPath == "/api/v1/available-filters/" || sRequestPath == "/api/v1/available-filters") {
        return true;    
    }
    return false;
}

// ----------------------------------------------------------------------

bool HttpHandlerAvailableFilters::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    // WsjcppLog::warn(_tag, sRequestPath);
    
    WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
    if (sRequestPath != "/api/v1/available-filters/" && sRequestPath != "/api/v1/available-filters") {
        return false;
    }

    nlohmann::json jsonResponse;
    nlohmann::json jsonResult = nlohmann::json::array();

    for (int i = 0; i < m_vFilters.size(); i++) {
        GuitarSoloPartGenerateFilterBase *pFilter = m_vFilters[i];
        nlohmann::json jsonFilter;
        jsonFilter["name"] = pFilter->getName();
        jsonFilter["datatype"] = pFilter->getDataTypeAsString();
        jsonFilter["caption"] = pFilter->getCaption();
        jsonFilter["description"] = pFilter->getDescription();
        jsonFilter["values"] = pFilter->getJsonValues();
        jsonResult.push_back(jsonFilter);
    }
    jsonResponse["result"] = jsonResult;
    resp.ok().noCache().sendJson(jsonResponse);
    return true;
}