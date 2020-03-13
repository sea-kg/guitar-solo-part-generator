#ifndef HTTP_HANDLER_MOVEMENT_RULES_H
#define HTTP_HANDLER_MOVEMENT_RULES_H

#include <wsjcpp_light_web_server.h>
#include "guitar_solo_part_generator_movement_rules.h"

class HttpHandlerMovementRules : WSJCppLightWebHttpHandlerBase {
    public:
        HttpHandlerMovementRules(GuitarSoloPartGeneratorMovementRules  *pMovementRules);
        virtual bool canHandle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest);

    private:
        std::string TAG;
        GuitarSoloPartGeneratorMovementRules  *m_pMovementRules;
};
#endif // HTTP_HANDLER_MOVEMENT_RULES_H