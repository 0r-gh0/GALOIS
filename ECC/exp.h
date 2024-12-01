#ifndef EXP_H
#define EXP_H

#include "main.h"
#include "utils.h"
#include "add_sub.h"
#include "mul.h"

// Right-to-Left Modular Exponentiation
void Exp_RTL(gf base, gf exp, gf* res);

// Left-to-Right Modular Exponentiation
void Exp_LTR(gf base, gf exp, gf* res);

// Efficient Left-to-Right Modular Exponentiation without branching
void Exp_LTR_e(gf base, gf exp, gf* res);

#endif // EXP_H