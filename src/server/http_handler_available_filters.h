#ifndef HTTP_HANDLER_AVAILABLE_FILTERS_H
#define HTTP_HANDLER_AVAILABLE_FILTERS_H

#include <wsjcpp_light_web_server.h>
#include "guitar_solo_part_generate_filters.h"

class HttpHandlerAvailableFilters : public WsjcppLightWebHttpHandlerBase {
    public:
        HttpHandlerAvailableFilters(const std::vector<GuitarSoloPartGenerateFilterBase *> &vFilters);
        virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) override;
        virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) override;

    private:
        std::string TAG;
        std::vector<GuitarSoloPartGenerateFilterBase *> m_vFilters; 
};
#endif // HTTP_HANDLER_AVAILABLE_FILTERS_H