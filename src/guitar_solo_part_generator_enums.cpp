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
    WsjcppLog::warn(TAG, "Unknown type of finger '" + sValue + "' will be return default GuitarTouchFinger::GUITAR_NO_FINGER");
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
    WsjcppLog::warn(TAG, "Unknown type of finger '" + std::to_string((int)nFinger) + "' will be return 'no'");
    return "no";
}

std::string GuitarSoloPartGeneratorEnums::durationToStringValue(GuitarDurationOfNote nDuration) {
    const std::string TAG = "GuitarSoloPartGeneratorEnums::durationToStringValue";
    if (nDuration == GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_8_1_MAXIMA) {
        return "8/1";
    } else if (nDuration == GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_4_1_LONG) {
        return "4/1";
    } else if (nDuration == GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_2_1_BREVE) {
        return "2/1";
    } else if (nDuration == GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE) {
        return "1/1";
    } else if (nDuration == GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_2_MINIM) {
        return "1/2";
    } else if (nDuration == GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_4_CROTCHET) {
        return "1/4";
    } else if (nDuration == GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_8_QUAVER) {
        return "1/8";
    } else if (nDuration == GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_16_SEMIQUARVER) {
        return "1/16";
    } else if (nDuration == GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_32_DEMISEMIQUARVER) {
        return "1/32";
    }
    WsjcppLog::warn(TAG, "Unknown type of duration '" + std::to_string((int)nDuration) + "' will be return ''");
    return "";
}
