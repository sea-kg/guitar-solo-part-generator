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
    std::string appName = std::string(WSJCPP_NAME);
    std::string appVersion = std::string(WSJCPP_VERSION);
    WSJCppCore::initRandom();
    if (!WSJCppCore::dirExists(".logs")) {
        WSJCppCore::makeDir(".logs");
    }
    WSJCppLog::setPrefixLogFile("gspg");
    WSJCppLog::setLogDirectory(".logs");

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
        if (WSJCppCore::getEnv("GSPG_PORT", sEnvPort)) {
            WSJCppLog::info(TAG, "Your GSPG_PORT is: '" + sEnvPort + "'");
            nPort = std::stoi(sEnvPort);
        } else {
            WSJCppLog::info(TAG, "GSPG_PORT will be used by default:  '" + std::to_string(nPort) + "'");
        }

        // TODO validate port
        std::string sWebFolder = "./web";
        if (WSJCppCore::getEnv("GSPG_WEB", sWebFolder)) {
            WSJCppLog::info(TAG, "Your GSPG_WEB is: '" + sWebFolder + "'");
        } else {
            WSJCppLog::info(TAG, "GSPG_WEB will be used by default: '" + sWebFolder + "'");
        }

        WSJCppLightWebServer server;
        server.setPort(nPort);
        server.setMaxWorkers(2);
        server.addHandler((WSJCppLightWebHttpHandlerBase *)new HttpHandlerAvailableFilters(vFilters));
        server.addHandler((WSJCppLightWebHttpHandlerBase *)new HttpHandlerMovementRules(pMovementRules));
        server.addHandler((WSJCppLightWebHttpHandlerBase *)new HttpHandlerSoloGenerate(pMovementRules, vFilters));
        server.addHandler((WSJCppLightWebHttpHandlerBase *)new WSJCppLightWebHttpHandlerWebFolder("/", "./web"));
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

    WSJCppLog::info(TAG, "start note: " + note.toPrintableString());
    SoloPartGuitar part;
    part.addNote(note);
    for (int i = 0; i < 12; i++) {
        std::vector<PositionNoteGuitar> vNotes = pMovementRules->findWithBegin(note);
        std::cout << "Found possible note " << vNotes.size() << std::endl;
        if (vNotes.size() == 0) { // TODO come to no finger
            WSJCppLog::err(TAG, "Not found");
            return -1;
        }
        note = vNotes[std::rand() % vNotes.size()];
        part.addNote(note);
        WSJCppLog::info(TAG, "note[" + std::to_string(i) + "] = " + note.toPrintableString());
    }
    std::cout << part.exportTabulatur() << std::endl;

    nlohmann::json j = part.exportToJson();
    std::cout << j.dump(4) << std::endl;
    return 0;
}

