/*
 *  cell.cpp //this is to tell the computer that each cell has an X and Y position, a Kai concentration, and some wasps; there exist grdients of Kai and their average is calculated numerically.
 *
 *
 *  Created by Andrew Burgess on 16/01/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "cell.h"
#include "larva.h"
#include "wasp.h"
#include <iostream>
using namespace std;

cell::cell(int G, int Y, int X)
{
    m_Gpos=G; //this m_Gpos will be passed on to int G
    m_Ypos=Y;
    m_Xpos=X;
}

cell::~cell()
{
    m_Wasps.clear();
    return;
}

void cell::clearPops()
{
	m_Wasps.clear(); //"clear()" removes all elements from vector (destroyed!) leaving the container with a size zero
    return;
}

void cell::addWasp(wasp * w)
{
    w->setCell(this);
    m_Wasps.push_back(w);
}


wasp * cell::getWasp(int i) 
{
	return m_Wasps[i];
}

int cell::getWaspIndex(wasp * Wasp)
{
	int temp = 0;
	for(int i = 0; i < m_Wasps.size(); i++) //loop through every m_Wasp vector entry until reached size of m_Wasps.
	{
		if(m_Wasps[i] == Wasp)
		{
			temp = i;
		}
	}
	return temp;
}		

int cell::getNumLarvae()
{
	return m_Wasps.size();
}


double cell::getTotalKprod() //28/6/7: I think this function finds out the total K (MDE) production at a grip point the following way: starting with i=0 Kai production, it loops through the m_Wasps vector, i.e. through each wasp (cc), adding on its K (MDE) production (NB this could be different depending on the wasp's (cc's) type, accessed by pointer. Once we've done that for every wasp in grip point, we have the total K(MDE) production at that grid point. This is fed into the Kairomone method in grid.cpp into fn called processKairomone.
{
	int i = 0;
	for(int j = 0; j < m_Wasps.size(); j++)
	{
		i = i + m_Wasps[j]->getKProd();
	}
	return i;
}





