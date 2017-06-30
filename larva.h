//
// File:   larva.h
// Author: pieta
//
// Created on 28 January 2008, 13:46
//

#ifndef _LARVA_H
#define	_LARVA_H
#include <vector>
#include "cell.h"
#include "wTraits.h"
using namespace std;
// define the trait value positions in trait vector
#define LMAXTRAITS 2
#define DIFCOEF 0
#define KPROD 1

// define the memory value positions in the memory vector
#define LMAXMEM 9
#define PSTAY 0
#define PWEST 1
#define PEAST 2
#define PNORTH 3
#define PSOUTH 4
#define REST 5
#define METAREST 6
#define LIFESPAN 7
#define CLOCK 8
class larva{
    
private:
    cell * loc;
    int m_newX;
    int m_newY;
	bool m_status;
	int m_eggpresent; //was a bool
	int m_clock;
	
	////Traits
	int m_lifeSpan;
	int m_fecundity;
	int m_maxEggLoad;
	double m_probLarvaDeathAferLifespan;
/*
//        vector <wTraits> eggTraits;
 // wTraits eggTraits[2];
//// Wasp behaviour is partly governed by traits stored in a vector
    vector <double> m_Traits;
//// Wasp behaviour is partly governed by memories stored in a vector
    vector <double> m_Memories;
	vector <double> m_eggtraits;
*/
public:
    larva();
    int getX() {return loc->getX();}
    int getY() {return loc->getY();}
    int getNewX(){return m_newX;}
    int getNewY(){return m_newY;}
    void setCell(cell * val) {loc=val;}
	cell* getCell(){return loc;}
  //  double getTrait(int i){return m_Traits[i];}
  //  double setTrait(int i){return m_Memories[i];}
  //  void setTrait(int i, double val){m_Traits[i]=val;}
  //  void setMemory(int i, double val){m_Memories[i]=val;}
//	double getMemory(int i){return m_Memories[i];}
    void processStep(int n);
	bool isAlive(){return m_status;}
	int parasitised(){return m_eggpresent;}
	void incrementEggsPresent(){ m_eggpresent =  m_eggpresent + 1;}
	int getNumEggsPresent(){return m_eggpresent;}
	void KillLarvae() {m_status = false;}
	int getLifeSpan(){return m_lifeSpan;}
	int getClock(){return m_clock;}
	void setLifeSpan(int val){m_lifeSpan = val;}
	void setClock(int val){m_clock = val;}
//	void ReproduceLarvae();
	void setNewX(int val) {m_newX = val;}
	void setNewY(int val) {m_newY = val;}
	int getFecundity(){return m_fecundity;}
	void setFecundity(int val){m_fecundity = val;}

	int getMaxEggLoad(){return m_maxEggLoad;}
	void setMaxEggLoad(int val){m_maxEggLoad = val;}
	void parasitise(wTraits wtraits);
//	wTraits getEggTraits(int n){return eggTraits[n];}
	// wTraits getEggTraits(){return eggTraits;}

	vector <int> incubatingWaspKResponse;
	vector <int> incubatingWaspKResponseSuper;
	vector <int> incubatingWaspGregarity;	
	int getIncubatingWaspGregarity(int i);
	double getprobLarvaDeathAferLifespan(){return m_probLarvaDeathAferLifespan;}
	void setprobLarvaDeathAferLifespan(double val){m_probLarvaDeathAferLifespan =val;}
		
	
};

#endif	/* _LARVA_H */

