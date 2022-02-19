#pragma once

#include <string>

using namespace std;

class ParserUtils {
private:
    static bool inHighlightZone(int currRow, int currCol, int fromRow, int fromCol, int toRow, int toCol);
    static bool inHighlightRow(int currRow, int fromRow, int toRow);
public:
    /**
     * Highlights a SIMPLE source from a starting row and column (inclusive) to an ending row and column (inclusive).
     * Whitespaces as given by isspace(char c) will not be highlighted.
     *
     * @param simple the SIMPLE source program
     * @param fromRow starting row, 0-indexed
     * @param fromCol starting col, 0-indexed
     * @param toRow ending row, 0-indexed
     * @param toCol ending row, 0-indexed
     * @return the highlighted SIMPLE source
     */
    static string highlight(const string& simple, int fromRow, int fromCol, int toRow, int toCol);
};
