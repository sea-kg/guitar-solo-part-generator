#include "unit_test_position_note_guitar.h"
#include <vector>
#include <wsjcpp_core.h>
#include <solo_part_guitar.h>

REGISTRY_UNIT_TEST(UnitTestPositionNoteGuitar)

UnitTestPositionNoteGuitar::UnitTestPositionNoteGuitar()
    : WSJCppUnitTestBase("UnitTestPositionNoteGuitar") {
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
        4
    );
    compareN(bTestSuccess, "String 3", (int)note.getGuitarString(), (int)GuitarNumberString::GUITAR_STRING_3);
    compareB(bTestSuccess, "Fret 4", note.getFret(), 4);
    compareB(bTestSuccess, "Finger index", (int)note.getFinger(), (int)GuitarTouchFinger::GUITAR_INDEX_FINGER);
    compareB(bTestSuccess, "Duration 4", note.getDuration(), 4);
    return bTestSuccess;
}

