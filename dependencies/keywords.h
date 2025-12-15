#ifndef KEYWORD_H
#define KEYWORD_H

#include <string>
#include <unordered_map>
#include <unordered_set>

enum ObjectType {
  NONE,
  KEYWORD,
  MATH_CONSTANT,
  SIMULATION,
  COMMENT,
  KEYVALUE,
  PORT,
  STRING,
};

// ------------------------------------------------------------
// keywordMap
// ------------------------------------------------------------v
static const std::unordered_set<std::string> keywordSet = {
    // ----------------
    // General Keywords
    // ----------------
    "altergroup", "correlate", "else", "end", "ends", "export", "for",
    "function", "global", "ic", "if", "in", "inline", "input", "invisible",
    "library", "local", "march", "model", "nodeset", "out", "output",
    "parameters", "paramset", "plot", "print", "protect", "pwr", "real",
    "return", "save", "sens", "statistics", "subckt", "to", "truncate",
    "unprotect", "vary", "visible",

    // --------------
    // Reserved Words
    // --------------
    "freq", "scalem", "temp", "temper", "time", "tnom",

    // --------------
    // Other Keywords
    // --------------
    "simulator", "include"};

static const std::unordered_set<std::string> mathConstantSet = {
    // --------------
    // Math Constants
    // --------------
    "M_1_PI", "M_2_PI",     "M_2_SQRTPI", "M_DEGPERRAD", "M_E",
    "M_LN10", "M_LN2",      "M_LOG10E",   "M_LOG2E",     "M_PI",
    "M_PI_2", "M_PI_4",     "M_SQRT1_2",  "M_SQRT2",     "M_TWO_PI",
    "P_C",    "P_CELSIUS0", "P_EPS0",     "P_H",         "P_K",
    "P_Q",    "P_U0",

};

static const std::unordered_set<std::string> simulationSet = {
    // --------------------
    // Simulations Keywords
    // --------------------
    "ac",         "acmatch", "alter",   "altergroup", "check",
    "checklimit", "cosim",   "dc",      "dcmatch",    "envlp",
    "hp",         "hbac",    "hbnoise", "hbsp",       "hbstb",
    "hbxf",       "info",    "lf",      "loadpull",   "montecarlo",
    "noise",      "options", "pac",     "pnoise",     "psp",
    "pss",        "pstb",    "pxf",     "pz",         "qpac",
    "qpnoise",    "qpsp",    "qpss",    "qpxf",       "reliability",
    "set",        "shell",   "sp",      "stb",        "stress",
    "sweep",      "tdr",     "thermal", "tran",       "uti",
    "xf",
};

static const std::unordered_map<char, ObjectType> charKeywordMap = {
    {'(', PORT}, {'=', KEYVALUE}

};

#endif
