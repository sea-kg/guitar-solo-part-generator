#include "unit_test_movement_rules.h"
#include <vector>
#include <wsjcpp_core.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestMovementRules)

UnitTestMovementRules::UnitTestMovementRules()
    : WsjcppUnitTestBase("UnitTestMovementRules") {
}

// ---------------------------------------------------------------------

void UnitTestMovementRules::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestMovementRules::run() {
    bool bTestSuccess = true;
    // compareB(bTestSuccess, "Not implemented", true, false);
    return bTestSuccess;
}

