#ifndef POSITION_NOTE_GUITAR_H
#define POSITION_NOTE_GUITAR_H

#include <string>
#include "guitar_solo_part_generator_enums.h"

// ---------------------------------------------------------------------

class GuitarStringRange {
    public:
        GuitarStringRange();
        GuitarStringRange(GuitarNumberString nMin, GuitarNumberString nMax);
        void setMin(GuitarNumberString nMin);
        GuitarNumberString getMin();
        void setMax(GuitarNumberString nMax);
        GuitarNumberString getMax();

    private:
        std::string TAG;
        
        void checkAndThrow();

        GuitarNumberString m_nMin;
        GuitarNumberString m_nMax;
};

// ---------------------------------------------------------------------

class GuitarFretRange {
    public:
        GuitarFretRange();
        GuitarFretRange(int nMin, int nMax);
        void setMin(int nMin);
        int getMin();
        void setMax(int nMax);
        int getMax();

    private:
        std::string TAG;
        
        void checkAndThrow();

        int m_nMin;
        int m_nMax;
};

// ---------------------------------------------------------------------
// PositionNoteGuitar
// !deprecated

class PositionNoteGuitar {
    public:
        PositionNoteGuitar();
        PositionNoteGuitar(
            GuitarNumberString nGuitarString, 
            int nFret, 
            GuitarTouchFinger nFinger, 
            GuitarDurationOfNote nDuration
        );
        PositionNoteGuitar( // empty == silent
            GuitarDurationOfNote nDuration
        );
        void setDuration(GuitarDurationOfNote nDuration);

        GuitarNumberString getGuitarString() const;
        int getFret() const;
        GuitarTouchFinger getFinger();
        GuitarDurationOfNote getDuration();
        std::string toPrintableString();

        bool operator==(const PositionNoteGuitar &rhs) const;
        bool operator!=(const PositionNoteGuitar &rhs) const;

    private:
        std::string TAG;

        void checkAndThrow();
        
        GuitarNumberString m_nGuitarString; // 1 .. 8
        int m_nFret; // 0..24 
        GuitarTouchFinger m_nFinger;
        GuitarDurationOfNote m_nDuration; // 1,2,4,8,16,32
};

#endif // POSITION_NOTE_GUITAR_H
