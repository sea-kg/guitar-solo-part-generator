#include "unit_test_position_note_guitar.h"
#include <vector>
#include <wsjcpp_core.h>
#include <solo_part_guitar.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestPositionNoteGuitar)

UnitTestPositionNoteGuitar::UnitTestPositionNoteGuitar()
    : WsjcppUnitTestBase("UnitTestPositionNoteGuitar") {
}

// ---------------------------------------------------------------------

void UnitTestPositionNoteGuitar::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestPositionNoteGuitar::run() {
    bool bTestSuccess = true;
    PositionNoteGuitar note(
        GuitarNumberString::GUITAR_STRING_3, 
        4, 
        GuitarTouchFinger::GUITAR_INDEX_FINGER,
        ::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE
    );
    compareN(bTestSuccess, "String 3", (int)note.getGuitarString(), (int)GuitarNumberString::GUITAR_STRING_3);
    compareB(bTestSuccess, "Fret 4", note.getFret(), 4);
    compareB(bTestSuccess, "Finger index", (int)note.getFinger(), (int)GuitarTouchFinger::GUITAR_INDEX_FINGER);
    return bTestSuccess;
}

