/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "MathLib.h"


/* Coefficients of basis matrix. */
#define CR00     -0.5
#define CR01      1.5
#define CR02     -1.5
#define CR03      0.5
#define CR10      1.0
#define CR11     -2.5
#define CR12      2.0
#define CR13     -0.5
#define CR20     -0.5
#define CR21      0.0
#define CR22      0.5
#define CR23      0.0
#define CR30      0.0
#define CR31      1.0
#define CR32      0.0
#define CR33      0.0

double clamp(double x, double a, double b);
double spline(double x, int nknots, double *knot);
Color spline(double x, int nknots, Color *knot);
