
#include "guitar_solo_part_generate_filters.h"
#include <wsjcpp_core.h>
#include <sstream>
#include <algorithm>

// ----------------------------------------------------------------------
// GuitarSoloPartGenerateFilterBase

GuitarSoloPartGenerateFilterBase::GuitarSoloPartGenerateFilterBase(
    const std::string &sName,
    GuitarSoloPartGeneratorFilterDataType nDataType,
    const std::string &sCaption,
    const std::string &sDescription
) {
    TAG = "GuitarSoloPartGenerateFilterBase-" + sName;
    m_sName = sName;
    m_nDataType = nDataType;
    m_sCaption = sCaption;
    m_sDescription = sDescription;
}

// ----------------------------------------------------------------------

std::string GuitarSoloPartGenerateFilterBase::getName() {
    return m_sName;
}

// ----------------------------------------------------------------------

std::string GuitarSoloPartGenerateFilterBase::getDataTypeAsString() {
    if (m_nDataType == ::GSPG_FILTER_DATATYPE_SELECT_LIST) {
        return "select_list";
    } else if (m_nDataType == ::GSPG_FILTER_DATATYPE_CHECKBOX_LIST) {
        return "checkbox_list";
    } else {
        WSJCppLog::throw_err(TAG, "Unknown datatype");
    }
    return "";
}

// ----------------------------------------------------------------------

GuitarSoloPartGeneratorFilterDataType GuitarSoloPartGenerateFilterBase::getDataType() {
    return m_nDataType;
}

// ----------------------------------------------------------------------

std::string GuitarSoloPartGenerateFilterBase::getCaption() {
    return m_sCaption;
}

// ----------------------------------------------------------------------

std::string GuitarSoloPartGenerateFilterBase::getDescription() {
    return m_sDescription;
}

// ----------------------------------------------------------------------
// GuitarSoloPartGenerateFilterMinFret

GuitarSoloPartGenerateFilterMinFret::GuitarSoloPartGenerateFilterMinFret()
: GuitarSoloPartGenerateFilterBase("min_fret", ::GSPG_FILTER_DATATYPE_SELECT_LIST, "Min Fret", "Filter by minimal fret") {
    WSJCppLog::info(TAG, "min_fret");
    m_nFromFret = 0;
    m_nToFret = 24;
}

// ----------------------------------------------------------------------

nlohmann::json GuitarSoloPartGenerateFilterMinFret::getJsonValues() {
    nlohmann::json jsonValues = nlohmann::json::array();
    for (int i = 0; i <= 24; i++) {
        nlohmann::json jsonValue;
        jsonValue["value"] = i;
        jsonValue["caption"] = "Fret #" + std::to_string(i);
        jsonValues.push_back(jsonValue);
    }
    return jsonValues;
}

// ----------------------------------------------------------------------

std::vector<PositionNoteGuitar> GuitarSoloPartGenerateFilterMinFret::applyFilter(
    const std::vector<PositionNoteGuitar> &vNotes, 
    const std::string &sValue
) {
    int nFilterMinFret = atoi(sValue.c_str());
    std::vector<PositionNoteGuitar> vRet;
    for (int i = 0; i < vNotes.size(); i++) {
        if (vNotes[i].getFret() >= nFilterMinFret) {
            vRet.push_back(vNotes[i]);
        }
    }
    return vRet;
}

// ----------------------------------------------------------------------
// GuitarSoloPartGenerateFilterMaxFret

GuitarSoloPartGenerateFilterMaxFret::GuitarSoloPartGenerateFilterMaxFret()
: GuitarSoloPartGenerateFilterBase("max_fret", ::GSPG_FILTER_DATATYPE_SELECT_LIST, "Max Fret", "Filter by maximal fret") {
    WSJCppLog::info(TAG, "");
    m_nFromFret = 0;
    m_nToFret = 24;
}

// ----------------------------------------------------------------------

nlohmann::json GuitarSoloPartGenerateFilterMaxFret::getJsonValues() {
    nlohmann::json jsonValues = nlohmann::json::array();
    for (int i = 24; i >= 0; i--) {
        nlohmann::json jsonValue;
        jsonValue["value"] = i;
        jsonValue["caption"] = "Fret #" + std::to_string(i);
        jsonValues.push_back(jsonValue);
    }
    return jsonValues;
}

// ----------------------------------------------------------------------

std::vector<PositionNoteGuitar> GuitarSoloPartGenerateFilterMaxFret::applyFilter(
    const std::vector<PositionNoteGuitar> &vNotes, 
    const std::string &sValue
) {
    int nFilterMaxFret = atoi(sValue.c_str());
    std::vector<PositionNoteGuitar> vRet;
    for (int i = 0; i < vNotes.size(); i++) {
        if (vNotes[i].getFret() < nFilterMaxFret) {
            vRet.push_back(vNotes[i]);
        }
    }
    return vRet;
}

// ----------------------------------------------------------------------
// GuitarSoloPartGenerateFilterNotes

GuitarSoloPartGenerateFilterNotes::GuitarSoloPartGenerateFilterNotes()
: GuitarSoloPartGenerateFilterBase("use_notes", ::GSPG_FILTER_DATATYPE_CHECKBOX_LIST, "Use a notes", "Filter by notes") {
    m_vNotes = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    // init m_vAllNameOfNotes
    for (int o = 0; o < 9; o++) {
        for (int n = 0; n < 12; n++) {
            m_vAllNameOfNotes.push_back(m_vNotes[n] + std::to_string(o));
        }
    }

    m_vGuitarStartStringNotes.push_back("E4"); // 1 string
    m_vGuitarStartStringNotes.push_back("B3");
    m_vGuitarStartStringNotes.push_back("G3");
    m_vGuitarStartStringNotes.push_back("D3");
    m_vGuitarStartStringNotes.push_back("A2");
    m_vGuitarStartStringNotes.push_back("E2"); // 6 string
}

// ----------------------------------------------------------------------

nlohmann::json GuitarSoloPartGenerateFilterNotes::getJsonValues() {
    nlohmann::json jsonValues = nlohmann::json::array();
    for (int i = 0; i < m_vNotes.size(); i++) {
        nlohmann::json jsonValue;
        jsonValue["value"] = m_vNotes[i];
        jsonValue["checked"] = true;
        jsonValue["caption"] = "" + m_vNotes[i];
        jsonValues.push_back(jsonValue);
    }
    return jsonValues;
}

// ----------------------------------------------------------------------

std::vector<PositionNoteGuitar> GuitarSoloPartGenerateFilterNotes::applyFilter(
    const std::vector<PositionNoteGuitar> &vNotes, 
    const std::string &sValue
) {
    std::vector<std::string> vFilteredNotes;
    std::stringstream ss(sValue);
    std::string sToken;
    while (std::getline(ss, sToken, '|')) {
        vFilteredNotes.push_back(sToken);
    }

    // todo redesign
    std::vector<PositionNoteGuitar> vRet;
    for (int i = 0; i < vNotes.size(); i++) {
        std::string sNote = findNoteByPosition(vNotes[i]);
        sNote = sNote.substr(0, sNote.size() - 1);

        bool bAdd = false;
        for (int s = 0; s < vFilteredNotes.size(); s++) {
            if (vFilteredNotes[s] == sNote) {
                bAdd = true;
            }
        }
        if (bAdd) {
            vRet.push_back(vNotes[i]);
        }
    }
    return vRet;
}

// ---------------------------------------------------------------------

std::string GuitarSoloPartGenerateFilterNotes::findNoteByPosition(const PositionNoteGuitar &note) {
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

// ----------------------------------------------------------------------
// GuitarSoloPartGenerateFilterUseStrings

GuitarSoloPartGenerateFilterUseStrings::GuitarSoloPartGenerateFilterUseStrings()
: GuitarSoloPartGenerateFilterBase("use_strings", ::GSPG_FILTER_DATATYPE_CHECKBOX_LIST, "Use a strings", "Filter by strings") {
    m_vStrings = {"1", "2", "3", "4", "5", "6"};
}

// ----------------------------------------------------------------------

nlohmann::json GuitarSoloPartGenerateFilterUseStrings::getJsonValues() {
    nlohmann::json jsonValues = nlohmann::json::array();
    for (int i = 0; i < m_vStrings.size(); i++) {
        nlohmann::json jsonValue;
        jsonValue["value"] = m_vStrings[i];
        jsonValue["checked"] = true;
        jsonValue["caption"] = "#" + m_vStrings[i];
        jsonValues.push_back(jsonValue);
    }
    return jsonValues;
}

// ----------------------------------------------------------------------

std::vector<PositionNoteGuitar> GuitarSoloPartGenerateFilterUseStrings::applyFilter(
    const std::vector<PositionNoteGuitar> &vNotes, 
    const std::string &sValue
) {
    std::vector<int> vFilteredStrings;
    std::stringstream ss(sValue);
    std::string sToken;
    while (std::getline(ss, sToken, '|')) {
        int nString = atoi(sToken.c_str());
        vFilteredStrings.push_back(nString);
    }
    
    std::vector<PositionNoteGuitar> vRet;
    for (int i = 0; i < vNotes.size(); i++) {
        int nString = vNotes[i].getGuitarString();
        bool bAdd = false;
        for (int s = 0; s < vFilteredStrings.size(); s++) {
            if (vFilteredStrings[s] == nString) {
                bAdd = true;
            }
        }
        if (bAdd) {
            vRet.push_back(vNotes[i]);
        }
    }
    return vRet;
}

// ----------------------------------------------------------------------