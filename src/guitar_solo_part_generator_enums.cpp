#include "guitar_solo_part_generator_enums.h"
#include <wsjcpp_core.h>



// ---------------------------------------------------------------------
// NoteGuitarConverter

GuitarTouchFinger GuitarSoloPartGeneratorEnums::valueToFinger(const std::string &sValue) {
    const std::string TAG = "GuitarSoloPartGeneratorEnums::valueToFinger";
    if (sValue == "no") {
        return GuitarTouchFinger::GUITAR_NO_FINGER;
    } else if (sValue == "index") {
        return GuitarTouchFinger::GUITAR_INDEX_FINGER;
    } else if (sValue == "middle") {
        return GuitarTouchFinger::GUITAR_MIDDLE_FINGER;
    } else if (sValue == "ring") {
        return GuitarTouchFinger::GUITAR_RING_FINGER;
    } else if (sValue == "little") {
        return GuitarTouchFinger::GUITAR_LITTLE_FINGER;
    }
    WSJCppLog::warn(TAG, "Unknown type of finger '" + sValue + "' will be return default GuitarTouchFinger::GUITAR_NO_FINGER");
    return GuitarTouchFinger::GUITAR_NO_FINGER;
}

// ---------------------------------------------------------------------

std::string GuitarSoloPartGeneratorEnums::fingerToValue(GuitarTouchFinger nFinger) {
    const std::string TAG = "GuitarSoloPartGeneratorEnums::fingerToValue";
    if (nFinger == GuitarTouchFinger::GUITAR_NO_FINGER) {
        return "no"; 
    } else if (nFinger == GuitarTouchFinger::GUITAR_INDEX_FINGER) {
        return "index";
    } else if (nFinger == GuitarTouchFinger::GUITAR_MIDDLE_FINGER) {
        return "middle";
    } else if (nFinger == GuitarTouchFinger::GUITAR_RING_FINGER) {
        return "ring";
    } else if (nFinger == GuitarTouchFinger::GUITAR_LITTLE_FINGER) {
        return "little";
    }
    WSJCppLog::warn(TAG, "Unknown type of finger '" + std::to_string((int)nFinger) + "' will be return 'no'");
    return "no";
}