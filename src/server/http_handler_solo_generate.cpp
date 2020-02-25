#include <wsjcpp_core.h>
#include "http_handler_solo_generate.h"
#include "solo_part_guitar.h"

// ----------------------------------------------------------------------

HttpHandlerSoloGenerate::HttpHandlerSoloGenerate(const SoloPartGuitarRules &rules)
: WSJCppLightWebHttpHandlerBase("solo-generate") {
    TAG = "HttpHandlerSoloGenerate";
    m_rules = rules;
}

// ----------------------------------------------------------------------

bool HttpHandlerSoloGenerate::canHandle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();

    if (sRequestPath == "/api/v1/solo-generate/" || sRequestPath == "/api/v1/solo-generate") {
        return true;    
    }
    return false;
}

// ----------------------------------------------------------------------

bool HttpHandlerSoloGenerate::handle(const std::string &sWorkerId, WSJCppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    // WSJCppLog::warn(_tag, sRequestPath);
    
    WSJCppLightWebHttpResponse resp(pRequest->getSockFd());
    if (sRequestPath != "/api/v1/solo-generate/" && sRequestPath != "/api/v1/solo-generate") {
        return false;
    }

    int nFirstString = (std::rand() % 6) + 1;
    int nFirstFret = (std::rand() % 24);
    int nFirstFinger = (std::rand() % 5);
    if (nFirstFret == 0) {
        nFirstFinger = 0;
    } else {
        nFirstFinger = (std::rand() % 4) + 1;
    }
    PositionNoteGuitar note(
        (GuitarNumberString)nFirstString, 
        nFirstFret, 
        (GuitarTouchFinger)nFirstFinger, 
        ::GUITAR_DURATION_OF_NOTE_CROTCHET
    );

    WSJCppLog::info(TAG, "start note: " + note.toPrintableString());
    SoloPartGuitar part;
    part.addNote(note);
    for (int i = 0; i < 16; i++) {
        std::vector<PositionNoteGuitar> vNotes = m_rules.findWithBegin(note);
        std::cout << "Found possible note " << vNotes.size() << std::endl;
        if (vNotes.size() == 0) { // TODO come to no finger
            WSJCppLog::err(TAG, "Not found");
            return false;
        }
        note = vNotes[std::rand() % vNotes.size()];
        part.addNote(note);
        WSJCppLog::info(TAG, "note[" + std::to_string(i) + "] = " + note.toPrintableString());
    }
    nlohmann::json jsonResponse;
    jsonResponse["tabulatur"] = part.exportTabulatur();
    jsonResponse["part"] = part.exportToJson();
    
    resp.ok().noCache().sendJson(jsonResponse);

    return true;
}