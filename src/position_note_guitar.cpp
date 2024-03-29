#include "position_note_guitar.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// GuitarStringRange

GuitarStringRange::GuitarStringRange() {
    TAG = "GuitarStringRange";
    m_nMin = GuitarNumberString::GUITAR_STRING_1;
    m_nMax = GuitarNumberString::GUITAR_STRING_6;
}

// ---------------------------------------------------------------------

GuitarStringRange::GuitarStringRange(GuitarNumberString nMin, GuitarNumberString nMax) {
    TAG = "GuitarStringRange";
    m_nMin = nMin;
    m_nMax = nMax;
    checkAndThrow();
}

// ---------------------------------------------------------------------

void GuitarStringRange::setMin(GuitarNumberString nMin) {
    m_nMin = nMin;
    checkAndThrow();
}

// ---------------------------------------------------------------------

GuitarNumberString GuitarStringRange::getMin() {
    return m_nMin;
}

// ---------------------------------------------------------------------

void GuitarStringRange::setMax(GuitarNumberString nMax) {
    m_nMax = nMax;
    checkAndThrow();
}

// ---------------------------------------------------------------------

GuitarNumberString GuitarStringRange::getMax() {
    return m_nMax;
}

// ---------------------------------------------------------------------

void GuitarStringRange::checkAndThrow() {
    if (m_nMin < 1 || m_nMin > 6) {
        WsjcppLog::throw_err(TAG, "Min must be in range [1..6]");
    }
    if (m_nMax < 1 || m_nMax > 6) {
        WsjcppLog::throw_err(TAG, "Min must be in range [1..6]");
    }
}

// ---------------------------------------------------------------------
// GuitarFretRange

GuitarFretRange::GuitarFretRange() {
    TAG = "GuitarFretRange";
    m_nMin = 0;
    m_nMax = 24;
    checkAndThrow();
}

// ---------------------------------------------------------------------

GuitarFretRange::GuitarFretRange(int nMin, int nMax) {
    TAG = "GuitarFretRange";
    m_nMin = nMin;
    m_nMax = nMax;
    checkAndThrow();
}

// ---------------------------------------------------------------------

void GuitarFretRange::setMin(int nMin) {
    m_nMin = nMin;
    checkAndThrow();
}

// ---------------------------------------------------------------------

int GuitarFretRange::getMin() {
    return m_nMin;
}

// ---------------------------------------------------------------------

void GuitarFretRange::setMax(int nMax) {
    m_nMax = nMax;
    checkAndThrow();
}

// ---------------------------------------------------------------------

int GuitarFretRange::getMax() {
    return m_nMax;
}

// ---------------------------------------------------------------------

void GuitarFretRange::checkAndThrow() {
    if (m_nMin < 0 || m_nMin > 24) {
        WsjcppLog::throw_err(TAG, "Min must be in range [0..24]");
    }
    if (m_nMax < 0 || m_nMax > 24) {
        WsjcppLog::throw_err(TAG, "Min must be in range [0..24]");
    }
}

// ---------------------------------------------------------------------
// PositionNoteGuitar

PositionNoteGuitar::PositionNoteGuitar() {
    TAG = "PositionNoteGuitar";
    m_nGuitarString = GuitarNumberString::GUITAR_STRING_6;
    m_nFret = 0;
    m_nFinger = GuitarTouchFinger::GUITAR_NO_FINGER;
    m_nDuration = GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_4_CROTCHET; // 1/4
    checkAndThrow();
}

PositionNoteGuitar::PositionNoteGuitar(
    GuitarNumberString nGuitarString, 
    int nFret, 
    GuitarTouchFinger nFinger, 
    GuitarDurationOfNote nDuration
) {
    TAG = "PositionNoteGuitar";
    m_nGuitarString = nGuitarString;
    m_nFret = nFret;
    m_nFinger = nFinger;
    m_nDuration = nDuration; // 1/1..1/32
    checkAndThrow();
}

PositionNoteGuitar::PositionNoteGuitar( // empty == silent
    GuitarDurationOfNote nDuration
) {
    m_nGuitarString = GuitarNumberString::GUITAR_STRING_NO;
    m_nFret = -1;
    m_nFinger = GuitarTouchFinger::GUITAR_NO_FINGER;
    m_nDuration = nDuration;
    checkAndThrow();
}

void PositionNoteGuitar::setDuration(GuitarDurationOfNote nDuration) {
    m_nDuration = nDuration;
}

GuitarNumberString PositionNoteGuitar::getGuitarString() const {
    return m_nGuitarString;
}

int PositionNoteGuitar::getFret() const {
    return m_nFret;
}

GuitarTouchFinger PositionNoteGuitar::getFinger() {
    return m_nFinger;
}

GuitarDurationOfNote PositionNoteGuitar::getDuration() {
    return m_nDuration;
}

bool PositionNoteGuitar::operator==(const  PositionNoteGuitar &rhs) const {
    return 
        m_nGuitarString == rhs.m_nGuitarString 
        && m_nFret == rhs.m_nFret
        && m_nFinger == rhs.m_nFinger
        && m_nDuration == rhs.m_nDuration
    ;
}

bool PositionNoteGuitar::operator!=(const PositionNoteGuitar &rhs) const {
    return 
        m_nGuitarString != rhs.m_nGuitarString 
        || m_nFret != rhs.m_nFret
        || m_nFinger != rhs.m_nFinger
        || m_nDuration != rhs.m_nDuration
    ;
}

std::string PositionNoteGuitar::toPrintableString() {
    std::string sRet = 
        "{" + std::to_string(m_nGuitarString) + " string, "
        + std::to_string(m_nFret) + " fret, "
        + GuitarSoloPartGeneratorEnums::fingerToValue(m_nFinger) + " finger"
        "}"
    ;

    return sRet;
}

void PositionNoteGuitar::checkAndThrow() {
    if (m_nGuitarString < 0 || m_nGuitarString > 8) {
        WsjcppLog::throw_err(TAG, "GuitarString must be in range 1..8");
    }
    if (m_nFret < -1 || m_nFret > 24) {
        WsjcppLog::throw_err(TAG, "Fret must be in range -1..24");
    }
    if (
        m_nFinger != GuitarTouchFinger::GUITAR_NO_FINGER 
        && m_nFinger != GuitarTouchFinger::GUITAR_INDEX_FINGER
        && m_nFinger != GuitarTouchFinger::GUITAR_MIDDLE_FINGER
        && m_nFinger != GuitarTouchFinger::GUITAR_RING_FINGER
        && m_nFinger != GuitarTouchFinger::GUITAR_LITTLE_FINGER
    ) {
        WsjcppLog::throw_err(TAG, "Finger must be in [GUITAR_NO_FINGER, GUITAR_INDEX_FINGER, GUITAR_MIDDLE_FINGER, GUITAR_RING_FINGER, GUITAR_LITTLE_FINGER]");
    }
    if (m_nDuration < 0 || m_nDuration > 32) {
        WsjcppLog::throw_err(TAG, "Duration must be in range 1..32");
    }
    if (m_nFret == 0 && m_nFinger != GuitarTouchFinger::GUITAR_NO_FINGER) {
        WsjcppLog::throw_err(TAG, "if fret == 0 then finger must be GUITAR_NO_FINGER");
    }
    if (m_nFret == -1 && m_nFinger != GuitarTouchFinger::GUITAR_NO_FINGER) {
        WsjcppLog::throw_err(TAG, "if fret == -1 then finger must be not GUITAR_NO_FINGER");
    }
    if (m_nFret > 0 && m_nFinger == GuitarTouchFinger::GUITAR_NO_FINGER) {
        WsjcppLog::throw_err(TAG, "if fret != 0 then finger must be not GUITAR_NO_FINGER");
    }
    if (m_nGuitarString == 0 && m_nFinger != GuitarTouchFinger::GUITAR_NO_FINGER) {
        WsjcppLog::throw_err(TAG, "if GuitarString == 0 then finger must be not GUITAR_NO_FINGER");
    }
}