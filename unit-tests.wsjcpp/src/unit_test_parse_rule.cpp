#include "unit_test_parse_rule.h"
#include <vector>
#include <wsjcpp_core.h>
#include "solo_part_guitar_rules.h"

REGISTRY_UNIT_TEST(UnitTestParseRule)

UnitTestParseRule::UnitTestParseRule()
    : WSJCppUnitTestBase("UnitTestParseRule") {
}

// ---------------------------------------------------------------------

void UnitTestParseRule::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestParseRule::run() {
    bool bTestSuccess = true;
    struct LTest {
        LTest(const std::string &sRule, int nExpectedSize) : sRule(sRule), nExpectedSize(nExpectedSize) {};
        std::string sRule;
        int nExpectedSize;
    };
    std::vector<LTest> tests;
    tests.push_back(LTest("{1 string, 0 fret, no finger} move to {1 string, 2 fret, index finger}", 2));
    tests.push_back(LTest("{X string, 0 fret, no finger} move to {X string, 2 fret, index finger}", 12));
    tests.push_back(LTest("{X string, 1 fret, index finger} move to {X+1 string, 2 fret, middle finger}",10));
    tests.push_back(LTest("{1 string, Y fret, index finger} move to {1 string, Y+1 fret, middle finger}", 46));
    tests.push_back(LTest("{X string, Y fret, middle finger} move to {X string, Y+1 fret, ring finger}", 276));
    tests.push_back(LTest("{X string, Y fret, index finger} move to {X+1 string, Y+1 fret, middle finger}", 230));
    tests.push_back(LTest("{X string, Y fret, ring finger} move to {X string, Y+1 fret, little finger}", 276));
    tests.push_back(LTest("{X string, X fret, no finger} move to {X string, X fret, index finger}", 288));
    tests.push_back(LTest("{X string, Y fret, index finger} move to {X string, Y fret, index finger}", 144));
    tests.push_back(LTest("{X string, Y fret, middle finger} move to {X string, Y fret, middle finger}", 144));
    tests.push_back(LTest("{X string, 1 fret, index finger} move to {X-1 string, 2 fret, middle finger}",10));
    tests.push_back(LTest("{X string, Y fret, ring finger} move to {X string, Y-1 fret, little finger}", 276));

    for (int i = 0; i < tests.size(); i++) {
        LTest test = tests[i];
        std::string sError = "";
        WSJCppLog::info(TAG, "Test rule: " + test.sRule + " -> " + std::to_string(test.nExpectedSize));
        SoloPartGuitarRules rules;
        bool bResult = rules.apply(test.sRule, sError);
        compareB(bTestSuccess, test.sRule + ", error: " + sError, bResult, true);
        compareN(bTestSuccess, test.sRule + ", rules size", rules.getSize(), test.nExpectedSize);
    }

    return bTestSuccess;
}

