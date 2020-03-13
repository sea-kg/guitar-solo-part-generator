#ifndef GUITAR_SOLO_PART_GENERATOR_FINGER_POSITION_H
#define GUITAR_SOLO_PART_GENERATOR_FINGER_POSITION_H

#include <string>
#include <vector>
#include "guitar_solo_part_generator_enums.h"
#include "position_note_guitar.h"

class GuitarSoloPartGeneratorFingerPosition {
    public:
        GuitarSoloPartGeneratorFingerPosition();
        GuitarSoloPartGeneratorFingerPosition(
            int nGuitarString, 
            int nFret, 
            GuitarTouchFinger nFinger
        );
        int getGuitarString() const;
        int getFret() const;
        GuitarTouchFinger getFinger();
        std::string toPrintableString();

        bool operator==(const GuitarSoloPartGeneratorFingerPosition &rhs) const;
        bool operator!=(const GuitarSoloPartGeneratorFingerPosition &rhs) const;

    private:
        std::string TAG;

        void checkAndThrow();
        int m_nGuitarString; // > 0;
        int m_nFret; // > 0..24
        GuitarTouchFinger m_nFinger;
};

#endif // GUITAR_SOLO_PART_GENERATOR_FINGER_POSITION_H