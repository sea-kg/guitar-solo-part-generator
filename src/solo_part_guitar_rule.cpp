#include "solo_part_guitar_rule.h"
#include <regex>

// ---------------------------------------------------------------------
// SoloPartGuitarRule

SoloPartGuitarRule::SoloPartGuitarRule(const PositionNoteGuitar &noteBegin, const PositionNoteGuitar &noteEnd) {
    TAG = "SoloPartGuitarRule";
    m_noteBegin = noteBegin;
    m_noteEnd = noteEnd;
}

// ---------------------------------------------------------------------

PositionNoteGuitar SoloPartGuitarRule::getNoteBegin() const {
    return m_noteBegin;
}

// ---------------------------------------------------------------------

PositionNoteGuitar SoloPartGuitarRule::getNoteEnd() const {
    return m_noteEnd;
}

// ---------------------------------------------------------------------

bool SoloPartGuitarRule::operator==(const SoloPartGuitarRule &rhs) const {
    return m_noteBegin == rhs.m_noteBegin && m_noteEnd == rhs.m_noteEnd;
}