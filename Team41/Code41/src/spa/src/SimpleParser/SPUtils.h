#pragma once

#include <string>
#include "Common/AstNode/TNode.h"

using namespace std;

class SPUtils {
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
    static string highlight(const string &simple, int fromRow, int fromCol, int toRow, int toCol);

    /**
     * Adds a very prominent banner to the top and bottom of a string to make it more noticeable.
     *
     * @param msg the message to bannerize.
     * @param topBannerMsg the message enclosed in the top banner.
     * @param btmBannerMsg the message enclosed in the bottom banner.
     * @return the bannerized message.
     */
    static string withBanner(const string &msg, const string &topBannerMsg, const string &btmBannerMsg);

    /**
     * Convenience function that highlights a message, then adds a banner.
     */
    static string
    highlightAndBanner(const string &simple, int fromRow, int fromCol, int toRow, int toCol, const string &top,
                       const string &btm);
};
