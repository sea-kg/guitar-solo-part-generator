#ifndef HTTP_HANDLER_SOLO_GENERATE_H
#define HTTP_HANDLER_SOLO_GENERATE_H

#include <wsjcpp_light_web_server.h>
#include "solo_part_guitar_rules.h"

class HttpHandlerSoloGenerate : WSJCppLightWebHttpHandlerBase {
    public:
        HttpHandlerSoloGenerate(const SoloPartGuitarRules &rules);
        virtual bool canHandle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest);

    private:
        std::string TAG;
        SoloPartGuitarRules m_rules;
        std::vector<PositionNoteGuitar> filterByMinFret(const std::vector<PositionNoteGuitar> &vNotes, int nFilterMinFret);
        std::vector<PositionNoteGuitar> filterByMaxFret(const std::vector<PositionNoteGuitar> &vNotes, int nFilterMaxFret);
};
#endif // HTTP_HANDLER_SOLO_GENERATE_H