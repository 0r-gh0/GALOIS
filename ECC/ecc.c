#include "main.h"
#include "utils.h"
#include "add_sub.h"
#include "mul.h
#include "exp.h"
#include "ecc.h"

gf inv_exp = {
    .num =(uint64[]){535425011, 174332635, 444665496, 192778653, 388389189, 518147849, 304619691, 363717891, 15281728};
    .size = 9;
}

gf zero = {
    .num = (uint64[]){0, 0,0,0,0,0,0,0,0},
    .size = 9
};

gf one = {
    .num = (uint64[]){1, 0,0,0,0,0,0,0,0},
    .size = 9
};

// Helper function to check equality
int Equal(gf x, gf y) {
    if (x.size != y.size) return 0;
    
    for (int i = 0; i < x.size; i++) {
        if (x.num[i] != y.num[i]) {
            return 0;
        }
    }
    return 1;
}

// Optimized Point Addition Function
void AddInEll(Point p1, Point p2, Point* p3, gf A, gf inv_exp) {
    // Temporary variables for calculations
    gf temp1 = initlz(9);
    gf temp2 = initlz(9);
    gf m = initlz(9);
    
    // Predefined constants (you should define these globally)
    gf zero = initlz(9);
    zero.num[0] = 0;
    
    gf one = initlz(9);
    one.num[0] = 1;
    
    // Point at Infinity handling
    if ((Equal(p1.x, one) && Equal(p1.y, one)) || 
        (Equal(p2.x, one) && Equal(p2.y, one))) {
        p3->x = (Equal(p1.x, one) && Equal(p1.y, one)) ? p2.x : p1.x;
        p3->y = (Equal(p1.x, one) && Equal(p1.y, one)) ? p2.y : p1.y;
        return;
    }
    
    // Different points addition
    if (!Equal(p1.x, p2.x)) {
        // Different x-coordinates
        ConvrtInZp(p1.y, &temp1);        // -y1
        AddInZp(temp1, p2.y, &temp1);    // y2 - y1
        
        ConvrtInZp(p1.x, &temp2);        // -x1
        AddInZp(p2.x, temp2, &m);        // x2 - x1
        
        Exp_RTL(m, inv_exp, &m);         // 1/(x2 - x1)
        MultInZp(temp1, m, &m);          // Slope (y2 - y1)/(x2 - x1)
        
        // X3 calculation
        ConvrtInZp(p2.x, &temp1);        // -x2
        AddInZp(temp2, temp1, &temp2);   // -x1 - x2
        MultInZp(m, m, &temp1);          // m^2
        AddInZp(temp1, temp2, &(p3->x)); // x3 = m^2 - x1 - x2
        
        // Y3 calculation
        ConvrtInZp(p3->x, &temp1);       // -x3
        AddInZp(temp1, p1.x, &temp1);    // x1 - x3
        MultInZp(m, temp1, &temp1);      // m(x1 - x3)
        ConvrtInZp(p1.y, &temp2);        // -y1
        AddInZp(temp2, temp1, &(p3->y)); // y3 = m(x1 - x3) - y1
        
        return;
    }
    
    // Point doubling (same x-coordinates)
    if (Equal(p1.x, p2.x)) {
        // Check if points are symmetric (y coordinates are additive inverses)
        if (!Equal(p1.y, p2.y)) {
            // Points are symmetric, result is point at infinity
            p3->x = one;
            p3->y = one;
            return;
        }
        
        // Check if point is at zero
        if (Equal(p1.y, zero)) {
            p3->x = one;
            p3->y = one;
            return;
        }
        
        // Point doubling calculation
        AddInZp(p1.y, p1.y, &temp1);     // 2y1
        Exp_RTL(temp1, inv_exp, &m);     // 1/(2y1)
        
        MultInZp(p1.x, p1.x, &temp1);    // x1^2
        AddInZp(temp1, temp1, &temp2);   // 2x1^2
        AddInZp(temp1, temp2, &temp2);   // 3x1^2
        AddInZp(A, temp2, &temp2);       // 3x1^2 + A
        MultInZp(temp2, m, &m);          // Slope
        
        ConvrtInZp(p1.x, &temp2);        // -x1
        AddInZp(temp2, temp2, &temp2);   // -2x1
        MultInZp(m, m, &temp1);          // m^2
        AddInZp(temp1, temp2, &(p3->x)); // x3 = m^2 - 2x1
        
        ConvrtInZp(p3->x, &temp1);       // -x3
        AddInZp(temp1, p1.x, &temp1);    // x1 - x3
        MultInZp(m, temp1, &temp1);      // m(x1 - x3)
        ConvrtInZp(p1.y, &temp2);        // -y1
        AddInZp(temp2, temp1, &(p3->y)); // y3 = m(x1 - x3) - y1
    }
    
    // Memory cleanup
    free_int(&temp1);
    free_int(&temp2);
    free_int(&m);
}

