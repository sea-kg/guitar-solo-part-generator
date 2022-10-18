#ifndef GUITAR_SOLO_PART_GENERATOR_ENUMS_H
#define GUITAR_SOLO_PART_GENERATOR_ENUMS_H

#include <string>

enum GuitarSoloPartGeneratorFilterDataType {
    GSPG_FILTER_DATATYPE_SELECT_LIST = 0,
    GSPG_FILTER_DATATYPE_CHECKBOX_LIST = 1
};

// ---------------------------------------------------------------------

enum GuitarNumberString {
    GUITAR_STRING_NO = 0,
    GUITAR_STRING_1 = 1,
    GUITAR_STRING_2 = 1,
    GUITAR_STRING_3 = 2,
    GUITAR_STRING_4 = 4,
    GUITAR_STRING_5 = 5,
    GUITAR_STRING_6 = 6,
    GUITAR_STRING_7 = 7,
    GUITAR_STRING_8 = 8
};

// ---------------------------------------------------------------------

enum GuitarTouchFinger {
    GUITAR_NO_FINGER = 0,
    GUITAR_INDEX_FINGER = 1,
    GUITAR_MIDDLE_FINGER = 2,
    GUITAR_RING_FINGER = 3,
    GUITAR_LITTLE_FINGER = 4,
};

// ---------------------------------------------------------------------

enum GuitarDurationOfNote {
    GUITAR_DURATION_OF_NOTE_8_1_MAXIMA = 256, // 8/1
    GUITAR_DURATION_OF_NOTE_4_1_LONG = 128, // 4/1
    GUITAR_DURATION_OF_NOTE_2_1_BREVE = 64, // 2/1
    GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE = 32, // 1/1
    GUITAR_DURATION_OF_NOTE_1_2_MINIM = 16,  // 1/2
    GUITAR_DURATION_OF_NOTE_1_4_CROTCHET = 8, // 1/4
    GUITAR_DURATION_OF_NOTE_1_8_QUAVER = 4, // 1/8
    GUITAR_DURATION_OF_NOTE_1_16_SEMIQUARVER = 2, // 1/16
    GUITAR_DURATION_OF_NOTE_1_32_DEMISEMIQUARVER = 1 // 1/32
    // GUITAR_DURATION_OF_NOTE_hemidemisemiquaver = 64 // 1/64
    // GUITAR_DURATION_OF_NOTE_semihemidemisemiquaver / rare = 128 // 1/128
    // GUITAR_DURATION_OF_NOTE_demisemihemidemisemiquaver = 256 // 1/256
};

// ---------------------------------------------------------------------

class GuitarSoloPartGeneratorEnums {
    public:
        static GuitarTouchFinger valueToFinger(const std::string &sValue);
        static std::string fingerToValue(GuitarTouchFinger nFinger);
        static std::string durationToStringValue(GuitarDurationOfNote nDuration);
};

#endif // GUITAR_SOLO_PART_GENERATOR_ENUMS_H