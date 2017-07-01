//
// File:   grid.h
// Author: pieta
//
// Created on 28 January 2008, 13:46
//

#ifndef _GRID_H
#define    _GRID_H
#define      GRIDSIZE 100

#include "cell.h"
#include "wasp.h"
//#include "larva.h"
#include <vector>

class grid {
private:
    int m_gridsize;
    cell *cells[2][GRIDSIZE][GRIDSIZE]; ///two copies of grid: cause changes depend on what happens in surrounding, when t increases, one is deleted and one updated // this is a an array (3d matrix)// 'loc' lives in here
    int m_maxNoWasps;
    int m_GPOS;  ///redundant? from more than one grid?
    double XCoord;
    double YCoord;

    /////////// Kairomone variables /////////
    double m_Kdiff;
    double m_Kdecay;
    double m_Kprod; ///28/6/17: I suspect this tells us how much Kai each wasp produces per time step (i.e. how much MDE each CC produces)
    double m_WaspDiff;
    double m_chi;
    double m_TotalK;


public:
    grid();

    ~grid();

    void init(int gid);

    void addWaspRandom(wasp *w, int when); ///one-line functions can go in here!
    void addWasp(wasp *w, int when, int X, int Y);

    void addLarvaRandom(larva *l, int when);

    void addWaspXY(wasp *w, int when);

    void addLarvaXY(larva *l, int when);

    void destroy();

    int getGridsize() { return m_gridsize; }

    void setGridsize(int val) { m_gridsize = val; }

    void writeGrid(int it, int gen, int when);

    void clearPops(int when);

    void setmaxNoWasps(int val) { m_maxNoWasps = val; }

    int getGPOS(int val) { return m_GPOS; }

    void setXCoord(double val) { XCoord = val; }

    double getXCoord() { return XCoord; }

    void setYCoord(double val) { YCoord = val; }

    double getYCoord() { return YCoord; }

    cell *getCell(int when, int Xcoord,
                  int Ycoord) { return cells[when][Xcoord][Ycoord]; }  // added by me, might be wrong !!!!!!

    ///// Kairomone methods ////
    void processKairomone(int when);

    void setKdecay(double val) { m_Kdecay = val; }

    void setKdiff(double val) { m_Kdiff = val; }

    void setKprod(double val) { m_Kprod = val; }

    void setTotalK(int val) { m_TotalK = val; }

    double getTotalK() { return m_TotalK; }

    void setWaspDiff(double val) { m_WaspDiff = val; }


};


#endif    /* _GRID_H */

