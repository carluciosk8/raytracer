/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __CELLTEXTURE_H__
#define __CELLTEXTURE_H__

#define MAX_TABLE 6

#include <vector>

#include "Texture.h"
#include "Command.h"

class TexCellular : public Texture
{
public:
    TexCellular(){}
    TexCellular(double c0, double c1, double c2, double c3,
         double e0, double e1, double e2, double e3,
         int octaves,
         Color Col1, Color Col2,
         std::vector<Color>& colst );
    virtual ~TexCellular() {}
    virtual Color getColor(vector3 pnt, Primitive* prim);


private:
    double C0,C1,C2,C3;
    double E0,E1,E2,E3;
    int oct;
    int mDistanceType;
    Color col1, col2;
    Color col[MAX_TABLE];

    double maxf, minf;
    static int Poisson_count[256];

    double f(vector3 p);
    //double ff(vector3 p);
    //double fff(vector3 p);

    void AddSamples(long xi, long yi, long zi, long max_order, double at[3], double *F, double delta[4][3], unsigned long *ID);
    void Worley(double at[3], long max_order, double* F, double delta[4][3], unsigned long* ID);

};

DECLARE_COMMAND(TexCellular);

#endif
