#include <wsjcpp_core.h>
#include "http_handler_solo_generate.h"
#include "solo_part_guitar.h"

// ----------------------------------------------------------------------

HttpHandlerSoloGenerate::HttpHandlerSoloGenerate(
    GuitarSoloPartGeneratorMovementRules *pMovementRules, 
    const std::vector<GuitarSoloPartGenerateFilterBase *> &vFilters
)
: WsjcppLightWebHttpHandlerBase("solo-generate") {
    TAG = "HttpHandlerSoloGenerate";
    m_pMovementRules = pMovementRules;
    for (int i = 0; i < vFilters.size(); i++) {
        m_vFilters.push_back(vFilters[i]);
    }
}

// ----------------------------------------------------------------------

bool HttpHandlerSoloGenerate::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();

    if (sRequestPath == "/api/v1/solo-generate/" || sRequestPath == "/api/v1/solo-generate") {
        return true;    
    }
    return false;
}

// ----------------------------------------------------------------------

bool HttpHandlerSoloGenerate::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    // WsjcppLog::warn(_tag, sRequestPath);
    
    WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
    if (sRequestPath != "/api/v1/solo-generate/" && sRequestPath != "/api/v1/solo-generate") {
        return false;
    }

    int nFilterMinFret = 0;
    int nFilterMaxFret = 24;
    std::vector<WsjcppLightWebHttpRequestQueryValue> vQueries = pRequest->getRequestQueryParams();

    int nFirstString = (std::rand() % 6) + 1;
    int nFirstFinger = (std::rand() % 5);
    PositionNoteGuitar note(
        (GuitarNumberString)nFirstString, 
        0, 
        ::GUITAR_NO_FINGER, 
        ::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE
    );

    WsjcppLog::info(TAG, "start note: " + note.toPrintableString());
    
    std::vector<GuitarDurationOfNote> vPossibleDurations;
    vPossibleDurations.push_back(GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE);
    vPossibleDurations.push_back(GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_2_MINIM);
    vPossibleDurations.push_back(GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_4_CROTCHET);
    vPossibleDurations.push_back(GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_4_CROTCHET);
    vPossibleDurations.push_back(GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_4_CROTCHET);
    vPossibleDurations.push_back(GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_4_CROTCHET);
    vPossibleDurations.push_back(GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_8_QUAVER);
    // vPossibleDurations.push_back(GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_16_SEMIQUARVER);
    // vPossibleDurations.push_back(GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_32_DEMISEMIQUARVER);

    std::vector<GuitarDurationOfNote> vRhythmTable;

    int nMaxDurationTact = GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE;
    for (int i = 0; i < 4; i++) { // 4 tacks
        int nCurrentDuration = 0;
        while (nCurrentDuration < nMaxDurationTact) {
            GuitarDurationOfNote nDur = vPossibleDurations[std::rand() % vPossibleDurations.size()];
            if ((nDur + nCurrentDuration) <= nMaxDurationTact) {
                vRhythmTable.push_back(nDur);
                WsjcppLog::info(TAG, "Duration: " + GuitarSoloPartGeneratorEnums::durationToStringValue(nDur));
                nCurrentDuration += nDur;
            }
        }
    }
    WsjcppLog::info(TAG, "vRhythmTable.size(): " + std::to_string(vRhythmTable.size()));

    SoloPartGuitar part;
    for (int i = 0; i < vRhythmTable.size(); i++) {
        std::vector<PositionNoteGuitar> vNotes = m_pMovementRules->findWithBegin(note);

        for (int f = 0; f < m_vFilters.size(); f++) {
            GuitarSoloPartGenerateFilterBase *pFilter = m_vFilters[f];
            std::string sFilterName = pFilter->getName();
            for (int q = 0; q < vQueries.size(); q++) {
                WsjcppLightWebHttpRequestQueryValue query = vQueries[q];
                if (query.getName() == sFilterName) {
                    std::string sValue = query.getValue();
                    vNotes = pFilter->applyFilter(vNotes, sValue);
                }
            }
        }

        std::cout << "Found possible note " << vNotes.size() << std::endl;
        if (vNotes.size() == 0) {
            // silent
            note = PositionNoteGuitar(::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE);
            part.addNote(note);
            // resp.badRequest().noCache().sendText("Could not generate with this params");
        } else {
            note = vNotes[std::rand() % vNotes.size()];
            note.setDuration(vRhythmTable[i]);
            // note
            part.addNote(note);
            WsjcppLog::info(TAG, "note[" + std::to_string(i) + "] = " + note.toPrintableString());
        }
    }
    nlohmann::json jsonResponse;
    jsonResponse["part"] = part.exportToJson();
    jsonResponse["guitarTuning"] = part.getGuitarTuning();
    
    resp.ok().noCache().sendJson(jsonResponse);

    return true;
}

// ----------------------------------------------------------------------
