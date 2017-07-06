/*
 *  wasp.cpp  WHAT THIS MAINLY DOES IS DECREASE CLOCK BY 1 AND DIFFUSE IT TO GET NEW X AND Y POS
 *
 *
 *  Created by Andrew Burgess on 16/01/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>
#include "wasp.h"
#include "grid.h"
#include "main.h"
// #include <stdlib.h>
// #include <math.h>




wasp::wasp() ///variables that say the same could have gone in constructor but those that mutate cannot //THIS CALLS 'WASP' ; it is for a default wasp; to make wasp offspring, we'd need to put x,y coordinates and then tell cell to make wasp //this is making the wasp in main cpp
{
    m_status=true; //default: o; which means wasp is alive
    setRest(0);// this may not have been used?
    setPlayingStatus(0);
    setPlayed(0); //means free to play if zero
    setFitness(0);
//	setOpponentsLastMove(0);
    return;
}

int wasp::getX(){return loc->getX();} //"::" means fn belongs to wasp class

int wasp::getY() {return loc->getY();}

int wasp::getNewX(){return m_newX;}

int wasp::getNewY(){return m_newY;}

void wasp::setCell(cell * val) //telling wasp cell to go to 'loc'
{
    loc = val;
}

void wasp::processStep(int) //going through one timestep
{
//	m_NewGpos = loc->getGpos();

    if(m_clock == 0 ) //wasp is alive but will be killed...
    {
       // double RAND = (float)rand()/(float)RAND_MAX;
        //if(RAND <= m_probWaspDeathAferLifespan) //this means death will not necessarily happen after 100 timesteps but could be at 101 etc. as there is a P of m_probWaspDeathAferLifespan that a wasp will survive after 100 timesteps
        //{
            KillWasp(); //...here!
       // }
       // else
       // {
       //   m_clock = 1; //
       // }

       // m_newX = loc->getX(); //wasp's new X is asking its own cell "what is my X?" and is returning it.
       // m_newY = loc->getY();
    }
    if(m_clock > 0)
    {


        double kGradNS=loc->getKgradientNS();
        double kGradEW=loc->getKgradientEW();

        if(m_kairomoneResponse == 3)
        {
            ////// Random Walk Movement ////////////////////
            double RAND1 = (float)rand()/((float)RAND_MAX);
            //	double RAND2 = (float)rand()/((float)RAND_MAX);
            if(RAND1 < 0.25) //Move west
            {
                if(getX()==0)
                {
                    m_newX = getX();
                }
                else
                {
                    m_newX = getX() - 1;
                }
                m_newY = getY();
            }
            else if(RAND1 < 0.5) //Move east
            {
                if(getX()== GRIDSIZE - 1)
                {
                    m_newX = getX();
                }
                else
                {
                    m_newX = getX() + 1;
                }
                m_newY = getY();
            }
            else if(RAND1 < 0.75) //Move South
            {
                if(getY()==0)
                {
                    m_newY = getY();
                }
                else
                {
                    m_newY = getY() - 1;
                }
                m_newX = getX();
            }
            else //Move North
            {
                if(getY()== GRIDSIZE - 1)
                {
                    m_newY = getY();
                }
                else
                {
                    m_newY = getY() + 1;
                }
                m_newX = getX();
            }


        }
        else if(m_kairomoneResponse == 4)
        {
            ///////////////////
            ////// Random Walk Movement ////////////////////
            double RAND1 = (float)rand()/((float)RAND_MAX);
            //double RAND2 = (float)rand()/((float)RAND_MAX);
            if(RAND1 < 0.2) //Move west
            {
                if(getX()==0)
                {
                    m_newX = getX();
                }
                else
                {
                    m_newX = getX() - 1;
                }
                m_newY = getY();
            }
            else if(RAND1 < 0.4) //Move east
            {
                if(getX()== GRIDSIZE - 1)
                {
                    m_newX = getX();
                }
                else
                {
                    m_newX = getX() + 1;
                }
                m_newY = getY();
            }
            else if(RAND1 < 0.6) //Move South
            {
                if(getY()==0)
                {
                    m_newY = getY();
                }
                else
                {
                    m_newY = getY() - 1;
                }
                m_newX = getX();
            }
            else if(RAND1 < 0.8) //Move North
            {
                if(getY()== GRIDSIZE - 1)
                {
                    m_newY = getY();
                }
                else
                {
                    m_newY = getY() + 1;
                }
                m_newX = getX();
            }
            else //Stay still
            {
                m_newX = getX();
                m_newY = getY();
            }



        }

        if(m_kairomoneResponse == 1)  //// Gradient Ascent. (Random walk if gradient is zero).
        {
            if(kGradNS == 0 && kGradEW ==0)
            {

                ////// Random Walk Movement ////////////////////
                double RAND1 = (float)rand()/((float)RAND_MAX);
                double RAND2 = (float)rand()/((float)RAND_MAX);
                if(RAND1 < 0.5) //Move west
                {
                    if(getX()==0)
                    {
                        m_newX = getX();
                    }
                    else
                    {
                        m_newX = getX() - 1;
                    }
                }
                else
                {
                    if(getX()== GRIDSIZE - 1)
                    {
                        m_newX = getX();
                    }
                    else
                    {
                        m_newX = getX() + 1;
                    }
                }
                if(RAND2 < 0.5)
                {
                    if(getY()==0)
                    {
                        m_newY = getY();
                    }
                    else
                    {
                        m_newY = getY() - 1;
                    }
                }
                else
                {
                    if(getY()== GRIDSIZE - 1)
                    {
                        m_newY = getY();
                    }
                    else
                    {
                        m_newY = getY() + 1;
                    }
                }
            }
            double RAND = (float)rand()/((float)RAND_MAX);
            if( kGradNS > 0 && kGradEW > 0)   //move north or east
            {
                double kGradTotal = kGradEW + kGradNS;
                if (kGradNS/kGradTotal <= RAND)  // moving north
                {
                    if ( loc->getY() == GRIDSIZE - 1)
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY() + 1;
                    }
                }
                else         //moving east
                {
                    if (loc->getX() == GRIDSIZE - 1 )
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else
                    {
                        m_newX = loc->getX() + 1;
                        m_newY = loc->getY();
                    }
                }
            }
            else if(kGradNS > 0 && kGradEW < 0)   // move north or west
            {
                double kGradTotal = -kGradEW + kGradNS;
                if (kGradNS/kGradTotal <= RAND)  // moving north
                {
                    if (loc->getY() == GRIDSIZE - 1)
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY() + 1;
                    }
                }
                else         // moving west
                {
                    if (loc->getX() == 0)
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else
                    {
                        m_newX = loc->getX() - 1;
                        m_newY = loc->getY();
                    }
                }
            }
            else if (kGradNS < 0 && kGradEW < 0)     //moving south or west
            {
                double kGradTotal = -kGradEW - kGradNS;
                if (-kGradNS/kGradTotal <= RAND)   // moving south
                {
                    if (loc->getY() == 0)
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY() - 1;
                    }
                }
                else                             // moving west
                {
                    if (loc->getX() == 0)
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else
                    {
                        m_newX = loc->getX() - 1;
                        m_newY = loc->getY();
                    }
                }
            }
            else if(kGradNS < 0 && kGradEW > 0)  // moving south or east
            {
                double kGradTotal = kGradEW - kGradNS;
                if (-kGradNS/kGradTotal <= RAND)  // moving south
                {
                    if (loc->getY() == 0)
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY() - 1;
                    }
                }
                else
                {
                    if (loc->getX() == GRIDSIZE - 1)
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else
                    {
                        m_newX = loc->getX() + 1;
                        m_newY = loc->getY();
                    }
                }
            }
        }
        ///////////////////////////////////////
        if(m_kairomoneResponse == 2)    //// Discretized Chemotactic PDE. ///this implements IBM PDE; by setting Chi=0 we use diffusion part only
        {
            //////////////////variables for movement rules /////////
            double WDiff =loc->getwaspDiffX();
            //double Chi = loc->getChi();
            double Chi = getChi();
            double p0 = 1 -DT*( (4*(WDiff/DX2) + Chi*(loc->getKLaplacian()) )); ///28/6/17: TBD: remove after + to add up to one
            double p1 = DTDX*((WDiff/DX) - (Chi/(2) )*kGradEW);; //28/6/17: TBD: use ecmGradEW instead etc.
            double p2 = DTDX*((WDiff/DX) + (Chi/(2) )*kGradEW);
            double p4 = DTDX*((WDiff/DX) - (Chi/(2) )*kGradNS);;
            double p3 = DTDX*((WDiff/DX) + (Chi/(2) )*kGradNS);
            //  std::cout << " gradients are " << kGradEW << " ' " << kGradNS << "\n" << " Chi is " << Chi << "\n" << " WDiff is " << WDiff << "\n" << " p0 is  " << p0 << "\n" << " p1 is " << p1 << "\n" << "p2 is " << p2 << "\n" << " p3 is " << p3 << "\n" << " p4 is " << p4 << "\n";
            //	if( kGradEW != 0) {std::cout << " gradients are " << kGradEW << " ' " << kGradNS << "\n";}
            //	if(p0 < 0 || p1 < 0 || p2 < 0 || p3 < 0 )
            if(p0 < -0.01 || p1 < -0.01 || p2 < -0.01 || p3 < -0.01 )
            {
                std::cout << " Negative probabilities have occured in wasp.cpp  "<< p0 << p1 << p2 << p3 << "\n";
            }//// if we change parameters, this can lead to violation in stab. cond => often neg prob. and will catch it here!

////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//// find where the wasp is in the grid and then apply the corresponding probability of applying the movement rule..

            int Halfgrid;
            Halfgrid = GRIDSIZE / 2;
            double RAND22 = (float)rand()/((float)RAND_MAX);

            if(getX()>= 0 && getX()< Halfgrid && getY() >=0 && getY()< Halfgrid) //bottom left of grid
            {
                if(RAND22<ProbQ1)
                {
                    double pTotal = p0 + p1 + p2 + p3 + p4; //Same concept as in my MatLAb code
                    double RAND = (float)rand()/((float)RAND_MAX);
                    if ( RAND >= 0 && RAND < p0/pTotal)  // STAY STILL
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else if( RAND >= p0/pTotal && RAND  < (p0 + p1)/pTotal)         //MOVING LEFT/WEST
                    {
                        if (loc->getX() == 0 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX() - 1;
                            m_newY = loc->getY();
                        }
                    }
                    else if(RAND >= (p0 + p1)/pTotal && RAND  < (p0 + p1 + p2)/pTotal )     //MOVING RIGHT/EAST
                    {
                        if (loc->getX() == GRIDSIZE - 1 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX() + 1;
                            m_newY = loc->getY();
                        }
                    }
                    else if(RAND >= (p0 + p1 + p2)/pTotal && RAND  < (p0 + p1 + p2 + p3)/pTotal )     //MOVING UP/NORTH
                    {
                        if (loc->getY() == GRIDSIZE - 1 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY() + 1;
                        }
                    }
                    else if(RAND >= (p0 + p1 + p2 + p3)/pTotal && RAND  <= 1)     //MOVING DOWN/SOUTH
                    {
                        if (loc->getY() == 0 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY() - 1;
                        }
                    }


                }
                else
                {
                    m_newX = loc->getX();
                    m_newY = loc->getY();

                }

            }
            else if( getX()>= Halfgrid && getX()<= GRIDSIZE - 1 && getY() >=0 && getY()< Halfgrid) //bottom right
            {
                if(RAND22<ProbQ2)
                {
                    double pTotal = p0 + p1 + p2 + p3 + p4;
                    double RAND = (float)rand()/((float)RAND_MAX);
                    if ( RAND >= 0 && RAND < p0/pTotal)  // STAY STILL
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else if( RAND >= p0/pTotal && RAND  < (p0 + p1)/pTotal)         //MOVING LEFT/WEST
                    {
                        if (loc->getX() == 0 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX() - 1;
                            m_newY = loc->getY();
                        }
                    }
                    else if(RAND >= (p0 + p1)/pTotal && RAND  < (p0 + p1 + p2)/pTotal )     //MOVING RIGHT/EAST
                    {
                        if (loc->getX() == GRIDSIZE - 1 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX() + 1;
                            m_newY = loc->getY();
                        }
                    }
                    else if(RAND >= (p0 + p1 + p2)/pTotal && RAND  < (p0 + p1 + p2 + p3)/pTotal )     //MOVING UP/NORTH
                    {
                        if (loc->getY() == GRIDSIZE - 1 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY() + 1;
                        }
                    }
                    else if(RAND >= (p0 + p1 + p2 + p3)/pTotal && RAND  <= 1)     //MOVING DOWN/SOUTH
                    {
                        if (loc->getY() == 0 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY() - 1;
                        }
                    }


                }
                else
                {
                    m_newX = loc->getX();
                    m_newY = loc->getY();

                }


            }
            else if(getX() >= Halfgrid && getX()<= GRIDSIZE - 1 && getY() >= Halfgrid && getY()<= GRIDSIZE -1) //top right
            {
                if(RAND22<ProbQ3)
                {
                    double pTotal = p0 + p1 + p2 + p3 + p4;
                    double RAND = (float)rand()/((float)RAND_MAX);
                    if ( RAND >= 0 && RAND < p0/pTotal)  // STAY STILL
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else if( RAND >= p0/pTotal && RAND  < (p0 + p1)/pTotal)         //MOVING LEFT/WEST
                    {
                        if (loc->getX() == 0 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX() - 1;
                            m_newY = loc->getY();
                        }
                    }
                    else if(RAND >= (p0 + p1)/pTotal && RAND  < (p0 + p1 + p2)/pTotal )     //MOVING RIGHT/EAST
                    {
                        if (loc->getX() == GRIDSIZE - 1 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX() + 1;
                            m_newY = loc->getY();
                        }
                    }
                    else if(RAND >= (p0 + p1 + p2)/pTotal && RAND  < (p0 + p1 + p2 + p3)/pTotal )     //MOVING UP/NORTH
                    {
                        if (loc->getY() == GRIDSIZE - 1 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY() + 1;
                        }
                    }
                    else if(RAND >= (p0 + p1 + p2 + p3)/pTotal && RAND  <= 1)     //MOVING DOWN/SOUTH
                    {
                        if (loc->getY() == 0 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY() - 1;
                        }
                    }


                }
                else
                {
                    m_newX = loc->getX();
                    m_newY = loc->getY();

                }

            }
            else if(getX() >= 0 && getX()< Halfgrid && getY() >= Halfgrid && getY()<= GRIDSIZE -1) //top left
            {
                if(RAND22<ProbQ4)
                {
                    double pTotal = p0 + p1 + p2 + p3 + p4;
                    double RAND = (float)rand()/((float)RAND_MAX);
                    if ( RAND >= 0 && RAND < p0/pTotal)  // STAY STILL
                    {
                        m_newX = loc->getX();
                        m_newY = loc->getY();
                    }
                    else if( RAND >= p0/pTotal && RAND  < (p0 + p1)/pTotal)         //MOVING LEFT/WEST
                    {
                        if (loc->getX() == 0 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX() - 1;
                            m_newY = loc->getY();
                        }
                    }
                    else if(RAND >= (p0 + p1)/pTotal && RAND  < (p0 + p1 + p2)/pTotal )     //MOVING RIGHT/EAST
                    {
                        if (loc->getX() == GRIDSIZE - 1 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX() + 1;
                            m_newY = loc->getY();
                        }
                    }
                    else if(RAND >= (p0 + p1 + p2)/pTotal && RAND  < (p0 + p1 + p2 + p3)/pTotal )     //MOVING UP/NORTH
                    {
                        if (loc->getY() == GRIDSIZE - 1 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY() + 1;
                        }
                    }
                    else if(RAND >= (p0 + p1 + p2 + p3)/pTotal && RAND  <= 1)     //MOVING DOWN/SOUTH
                    {
                        if (loc->getY() == 0 )
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY();
                        }
                        else
                        {
                            m_newX = loc->getX();
                            m_newY = loc->getY() - 1;
                        }
                    }


                }
                else
                {
                    m_newX = loc->getX();
                    m_newY = loc->getY();

                }

            }




















        }

        m_clock = m_clock - 1; //if m_clock>0, renew clock after timestep
    }
}

