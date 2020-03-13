#include "guitar_solo_part_generator_movement_rules.h"
#include <wsjcpp_core.h>
#include <regex>
#include <iostream>
#include <wsjcpp_core.h>
#include <wsjcpp_validators.h>

// ---------------------------------------------------------------------
// GuitarSoloPartGeneratorMovementRule

GuitarSoloPartGeneratorMovementRule::GuitarSoloPartGeneratorMovementRule(
    const std::string &sRule,
    const GuitarSoloPartGeneratorFingerPosition &positionBegin, 
    const GuitarSoloPartGeneratorFingerPosition &positionEnd
) {
    TAG = "GuitarSoloPartGeneratorMovementRule";
    m_sRule = sRule;
    m_positionBegin = positionBegin;
    m_positionEnd = positionEnd;
}

// ---------------------------------------------------------------------

GuitarSoloPartGeneratorFingerPosition GuitarSoloPartGeneratorMovementRule::getPositionBegin() const {
    return m_positionBegin;
}

// ---------------------------------------------------------------------

GuitarSoloPartGeneratorFingerPosition GuitarSoloPartGeneratorMovementRule::getPositionEnd() const {
    return m_positionEnd;
}

// ---------------------------------------------------------------------

bool GuitarSoloPartGeneratorMovementRule::operator==(const GuitarSoloPartGeneratorMovementRule &rhs) const {
    return m_positionBegin == rhs.m_positionBegin && m_positionEnd == rhs.m_positionEnd;
}


// ---------------------------------------------------------------------
// GuitarSoloPartGeneratorMovementRules

GuitarSoloPartGeneratorMovementRules::GuitarSoloPartGeneratorMovementRules() {
    TAG = "GuitarSoloPartGeneratorMovementRules";
    m_vPredefineRules = {
        // allow three fingers for solo (for begginer)
        // index finger
        // middle finger
        // ring finger
        // little finger

        // moving by one string
        "{X string, Y fret, no finger} move to {X string, Y fret, index finger}",
        "{X string, Y fret, no finger} move to {X string, Y fret, middle finger}",
        "{X string, Y fret, no finger} move to {X string, Y fret, ring finger}",
        "{X string, Y fret, no finger} move to {X string, Y fret, little finger}",

        // moving by one string forward
        "{X string, Y fret, index finger} move to {X string, Y+1 fret, index finger}",
        "{X string, Y fret, index finger} move to {X string, Y+1 fret, middle finger}",
        "{X string, Y fret, index finger} move to {X string, Y+2 fret, ring finger}",
        "{X string, Y fret, index finger} move to {X string, Y+3 fret, ring finger}",
        "{X string, Y fret, index finger} move to {X string, Y+3 fret, little finger}",
        "{X string, Y fret, index finger} move to {X string, Y+4 fret, little finger}",

        // moving by one string backward
        "{X string, Y fret, middle finger} move to {X string, Y-1 fret, middle finger}",
        "{X string, Y fret, middle finger} move to {X string, Y-2 fret, middle finger}",
        // "{X string, Y fret, middle finger} move to {X string, Y-1 fret, index finger}",
        // "{X string, Y fret, ring finger} move to {X string, Y-1 fret, middle finger}",
        // "{X string, Y fret, ring finger} move to {X string, Y-2 fret, index finger}",

        "{X string, Y fret, index finger} move to {X+2 string, 0 fret, no finger}",

        "{X string, Y fret, index finger} move to {X+1 string, Y+2 fret, middle finger}",
        "{X string, Y fret, index finger} move to {X+2 string, Y+2 fret, ring finger}",
        "{X string, Y fret, index finger} move to {X+3 string, Y+2 fret, little finger}"
    };
}

// ---------------------------------------------------------------------

std::vector<std::string> GuitarSoloPartGeneratorMovementRules::getPredefinedRules() {
    return m_vPredefineRules;
}

// ---------------------------------------------------------------------

void GuitarSoloPartGeneratorMovementRules::applyPredefinedRules() {
    
    // from silent
    GuitarSoloPartGeneratorFingerPosition silentNote;
    for (int nFret = 0; nFret < 24; nFret++) {
        for (int nString = 1; nString < 6; nString++) {
            for (int nFinger = 1; nFinger < 4; nFinger++) {
                add(
                    GuitarSoloPartGeneratorMovementRule(
                        "silent", 
                        silentNote, 
                        GuitarSoloPartGeneratorFingerPosition(
                            (GuitarNumberString)nString,
                            nFret,
                            (nFret == 0 ? ::GUITAR_NO_FINGER : (GuitarTouchFinger)nFinger)
                        )
                    )
                );
            }
        }
    }

    std::string sError;
    for (int i = 0; i < m_vPredefineRules.size(); i++) {
        std::string sPredefinedRule = m_vPredefineRules[i];
        if (!apply(sPredefinedRule, sError)) {
            WSJCppLog::throw_err(TAG, sError);
        }
    }
}

// ---------------------------------------------------------------------

bool GuitarSoloPartGeneratorMovementRules::apply(const std::string &sRule, std::string &sError) {
    // TODO parse
    // {X string, 0 fret, no finger} move to {X string, 1 fret, any finger}

    std::regex rgx("^\\{[ ]*([0-6]{1}|[A-Z]{1})[ ]+string[ ]*,"
        "[ ]+([A-Z]{1}|[0-9]+)[ ]+fret[ ]*,"
        "[ ]+(no|index|middle|ring|little)[ ]+finger[ ]*\\}"
        "[ ]+move[ ]+to[ ]+\\{"
        "[ ]*([0-6]{1}|[A-Z]{1}|[A-Z]{1}[+-]+[0-6]{1})[ ]+string[ ]*,"
        "[ ]+([0-9]+|[A-Z]{1}|[A-Z]{1}[+-]+[0-9]+)[ ]+fret[ ]*,"
        "[ ]+(no|index|middle|ring|little)[ ]+finger[ ]*"
        "\\}$");
    std::smatch matches;

    if (std::regex_search(sRule, matches, rgx)) {
        if (matches.size() != 7) {
            sError = "Wrong match string";
            return false;
        }
        WSJCppValidatorNumber validatorNumber;
        std::string sIgnoreError;

        std::string sBeginGuitarString = matches[1].str();
        GuitarStringRange beginStringRange;
        bool bBeginStringVar = false;
        if (validatorNumber.isValid(sBeginGuitarString, sIgnoreError)) {
            int n = std::stoi(sBeginGuitarString);
            beginStringRange.setMin((GuitarNumberString)n);
            beginStringRange.setMax((GuitarNumberString)n);
        } else {
            bBeginStringVar = true;
        }
        std::string sBeginFret = matches[2].str();
        bool bBeginFretVar = false;
        GuitarFretRange beginFretRange;
        if (validatorNumber.isValid(sBeginFret, sIgnoreError)) {
            int n = std::stoi(sBeginFret);
            beginFretRange.setMin(n);
            beginFretRange.setMax(n);
        } else {
            bBeginFretVar = true;
        }
        GuitarTouchFinger nBeginFinger = GuitarSoloPartGeneratorEnums::valueToFinger(matches[3].str());

        std::string sEndGuitarString = matches[4].str();
        int nDefinedEndGuitarString = -1;
        bool bEndStringExpr = false;
        if (validatorNumber.isValid(sEndGuitarString, sIgnoreError)) {
            nDefinedEndGuitarString = std::stoi(sEndGuitarString);
        } else if (bBeginStringVar) {
            bEndStringExpr = true;
        } else {
            sError = "Has not defined variable in left part (for string)";
            return false;
        }
        std::string sEndFret = matches[5].str();
        int nDefinedEndFret = -1;
        bool bEndFretExpr = false;
        if (validatorNumber.isValid(sEndFret, sIgnoreError)) {
            nDefinedEndFret = std::stoi(sEndFret);
        } else if (bBeginFretVar) {
            bEndFretExpr = true;
        } else {
            sError = "Has not defined variable in left part (for fret)";
            return false;
        }
        GuitarTouchFinger nEndFinger = GuitarSoloPartGeneratorEnums::valueToFinger(matches[6].str());
        for (int beginString = beginStringRange.getMin(); beginString <= beginStringRange.getMax(); beginString++) {
            int nEndString = nDefinedEndGuitarString;
            for (int beginFret = beginFretRange.getMin(); beginFret <= beginFretRange.getMax(); beginFret++) {
                int nEndFret = nDefinedEndFret;

                if (beginFret == 0 && nBeginFinger != GuitarTouchFinger::GUITAR_NO_FINGER) {
                    // skip empty strings
                    continue;
                }

                /*WSJCppLog::warn(TAG, "Try exp "
                    "bBeginStringVar = " + std::string(bBeginStringVar ? "yes" : "no")
                    + " bEndStringExpr = " + std::string(bEndStringExpr ? "yes" : "no")
                    + " sEndGuitarString = " + sEndGuitarString
                );*/

                if (bBeginStringVar && bEndStringExpr) {
                    if (!replaceAndExecute(sEndGuitarString, sBeginGuitarString, beginString, nEndString, sError)) {
                        return false;
                    }
                    if (nEndString < 1 || nEndString > 6) {
                        // skip unavalable strings
                        continue;
                    }
                }
                
                if (bBeginFretVar && bEndFretExpr) {
                    if (!replaceAndExecute(sEndFret, sBeginFret, beginFret, nEndFret, sError)) {
                        return false;
                    }

                    if (nEndFret < 0 || nEndFret > 24) {
                        // skip unavalable frets
                        continue;
                    }
                }

                if (nEndFret == 0 && nEndFinger != GuitarTouchFinger::GUITAR_NO_FINGER) {
                    // skip empty strings
                    continue;
                }

                GuitarSoloPartGeneratorFingerPosition beginPos(
                    beginString,
                    nBeginFinger == GuitarTouchFinger::GUITAR_NO_FINGER ? 0 : beginFret, 
                    nBeginFinger
                );
                // std::cout << beginPos.toPrintableString();

                GuitarSoloPartGeneratorFingerPosition endPos(
                    nEndString,
                    nEndFinger == GuitarTouchFinger::GUITAR_NO_FINGER ? 0 : nEndFret, 
                    nEndFinger
                );
                // std::cout << " -> " << endPos.toPrintableString() << std::endl;
                add(GuitarSoloPartGeneratorMovementRule(sRule, beginPos, endPos));
                if (beginPos != endPos) {
                    add(GuitarSoloPartGeneratorMovementRule(sRule, endPos, beginPos)); // reverted
                }
            }
        }
    } else {
        sError = "Wrong string";
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

void GuitarSoloPartGeneratorMovementRules::add(const GuitarSoloPartGeneratorMovementRule &rule) {
    for (int i = 0; i < m_vRules.size(); i++) {
        if (rule == m_vRules[i]) {
            WSJCppLog::warn(TAG, 
                "Skip rule " 
                + rule.getPositionBegin().toPrintableString() 
                + " -> "  + rule.getPositionEnd().toPrintableString()
            );
            return;
        }
    }
    m_vRules.push_back(rule);
}

// ---------------------------------------------------------------------

int GuitarSoloPartGeneratorMovementRules::getSize() {
    return m_vRules.size();
}

// ---------------------------------------------------------------------

std::vector<PositionNoteGuitar> GuitarSoloPartGeneratorMovementRules::findWithBegin(PositionNoteGuitar note) {
    std::vector<PositionNoteGuitar> vFoundNotes;
    GuitarSoloPartGeneratorFingerPosition currentPosition(
        note.getGuitarString(),
        note.getFret(),
        note.getFinger()
    );

    

    for (int i = 0; i < m_vRules.size(); i++) {
        if (m_vRules[i].getPositionBegin() == currentPosition) {
            GuitarSoloPartGeneratorFingerPosition positionEnd = m_vRules[i].getPositionEnd();
            vFoundNotes.push_back(PositionNoteGuitar(
                (GuitarNumberString)positionEnd.getGuitarString(),
                positionEnd.getFret(),
                positionEnd.getFinger(),
                GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE
            ));
        }   
    }

    return vFoundNotes;
}

// ---------------------------------------------------------------------
// TODO move to core

std::string GuitarSoloPartGeneratorMovementRules::replaceSubstring(
    const std::string& sValue, 
    const std::string& sSource, 
    const std::string& sTarget
) {
    size_t start_pos = sValue.find(sSource);
    if (start_pos == std::string::npos) {
        return sValue;
    }
    std::string sRet = sValue;
    sRet.replace(start_pos, sSource.length(), sTarget);
    return sRet;
}

// ---------------------------------------------------------------------

bool GuitarSoloPartGeneratorMovementRules::replaceAndExecute(
    const std::string &sExpr, 
    const std::string &sVarName, 
    int nValue, 
    int &nResult, 
    std::string &sError
) {
    std::string sExpr2 = replaceSubstring(sExpr, sVarName, std::to_string(nValue));
    sExpr2 = WSJCppCore::trim(sExpr2);

    std::string sErrorInfo = "'" + sExpr + "' -> '" + sExpr2 + "', var_name = '" + sVarName + "', var_value = '" + std::to_string(nValue) + "'";
    if (sExpr2 == sExpr) {
        sError = "Wrong expression (1) " + sErrorInfo;
        return false;
    }

    std::regex rgx1("^([0-9]+)(\\+|-)([0-9]+)$");
    std::smatch matches1;
    std::regex rgx2("^([0-9]+)$");
    std::smatch matches2;
    if (std::regex_search(sExpr2, matches1, rgx1)) {
        if (matches1.size() != 4) {
            sError = "Wrong regexp (1). " + sErrorInfo;
            return false;
        }

        int nX1 = std::atoi(matches1[1].str().c_str());
        std::string sOper = matches1[2].str();
        int nX2 = std::atoi(matches1[3].str().c_str());
        if (sOper == "+") {
            nResult = nX1 + nX2;
        } else if (sOper == "-") {
            nResult = nX1 - nX2;
        }
    } else if (std::regex_search(sExpr2, matches2, rgx2)) {
        if (matches2.size() != 2) {
            sError = "Wrong regexp (2). " + sErrorInfo;
            return false;
        }
        nResult = std::atoi(matches2[1].str().c_str());
    } else {
        sError = "Regexp not match. " + sErrorInfo;
        return false;
    }
    return true;
}
