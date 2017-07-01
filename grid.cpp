/*
 *  grid.cpp
 *
 *
 *  Created by Andrew Burgess on 17/01/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>

#include "grid.h"
#include "main.h"
#include <math.h>///what fllows is the maths library
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "cell.h"

grid::grid() {} // :: means belongs to class

void grid::init(int gid) ///initiate the grid: when I call grid in main, program will need to call this function and it will always define the gridsize etc.
{
    m_gridsize = GRIDSIZE; //currently 1
    m_GPOS = gid;
    for(int pwk=0; pwk<2;pwk++) //produce 2 of the same grids
    {
        for(int ywk=0;ywk<GRIDSIZE;ywk++)
        {
            for(int xwk=0;xwk<GRIDSIZE;xwk++)// we're allocating memory for the new cell objects called 'tempcell' and are thus creating 100x100 cells
            {
                cell * tmpcell = new cell(gid, ywk, xwk); // allocates memory for cell object // go to cell to follow through program: should find function 'cell' with input (gid, ywk, xwk) and output pointer to cell // 1 cell, written into grid pos, call 'tempcell' and put into array cells and then write some values to in (member variables)
                cells[pwk][ywk][xwk]=tmpcell;             // put the address of that memory in array// array of pointers to all cells //
                cells[pwk][ywk][xwk]->setmaxNoWasps(m_maxNoWasps); // [A] there exists a function in cell. When I put m_maxNoWasps in grid, cells will inherit it (?)
                cells[pwk][ywk][xwk]->setGpos(gid); // [B]
                cells[pwk][ywk][xwk]->setwaspDiff(m_WaspDiff); // [C] cells inherit [pwk][ywk][xwk] from grid
            }
        }
    }
}

void grid::clearPops(int when) // fn. makes blank grid e.g. pGrid-> clearpops(2) means clear 2nd grid
{
    m_gridsize = GRIDSIZE;
    for(int ywk=0;ywk<GRIDSIZE;ywk++)
    {
        for(int xwk=0;xwk<GRIDSIZE;xwk++)
        {
            cells[when][ywk][xwk]->clearPops();    // [A-C] stay but things like pop number etc (i.e. changeable things) clear
        }
    }
}

void grid::addWasp(wasp *w, int when, int X, int Y) // when is 1 means now , 2 next // int when, int X, int Y determines what cell we add the wasp to
{
    cells[when][Y][X]->addWasp(w); //wasp added to specific cell
    w->setCell(cells[when][Y][X]);  // tells wasp which cell it's in (does it? or wouldn't we rather need to write a fn w_getxpos in wasp?

}


void grid::addWaspRandom(wasp * w, int when) //this populates the grid randomly with wasps (currently used for init. setup only
{
    // Pick a random x and y and put wasp in that cell
    double RAND1 = (float)rand()/((float)RAND_MAX);
    double RAND2 = (float)rand()/((float)RAND_MAX);
    int x;
    int y;
    if (RAND1 == 1)
    {
        x = GRIDSIZE -1;
    }
    else
    {
        x = (int)(GRIDSIZE * RAND1);
    }

    if (RAND2 == 1)
    {
        y = GRIDSIZE - 1;
    }
    else
    {
        y = (int)(GRIDSIZE * RAND2);
    }
    if( x < 0 || x > GRIDSIZE -1 || y < 0 || y > GRIDSIZE - 1)
    {
        std::cout << " cordinates off grid " << "\n"; //error message to ensure all wasps have coordinates between 0 and 100  (as grid is 0 to 100)
    }
    cells[when][y][x]->addWasp(w); // once random wasp in being generated , add wasp at rand pos (x,y) but specified 'when' value // would need loop in main to make more
    w->setCell(cells[when][y][x]);
}

void grid::addWaspXY(wasp * w, int now) //the grid is called 'now' //wasps store the next pos (x,y) they are going to. Only wasps know their positions. So need to ask those wasps first by "[w->getNewY()][w->getNewX()]" below where they are going to and THEN generate a new vector addWasp(w) by this fn.
{
    cells[now][w->getNewY()][w->getNewX()]->addWasp(w);    //[i have changed getx to getnewx !!!! OLD COMMENT] //'cells' here is always one specific cell: we ask wasp what new (x,y)-position it has and then add it to that cell
    w->setCell(cells[now][w->getNewY()][w->getNewX()]); //we tell wasp its new cell from above
}

void grid::writeGrid(int it, int gen, int now)
{
    //////// Doves ////////////// this makes pic of grid numbers of agents on grid; output files are MADE but not FILLED here
    ofstream wout;
    string wfilename;
    ostringstream wfilestring;
    wfilestring << "D_" << gen << "_" << it << ".txt" << ends;
    wfilename=wfilestring.str();
    wout.open(wfilename.data());
    wout.setf(ios::fixed);

    ////// HAWKS
    ofstream lout;
    string lfilename;
    ostringstream lfilestring;
    lfilestring << "H_" << gen << "_" << it << ".txt" << ends;
    lfilename=lfilestring.str();
    lout.open(lfilename.data());
    lout.setf(ios::fixed);

    ////// KAIROMONE /////// pic of Kai distribution if you want it
    /*
    ofstream kout;
    string kfilename;
    ostringstream kfilestring;
    kfilestring << "K_" << it << ".txt" << ends;
    kfilename=kfilestring.str();
    kout.open(kfilename.data());
    kout.setf(ios::fixed);
    */

    int ll = 0;
    for(int ywk=0;ywk<GRIDSIZE;ywk++)
    {
        for(int xwk=0;xwk<GRIDSIZE;xwk++)
        {
            //wout << cells[now][ywk][xwk]->getNumWasps() << " ";
            double h = 0;
            double d = 0;
            double K = 0;
            for(int i = 0; i < cells[now][ywk][xwk]->getNumWasps(); i++)
            {
                /*
                double PROB = cells[now][ywk][xwk]->getWasp(i)->getProbDoveGivenHawk();
                if(PROB >= 0.5)
                {
                    j = j + 1;
                    ll = ll + 1;
                }
                else
                {
                    k = k + 1;
                }
                */
                if(cells[now][ywk][xwk]->getWasp(i)->getName() == 0)
                {
                    h = h + 1;
                    ll = ll + 1;
                }
                else
                {
                    d = d + 1;
                }

                K = cells[now][ywk][xwk]->getKairomone();
            }
            wout << d << " ";
            lout << h << " ";
            //	kout << K << " ";

        }
        wout << endl;
        lout << endl;
//		kout << endl;
    }
    wout.close();
    lout.close();
//	kout.close();
    std::cout << " Hawks  " << ll << "\n";
}


///////////////////// KAIROMONE METHOD /////////////////////////////////////////


void grid::processKairomone(int now) //2 grids: 1 needed new Kai
{
// USE THIS ROUTINE TO DIFFUSE AND DECAY THE KAIROMONE AND CALCULATE THE KAIROMONE GRADIENTS IN EACH CELL
// I HAVE INCLUDED THE DIFFUSION EQUATION STUFF BELOW THAT IS A ROUGH ATTEMPT TO AMMEND YOU ORIGINAL CODE
//
/// Calculates K for t + 1 using K at t.  Then calculates K gradients for t using K values at t necessary for movemement at time t (used in wasp process step).

    double kt_x =  DTDX2*m_Kdiff; ///check if this should not be DT/DX2 as elsewhere -> no as DTDX2 is defined properly elsewhere!:)
    for (int i = 0; i  < m_gridsize; i++) // i is in y direction, j in x !!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        for (int j = 0; j < m_gridsize; j++)
        {
            if (i != 0 && i != m_gridsize -1 && j != 0 && j != m_gridsize -1)					// ie if not at grid edge
            {
                cells[1-now][i][j]->setKairomone(cells[now][i][j]->getKairomone() + kt_x*    ///[1-now] is the next grid: So we write the new Kairo into the next grid and then update
                                                                                    (cells[now][i][j+1]->getKairomone() + cells[now][i][j-1]->getKairomone() +
                                                                                     cells[now][i+1][j]->getKairomone() + cells[now][i-1][j]->getKairomone() -
                                                                                     4*cells[now][i][j]->getKairomone()) + DT*( - m_Kdecay*cells[now][i][j]->getKairomone()   + cells[now][i][j]->getTotalKprod() )  ); //What are the last two things? They are Kairomone decay and production! :) Kairomone production depends on the number of
            }
            else if(i == 0 && j != m_gridsize -1 && j != 0) //BCs
            {
                cells[1-now][i][j]->setKairomone(cells[now][i][j]->getKairomone() + kt_x*
                                                                                    (cells[now][i][j+1]->getKairomone() + cells[now][i][j-1]->getKairomone() +
                                                                                     cells[now][i+1][j]->getKairomone() + cells[now][i][j]->getKairomone() -
                                                                                     4* cells[now][i][j]->getKairomone() )  + DT*(- m_Kdecay*cells[now][i][j]->getKairomone()+ cells[now][i][j]->getTotalKprod() )  );
            }
            else if (i == m_gridsize - 1 && j != m_gridsize -1 && j != 0)
            {
                cells[1-now][i][j]->setKairomone(cells[now][i][j]->getKairomone() + kt_x*
                                                                                    (cells[now][i][j+1]->getKairomone() + cells[now][i][j-1]->getKairomone() +
                                                                                     cells[now][i][j]->getKairomone() + cells[now][i-1][j]->getKairomone() -
                                                                                     4* cells[now][i][j]->getKairomone())  + DT*( - m_Kdecay*cells[now][i][j]->getKairomone()+ cells[now][i][j]->getTotalKprod() ) );
            }
            else if (i != 0 && i != m_gridsize -1 && j == 0)
            {
                cells[1-now][i][j]->setKairomone(cells[now][i][j]->getKairomone() + kt_x*
                                                                                    (cells[now][i][j+1]->getKairomone() + cells[now][i][j]->getKairomone() +
                                                                                     cells[now][i+1][j]->getKairomone() + cells[now][i-1][j]->getKairomone() -
                                                                                     4* cells[now][i][j]->getKairomone())  + DT*( - m_Kdecay*cells[now][i][j]->getKairomone()+ m_Kprod*cells[now][i][j]->getNumLarvae()));
            }
            else if (i != 0 && i != m_gridsize -1 && j == m_gridsize -1)
            {
                cells[1-now][i][j]->setKairomone(cells[now][i][j]->getKairomone() + kt_x*
                                                                                    (cells[now][i][j]->getKairomone() + cells[now][i][j-1]->getKairomone() +
                                                                                     cells[now][i+1][j]->getKairomone() + cells[now][i-1][j]->getKairomone() -
                                                                                     4* cells[now][i][j]->getKairomone())  + DT*( - m_Kdecay*cells[now][i][j]->getKairomone()+ cells[now][i][j]->getTotalKprod()));
            }
            else if (i ==0 && j == 0)
            {
                cells[1-now][i][j]->setKairomone(cells[now][i][j]->getKairomone() + kt_x*
                                                                                    (cells[now][i][j+1]->getKairomone() + cells[now][i][j]->getKairomone() +
                                                                                     cells[now][i+1][j]->getKairomone() + cells[now][i][j]->getKairomone() -
                                                                                     4* cells[now][i][j]->getKairomone())  + DT*( - m_Kdecay*cells[now][i][j]->getKairomone()+ cells[now][i][j]->getTotalKprod()));
            }
            else if (i == m_gridsize - 1 && j == 0)
            {
                cells[1-now][i][j]->setKairomone(cells[now][i][j]->getKairomone() + kt_x*
                                                                                    (cells[now][i][j+1]->getKairomone() + cells[now][i][j]->getKairomone() +
                                                                                     cells[now][i][j]->getKairomone() + cells[now][i-1][j]->getKairomone() -
                                                                                     4* cells[now][i][j]->getKairomone())  + DT*( - m_Kdecay*cells[now][i][j]->getKairomone()+ cells[now][i][j]->getTotalKprod()));
            }
            else if (i == 0 && j == m_gridsize - 1)
            {
                cells[1-now][i][j]->setKairomone(cells[now][i][j]->getKairomone() + kt_x*
                                                                                    (cells[now][i][j]->getKairomone() + cells[now][i][j-1]->getKairomone() +
                                                                                     cells[now][i+1][j]->getKairomone() + cells[now][i][j]->getKairomone() -
                                                                                     4* cells[now][i][j]->getKairomone())  + DT*(- m_Kdecay*cells[now][i][j]->getKairomone()+ cells[now][i][j]->getTotalKprod()));
            }
            else if(i == m_gridsize - 1 && j == m_gridsize - 1)
            {
                cells[1-now][i][j]->setKairomone(cells[now][i][j]->getKairomone() + kt_x*
                                                                                    (cells[now][i][j]->getKairomone() + cells[now][i][j-1]->getKairomone() +
                                                                                     cells[now][i][j]->getKairomone() + cells[now][i-1][j]->getKairomone() -
                                                                                     4* cells[now][i][j]->getKairomone())  + DT*( - m_Kdecay*cells[now][i][j]->getKairomone()+ cells[now][i][j]->getTotalKprod()));
            }
            else
            {
                cout << "ERROR in grid::calcKairomone";
            }
            if(cells[1-now][i][j]->getKairomone() < 0)
            {
                std::cout << " Negative K in discretized pde at cell  " << i << "   " << j <<  "K = " << cells[1-now][i][j]->getKairomone()  << " \n";
            }
        }
    }

    //now = 1-now;
//////////Here we calculate Kairomone concentrations and write them to the cells. We take central differences where possible.


    double TotalKtemp = 0;
//	int NOW = 1-now;
    for (int x = 0; x  < m_gridsize; x++)
    {
        for (int y = 0; y < m_gridsize; y++)
        {
            TotalKtemp = TotalKtemp + cells[1-now][y][x]->getKairomone();  //calculates total kairomone on grid.
            if (cells[now][y][x]->getNumWasps() > 0)
            {
                if (x != 0 && x != m_gridsize -1 && y != 0 && y != m_gridsize -1)
                {
                    cells[now][y][x]->setKgradientEW( (cells[now][y][x+1]->getKairomone()   -   cells[now][y][x-1]->getKairomone())/(2*DX)   );
                    cells[now][y][x]->setKgradientNS( (cells[now][y+1][x]->getKairomone()   -   cells[now][y-1][x]->getKairomone())/(2*DX)   );

                    cells[now][y][x]->setKLaplacian( IDX2 *  (cells[now][y][x+1]->getKairomone()   +   cells[now][y][x-1]->getKairomone() + cells[now][y+1][x]->getKairomone()   +   cells[now][y-1][x]->getKairomone() - 4*cells[now][y][x]->getKairomone()) );
                }
                if(x == 0 && y != m_gridsize -1 && y != 0)
                {
                    cells[now][y][x]->setKgradientEW( (cells[now][y][x+1]->getKairomone()   -   cells[now][y][x]->getKairomone())/DX   );
                    cells[now][y][x]->setKgradientNS( (cells[now][y+1][x]->getKairomone()   -   cells[now][y-1][x]->getKairomone())/(2*DX)   );

                    cells[now][y][x]->setKLaplacian( IDX2 *  (cells[now][y][x+1]->getKairomone()   +   cells[now][y][x]->getKairomone() + cells[now][y+1][x]->getKairomone()   +   cells[now][y-1][x]->getKairomone() - 4*cells[now][y][x]->getKairomone()) );
                }
                if (x == m_gridsize - 1 && y != m_gridsize -1 && y != 0)
                {
                    cells[now][y][x]->setKgradientEW( (cells[now][y][x]->getKairomone()   -   cells[now][y][x-1]->getKairomone())/DX   );
                    cells[now][y][x]->setKgradientNS( (cells[now][y+1][x]->getKairomone()   -   cells[now][y-1][x]->getKairomone())/(2*DX)   );

                    cells[now][y][x]->setKLaplacian( IDX2 *  (cells[now][y][x]->getKairomone()   +   cells[now][y][x-1]->getKairomone() + cells[now][y+1][x]->getKairomone()   +   cells[now][y-1][x]->getKairomone() - 4*cells[now][y][x]->getKairomone()) );
                }
                if (x != 0 && x != m_gridsize -1 && y == 0)
                {
                    cells[now][y][x]->setKgradientEW( (cells[now][y][x+1]->getKairomone()   -   cells[now][y][x-1]->getKairomone())/(2*DX)   );
                    cells[now][y][x]->setKgradientNS( (cells[now][y+1][x]->getKairomone()   -   cells[now][y][x]->getKairomone())/DX   );

                    cells[now][y][x]->setKLaplacian( IDX2 *  (cells[now][y][x+1]->getKairomone()   +   cells[now][y][x-1]->getKairomone() + cells[now][y+1][x]->getKairomone()   +   cells[now][y][x]->getKairomone() - 4*cells[now][y][x]->getKairomone()) );
                }
                if (x != 0 && x != m_gridsize -1 && y == m_gridsize -1)
                {
                    cells[now][y][x]->setKgradientEW( (cells[now][y][x+1]->getKairomone()   -   cells[now][y][x-1]->getKairomone())/(2*DX)   );
                    cells[now][y][x]->setKgradientNS( (cells[now][y][x]->getKairomone()   -   cells[now][y-1][x]->getKairomone())/DX   );

                    cells[now][y][x]->setKLaplacian( IDX2 *  (cells[now][y][x+1]->getKairomone()   +   cells[now][y][x-1]->getKairomone() + cells[now][y][x]->getKairomone()   +   cells[now][y-1][x]->getKairomone() - 4*cells[now][y][x]->getKairomone()) );
                }
                if (x ==0 && y == 0)
                {
                    cells[now][y][x]->setKgradientEW( (cells[now][y][x+1]->getKairomone()   -   cells[now][y][x]->getKairomone())/DX   );
                    cells[now][y][x]->setKgradientNS( (cells[now][y+1][x]->getKairomone()   -   cells[now][y][x]->getKairomone())/DX   );

                    cells[now][y][x]->setKLaplacian( IDX2 *  (cells[now][y][x+1]->getKairomone()   +   cells[now][y][x]->getKairomone() + cells[now][y+1][x]->getKairomone()   +   cells[now][y][x]->getKairomone() - 4*cells[now][y][x]->getKairomone()) );
                }
                if (x == m_gridsize - 1 && y == 0)
                {
                    cells[now][y][x]->setKgradientEW( (cells[now][y][x]->getKairomone()   -   cells[now][y][x-1]->getKairomone())/DX   );
                    cells[now][y][x]->setKgradientNS( (cells[now][y+1][x]->getKairomone()   -   cells[now][y][x]->getKairomone())/DX   );

                    cells[now][y][x]->setKLaplacian( IDX2 *  (cells[now][y][x]->getKairomone()   +   cells[now][y][x-1]->getKairomone() + cells[now][y+1][x]->getKairomone()   +   cells[now][y][x]->getKairomone() - 4*cells[now][y][x]->getKairomone()) );
                }
                if (x == 0 && y == m_gridsize - 1)
                {
                    cells[now][y][x]->setKgradientEW( (cells[now][y][x+1]->getKairomone()   -   cells[now][y][x]->getKairomone())/DX   );
                    cells[now][y][x]->setKgradientNS( (cells[now][y][x]->getKairomone()   -   cells[now][y-1][x]->getKairomone())/DX   );

                    cells[now][y][x]->setKLaplacian( IDX2 *  (cells[now][y][x+1]->getKairomone()   +   cells[now][y][x]->getKairomone() + cells[now][y][x]->getKairomone()   +   cells[now][y-1][x]->getKairomone() - 4*cells[now][y][x]->getKairomone()) );
                }
                if(x == m_gridsize - 1 && y == m_gridsize - 1)
                {
                    cells[now][y][x]->setKgradientEW( (cells[now][y][x]->getKairomone()   -   cells[now][y][x-1]->getKairomone())/DX   );
                    cells[now][y][x]->setKgradientNS( (cells[now][y][x]->getKairomone()   -   cells[now][y-1][x]->getKairomone())/DX   );

                    cells[now][y][x]->setKLaplacian( IDX2 *  (cells[now][y][x]->getKairomone()   +   cells[now][y][x-1]->getKairomone() + cells[now][y][x]->getKairomone()   +   cells[now][y-1][x]->getKairomone() - 4*cells[now][y][x]->getKairomone()) );
                }
            }
        }
    }
    m_TotalK = TotalKtemp; ///K all added up from different wasps (NB every wasp prod. Kai)

}


/////////////////////////////////////////////////////////////////////////////// frees up memoroy at end of simulation: deletes everything!

void grid::destroy()
{
    for(int pwk=0;pwk<2;pwk++)
    {
        for(int ywk = 0; ywk < m_gridsize; ywk++)
        {
            for(int xwk = 0; xwk < m_gridsize; xwk++)
            {
                delete cells[pwk][ywk][xwk];
            }
        }
    }
}
