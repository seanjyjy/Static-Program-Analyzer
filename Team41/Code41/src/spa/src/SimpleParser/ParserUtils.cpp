#include "ParserUtils.h"

#include <cctype>

using namespace std;

string ParserUtils::highlight(const string &simple, int fromRow, int fromCol, int toRow, int toCol) {
    int currRow = 0;
    int currCol = 0;
    string ret;
    string highlight;
    string prog;

    int i = 0;
    size_t n = simple.size();
    while (i < n) {
        // highlight row by row
        int row = currRow;
        while (i < n && row == currRow) {
            prog += simple[i];

            // build the highlight as long as we are within range
            if (inHighlightZone(currRow, currCol, fromRow, fromCol, toRow, toCol)) {
                highlight += (isspace(simple[i]) && simple[i] != ' ') ? simple[i] : '^';
            } else if (inHighlightRow(currRow, fromRow, toRow)){
                highlight += (isspace(simple[i])) ? simple[i] : ' ';
            }
            
            // update row, col and index
            currCol++;
            if (simple[i] == '\n') {
                currRow++;
                currCol = 0;
            }
            i++;
        }

        // done with a row, append onto final string
        ret += prog;
        prog.clear();
        if (!highlight.empty()) {
            while (!highlight.empty() && highlight.back() == '\n') highlight.pop_back();
            ret += '\n' + highlight + '\n';
            highlight.clear();
        }
    }
    return ret;
}

bool ParserUtils::inHighlightZone(int currRow, int currCol, int fromRow, int fromCol, int toRow, int toCol) {
    if (fromRow == toRow) {
        // highlight in the same row
        return (currRow == fromRow) && (currCol >= fromCol) && (currCol <= toCol);
    } else {
        // highlight across multiple rows
        return (currRow == fromRow && currCol >= fromCol) ||
               (currRow > fromRow && currRow < toRow) ||
               (currRow == toRow && currCol <= toCol);
    }
}

bool ParserUtils::inHighlightRow(int currRow, int fromRow, int toRow) {
    return currRow >= fromRow && currRow <= toRow;
}
