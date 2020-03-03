#ifndef GUITAR_SOLO_PART_GENERATE_FILTERS_H
#define GUITAR_SOLO_PART_GENERATE_FILTERS_H

#include <string>
#include <vector>
#include <json.hpp>

#include "guitar_solo_part_generator_enums.h"
#include "position_note_guitar.h" // TODO rename guitar finger position

class GuitarSoloPartGenerateFilterBase {
    public:
        GuitarSoloPartGenerateFilterBase(
            const std::string &sName,
            GuitarSoloPartGeneratorFilterDataType nDataType,
            const std::string &sCaption,
            const std::string &sDescription
        );
        std::string getName();
        std::string getDataTypeAsString();
        GuitarSoloPartGeneratorFilterDataType getDataType();
        std::string getCaption();
        std::string getDescription();
        virtual nlohmann::json getJsonValues() = 0;
        virtual std::vector<PositionNoteGuitar> applyFilter(
            const std::vector<PositionNoteGuitar> &vNotes, 
            const std::string &sValue
        ) = 0;
    protected:
        std::string TAG;
    private:
        std::string m_sName;
        GuitarSoloPartGeneratorFilterDataType m_nDataType;
        std::string m_sCaption;
        std::string m_sDescription;
};

// ----------------------------------------------------------------------

class GuitarSoloPartGenerateFilterMinFret : public GuitarSoloPartGenerateFilterBase {
    public: 
        GuitarSoloPartGenerateFilterMinFret();
        virtual nlohmann::json getJsonValues();
        virtual std::vector<PositionNoteGuitar> applyFilter(
            const std::vector<PositionNoteGuitar> &vNotes, 
            const std::string &sValue
        );
    private:
        int m_nFromFret;
        int m_nToFret;
};

// ----------------------------------------------------------------------

class GuitarSoloPartGenerateFilterMaxFret : public GuitarSoloPartGenerateFilterBase {
    public: 
        GuitarSoloPartGenerateFilterMaxFret();
        virtual nlohmann::json getJsonValues();
        virtual std::vector<PositionNoteGuitar> applyFilter(
            const std::vector<PositionNoteGuitar> &vNotes, 
            const std::string &sValue
        );
    private:
        int m_nFromFret;
        int m_nToFret;
};

// ----------------------------------------------------------------------

class GuitarSoloPartGenerateFilterNotes : public GuitarSoloPartGenerateFilterBase {
    public: 
        GuitarSoloPartGenerateFilterNotes();
        virtual nlohmann::json getJsonValues();
        virtual std::vector<PositionNoteGuitar> applyFilter(
            const std::vector<PositionNoteGuitar> &vNotes, 
            const std::string &sValue
        );
    private:
        std::vector<std::string> m_vNotes;
};

// ----------------------------------------------------------------------

class GuitarSoloPartGenerateFilterUseStrings : public GuitarSoloPartGenerateFilterBase {
    public: 
        GuitarSoloPartGenerateFilterUseStrings();
        virtual nlohmann::json getJsonValues();
        virtual std::vector<PositionNoteGuitar> applyFilter(
            const std::vector<PositionNoteGuitar> &vNotes, 
            const std::string &sValue
        );
    private:
        std::vector<std::string> m_vStrings;
};

#endif // GUITAR_SOLO_PART_GENERATE_FILTERS_H
