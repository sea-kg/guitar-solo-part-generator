#ifndef SOLO_PART_GUITAR_RULES_H
#define SOLO_PART_GUITAR_RULES_H

#include <string>
#include <vector>
#include "solo_part_guitar_rule.h"

// ---------------------------------------------------------------------

class SoloPartGuitarRules {
    public:
        SoloPartGuitarRules();
        bool apply(const std::string &sRule, std::string &sError);
        void add(const SoloPartGuitarRule &rule);
        int getSize();
        std::vector<PositionNoteGuitar> findWithBegin(PositionNoteGuitar note);

    private:
        std::string TAG;

        std::string replaceSubstring(const std::string& sValue, const std::string& sSource, const std::string& sTarget);
        bool replaceAndExecute(const std::string &sExpr, const std::string &sVarName, int nValue, int &nResult, std::string &sError);
        

        std::vector<SoloPartGuitarRule> m_vRules;
};


#endif // SOLO_PART_GUITAR_RULES_H