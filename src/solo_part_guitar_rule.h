#ifndef SOLO_PART_GUITAR_RULE_H
#define SOLO_PART_GUITAR_RULE_H

#include <string>
#include <vector>
#include "position_note_guitar.h"
#include "solo_part_guitar_rule.h"

// ---------------------------------------------------------------------

class SoloPartGuitarRule {
    public:
        SoloPartGuitarRule(const PositionNoteGuitar &noteBegin, const PositionNoteGuitar &noteEnd);
        PositionNoteGuitar getNoteBegin() const;
        PositionNoteGuitar getNoteEnd() const;

        bool operator == (const SoloPartGuitarRule &rhs) const;

    private:
        std::string TAG;
        PositionNoteGuitar m_noteBegin;
        PositionNoteGuitar m_noteEnd;
};

#endif // SOLO_PART_GUITAR_RULE_H