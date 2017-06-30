/*
 *  larva.cpp
 *  
 *
 *  Created by Andrew Burgess on 16/01/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "larva.h"
#include <iostream>

larva::larva()
{
	m_status=true;
	m_eggpresent=0;
	
	return;
}

void larva::processStep(int n)
{
// HERE IS WHERE LARVA MOVEMENT IS HANDLED ALSO KAIROMONE PRODUCTION GOES HERE
    
	if(getClock() == 0) //here we kill old and overparasitized hosts.
	{
		double Rand = (float)rand()/(float)RAND_MAX;
		if(Rand <= m_probLarvaDeathAferLifespan)
		{
			KillLarvae();
		}
		else
		{
			m_clock =  1;
		}
	}
	if (m_eggpresent > m_maxEggLoad)
	{
			KillLarvae();
	}
	
	if(getClock() > 0)
	{
		if(loc->getNumLarvae()>loc->getmaxNoLarvae() )     /////////HERE IS SET THE MAXIMUM LARVA POPULATION IN A CELL !!!!!!!!!!!!
		{
			KillLarvae();
		}
		else if(m_eggpresent > m_maxEggLoad)
		{
			KillLarvae();			///////////HERE WE KILL ALL LARVA THAT ARE OVER PARASITIZED
		}
		else
		{
		}
		setClock(getClock() - 1);
		m_newX = loc->getX();
		m_newY = loc->getY();
	}
	else if(getClock() < 0)
	{
		std::cout << "Larva survived death!" << "\n";
	} 
	return; 
}

int larva::getIncubatingWaspGregarity(int i)
{
	if (i < incubatingWaspGregarity.size())
	{
		return incubatingWaspGregarity[i];
	}
	else 
	{
		std::cout << "error in lava.h, getIncubatingWaspgregarity" << "\n";
		std::cout << "i = " << i << "  & incubatingWaspGregarity.size() = " << incubatingWaspGregarity.size() << "\n";
		return 0;
	}
}
/*
void larva::parasitise(wTraits wtraits)
{
    if(m_eggpresent < m_maxEggLoad)
	{
		eggTraits[m_eggpresent]=wtraits;
		m_eggpresent =  m_eggpresent + 1;
    }
}
*/
/*
void larva::ReproduceLarvae()
{
		for(int i = 0; i < m_fecundity; i++)
		{
			
			double RAND = (float)random()/((float)RAND_MAX);
			int XRand = RAND * 100;
			int YRand = RAND * 100;
			larva* l = new larva;
			l->setNewX(XRand);
			l->setNewY(YRand);
			loc->addLarva(l);
			
		}



}
*/