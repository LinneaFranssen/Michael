//
// File:   cell.h
// Author: pieta
//
// Created on 28 January 2008, 13:46
//

#ifndef _CELL_H
#define	_CELL_H

#include <vector>
using namespace std;
class wasp;
class larva; //not using at,. maybe if we introduce a second species that is Very different
class cell
{
private:
    int m_Gpos;   // Grid number //"m_" stands for member variables of a class: EVERY cell needs to have these member variables defined or it won't work.// cnnot ask for them as in private
    int m_Xpos;   // X coordinate
    int m_Ypos;   // Y coridinate

    vector < wasp* > m_Wasps; //can access individual wasp-pointers e.g. by m_Wasp[0], ...[1] etc. //the datat type is pointer (could also put in a built-in or a user-defined class, like wasp)
//	vector < wasp* > m_cooperators; //vectors can vary in size accoring to need; arrays are fixed. but use up more storage

	int m_maxNoWasps; //this is the max no of wasps that can be in m_Wasps
	int m_numGrids;
	
	///////// Kairomone variables ///////////
	double m_Kairomone;
    double m_KgradientNS;
    double m_KgradientEW;
	double m_KLaplacian;
	double m_waspDiff;			
//	double m_chi;
	
public:
    cell(); //these two are constructors: same name as class name and no void etc. in front, i.e. not return type. Constructors will be called as soon as you call an object automatically // can have more than one constructor as long as the number of arguments or their types are different; always need one constructor!
    cell(int G, int Y, int X); //this constructor takes 3 integers calld G, Y, X.
    ~cell();
    int getX() {return m_Xpos;}
    int getY() {return m_Ypos;}
    void Setx(int X) {m_Xpos = X;}
    void Sety(int Y) {m_Ypos = Y;}
    void clearPops();
///////////  wasp Methods  /////////////////
    void addWasp(wasp * Wasp);
    void clearWasps(){m_Wasps.clear();}
    int getNumWasps(){return m_Wasps.size();}
    wasp * getWasp(int i) ;  
	void  setmaxNoWasps (int val) {m_maxNoWasps = val;}  //val can be used elsewhere too because it is only stored/valid while in this fn
    int getmaxNoWasps(){return m_maxNoWasps;}	  
	void removeWasp(int i){m_Wasps.erase(m_Wasps.begin() + i);}	
		    
	int getWaspIndex(wasp * Wasp); // needs a wasp pointer as input, that is called Wasp

	void  setGpos (int val) {m_Gpos = val;}
    int getGpos(){return m_Gpos;}

	void setNumGrids(int val) {m_numGrids = val;}
	int getNumGrids() {return m_numGrids;}
	
	int  getNumLarvae() ; //returns the number of cooperators in the cell
	//////////   kairomone methods  /////////////////
    double getKairomone(){return m_Kairomone;}
    void setKairomone(double K) {m_Kairomone = K;}
    double getKgradientNS(){return m_KgradientNS;}
    double getKgradientEW(){return m_KgradientEW;}
    void setKgradientNS(double val){m_KgradientNS=val;}
    void setKgradientEW(double val){m_KgradientEW=val;}
	void setKLaplacian(double val){m_KLaplacian=val;}
    double getKLaplacian(){return m_KLaplacian;}
	void setwaspDiff(double val){m_waspDiff=val;}
    double getwaspDiffX(){return m_waspDiff;}
	
	double getTotalKprod();
	

};

#endif	/* _CELL_H */


