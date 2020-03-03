#include <wsjcpp_core.h>
#include "http_handler_solo_generate.h"
#include "solo_part_guitar.h"

// ----------------------------------------------------------------------

HttpHandlerSoloGenerate::HttpHandlerSoloGenerate(
    const SoloPartGuitarRules &rules, 
    const std::vector<GuitarSoloPartGenerateFilterBase *> &vFilters
)
: WSJCppLightWebHttpHandlerBase("solo-generate") {
    TAG = "HttpHandlerSoloGenerate";
    m_rules = rules;
    for (int i = 0; i < vFilters.size(); i++) {
        m_vFilters.push_back(vFilters[i]);
    }
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

    int nFilterMinFret = 0;
    int nFilterMaxFret = 24;
    std::vector<WSJCppLightWebHttpRequestQueryValue> vQueries = pRequest->getRequestQueryParams();

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
        ::GUITAR_DURATION_OF_NOTE_1_4_CROTCHET
    );

    WSJCppLog::info(TAG, "start note: " + note.toPrintableString());
    SoloPartGuitar part;
    part.addNote(note);
    for (int i = 0; i < 15; i++) {
        std::vector<PositionNoteGuitar> vNotes = m_rules.findWithBegin(note);

        for (int f = 0; f < m_vFilters.size(); f++) {
            GuitarSoloPartGenerateFilterBase *pFilter = m_vFilters[f];
            std::string sFilterName = pFilter->getName();
            for (int q = 0; q < vQueries.size(); q++) {
                WSJCppLightWebHttpRequestQueryValue query = vQueries[q];
                if (query.getName() == sFilterName) {
                    vNotes = pFilter->applyFilter(vNotes, query.getValue());
                }

                /*std::string sName = query.getName();
                std::string sValue = query.getValue();
                if (sName == "min_fret") {
                    nFilterMinFret = atoi(sValue.c_str());
                } else if (sName == "max_fret") {
                    nFilterMaxFret = atoi(sValue.c_str());
                }*/
            }
        }

        std::cout << "Found possible note " << vNotes.size() << std::endl;
        if (vNotes.size() == 0) { // TODO come to no finger
            resp.badRequest().noCache().sendText("Could not generate with this params");
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

// ----------------------------------------------------------------------

std::vector<PositionNoteGuitar> HttpHandlerSoloGenerate::filterByMinFret(const std::vector<PositionNoteGuitar> &vNotes, int nFilterMinFret) {
    std::vector<PositionNoteGuitar> vRet;
    for (int i = 0; i < vNotes.size(); i++) {
        if (vNotes[i].getFret() >= nFilterMinFret) {
            vRet.push_back(vNotes[i]);
        }
    }
    return vRet;
}

// ----------------------------------------------------------------------

std::vector<PositionNoteGuitar> HttpHandlerSoloGenerate::filterByMaxFret(const std::vector<PositionNoteGuitar> &vNotes, int nFilterMaxFret) {
    std::vector<PositionNoteGuitar> vRet;
    for (int i = 0; i < vNotes.size(); i++) {
        if (vNotes[i].getFret() < nFilterMaxFret) {
            vRet.push_back(vNotes[i]);
        }
    }
    return vRet;
}

// ----------------------------------------------------------------------
