//
// Created by lum jian yang sean on 26/1/22.
//
#pragma once

class PQLTable {
public:
    PQLTable();
    bool isEmpty();
    void intersect(PQLTable* intermediatePQLTable);
};