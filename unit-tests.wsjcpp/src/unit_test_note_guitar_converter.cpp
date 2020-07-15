#include "unit_test_note_guitar_converter.h"
#include <vector>
#include <wsjcpp_core.h>
#include "position_note_guitar.h"
#include "guitar_solo_part_generator_enums.h"

REGISTRY_WSJCPP_UNIT_TEST(UnitTestNoteGuitarConverter)

UnitTestNoteGuitarConverter::UnitTestNoteGuitarConverter()
    : WsjcppUnitTestBase("UnitTestNoteGuitarConverter") {
}

// ---------------------------------------------------------------------

void UnitTestNoteGuitarConverter::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestNoteGuitarConverter::run() {
    bool bTestSuccess = true;

    struct LTest {
        LTest(const std::string &sValue, GuitarTouchFinger nFinger) {
            this->sValue = sValue;
            this->nFinger = nFinger;
        };
        std::string sValue;
        GuitarTouchFinger nFinger;
    };

    std::vector<LTest> tests;
    tests.push_back(LTest("no", GuitarTouchFinger::GUITAR_NO_FINGER));
    tests.push_back(LTest("index", GuitarTouchFinger::GUITAR_INDEX_FINGER));
    tests.push_back(LTest("middle", GuitarTouchFinger::GUITAR_MIDDLE_FINGER));
    tests.push_back(LTest("ring", GuitarTouchFinger::GUITAR_RING_FINGER));
    tests.push_back(LTest("little", GuitarTouchFinger::GUITAR_LITTLE_FINGER));

    for (int i = 0; i < tests.size(); i++) {
        LTest test = tests[i];
        std::string sExpectedValue = test.sValue;
        GuitarTouchFinger nExpectedFinger = test.nFinger;
        std::string sGotValue = GuitarSoloPartGeneratorEnums::fingerToValue(nExpectedFinger);
        GuitarTouchFinger nGotFinger = GuitarSoloPartGeneratorEnums::valueToFinger(sExpectedValue);
        compareS(
            bTestSuccess, 
            "Convert '" + std::to_string(nExpectedFinger) + "' -> '" + sExpectedValue + "'", 
            sGotValue, 
            sExpectedValue
        );

        compareN(
            bTestSuccess, 
            "Convert '" + sExpectedValue + "' -> '" + std::to_string(nExpectedFinger) + "'", 
            nGotFinger, 
            nExpectedFinger
        );
    }

    return bTestSuccess;
}

