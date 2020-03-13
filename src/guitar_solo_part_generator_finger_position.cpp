#include "guitar_solo_part_generator_finger_position.h"
#include <wsjcpp_core.h>
#include <regex>
#include <iostream>
#include <wsjcpp_core.h>
#include <wsjcpp_validators.h>

// ---------------------------------------------------------------------
// GuitarSoloPartGeneratorFingerPosition

GuitarSoloPartGeneratorFingerPosition::GuitarSoloPartGeneratorFingerPosition() {
    TAG = "GuitarSoloPartGeneratorFingerPosition";
    m_nGuitarString = 0;
    m_nFret = -1;
    m_nFinger = GuitarTouchFinger::GUITAR_NO_FINGER;
    checkAndThrow();
}

// ---------------------------------------------------------------------

GuitarSoloPartGeneratorFingerPosition::GuitarSoloPartGeneratorFingerPosition(
    int nGuitarString, 
    int nFret, 
    GuitarTouchFinger nFinger
) {
    TAG = "GuitarSoloPartGeneratorFingerPosition";
    m_nGuitarString = nGuitarString;
    m_nFret = nFret;
    m_nFinger = nFinger;
    checkAndThrow();
}

// ---------------------------------------------------------------------

int GuitarSoloPartGeneratorFingerPosition::getGuitarString() const {
    return m_nGuitarString;
}

// ---------------------------------------------------------------------

int GuitarSoloPartGeneratorFingerPosition::getFret() const {
    return m_nFret;
}

// ---------------------------------------------------------------------

GuitarTouchFinger GuitarSoloPartGeneratorFingerPosition::getFinger() {
    return m_nFinger;
}

// ---------------------------------------------------------------------

bool GuitarSoloPartGeneratorFingerPosition::operator==(const  GuitarSoloPartGeneratorFingerPosition &rhs) const {
    return 
        m_nGuitarString == rhs.m_nGuitarString 
        && m_nFret == rhs.m_nFret
        && m_nFinger == rhs.m_nFinger
    ;
}

// ---------------------------------------------------------------------

bool GuitarSoloPartGeneratorFingerPosition::operator!=(const GuitarSoloPartGeneratorFingerPosition &rhs) const {
    return 
        m_nGuitarString != rhs.m_nGuitarString 
        || m_nFret != rhs.m_nFret
        || m_nFinger != rhs.m_nFinger
    ;
}

// ---------------------------------------------------------------------

std::string GuitarSoloPartGeneratorFingerPosition::toPrintableString() {
    std::string sRet = 
        "{" + std::to_string(m_nGuitarString) + " string, "
        + std::to_string(m_nFret) + " fret, "
        + GuitarSoloPartGeneratorEnums::fingerToValue(m_nFinger) + " finger"
        "}"
    ;

    return sRet;
}

// ---------------------------------------------------------------------

void GuitarSoloPartGeneratorFingerPosition::checkAndThrow() {
    if (m_nGuitarString < 0 || m_nGuitarString > 8) {
        WSJCppLog::throw_err(TAG, "GuitarString must be in range 1..8");
    }
    if (m_nFret < -1 || m_nFret > 24) {
        WSJCppLog::throw_err(TAG, "Fret must be in range 0..24");
    }
    if (
        m_nFinger != GuitarTouchFinger::GUITAR_NO_FINGER 
        && m_nFinger != GuitarTouchFinger::GUITAR_INDEX_FINGER
        && m_nFinger != GuitarTouchFinger::GUITAR_MIDDLE_FINGER
        && m_nFinger != GuitarTouchFinger::GUITAR_RING_FINGER
        && m_nFinger != GuitarTouchFinger::GUITAR_LITTLE_FINGER
    ) {
        WSJCppLog::throw_err(TAG, "Finger must be in [GUITAR_NO_FINGER, GUITAR_INDEX_FINGER, GUITAR_MIDDLE_FINGER, GUITAR_RING_FINGER, GUITAR_LITTLE_FINGER]");
    }
    if (m_nFret == 0 && m_nFinger != GuitarTouchFinger::GUITAR_NO_FINGER) {
        WSJCppLog::throw_err(TAG, "if fret == 0 then finger must be GUITAR_NO_FINGER");
    }
    if (m_nFret == -1 && m_nFinger != GuitarTouchFinger::GUITAR_NO_FINGER) {
        WSJCppLog::throw_err(TAG, "if fret == -1 then finger must be not GUITAR_NO_FINGER");
    }
    if (m_nFret > 0 && m_nFinger == GuitarTouchFinger::GUITAR_NO_FINGER) {
        WSJCppLog::throw_err(TAG, "if fret != 0 then finger must be not GUITAR_NO_FINGER");
    }
    if (m_nGuitarString == 0 && m_nFinger != GuitarTouchFinger::GUITAR_NO_FINGER) {
        WSJCppLog::throw_err(TAG, "if GuitarString == 0 then finger must be not GUITAR_NO_FINGER");
    }
}