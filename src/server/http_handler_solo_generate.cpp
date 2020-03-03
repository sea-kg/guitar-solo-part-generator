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
    int nFirstFinger = (std::rand() % 5);
    PositionNoteGuitar note(
        (GuitarNumberString)nFirstString, 
        0, 
        ::GUITAR_NO_FINGER, 
        ::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE
    );

    WSJCppLog::info(TAG, "start note: " + note.toPrintableString());
    SoloPartGuitar part;
    for (int i = 0; i < 16; i++) {
        std::vector<PositionNoteGuitar> vNotes = m_rules.findWithBegin(note);

        for (int f = 0; f < m_vFilters.size(); f++) {
            GuitarSoloPartGenerateFilterBase *pFilter = m_vFilters[f];
            std::string sFilterName = pFilter->getName();
            for (int q = 0; q < vQueries.size(); q++) {
                WSJCppLightWebHttpRequestQueryValue query = vQueries[q];
                if (query.getName() == sFilterName) {
                    std::string sValue = decodeURIElement(query.getValue());
                    vNotes = pFilter->applyFilter(vNotes, sValue);
                }
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

void HttpHandlerSoloGenerate::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

// ----------------------------------------------------------------------

std::string HttpHandlerSoloGenerate::decodeURIElement(const std::string& sValue) {
    // TODO replace to full solution
    std::string sRet = sValue;
    replaceAll(sRet, "%7C", "|");
    replaceAll(sRet, "%23", "#");
    return sRet;
}

// ----------------------------------------------------------------------
