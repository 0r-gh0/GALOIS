#include<add&sub.h>
void MultiPrecisionREDC(struct number* N, struct number* R, u32 Np, struct number* T, struct number* S) {
    //Set T[r + p] = 0  (extra carry word)
    padZero(T,T->len + 1);
    for (int i=0; i<R->len; i++){
        //--loop1- Make T divisible by Bi+1

        c ← 0
        m ← T[i] ⋅ N′ mod B
        for 0 ≤ j < p do
            --loop2- Add the m ⋅ N[j] and the carry from earlier, and find the new carry

            x ← T[i + j] + m ⋅ N[j] + c
            T[i + j] ← x mod B
            c ← ⌊x / B⌋
        end for
        for p ≤ j ≤ r + p − i do
            --loop3- Continue carrying

            x ← T[i + j] + c
            T[i + j] ← x mod B
            c ← ⌊x / B⌋
        end for
    }

    for 0 ≤ i ≤ p do
        S[i] ← T[i + r]
    end for

    if S ≥ N then
        return S − N
    else
        return S
    end if
}