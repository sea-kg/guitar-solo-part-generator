#ifndef HTTP_HANDLER_SOLO_GENERATE_H
#define HTTP_HANDLER_SOLO_GENERATE_H

#include <wsjcpp_light_web_server.h>
#include "guitar_solo_part_generate_filters.h"
#include "guitar_solo_part_generator_movement_rules.h"

class HttpHandlerSoloGenerate : public WsjcppLightWebHttpHandlerBase {
    public:
        HttpHandlerSoloGenerate(
            GuitarSoloPartGeneratorMovementRules *pMovementRules, 
            const std::vector<GuitarSoloPartGenerateFilterBase *> &vFilters
        );
        virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) override;
        virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) override;

    private:
        std::string TAG;
        GuitarSoloPartGeneratorMovementRules *m_pMovementRules;
        std::vector<GuitarSoloPartGenerateFilterBase *> m_vFilters;
};
#endif // HTTP_HANDLER_SOLO_GENERATE_H
