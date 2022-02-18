/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "spline.h"


double clamp(double x, double a, double b)
{
    return (x < a ? a : (x > b ? b : x));
}


double spline(double x, int nknots, double *knot)
{
    int span;
    int nspans = nknots - 3;
    double c0, c1, c2, c3;	/* coefficients of the cubic.*/

    if (nspans < 1) {
        return 0;
    }

    /* Find the appropriate 4-point span of the spline. */
    x = clamp(x, 0, 1) * nspans;
    span = (int) x;
    if (span >= nknots - 3)
        span = nknots - 3;
    x -= span;
    knot += span;

    /* Evaluate the span cubic at x using Horner's rule. */
    c3 = CR00*knot[0] + CR01*knot[1]
       + CR02*knot[2] + CR03*knot[3];
    c2 = CR10*knot[0] + CR11*knot[1]
       + CR12*knot[2] + CR13*knot[3];
    c1 = CR20*knot[0] + CR21*knot[1]
       + CR22*knot[2] + CR23*knot[3];
    c0 = CR30*knot[0] + CR31*knot[1]
       + CR32*knot[2] + CR33*knot[3];

    return ((c3*x + c2)*x + c1)*x + c0;
}


Color spline(double x, int nknots, Color *knot)
{
    double* r = new double[nknots];
    double* g = new double[nknots];
    double* b = new double[nknots];

    for (int i=0; i<nknots; ++i)
    {
        r[i] = knot[i].r;
        g[i] = knot[i].g;
        b[i] = knot[i].b;
    }

    return Color(spline(x, nknots, r), spline(x, nknots, g), spline(x, nknots, b));
}
