#ifndef ECC_H
#define ECC_H

#include "main.h"
#include "utils.h"
#include "add_sub.h"
#include "mul.h"
#include "exp.h"

// Point structure for Elliptic Curve
typedef struct {
    gf x;
    gf y;
} Point;

// Helper function to check equality
int Equal(gf x, gf y);

// Optimized Point Addition Function
void AddInEll(Point p1, Point p2, Point* p3, gf A, gf inv_exp);

#endif // ECC_H
