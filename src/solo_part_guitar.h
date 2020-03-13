#ifndef SOLO_PART_GUITAR_H
#define SOLO_PART_GUITAR_H

#include <string>
#include <vector>
#include "position_note_guitar.h"
#include <json.hpp>

// ---------------------------------------------------------------------

class SoloPartGuitar {
    public:
        SoloPartGuitar();
        void addNote(PositionNoteGuitar note);
        std::vector<PositionNoteGuitar> getListOfNotes();
        std::string exportTabulatur();
        nlohmann::json exportToJson();

    private:
        std::string TAG;
        std::vector<PositionNoteGuitar> m_vNotes;
        std::vector<std::string> m_vAllNameOfNotes;
        std::vector<std::string> m_vGuitarStartStringNotes;
        std::string findNoteByPosition(const PositionNoteGuitar &note);
        // double getFrequencyOfNote(std::string sNoteName);
};

#endif // SOLO_PART_GUITAR_H