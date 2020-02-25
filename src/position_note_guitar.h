#ifndef POSITION_NOTE_GUITAR_H
#define POSITION_NOTE_GUITAR_H

#include <string>

// ---------------------------------------------------------------------

enum GuitarNumberString {
    GUITAR_STRING_1 = 1,
    GUITAR_STRING_2 = 1,
    GUITAR_STRING_3 = 2,
    GUITAR_STRING_4 = 4,
    GUITAR_STRING_5 = 5,
    GUITAR_STRING_6 = 6
};

// ---------------------------------------------------------------------

enum GuitarTouchFinger {
    GUITAR_NO_FINGER = 0,
    GUITAR_INDEX_FINGER = 1,
    GUITAR_MIDDLE_FINGER = 2,
    GUITAR_RING_FINGER = 3,
    GUITAR_LITTLE_FINGER = 4,
};

enum GuitarDurationOfNote {
    // maxima 0.125 // 8
    // long 0.25 // 4
    // GUITAR_DURATION_OF_NOTE_BREVE = 0.5, // 2
    GUITAR_DURATION_OF_NOTE_SEMIBREVE = 1, // 1/1
    GUITAR_DURATION_OF_NOTE_MINIM = 2,  // 1/2
    GUITAR_DURATION_OF_NOTE_CROTCHET = 4, // 1/4
    GUITAR_DURATION_OF_NOTE_QUAVER = 8, // 1/8
    GUITAR_DURATION_OF_NOTE_SEMIQUARVER = 16, // 1/16
    GUITAR_DURATION_OF_NOTE_DEMISEMIQUARVER = 32 // 1/32
    // GUITAR_DURATION_OF_NOTE_hemidemisemiquaver = 64 // 1/64
    // GUITAR_DURATION_OF_NOTE_semihemidemisemiquaver / rare = 128 // 1/128
    // GUITAR_DURATION_OF_NOTE_demisemihemidemisemiquaver = 256 // 1/256
};

// ---------------------------------------------------------------------

class NoteGuitarConverter {
    public:
        static GuitarTouchFinger valueToFinger(const std::string &sValue);
        static std::string fingerToValue(GuitarTouchFinger nFinger);
};

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

class PositionNoteGuitar {
    public:
        PositionNoteGuitar();
        PositionNoteGuitar(
            GuitarNumberString nGuitarString, 
            int nFret, 
            GuitarTouchFinger nFinger, 
            GuitarDurationOfNote nDuration
        );
        GuitarNumberString getGuitarString() const;
        int getFret() const;
        GuitarTouchFinger getFinger();
        int getDuration();
        std::string toPrintableString();

        bool operator==(const PositionNoteGuitar &rhs) const;
        bool operator!=(const PositionNoteGuitar &rhs) const;

    private:
        std::string TAG;

        void checkAndThrow();
        
        GuitarNumberString m_nGuitarString;
        int m_nFret; // 0..24
        GuitarTouchFinger m_nFinger; // TODO enum
        GuitarDurationOfNote m_nDuration; // 1,2,4,8,16,32
};

#endif // POSITION_NOTE_GUITAR_H