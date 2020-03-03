#include "solo_part_guitar.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// SoloPartGuitar

SoloPartGuitar::SoloPartGuitar() {
    TAG = "SoloPartGuitar";

    // init m_vAllNameOfNotes
    std::string arrNotesInOctave[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    for (int o = 0; o < 9; o++) {
        for (int n = 0; n < 12; n++) {
            m_vAllNameOfNotes.push_back(arrNotesInOctave[n] + std::to_string(o));
        }
    }

    m_vGuitarStartStringNotes.push_back("E4"); // 1 string
    m_vGuitarStartStringNotes.push_back("B3");
    m_vGuitarStartStringNotes.push_back("G3");
    m_vGuitarStartStringNotes.push_back("D3");
    m_vGuitarStartStringNotes.push_back("A2");
    m_vGuitarStartStringNotes.push_back("E2"); // 6 string
    
}

// ---------------------------------------------------------------------

void SoloPartGuitar::addNote(PositionNoteGuitar note) {
    m_vNotes.push_back(note);
}

// ---------------------------------------------------------------------

std::vector<PositionNoteGuitar> SoloPartGuitar::getListOfNotes() {
    return m_vNotes;
}

// ---------------------------------------------------------------------

std::string SoloPartGuitar::exportTabulatur() {
    std::string sRet = "";
    std::string stringNames[] = {"", "E4", "B3", "G3", "D3", "A2", "E2"};

    for (int iString = 1; iString <= 6; iString++) {
        sRet += std::to_string(iString) + "|" + stringNames[iString] + "|-";
        for (int x = 0; x < m_vNotes.size(); x++) {
            PositionNoteGuitar note = m_vNotes[x];
            if (note.getGuitarString() == iString) {
                int nFret = note.getFret();
                std::string sPrefix = ((nFret < 10) ? "--" : "-");
                sRet += sPrefix + std::to_string(nFret);
            } else {
                sRet += "---";
            }
        }
        sRet += "\n";
    }
    sRet += "HAND|-";
    for (int x = 0; x < m_vNotes.size(); x++) {
        PositionNoteGuitar note = m_vNotes[x];
        GuitarTouchFinger nFinger = note.getFinger();
        if (nFinger == GuitarTouchFinger::GUITAR_NO_FINGER) {
            sRet += "--N";
        } else if (nFinger == GuitarTouchFinger::GUITAR_INDEX_FINGER) {
            sRet += "--I";
        } else if (nFinger == GuitarTouchFinger::GUITAR_MIDDLE_FINGER) {
            sRet += "--M";
        } else if (nFinger == GuitarTouchFinger::GUITAR_RING_FINGER) {
            sRet += "--R";
        } else if (nFinger == GuitarTouchFinger::GUITAR_LITTLE_FINGER) {
            sRet += "--L";
        }        
    }
    sRet += "\n";
    sRet += "N - No finger / empty string\n";
    sRet += "I - Index finger\n";
    sRet += "M - Middle finger\n";
    sRet += "R - Ring finger\n";
    sRet += "L - Little finger\n";
    return sRet;
}

// ---------------------------------------------------------------------

nlohmann::json SoloPartGuitar::exportToJson() {
    nlohmann::json json = nlohmann::json::array();
    for (int i = 0; i < m_vNotes.size(); i++) {
        PositionNoteGuitar note = m_vNotes[i];
        nlohmann::json jsonNote;
        jsonNote["string"] = note.getGuitarString();
        jsonNote["fret"] = note.getFret();
        jsonNote["finger"] = GuitarSoloPartGeneratorEnums::fingerToValue(note.getFinger());
        jsonNote["duration"] = note.getDuration();
        jsonNote["note"] = findNoteByPosition(note);
        json.push_back(jsonNote);
    }
    return json;
}

// ---------------------------------------------------------------------

std::string SoloPartGuitar::findNoteByPosition(const PositionNoteGuitar &note) {
    int nString = note.getGuitarString();
    std::string sPos0 = m_vGuitarStartStringNotes[nString-1];
    int nPos0 = -1;
    for (int i = 0; i < m_vAllNameOfNotes.size(); i++) {
        if (m_vAllNameOfNotes[i] == sPos0) {
            nPos0 = i;
            break;
        }
    }
    return m_vAllNameOfNotes[nPos0 + note.getFret()];
}

// ---------------------------------------------------------------------
