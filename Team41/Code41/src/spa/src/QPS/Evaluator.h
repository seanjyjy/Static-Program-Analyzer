//
// Created by lum jian yang sean on 26/1/22.
//
#pragma once
#include "PQLTable.h"

class Evaluator {
public:
    virtual PQLTable* evaluate() const = 0;
};