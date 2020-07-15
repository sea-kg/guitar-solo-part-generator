#include <string.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <wsjcpp_core.h>
#include <wsjcpp_light_web_server.h>
#include "solo_part_guitar.h"
#include <wsjcpp_light_web_http_handler_web_folder.h>

#include "http_handler_available_filters.h"
#include "http_handler_solo_generate.h"
#include "http_handler_movement_rules.h"

int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_APP_NAME);
    std::string appVersion = std::string(WSJCPP_APP_VERSION);
    WsjcppCore::initRandom();
    if (!WsjcppCore::dirExists(".logs")) {
        WsjcppCore::makeDir(".logs");
    }
    WsjcppLog::setPrefixLogFile("gspg");
    WsjcppLog::setLogDirectory(".logs");

    GuitarSoloPartGeneratorMovementRules *pMovementRules = new GuitarSoloPartGeneratorMovementRules();
    pMovementRules->applyPredefinedRules();

    std::cout << "Found rules " << pMovementRules->getSize() << std::endl;

    std::vector<GuitarSoloPartGenerateFilterBase *> vFilters;
    vFilters.push_back(new GuitarSoloPartGenerateFilterMinFret());
    vFilters.push_back(new GuitarSoloPartGenerateFilterMaxFret());
    vFilters.push_back(new GuitarSoloPartGenerateFilterNotes());
    vFilters.push_back(new GuitarSoloPartGenerateFilterUseStrings());

    if (argc == 2 && std::string(argv[1]) == "start-server") {

        // TODO refactor to like Employ settings
        int nPort = 1234;
        std::string sEnvPort;
        if (WsjcppCore::getEnv("GSPG_PORT", sEnvPort)) {
            WsjcppLog::info(TAG, "Your GSPG_PORT is: '" + sEnvPort + "'");
            nPort = std::stoi(sEnvPort);
        } else {
            WsjcppLog::info(TAG, "GSPG_PORT will be used by default:  '" + std::to_string(nPort) + "'");
        }

        // TODO validate port
        std::string sWebFolder = "./web";
        if (WsjcppCore::getEnv("GSPG_WEB", sWebFolder)) {
            WsjcppLog::info(TAG, "Your GSPG_WEB is: '" + sWebFolder + "'");
        } else {
            WsjcppLog::info(TAG, "GSPG_WEB will be used by default: '" + sWebFolder + "'");
        }

        WsjcppLightWebServer server;
        server.setPort(nPort);
        server.setMaxWorkers(2);
        server.addHandler(new HttpHandlerAvailableFilters(vFilters));
        server.addHandler(new HttpHandlerMovementRules(pMovementRules));
        server.addHandler(new HttpHandlerSoloGenerate(pMovementRules, vFilters));
        server.addHandler(new WsjcppLightWebHttpHandlerWebFolder("/", "./web"));
        server.startSync();
        return -1;
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
        ::GUITAR_DURATION_OF_NOTE_1_4_CROTCHET
    );

    WsjcppLog::info(TAG, "start note: " + note.toPrintableString());
    SoloPartGuitar part;
    part.addNote(note);
    for (int i = 0; i < 12; i++) {
        std::vector<PositionNoteGuitar> vNotes = pMovementRules->findWithBegin(note);
        std::cout << "Found possible note " << vNotes.size() << std::endl;
        if (vNotes.size() == 0) { // TODO come to no finger
            WsjcppLog::err(TAG, "Not found");
            return -1;
        }
        note = vNotes[std::rand() % vNotes.size()];
        part.addNote(note);
        WsjcppLog::info(TAG, "note[" + std::to_string(i) + "] = " + note.toPrintableString());
    }
    std::cout << part.exportTabulatur() << std::endl;

    nlohmann::json j = part.exportToJson();
    std::cout << j.dump(4) << std::endl;
    return 0;
}

