/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "CellTexture.h"
#include <cmath>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <iostream>

#include "Scene.h"
#include "spline.h"


TexCellular::TexCellular(double c0, double c1, double c2, double c3,
           double e0, double e1, double e2, double e3,
           int octaves,
           Color Col1, Color Col2,
           std::vector<Color>& colst )
:
    C0(c0),
    C1(c1),
    C2(c2),
    C3(c3),
    E0(e0),
    E1(e1),
    E2(e2),
    E3(e3),
    oct(octaves),
    mDistanceType(0),
    col1(Col1),
    col2(Col2)
{
    static unsigned int counter = 1;
    srand(counter++^time(0));
    srand( static_cast<unsigned int>(time(0))^counter++ );

    for (int i=0 ; i< MAX_TABLE ; i+=2)
    {
        col[i] = colst[i/2];
        col[i+1] = 0.5*col[i];
    }

    vector3 p(0,0,0);
    minf = maxf = f(p);
    for (p.x=0.0 ; p.x<4.0 ; p.x+=0.1)
    {
        for (p.y=0.0 ; p.y<4.0 ; p.y+=0.1)
        {
            for (p.z=0.0 ; p.z<4.0 ; p.z+=0.1)
            {
                double t = f(p);
                if (t<minf) minf=t;
                if (t>maxf) maxf=t;
            }
        }
    }
}


void CmdTexCellular::parse(std::istringstream& line, Scene* scene)
{
    double c0, c1, c2, c3;
    double e0, e1, e2, e3;
    int octaves;
    Color col1, col2;
    std::vector<Color> colst(3);

    read_number(line, c0);
    read_number(line, c1);
    read_number(line, c2);
    read_number(line, c3);

    read_number(line, e0);
    read_number(line, e1);
    read_number(line, e2);
    read_number(line, e3);

    read_number(line, octaves);

    read_vector3(line, col1);
    read_vector3(line, col2);

    read_vector3(line, colst[0]);
    read_vector3(line, colst[1]);
    read_vector3(line, colst[2]);

    Texture* opTexture = new TexCellular(c0, c1, c2, c3, e0, e1, e2, e3, octaves, col1, col2, colst);
    scene->addTexture(opTexture);

    //std::cout << "TexCellular " << c0 << c1 << c2 << c3 << e0 << e1 << e2 << e3 << octaves << std::endl;
    printf("TexCellular %f %f %f %f   %f %f %f %f   %d   %f %f %f   %f %f %f      %f %f %f   %f %f %f   %f %f %f\n", c0, c1, c2, c3, e0, e1, e2, e3, octaves, col1.r, col1.g, col1.b, col2.r, col2.g, col2.b, colst[0].r, colst[0].g, colst[0].b, colst[1].r, colst[1].g, colst[1].b, colst[2].r, colst[2].g, colst[2].b);
}

double TexCellular::f(vector3 p)
{
    double at[3]={p.x, p.y, p.z};
    double F[4];
    double delta[4][3];
    unsigned long ID[4];

    double r=0;
    double pw=1;
    for (int i=0; i<oct; ++i)
    {
        at[0] = pw*p.x ; at[1] = pw*p.y ; at[2] = pw*p.z;
        Worley(at, 4, F, delta, ID);
        r += (C0*pow(F[0], E0) + C1*pow(F[1], E1) + C2*pow(F[2], E2) + C3*pow(F[3], E3))/pw;
        pw *= 2;
    }

    return r;
}



Color TexCellular::getColor(vector3 pnt, Primitive* prim)
{
    double fn = (f(pnt)-minf)/(maxf-minf);
    return spline(fn, MAX_TABLE, col);
    //return Color(fn, fn, fn);
}




// Copyright 1994, 2002 by Steven Worley
// This software may be modified and redistributed without restriction
// provided this comment header remains intact in the source code.
// This code is provided with no warrantee, express or implied, for
// any purpose.
//
// A detailed description and application examples can be found in the
// 1996 SIGGRAPH paper "A Cellular Texture Basis Function" and
// especially in the 2002 book "Texturing and Modeling, a Procedural
// Approach, 3rd edition." There is also extra information on the web
// site http://www.worley.com/cellular.html .
//
// If you do find interesting uses for this tool, and especially if
// you enhance it, please drop me an email at steve@worley.com.


// A hardwired lookup table to quickly determine how many feature
// points should be in each spatial cube. We use a table so we don't
// need to make multiple slower tests.  A random number indexed into
// this array will give an approximate Poisson distribution of mean
// density 2.5. Read the book for the longwinded explanation.

int TexCellular::Poisson_count[256]=
{4,3,1,1,1,2,4,2,2,2,5,1,0,2,1,2,2,0,4,3,2,1,2,1,3,2,2,4,2,2,5,1,2,3,2,2,2,2,2,3,
 2,4,2,5,3,2,2,2,5,3,3,5,2,1,3,3,4,4,2,3,0,4,2,2,2,1,3,2,2,2,3,3,3,1,2,0,2,1,1,2,
 2,2,2,5,3,2,3,2,3,2,2,1,0,2,1,1,2,1,2,2,1,3,4,2,2,2,5,4,2,4,2,2,5,4,3,2,2,5,4,3,
 3,3,5,2,2,2,2,2,3,1,1,4,2,1,3,3,4,3,2,4,3,3,3,4,5,1,4,2,4,3,1,2,3,5,3,2,1,3,1,3,
 3,3,2,3,1,5,5,4,2,2,4,1,3,4,1,5,3,3,5,3,4,3,2,2,1,1,1,1,1,2,4,5,4,5,4,2,1,5,1,1,
 2,3,3,3,2,5,2,3,3,2,0,2,1,1,4,2,1,3,2,1,2,2,3,2,5,5,3,4,5,5,2,4,4,5,3,2,2,2,1,4,
 2,3,3,4,2,5,4,2,4,2,2,2,4,5,3,2};


// This constant is manipulated to make sure that the mean value of F[0]
// is 1.0. This makes an easy natural "scale" size of the cellular features. */
#define DENSITY_ADJUSTMENT  0.398150



// The main function! */
void TexCellular::Worley(double at[3], long max_order, double *F, double delta[4][3], unsigned long *ID)
{
    double x2,y2,z2, mx2, my2, mz2;
    double new_at[3];
    long int_at[3], i;

    // Initialize the F values to "huge" so they will be replaced by the
    // first real sample tests. Note we'll be storing and comparing the
    // SQUARED distance from the feature points to avoid lots of slow
    // sqrt() calls. We'll use sqrt() only on the final answer. */
    for (i=0; i<max_order; i++) F[i]=999999.9;

    // Make our own local copy, multiplying to make mean(F[0])==1.0  */
    new_at[0]=DENSITY_ADJUSTMENT*at[0];
    new_at[1]=DENSITY_ADJUSTMENT*at[1];
    new_at[2]=DENSITY_ADJUSTMENT*at[2];

    // Find the integer cube holding the hit point */
    int_at[0]=(long)floor(new_at[0]); /* A macro could make this slightly faster */
    int_at[1]=(long)floor(new_at[1]);
    int_at[2]=(long)floor(new_at[2]);

    // A simple way to compute the closest neighbors would be to test all
    // boundary cubes exhaustively. This is simple with code like:
    // {
    //     long ii, jj, kk;
    //     for (ii=-1; ii<=1; ii++) for (jj=-1; jj<=1; jj++) for (kk=-1; kk<=1; kk++)
    //     AddSamples(int_at[0]+ii,int_at[1]+jj,int_at[2]+kk,
    //     max_order, new_at, F, delta, ID);
    // }
    // But this wastes a lot of time working on cubes which are known to be
    // too far away to matter! So we can use a more complex testing method
    // that avoids this needless testing of distant cubes. This doubles the
    // speed of the algorithm. */

    // Test the central cube for closest point(s). */
    AddSamples(int_at[0], int_at[1], int_at[2], max_order, new_at, F, delta, ID);

    // We test if neighbor cubes are even POSSIBLE contributors by examining the
    // combinations of the sum of the squared distances from the cube's lower
    // or upper corners.
    x2=new_at[0]-int_at[0];
    y2=new_at[1]-int_at[1];
    z2=new_at[2]-int_at[2];
    mx2=(1.0-x2)*(1.0-x2);
    my2=(1.0-y2)*(1.0-y2);
    mz2=(1.0-z2)*(1.0-z2);
    x2*=x2;
    y2*=y2;
    z2*=z2;

    // Test 6 facing neighbors of center cube. These are closest and most
    // likely to have a close feature point. */
    if (x2<F[max_order-1])  AddSamples(int_at[0]-1, int_at[1]  , int_at[2]  , max_order, new_at, F, delta, ID);
    if (y2<F[max_order-1])  AddSamples(int_at[0]  , int_at[1]-1, int_at[2]  , max_order, new_at, F, delta, ID);
    if (z2<F[max_order-1])  AddSamples(int_at[0]  , int_at[1]  , int_at[2]-1, max_order, new_at, F, delta, ID);

    if (mx2<F[max_order-1]) AddSamples(int_at[0]+1, int_at[1]  , int_at[2]  , max_order, new_at, F, delta, ID);
    if (my2<F[max_order-1]) AddSamples(int_at[0]  , int_at[1]+1, int_at[2]  , max_order, new_at, F, delta, ID);
    if (mz2<F[max_order-1]) AddSamples(int_at[0]  , int_at[1]  , int_at[2]+1, max_order, new_at, F, delta, ID);

    // Test 12 "edge cube" neighbors if necessary. They're next closest. */
    if ( x2+ y2<F[max_order-1]) AddSamples(int_at[0]-1, int_at[1]-1, int_at[2]  , max_order, new_at, F, delta, ID);
    if ( x2+ z2<F[max_order-1]) AddSamples(int_at[0]-1, int_at[1]  , int_at[2]-1, max_order, new_at, F, delta, ID);
    if ( y2+ z2<F[max_order-1]) AddSamples(int_at[0]  , int_at[1]-1, int_at[2]-1, max_order, new_at, F, delta, ID);
    if (mx2+my2<F[max_order-1]) AddSamples(int_at[0]+1, int_at[1]+1, int_at[2]  , max_order, new_at, F, delta, ID);
    if (mx2+mz2<F[max_order-1]) AddSamples(int_at[0]+1, int_at[1]  , int_at[2]+1, max_order, new_at, F, delta, ID);
    if (my2+mz2<F[max_order-1]) AddSamples(int_at[0]  , int_at[1]+1, int_at[2]+1, max_order, new_at, F, delta, ID);
    if ( x2+my2<F[max_order-1]) AddSamples(int_at[0]-1, int_at[1]+1, int_at[2]  , max_order, new_at, F, delta, ID);
    if ( x2+mz2<F[max_order-1]) AddSamples(int_at[0]-1, int_at[1]  , int_at[2]+1, max_order, new_at, F, delta, ID);
    if ( y2+mz2<F[max_order-1]) AddSamples(int_at[0]  , int_at[1]-1, int_at[2]+1, max_order, new_at, F, delta, ID);
    if (mx2+ y2<F[max_order-1]) AddSamples(int_at[0]+1, int_at[1]-1, int_at[2]  , max_order, new_at, F, delta, ID);
    if (mx2+ z2<F[max_order-1]) AddSamples(int_at[0]+1, int_at[1]  , int_at[2]-1, max_order, new_at, F, delta, ID);
    if (my2+ z2<F[max_order-1]) AddSamples(int_at[0]  , int_at[1]+1, int_at[2]-1, max_order, new_at, F, delta, ID);

    // Final 8 "corner" cubes */
    if ( x2+ y2+ z2<F[max_order-1]) AddSamples(int_at[0]-1, int_at[1]-1, int_at[2]-1, max_order, new_at, F, delta, ID);
    if ( x2+ y2+mz2<F[max_order-1]) AddSamples(int_at[0]-1, int_at[1]-1, int_at[2]+1, max_order, new_at, F, delta, ID);
    if ( x2+my2+ z2<F[max_order-1]) AddSamples(int_at[0]-1, int_at[1]+1, int_at[2]-1, max_order, new_at, F, delta, ID);
    if ( x2+my2+mz2<F[max_order-1]) AddSamples(int_at[0]-1, int_at[1]+1, int_at[2]+1, max_order, new_at, F, delta, ID);
    if (mx2+ y2+ z2<F[max_order-1]) AddSamples(int_at[0]+1, int_at[1]-1, int_at[2]-1, max_order, new_at, F, delta, ID);
    if (mx2+ y2+mz2<F[max_order-1]) AddSamples(int_at[0]+1, int_at[1]-1, int_at[2]+1, max_order, new_at, F, delta, ID);
    if (mx2+my2+ z2<F[max_order-1]) AddSamples(int_at[0]+1, int_at[1]+1, int_at[2]-1, max_order, new_at, F, delta, ID);
    if (mx2+my2+mz2<F[max_order-1]) AddSamples(int_at[0]+1, int_at[1]+1, int_at[2]+1, max_order, new_at, F, delta, ID);

    // We're done! Convert everything to right size scale */
    for (i=0; i<max_order; i++)
    {
        F[i]=sqrt(F[i])*(1.0/DENSITY_ADJUSTMENT);
        delta[i][0]*=(1.0/DENSITY_ADJUSTMENT);
        delta[i][1]*=(1.0/DENSITY_ADJUSTMENT);
        delta[i][2]*=(1.0/DENSITY_ADJUSTMENT);
    }

    return;
}


#define CHURN(a) (1402024253*(a)+586950981)

// the function to merge-sort a "cube" of samples into the current best-found
// list of values. */
void TexCellular::AddSamples(long xi, long yi, long zi, long max_order, double at[3], double *F, double delta[4][3], unsigned long *ID)
{
    double dx, dy, dz, fx, fy, fz, d2;
    long count, i, j, index;
    unsigned int seed, this_id;

    // Each cube has a random number seed based on the cube's ID number.
    // The seed might be better if it were a nonlinear hash like Perlin uses
    // for noise but we do very well with this faster simple one.
    // Our LCG uses Knuth-approved constants for maximal periods. */
    seed=702395077*xi + 915488749*yi + 2120969693*zi;

    // How many feature points are in this cube? */
    count=Poisson_count[(unsigned char)(seed>>24)]; /* 256 element lookup table. Use MSB */

    seed=CHURN(seed);

    for (j=0; j<count; j++) /* test and insert each point into our solution */
    {
        this_id=seed;
        seed=CHURN(seed);

        // compute the 0..1 feature point location's XYZ
        fx=(seed+0.5)*(1.0/4294967296.0);
        seed=CHURN(seed);
        fy=(seed+0.5)*(1.0/4294967296.0);
        seed=CHURN(seed);
        fz=(seed+0.5)*(1.0/4294967296.0);
        seed=CHURN(seed);

        // delta from feature point to sample location
        dx=xi+fx-at[0];
        dy=yi+fy-at[1];
        dz=zi+fz-at[2];

        // Distance computation!  Lots of interesting variations are
        // possible here!
        // Biased "stretched"   A*dx*dx+B*dy*dy+C*dz*dz
        // Manhattan distance   fabs(dx)+fabs(dy)+fabs(dz)
        // Radial Manhattan:    A*fabs(dR)+B*fabs(dTheta)+C*dz
        // Superquadratic:      pow(fabs(dx), A) + pow(fabs(dy), B) + pow(fabs(dz),C)
        //
        // Go ahead and make your own! Remember that you must insure that
        // new distance function causes large deltas in 3D space to map into
        // large deltas in your distance function, so our 3D search can find
        // them! [Alternatively, change the search algorithm for your special
        // cases.]
        //d2=(dx*dx+dy*dy+dz*dz); // Euclidian distance, squared
        switch (mDistanceType)
        {
        case 0:
            d2=(dx*dx+dy*dy+dz*dz);
            break;
        case 1:
            d2=fabs(dx)+fabs(dy)+fabs(dz);
            break;
        }

        //d2=fabs(dx)+fabs(dy)+fabs(dz); // Euclidian distance, squared

        if (d2<F[max_order-1]) // Is this point close enough to rememember?
        {
            // Insert the information into the output arrays if it's close enough.
            // We use an insertion sort.  No need for a binary search to find
            // the appropriate index.. usually we're dealing with order 2,3,4 so
            // we can just go through the list. If you were computing order 50
            // (wow!!) you could get a speedup with a binary search in the sorted
            // F[] list.

            index=max_order;
            while (index>0 && d2<F[index-1]) index--;

            // We insert this new point into slot # <index> */

            // Bump down more distant information to make room for this new point.
            for (i=max_order-1; i-->index;)
            {
                F[i+1]=F[i];
                ID[i+1]=ID[i];
                delta[i+1][0]=delta[i][0];
                delta[i+1][1]=delta[i][1];
                delta[i+1][2]=delta[i][2];
            }
            // Insert the new point's information into the list.
            F[index]=d2;
            ID[index]=this_id;
            delta[index][0]=dx;
            delta[index][1]=dy;
            delta[index][2]=dz;
        }
    }
}
