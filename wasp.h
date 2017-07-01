//
// File:   wasp.h
// Author: pieta
//
// Created on 28 January 2008, 13:45
//

#ifndef _WASP_H
#define    _WASP_H

#include <vector>
#include "cell.h" //by including this, we can call up fns in "cell"
//#include "wTraits.h"
//using namespace std;






class wasp { //a class is a type of data that you name yourself // wasp is an object in class wasp
private: //what follows are the variables of a wasp
    double m_fitness;
//	int m_strategyType;
//	double m_probdefect;
    double m_mutationAmount; //some wasp properties may change with time and this defines by how much.
    int m_name;
    int m_played;

    double m_probDoveGivenDove; //if 1, D; if 0, H. // code was chNGED FROM ONE STEP CONDITIONAL BUT NOT anymore => set this same as below
    double m_probDoveGivenHawk;
//	int m_opponentsLastMove;  // 0 = Hawk, 1 = Dove.

    int m_playedInPreviousStep; //0 = not played, 1 = played //encorporates memory: what strategy played now depends on previous strategy // whether it has played so that they don't play again
    int m_playedInThisStep;
    int m_playingStatus; // 0 = free to play, 1 = played in this step, 2 = played in last step

    cell *loc;  //every wasp has a cell in location loc [X,Y,Z]
    int m_newX; //the new (x,y) of the wasp
    int m_newY;
    bool m_status;
    int m_flightTime; //time of flight between grids
    int m_lifeSpan; //could mutate
    int m_clock;  //time elapsed of wasp's lifetime, always ticks
    int m_rest;
    double m_probLDiscover;

    int m_NewGpos; //in case wasp goes to new grid WATCH: MAY NOT BE POSSIBLE/CODE MAY NOT BE READY TO HAVE MULTIPLE GRIDS AT THIS POINT (DATE: MArch 2017)
    double m_probWaspDeathAferLifespan;
    double m_probMutation;

    //////Kairomone variables /////////
    double m_chi;
    double m_chiIncrement; //this and next for Chi mutation (are set in main)
    double m_probChiMutate;
    int m_kairomoneResponse; // 0 = discretized pde; 1 = gradient asscent.
    double m_kProd;

//	double m_KProd;

public: //these are things one can "tell" wasp; here one can decide which things out of 'private' one can ask a wasp object
    wasp();

    wasp(int kr);

    ///// Kairomone methods ///////////
    double getChiIncrement() { return m_chiIncrement; }

    void setChiIncrement(
            double val) { m_chiIncrement = val; } //what this does is set a value (m_kProd) but this would normally go into cpp file if it was longer

    double getChiMutation() { return m_probChiMutate; }

    void setChiMutation(double val) { m_probChiMutate = val; }

    double getChi() { return m_chi; }

    void setChi(double val) { m_chi = val; }

    int getKairomoneResponse() { return m_kairomoneResponse; }

    void setKairomoneResponse(int val) { m_kairomoneResponse = val; }

    double getKProd() { return m_kProd; }

    void setKProd(double val) { m_kProd = val; }



//	double getKProd() {return m_KProd;}
//	void setKProd(double val) {m_KProd = val;}



    double getFitness() { return m_fitness; } //m_fitness is all payoff added
    void setFitness(double val) { m_fitness = val; }

    void incrementFitness(double val) {
        m_fitness = m_fitness + val;
    } //if in ma                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             in interaction occurs with payoff 0.5, would set val=0.5

    double
    getPlayed() { return m_played; }  //next 3 lines is what we call in main if agent changes playing status by playing a game
    void setPlayed(int val) { m_played = val; }

    void incrementPlayed() { m_played = m_played + 1; }

    double getProbMutation() { return m_probMutation; }

    void setProbMutation(double val) { m_probMutation = val; }

    void setMutationAmount(double val) { m_mutationAmount = val; }

    double getMutationAmount() { return m_mutationAmount; }

    double getProbDoveGivenDove() { return m_probDoveGivenDove; }

    void setProbDoveGivenDove(double val) { m_probDoveGivenDove = val; }

    double
    getProbDoveGivenHawk() { return m_probDoveGivenHawk; } //call this to know strat of parent upon their death if they produce offspring and we use sync. prolif. //this is really P(play Dove) as we do not work with cond P // ask this and would get 0 or 1 unless there is a mutation allowed ton change from 1 or 0
    void setProbDoveGivenHawk(double val) { m_probDoveGivenHawk = val; }


//	void setOpponentsLastMove(int val) {m_opponentsLastMove = val;}  // this is for Cond. Prob.
//	int  getOpponentsLastMove() {return m_opponentsLastMove;}


    void setName(int val) { m_name = val; }

    int getName() { return m_name; }

    int getX();  //if one says "getX", looks into wasp.cpp file to look it up
    int getY();

    int getNewX();

    int getNewY();

    void setCell(cell *val);

    bool isAlive() { return m_status; }

    void KillWasp() { m_status = false; }

    void processStep(int n);

    void setNewX(int val) { m_newX = val; }

    void setNewY(int val) { m_newY = val; }

    int getLifeSpan() { return m_lifeSpan; }

    int getClock() { return m_clock; }

    double getprobLDiscover() { return m_probLDiscover; }

    void setFlightTime(int val) { m_flightTime = val; }

    void setLifeSpan(int val) { m_lifeSpan = val; }

    cell *getCell() { return loc; }

    void setClock(int val) { m_clock = val; } //can tell every wasp what m_clock is, i.e. how long it'll live
    void setRest(int val) { m_rest = val; }

    void setPlayingStatus(int val) { m_playingStatus = val; }

    int getPlayingStatus() { return m_playingStatus; }

    void setprobLDiscover(double val) { m_probLDiscover = val; }

    void setNewGpos(double val) { m_NewGpos = val; }

    int getNewGpos() { return m_NewGpos; }

    double getprobWaspDeathAferLifespan() { return m_probWaspDeathAferLifespan; }

    void setprobWaspDeathAferLifespan(
            double val) { m_probWaspDeathAferLifespan = val; } //this is for the prob that it lives above 100/lifespann (if it was in constructor, it would set automatically but this way we have to set it up for each wasp)

};

#endif    /* _WASP_H */

