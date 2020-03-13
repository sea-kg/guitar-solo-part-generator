#ifndef GUITAR_SOLO_PART_GENERATOR_MOVEMENT_RULES_H
#define GUITAR_SOLO_PART_GENERATOR_MOVEMENT_RULES_H

#include <string>
#include <vector>
#include "guitar_solo_part_generator_finger_position.h"
#include "position_note_guitar.h"

// ---------------------------------------------------------------------

class GuitarSoloPartGeneratorMovementRule {
    public:
        GuitarSoloPartGeneratorMovementRule(
            const std::string &sRule,
            const GuitarSoloPartGeneratorFingerPosition &positionBegin, 
            const GuitarSoloPartGeneratorFingerPosition &positionEnd
        );
        GuitarSoloPartGeneratorFingerPosition getPositionBegin() const;
        GuitarSoloPartGeneratorFingerPosition getPositionEnd() const;

        bool operator == (const GuitarSoloPartGeneratorMovementRule &rhs) const;

    private:
        std::string TAG;
        std::string m_sRule;
        GuitarSoloPartGeneratorFingerPosition m_positionBegin;
        GuitarSoloPartGeneratorFingerPosition m_positionEnd;
};

// ---------------------------------------------------------------------

class GuitarSoloPartGeneratorMovementRules {
    public:
        GuitarSoloPartGeneratorMovementRules();
        bool apply(const std::string &sRule, std::string &sError);
        void add(const GuitarSoloPartGeneratorMovementRule &rule);
        int getSize();
        std::vector<PositionNoteGuitar> findWithBegin(PositionNoteGuitar note);
        std::vector<std::string> getPredefinedRules();
        void applyPredefinedRules();

    private:
        std::string TAG;

        std::string replaceSubstring(const std::string& sValue, const std::string& sSource, const std::string& sTarget);
        bool replaceAndExecute(const std::string &sExpr, const std::string &sVarName, int nValue, int &nResult, std::string &sError);
        
        std::vector<std::string> m_vPredefineRules;
        std::vector<GuitarSoloPartGeneratorMovementRule> m_vRules;
};

#endif // GUITAR_SOLO_PART_GENERATOR_MOVEMENT_RULES_H