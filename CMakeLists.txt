cmake_minimum_required(VERSION 3.16)

project(qtcalcsheet VERSION 0.1 LANGUAGES CXX)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Bison and flesupport for formula grammar
find_package(BISON)
find_package(FLEX)

# Paths for grammar files
#set(FORMULA_GRAMMAR_SRC ${CMAKE_SOURCE_DIR}/src/formula/formulagrammer.y)
#set(FORMULA_GRAMMAR_CPP ${CMAKE_SOURCE_DIR}/src/formula/formulagrammer.cpp)
#set(FORMULA_GRAMMAR_HPP ${CMAKE_SOURCE_DIR}/src/formula/formulagrammer.h)


#FLEX_TARGET(FormulaLexer ${CMAKE_SOURCE_DIR}/src/formula/formulalexer.l ${CMAKE_SOURCE_DIR}/src/formula/formulalexer.cpp)
#set(GENERATED_FORMULA_LEXER ${FLEX_FormulaLexer_OUTPUTS})

#BISON_TARGET(FormulaGrammar ${FORMULA_GRAMMAR_SRC} ${FORMULA_GRAMMAR_CPP}
#    DEFINES_FILE ${FORMULA_GRAMMAR_HPP}
#)
#set(GENERATED_FORMULA_GRAMMAR ${BISON_FormulaGrammar_OUTPUTS})

BISON_TARGET(FormulaParser
    ${CMAKE_SOURCE_DIR}/src/formula/formulagrammer.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/formulagrammer.h
)
FLEX_TARGET(FormulaScanner ${CMAKE_SOURCE_DIR}/src/formula/formulalexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(FormulaScanner FormulaParser)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Recursively find all .h and .hpp files in src/ and subdirectories
file(GLOB_RECURSE PROJECT_SOURCES
    "src/*.h"
    "src/*.cpp"
    "src/*.qrc"
    "src/*.ui"
)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${BISON_FormulaParser_OUTPUTS}
    ${FLEX_FormulaScanner_OUTPUTS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

qt_finalize_executable(${PROJECT_NAME})
