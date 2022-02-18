/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __TWISTER_H__
#define __TWISTER_H__


#define mtRand_N 624

class Twister
{
	public:
		void Seed( unsigned long seed );	
		Twister( unsigned long seed )
		{
			if (seed) { Seed(seed); } 
			else { Seed( (unsigned long)0xf2710812 ); }
		}
		Twister() { Seed( (unsigned long)0xf2710812 ); }
		double Rand();
		unsigned long RandL();


	protected:
		 unsigned long mt[mtRand_N];
		 int mti;
};



#endif
