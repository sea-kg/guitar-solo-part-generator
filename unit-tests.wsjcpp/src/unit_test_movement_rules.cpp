#include "unit_test_movement_rules.h"
#include <vector>
#include <wsjcpp_core.h>

REGISTRY_UNIT_TEST(UnitTestMovementRules)

UnitTestMovementRules::UnitTestMovementRules()
    : WSJCppUnitTestBase("UnitTestMovementRules") {
}

// ---------------------------------------------------------------------

void UnitTestMovementRules::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestMovementRules::run() {
    bool bTestSuccess = true;
    compareB(bTestSuccess, "Not implemented", true, false);
    return bTestSuccess;
}

