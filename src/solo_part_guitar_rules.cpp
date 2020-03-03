#include "solo_part_guitar_rules.h"
#include <regex>
#include <iostream>
#include <wsjcpp_core.h>
#include <wsjcpp_validators.h>

// ---------------------------------------------------------------------
// SoloPartGuitarRules

SoloPartGuitarRules::SoloPartGuitarRules() {
    TAG = "SoloPartGuitarRules";
}

// ---------------------------------------------------------------------

bool SoloPartGuitarRules::apply(const std::string &sRule, std::string &sError) {
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

                PositionNoteGuitar beginPos(
                    (GuitarNumberString)beginString, 
                    nBeginFinger == GuitarTouchFinger::GUITAR_NO_FINGER ? 0 : beginFret, 
                    nBeginFinger, 
                    GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE
                );
                // std::cout << beginPos.toPrintableString();

                PositionNoteGuitar endPos(
                    (GuitarNumberString)nEndString,
                    nEndFinger == GuitarTouchFinger::GUITAR_NO_FINGER ? 0 : nEndFret, 
                    nEndFinger, 
                    GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE
                );
                // std::cout << " -> " << endPos.toPrintableString() << std::endl;
                add(SoloPartGuitarRule(beginPos, endPos));
                if (beginPos != endPos) {
                    add(SoloPartGuitarRule(endPos, beginPos)); // reverted
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

void SoloPartGuitarRules::add(const SoloPartGuitarRule &rule) {
    for (int i = 0; i < m_vRules.size(); i++) {
        if (rule == m_vRules[i]) {
            WSJCppLog::warn(TAG, 
                "Skip rule " 
                + rule.getNoteBegin().toPrintableString() 
                + " -> "  + rule.getNoteEnd().toPrintableString()
            );
            return;
        }
    }
    m_vRules.push_back(rule);
}

// ---------------------------------------------------------------------

int SoloPartGuitarRules::getSize() {
    return m_vRules.size();
}

// ---------------------------------------------------------------------

std::vector<PositionNoteGuitar> SoloPartGuitarRules::findWithBegin(PositionNoteGuitar note) {
    std::vector<PositionNoteGuitar> vFoundNotes;
    PositionNoteGuitar currectNote(
        note.getGuitarString(),
        note.getFret(),
        note.getFinger(),
        GuitarDurationOfNote::GUITAR_DURATION_OF_NOTE_1_1_SEMIBREVE
    );

    for (int i = 0; i < m_vRules.size(); i++) {
        if (m_vRules[i].getNoteBegin() == currectNote) {
            vFoundNotes.push_back(m_vRules[i].getNoteEnd());
        }   
    }

    return vFoundNotes;
}

// ---------------------------------------------------------------------
// TODO move to core

std::string SoloPartGuitarRules::replaceSubstring(
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

bool SoloPartGuitarRules::replaceAndExecute(
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