//
// File:   main.cc
// Author: pieta
//
// Created on 28 January 2008, 13:41
//
/////////////////////////////////
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "grid.h"	
#include "wasp.h"
#include <math.h>
#include "main.h"
#include <string>   //
#include <sstream>
#include <algorithm>




 
//Values in green are those used in Axelrods tournaments. /////////////

double const V = 0.03;
double const C = 0.04;
double const DoveHawk =  0 ;  //5;   //Payoff to Defector when playing a Cooperator.
//double const HawkHawk = 0.5*(V - C);   // 3;
double HawkHawkWin = 0.5*(V-C) ; //here 1 H wins and one loses; no average as above scenario
double HawkHawkLoose = 0.5*(V- C);
double const DoveDove =   V*0.5;   //1;  //doves share resources
double const HawkDove =  V;  //0; //H gets this when it meets D

double const DT =  0.05;
double const DX = 1;
double const DX2 = DX*DX; 
double const DTDX = DT/DX; 
double const IDX2 = 1/DX2;  
double const DTDX2= DT/DX2;

///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//defines the 4 probabilities of  applying the diffusion movement rule for the 4 quadrants. See also main.h for variable declaration.
double const ProbQ1 = 1.0;//lower left
double const ProbQ2 = 0.5; //lower right
double const ProbQ3 = 0.1; //upper right
double const ProbQ4 = 0.05; //upper left
/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


int main() 
{
    
////// HOUSEKEEPING CREATE VARIABLES AND RANDOM NUMBER AND GET ARGUMENTS
    ///MODEL PARAMETERS///
    int maxGrids = 1;       // number of grids
    int timeSteps= 5000000;        // total number of iterations
	int initWasps = 1000;      // initial wasp population
	int MaxWasps = 4;  // maximum no of wasps interactions per timestep.
	int WaspLifeSpan = 100;  //gregarious lifespan
	double probWaspDeathAferLifespan = 1;
	double probLarvaDiscovery = 1;  //this could be altered to incl. P(do not interact despite in same position)
	vector <grid*> Grids;               // vector of pointers (addresses) of grids ///this is for multi-grids; vector of pointers to grids
	vector <wasp*> Wasps[2];
	double const probMutation = 0.0; // any time offspring is born, this is P(it'll mutate), which is a change in the P(play dove) [unless it's 0 or 1 already, when it cannot mutate]; if it is decided that playver will mutate, its P(playDove)  will increase/decrease half the time
	double const mutationAmount = 0.0;
	int now = 0;
	////////// Kairomone variables //////////
	int KRes = 2; // kairomone response mechanism. 1 = gradient ascent, 2 = PDE, 3 = Random walk 1/4, 4 = Random walk 1/5.
	double chi_increment = 0.00001; ///10000;
	double probChiMutate = 0.0; 
	double probGrossChiMutate = 0.; // fraction of the time chi mutates bu a larger amount
	double GrossChi_increment = 0.00005;
	double Kdiff = 0.0005;  // 0.0001 ; //1/(10000) ;
	double Kdeccay = 1;
	double Kprod = 0; ///this is the MDE production. I may  want to change the name as it's not ECM production and I want to be able to distinguish.
	double chi = 0.000;
	double WaspDiff = 0.005; // 5/(1000000);  //5/(10000) ;
	double probMutationKProd = 0.0;
	double probExtremeMutation = 0.00;
	double mutationKProd = 0.005;	
	double extremeMutationKProd = 0.05;
	
////// CREATE THE GRIDS
    // at the moment there are no between grid movement rules so multiple grids 
    // are slightly redundent but will become useful
    for(int gwk=0;gwk<maxGrids;gwk++)  //2 grids=>1x loop
	{
        grid * pGrid = new grid();   // allocates memory for a grid object and returns the address //"pGrid" id the name of the new grid //" grid()" means that we're naking a grid object // pGrid p means pointer to grid
		pGrid->setmaxNoWasps(MaxWasps); //this calls a fn on the 'grid' // we do all of these below in THIS new grid called pGrid // each element is a pointer to a grid object  // "->' calls function on the 'grid' //
		pGrid->setKdiff(Kdiff);  
		pGrid->setKdecay(Kdeccay);
		pGrid->setKprod(Kprod);	
		pGrid->setWaspDiff(WaspDiff);		
		pGrid->init(gwk); //makes the grid to be composed of cells as required
        Grids.push_back(pGrid);  // store that address on the vector of grid addresses // to make more than one grid //"Grids" defines a new vector called "Grids" thie process is : [grid2;grid1]
	}
    
////// CREATE THE INITIAL WASPS AND ALLOCATE THEM TO RANDOM CELL ON RANDOM GRID // TO change the ICs, change here
    
    for(int iwk = 0; iwk < initWasps; iwk++) //iwk is number of wasps
	{
		int gPos = 0; //this needs changed if more than 1 grid (make a loop)

		wasp * pWasp = new wasp();
		//pWasp->setName(iwk);
		/*
		
		double RAND = (float)rand()/(float)RAND_MAX;
		if (RAND < 0.9995)  //DOVES 1
		{
			pWasp->setProbDoveGivenHawk(1);
			pWasp->setProbDoveGivenDove(1);
			pWasp->setName(1);

		}
		else   //HAWKS 0
		{
			pWasp->setProbDoveGivenHawk(0);
			pWasp->setProbDoveGivenDove(0);
			pWasp->setName(0);
		}
		*/ // need to set these every time we make a wasp
		pWasp->setProbDoveGivenHawk(1); //if this is set to 1, this corresponds to all doves initially // both of these statements currently mean the same, one is redundant but still keeping it in case we want to use cond prob later
		pWasp->setProbDoveGivenDove(1);
		pWasp->setName(1);		//don't seem to use this. if delete, delete EVERYWHERE it is used for tracking lineages; individua have a particular lineage at start and pass it onto children
		
		pWasp->setLifeSpan(WaspLifeSpan);
		pWasp->setClock(WaspLifeSpan);  //set to 100 and counts downs
		pWasp->setProbMutation(probMutation); //if this is e.g. 0.1, means there is a 0.1 chance that offspring has a different strategy
		pWasp->setMutationAmount(mutationAmount);
		pWasp->setprobLDiscover(probLarvaDiscovery);
		pWasp->setprobWaspDeathAferLifespan(probWaspDeathAferLifespan);
		pWasp->setKProd(Kprod);
				
		////////// Kairomone variables ///////////
		pWasp->setChiMutation(probChiMutate);
		pWasp->setChiIncrement(chi_increment);		
		pWasp->setChi(chi);
		pWasp->setKairomoneResponse(KRes);// 2 = discretized pde; 1 = gradient asscent; 		
	
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////// sets initial configuration as a 10 by 10 square of wasps placed centrally on the grid. Each wasp being allocated to a random position within that space.
        
        double RANDX =(float)rand()/((float)RAND_MAX);  //generates a random number we will use to calculate a random x coordinate within the initial saquare.
        double RANDY = (float)rand()/((float)RAND_MAX); //generates a random number we will use to calculate a random y coordinate within the initial saquare.
        int centre = int(GRIDSIZE/10);
        int x;
        int y;
    
            x = 45+(int)(centre * RANDX);

            y = 45+(int)(centre * RANDY);
        
            Wasps[now].push_back(pWasp); //once a wasp is made, it becomes an element in Wasps (list of wasps) AND
			Grids[gPos]->addWasp(pWasp, now, y, x); //a wasp in the grds (i.e. an element in Grids); if there was more than one grid, gPos would need to loop through e.g. 1 to 5
        
//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

		
		
		
		
		//Different ICs
		
	//	Wasps[now].push_back(pWasp);  
	//	Grids[gPos]->addWaspRandom(pWasp, now);
		/*
		if(pWasp->getCell()->getY() > int(GRIDSIZE/2) - 5 && pWasp->getCell()->getY() < int(GRIDSIZE/2) + 5 && pWasp->getCell()->getX() > int(GRIDSIZE/2) - 5 && pWasp->getCell()->getX() < int(GRIDSIZE/2) + 5)
		{
			pWasp->setProbDefectGivenCooperate(1);
			pWasp->setProbDefectGivenDefect(1);
		}
		else
		{
			pWasp->setProbDefectGivenCooperate(0);
			pWasp->setProbDefectGivenDefect(0);			
		}
		*/
    }
    ////here i DECLARE i have output files  but they are written somewhere else
	ofstream TOTAL("TOTAL.txt"); //these are lists with total  no. of H / D / all etc. ; their posistion in the list corresponds to the time step
	ofstream TOTALD("TOTALD.txt");
	ofstream TOTALH("TOTALH.txt");

/////////////////////THE MAIN LOOP ////////////////////////////////////	

/////// Play game, then calculate K for next timestep, due to configuration in this timestep,
///////// then move in response to K,  then reproduce and/or die if necessary


	for(int twk = 0; twk < timeSteps; twk++)
	{
		random_shuffle(Wasps[now].begin(), Wasps[now].end());

		/////////////// OUTPUT CODE ////////////////////////////////////////////////				
		int Totaloutemp = 0; //starting like this is good practisee
		Totaloutemp = Wasps[now].size(); //size of wasp vector, Hawks and Doves
		TOTAL << Totaloutemp << endl;  // means: GO INTO TOTAL and write number Totaloutemp//"endl" means next time we're writingg in TOTAL.txt, will be on new line, so that we get a column //
		
		int TotalD = 0; //this and below are the same (?)
		int TotalH = 0;
		if(twk % 100 == 0) //% means modulo // produce every 100 time steps on screen
		{
			std::cout << "time  " << twk << "   Number "  << Wasps[now].size() << "\n";
		}
		/*
		if(twk % 100 == 0)
		{
		///these are other output files to look at other things
         
         //if use mutations, every 100 time steps P(play dove) would be the output. E.g. we start [1;1;1;1;1;1...] and then [1;0.99;1;...]
         
			ofstream probDefectGivenDefectout;
			string wfilename;
			ostringstream wfilestring;
			wfilestring << "DefectGivenDefect_" << twk << ".txt" << ends;
			wfilename=wfilestring.str();
			probDefectGivenDefectout.open(wfilename.data());
			probDefectGivenDefectout.setf(ios::fixed);
			
			ofstream probDefectGivenCooperateout;
			string wwfilename;
			ostringstream wwfilestring;
			wwfilestring << "DefectGivenCooperate_" << twk << ".txt" << ends;
			wwfilename=wwfilestring.str();
			probDefectGivenCooperateout.open(wwfilename.data());
			probDefectGivenCooperateout.setf(ios::fixed);			
		
         
        ///Chi for always defect mutation writes list
			ofstream Chiout;
			string wwwfilename;
			ostringstream wwwfilestring;
			wwwfilestring << "ChiAC_" << twk << ".txt" << ends;
			wwwfilename=wwwfilestring.str();
			Chiout.open(wwwfilename.data());
			Chiout.setf(ios::fixed);
			
         ///Chi for always cooperate mutation writes list
         
			ofstream Chiout2;
			string wwwwfilename;
			ostringstream wwwwfilestring;
			wwwwfilestring << "ChiAD_" << twk << ".txt" << ends;
			wwwwfilename=wwwwfilestring.str();
			Chiout2.open(wwwwfilename.data());
			Chiout2.setf(ios::fixed);	
		
         ///Chi for total mutation writes list
         
			ofstream KProd;
			string wwwwwfilename;
			ostringstream wwwwwfilestring;
			wwwwwfilestring << "KProd_" << twk << ".txt" << ends;
			wwwwwfilename=wwwwwfilestring.str();
			KProd.open(wwwwwfilename.data());
			KProd.setf(ios::fixed);	
				
         ///Chi mutation list H/D
         
			ofstream KProdH;
			string ufilename;
			ostringstream ufilestring;
			ufilestring << "KProdH_" << twk << ".txt" << ends;
			ufilename=ufilestring.str();
			KProdH.open(ufilename.data());
			KProdH.setf(ios::fixed);				
			
			ofstream KProdD;
			string uufilename;
			ostringstream uufilestring;
			uufilestring << "KProdD_" << twk << ".txt" << ends;
			uufilename=uufilestring.str();
			KProdD.open(uufilename.data());
			KProdD.setf(ios::fixed);				
			
         
         //calculate what to write into files//Note that we're still in the every 100 time-steps-loop
			
			for(int cwk = 0; cwk < Wasps[now].size(); cwk++) // swk is a wasp; " Wasps[now].size()" the length of the current wasp vector
			{	
	//			double temp = Wasps[now][cwk]->getProbDefectGivenDefect();
	//			probDefectGivenDefectout << temp << " ";
	//			probDefectGivenDefectout << endl; // for each wasp find probability and write in list
				
	//			double temp2 = Wasps[now][cwk]->getProbDefectGivenCooperate(); //this one is for using cond prob
	//			probDefectGivenCooperateout << temp2 << " ";
	//			probDefectGivenCooperateout << endl;
				
				if(Wasps[now][cwk]->getProbDoveGivenHawk() == 0) //looking at P(dove)=0, which means we are looking at a hawk, and that we are dealing with a Hawk's Kai sensitivity and Kai production //usually one at once
				{
		//			double chitemp = Wasps[now][cwk]->getChi();
		//			Chiout << chitemp << " ";
		//			Chiout << endl;
					
					double kprodtempH = Wasps[now][cwk]->getKProd();
					KProdH << kprodtempH << " " ;
					KProdH << endl;
					
				}
				if(Wasps[now][cwk]->getProbDoveGivenDove() == 1)
				{
					double kprodtempD = Wasps[now][cwk]->getKProd();
					KProdD << kprodtempD << " " ;
					KProdD << endl;					
					//	double chitemp2 = Wasps[now][cwk]->getChi();
				//	Chiout2 << chitemp2 << " ";
				//	Chiout2 << endl;
				}
				double kprodtempH = Wasps[now][cwk]->getKProd();
				KProd << kprodtempH << " " ;
				KProd << endl;				
				
			}
         
         
         ///we need to close output files
         
         
	//		probDefectGivenDefectout.close();	
	//		probDefectGivenCooperateout.close();
	//		Chiout.close();		
	//		Chiout2.close();
			KProd.close();
			KProdH.close();
			KProdD.close();
				
       ///  puts the names to a file
         
         
			ofstream namesout;
			string wwwfilename;
			ostringstream wwwfilestring;
			wwwfilestring << "Names_" << twk << ".txt" << ends;
			wwwfilename=wwwfilestring.str();
			namesout.open(wwwfilename.data());
			namesout.setf(ios::fixed);
		
			for(int cwk = 0; cwk < Wasps[now].size(); cwk++)
			{
				double temp = Wasps[now][cwk]->getName();
				namesout << temp << " ";
				namesout << endl;
			}
			namesout.close();
			    
		}
*/		
		//////////////// END OUTPUT CODE /////////////////////
		
		////// Process kairomone /////
		Grids[0]->processKairomone(now); //move the chemicals about a bit /// 0 because we only have one grid in our model; would need loop here if larger no grids  // we call up fn on the grid called "processKairomone(now)", which does all the diffusion (to see this, look at grid code
		
		///////////////////random number to change simulation-otherwise always the same psuedorandom numbers///////////////
		double MUTATION_DISCRIMINANT22 = (float)rand()/((float)RAND_MAX);																				
		double alfred = MUTATION_DISCRIMINANT22;
		///////////////////
		
		////////////////// Game playing and movement //////////////
		for(int wwk=0;wwk<Wasps[now].size();wwk++)
		{
		
					
											
			cell * CELL =Wasps[now][wwk]->getCell(); //says: "find cell of 1st wasp (then second and so on) in vector and ...
			wasp * WASP = Wasps[now][wwk];  //..call it WASP
		//	double FITNESS = WASP->getFitness();
		//	if(Wasps[now][wwk]->getPlayingStatus() == 0 &&  CELL->getNumWasps() <= MaxWasps)
			if(Wasps[now][wwk]->getPlayingStatus() == 0 && Wasps[now][wwk]->getPlayed() <= MaxWasps) ///since"Wasps[now][wwk]"is WASP, could have called it "WASP" //"getPlayingStatus() == 0 " means WASP is allowed to play as getPlayingStatus() checks whether wasp has played yet
			{
				
				for(int i = 0; i < CELL->getNumWasps(); i++) //CELL->getNumWasps() asks how many other wasps are in the cell of first (second...) wasp // we cycle through wasps in ONE cell
				{
					if(CELL->getWasp(i)->getPlayingStatus() == 0 && CELL->getWaspIndex(Wasps[now][wwk]) != i && CELL->getWasp(i)->getPlayed() <= MaxWasps && WASP->getPlayed() <= MaxWasps) // 4 is MaxWasp currently, meaning that each wasp can play up to 4 times per time step  //"getPlayingStatus() == 0 " means the ith wasp is allowed to play as getPlayingStatus() checks whether wasp has played yet //"getWaspIndex(Wasps[now][wwk]) != i " makes sure ith wasp is not WASP //"&& WASP->getPlayed() <= MaxWasps" checks again...
					{
						
						double RAND6 =(float)rand()/((float)RAND_MAX); //generates rand no between 0 and 1 // determines  which hawk wins contest
						if(WASP->getProbDoveGivenHawk() == 0 && CELL->getWasp(i)->getProbDoveGivenHawk() == 0)  /// H Vs H
						{
							if(RAND6 < 0.5) ///determines which H wins/loses
							{ //here WASP gets V, i loses C
								WASP->incrementFitness(HawkHawkWin);
								CELL->getWasp(i)->incrementFitness(HawkHawkLoose);								
							}
							else
							{
								WASP->incrementFitness(HawkHawkLoose);
								CELL->getWasp(i)->incrementFitness(HawkHawkWin);								
							}								
					
						}
						else if(WASP->getProbDoveGivenHawk() == 0 && CELL->getWasp(i)->getProbDoveGivenHawk() == 1) // H Vs D
						{
							WASP->incrementFitness(HawkDove);
							CELL->getWasp(i)->incrementFitness(DoveHawk);
							
						}
						else if(WASP->getProbDoveGivenHawk() == 1 && CELL->getWasp(i)->getProbDoveGivenHawk() == 0) // D Vs H
						{
							WASP->incrementFitness(DoveHawk);
							CELL->getWasp(i)->incrementFitness(HawkDove);
							
						}
						else if(WASP->getProbDoveGivenHawk() == 1 && CELL->getWasp(i)->getProbDoveGivenHawk() == 1) //D Vs D
						{
							WASP->incrementFitness(DoveDove);
							CELL->getWasp(i)->incrementFitness(DoveDove); //there is a function on CELL called "getWasp(i)" and on the wasp i we increase fitness
						
						}
					}
					CELL->getWasp(i)->incrementPlayed(); //tell ith wasp in CELL to increase number of games played
					WASP->incrementPlayed(); //tell WASP same
				}
				WASP->setPlayingStatus(1);  //ensures WASP is not in loop anymore once it has played 4 games
								
			
			}
            
            //note that we are still in loop through all wasps and use this to ask what strategy they are playing
            
			if(Wasps[now][wwk]->getProbDoveGivenHawk() == 1)
			{
				TotalD = TotalD + 1;
			}
			else
			{
				TotalH = TotalH + 1;
			}
			
		}
		TOTALD << TotalD << endl;
		TOTALH << TotalH << endl;		////// Process kairomone ///// at the moment we process Kai before they play but we can uncomment this to change order
		Grids[0]->processKairomone(now); //move the chemicals about a bit
		/// 1) Calculates K for next time step using K at this timestep.
		/// 2) Calculates present K gradients for use in the wasp movement in wasp process step.
		
		//////////// MOVEMENT OR REPRODUCTION ACCORDING TO ACCUMULATED PAYOFF & MUTATION /////////////////////////////
		for(int wwk=0;wwk<Wasps[now].size();wwk++)
		{
			
			Wasps[now][wwk]->processStep(now);  /// Movement in response to K at time t //calculate movement prob for all w ask say where they are!
			Wasps[now][wwk]->setPlayed(0);      ///Re set playing status of all wasps //ga,es finished => all can play again
			if(Wasps[now][wwk]->getPlayingStatus() == 1)
			{
				Wasps[now][wwk]->setPlayingStatus(0);
			}			
			int wgd = Wasps[now][wwk]->getCell()->getGpos();
			if(Wasps[now][wwk]->isAlive() == true)
			{
				Grids[wgd]->addWaspXY(Wasps[now][wwk],1-now);  //putting wasp into other copy if alive
				Wasps[1-now].push_back(Wasps[now][wwk]); //add this wasp to the big Wasp vector
				
///////////////////////////////////////////////////////////////////////////
				
				if( Wasps[now][wwk]->getFitness() > 0 )
				{
					int fitness = int(Wasps[now][wwk]->getFitness()); ///take int part of fitness
					for(int rpd = 0; rpd < fitness; rpd++ ) // reproduce and loop through reproduction times
					{
						wasp* l = new wasp(); //new wasp called "l" // could have what follows in the constructor
						l->setLifeSpan(WaspLifeSpan);
						l->setClock(WaspLifeSpan);	
						l->setprobWaspDeathAferLifespan(probWaspDeathAferLifespan);
						l->setprobLDiscover(probLarvaDiscovery);
						l->setprobWaspDeathAferLifespan(probWaspDeathAferLifespan);	
						l->setProbMutation(probMutation);
						l->setMutationAmount(mutationAmount);
						l->setName(Wasps[now][wwk]->getName() ); // name set to parent
						l->setKairomoneResponse(Wasps[now][wwk]->getKairomoneResponse() );
						l->setKProd(Wasps[now][wwk]->getKProd() );
						
						/////////////////// Kairomone Code /////////////// the following 3 aren't relevant unless Chi mutates
						l->setChiMutation(probChiMutate);
						l->setChiIncrement(chi_increment);
						l->setChi(Wasps[now][wwk]->getChi());	
						
                        
                        //just to check
                        
						if( Wasps[now][wwk]->getProbDoveGivenDove() > 1  ||  Wasps[now][wwk]->getProbDoveGivenDove() < 0)
						{
							std::cout << " prob defect given defect out of range  " << Wasps[now][wwk]->getProbDoveGivenDove() << "\n"; 
						}	
						
						if(Wasps[now][wwk]->getProbDoveGivenHawk() > 1  ||  Wasps[now][wwk]->getProbDoveGivenHawk() < 0)
						{
							std::cout << " prob defect given defect out of range  " << Wasps[now][wwk]->getProbDoveGivenHawk() << "\n"; 
						}	
						
						
						//////// MUTATION CODE FOR PROBABILITIES (at reproduction) /////////////////////////////////////////// we are still in the wasp ++ loop
						double MUTATION_DISCRIMINANT = (float)rand()/((float)RAND_MAX);  //Mutation of ProbDefectGivenDefect //decides: Mut Y or N?
						if(MUTATION_DISCRIMINANT <= probMutation && Wasps[now][wwk]->getName()  <= initWasps)
						{
							double MUTATION_DIRECTION = (float)rand()/((float)RAND_MAX);
							if(MUTATION_DIRECTION < 0.5 )  // mutation decreases prob defect
							{
								if(Wasps[now][wwk]->getProbDoveGivenDove() >= mutationAmount)
								{
									l->setProbDoveGivenDove( Wasps[now][wwk]->getProbDoveGivenDove() - mutationAmount); //apply mutation if non-neg P(Dove)
								}
								else //else leave
								{
									l->setProbDoveGivenDove(Wasps[now][wwk]->getProbDoveGivenDove() );
								}
							}
							else
							{
								if(Wasps[now][wwk]->getProbDoveGivenDove() <= 1 - mutationAmount) //P(D)>1 vs else
								{
									l->setProbDoveGivenDove(Wasps[now][wwk]->getProbDoveGivenDove() + mutationAmount );
								}
								else
								{
									l->setProbDoveGivenDove(Wasps[now][wwk]->getProbDoveGivenDove()  );
								}
							} 
						}
						else
						{
							l->setProbDoveGivenDove(Wasps[now][wwk]->getProbDoveGivenDove()  ); //else leave
						}
						
                        ////////////////////////////////////////////////////////////////////////////////////////////what follows is for a memory-one-timestep and hence not currently needed [corresponding output files for when it was needed are called tft for "tit for tat"////////////////////////////////////
                        
						double MUTATION_DISCRIMINANT2 = (float)rand()/((float)RAND_MAX);   //// Mutation of DefectGivenCooperate
						if(MUTATION_DISCRIMINANT2 <= probMutation && Wasps[now][wwk]->getName()  <= initWasps)
						{
							double MUTATION_DIRECTION2 = (float)rand()/((float)RAND_MAX);
							if(MUTATION_DIRECTION2 < 0.5 )  // mutation decreases prob defect
							{
								if(Wasps[now][wwk]->getProbDoveGivenHawk() >= mutationAmount)
								{
									l->setProbDoveGivenHawk( Wasps[now][wwk]->getProbDoveGivenHawk() - mutationAmount);
								}
								else
								{
									l->setProbDoveGivenHawk(Wasps[now][wwk]->getProbDoveGivenHawk()  );
								}
							}
							else
							{
								if(Wasps[now][wwk]->getProbDoveGivenHawk() <= 1 - mutationAmount)
								{
									l->setProbDoveGivenHawk( Wasps[now][wwk]->getProbDoveGivenHawk() + mutationAmount);
								}
								else
								{
									l->setProbDoveGivenHawk(Wasps[now][wwk]->getProbDoveGivenHawk() );
								}
							}
						}
						else
						{
							l->setProbDoveGivenHawk(Wasps[now][wwk]->getProbDoveGivenHawk());
						}
						///////// MUTATION OF KPROD ///////////////////////////////// the Chi production coeff could mutate
						double MUTATION_DISCRIMINANT3 = (float)rand()/((float)RAND_MAX);   //// Mutation of KPROD
						if(MUTATION_DISCRIMINANT3 <= probMutationKProd )// && Wasps[now][wwk]->getName()  <= initWasps) ///this decides WHETHER mutate
						{
							double MUTATION_DIRECTION3 = (float)rand()/((float)RAND_MAX); ///this is a small frequent mutation
							
							double ExtremeMUTATION_DISCRIMINANT = (float)rand()/((float)RAND_MAX);///this is a large infrequent mutation so that about 1/10 of all mutations are these extreme mutations
							
							if(MUTATION_DIRECTION3 < 0.5 )  // mutation decreases KPROD
							{
								if(ExtremeMUTATION_DISCRIMINANT < probExtremeMutation && Wasps[now][wwk]->getKProd() >= extremeMutationKProd)
								{
									l->setKProd( Wasps[now][wwk]->getKProd() - extremeMutationKProd);
								}
								else if(Wasps[now][wwk]->getKProd() >= mutationKProd)
								{
									l->setKProd( Wasps[now][wwk]->getKProd() - mutationKProd);
								}
								else
								{
									l->setKProd(Wasps[now][wwk]->getKProd()  );
								}
							}
							else  //mutation increases KPROD.
							{
								if(ExtremeMUTATION_DISCRIMINANT < probExtremeMutation)
								{
									l->setKProd( Wasps[now][wwk]->getKProd() + extremeMutationKProd);
								}
								else
								{
									l->setKProd(Wasps[now][wwk]->getKProd() + mutationKProd );
								}
							}
							
						}
						else
						{
							l->setKProd(Wasps[now][wwk]->getKProd());
						}
						
						
						///////// MUTATION OF CHI /////////////////////////////////
						double MUTATION_DISCRIMINANT4 = (float)rand()/((float)RAND_MAX);   //// Mutation of CHI
						if(MUTATION_DISCRIMINANT4 <= probChiMutate) //// MUTATE IN CHI
						{
							double MUTATION_DIRECTION4 = (float)rand()/((float)RAND_MAX);
							double MUTATION_DISCRIMINANT5 =  (float)rand()/((float)RAND_MAX);
							if(MUTATION_DIRECTION4 < 0.5 )  // mutation decreases Chi
							{
								
								
								if(MUTATION_DISCRIMINANT5 < probGrossChiMutate)
								{
									if(Wasps[now][wwk]->getChi() >= GrossChi_increment) ///"GrossChi_increment" is extreme mutation
									{
										l->setChi( Wasps[now][wwk]->getChi() - GrossChi_increment);
									}
									else 
									{
										l->setChi(Wasps[now][wwk]->getChi()  );
									}
								}
								else 
								{
									if(Wasps[now][wwk]->getChi() >= chi_increment)
									{
										l->setChi( Wasps[now][wwk]->getChi() - chi_increment);
									}
									else
									{
										l->setChi(Wasps[now][wwk]->getChi()  );
									}
									
								}
							}
							else 
							{
								if(MUTATION_DISCRIMINANT5 < probGrossChiMutate)
								{
									l->setChi( Wasps[now][wwk]->getChi() + GrossChi_increment);
								}
								else 
								{
									l->setChi(Wasps[now][wwk]->getChi() + chi_increment );
								}
								
								
							}
						}
						else
						{
							l->setChi(Wasps[now][wwk]->getChi());
						}
						
						
						
						
						
						l->setNewX(Wasps[now][wwk]->getX() );  // offspring born at site of parental death.
						l->setNewY(Wasps[now][wwk]->getY() );
						
						int lgd = Wasps[now][wwk]->getCell()->getGpos(); //offspring on same grid
						Grids[lgd]->addWaspXY(l,1-now);  //find out what grid and ask wasp to grid for next
						Wasps[1-now].push_back(l);	//add to 'next' wasp vector
					}
					Wasps[now][wwk]->setFitness(Wasps[now][wwk]->getFitness() - int(Wasps[now][wwk]->getFitness()) );
				}

			
			
				
				
				
				
				
				
				
				
				
				
////////////////////////////////////////////////////////////////////////////				
				
			}
			else  //reproduce wasps according to their fitness at death
			{
				if( Wasps[now][wwk]->getFitness() > 0 )
				{
					int fitness = int(Wasps[now][wwk]->getFitness());
					for(int rpd = 0; rpd < fitness; rpd++ )
					{
						wasp* l = new wasp();
						l->setLifeSpan(WaspLifeSpan);
						l->setClock(WaspLifeSpan);	
						l->setprobWaspDeathAferLifespan(probWaspDeathAferLifespan);
						l->setprobLDiscover(probLarvaDiscovery);
						l->setprobWaspDeathAferLifespan(probWaspDeathAferLifespan);	
						l->setProbMutation(probMutation);
						l->setMutationAmount(mutationAmount);
						l->setName(Wasps[now][wwk]->getName() );
						l->setKairomoneResponse(Wasps[now][wwk]->getKairomoneResponse() );
						l->setKProd(Wasps[now][wwk]->getKProd() );
						
						/////////////////// Kairomone Code ///////////////
						l->setChiMutation(probChiMutate);
						l->setChiIncrement(chi_increment);
						l->setChi(Wasps[now][wwk]->getChi());	
						
						if( Wasps[now][wwk]->getProbDoveGivenDove() > 1  ||  Wasps[now][wwk]->getProbDoveGivenDove() < 0)
						{
							std::cout << " prob defect given defect out of range  " << Wasps[now][wwk]->getProbDoveGivenDove() << "\n"; 
						}	
						
						if(Wasps[now][wwk]->getProbDoveGivenHawk() > 1  ||  Wasps[now][wwk]->getProbDoveGivenHawk() < 0)
						{
							std::cout << " prob defect given defect out of range  " << Wasps[now][wwk]->getProbDoveGivenHawk() << "\n"; 
						}	
						
						//////// MUTATION CODE FOR PROBABILITIES (at reproduction) ///////////////////////////////////////////						
						double MUTATION_DISCRIMINANT = (float)rand()/((float)RAND_MAX);  //Mutation of ProbDefectGivenDefect
						if(MUTATION_DISCRIMINANT <= probMutation && Wasps[now][wwk]->getName()  <= initWasps)
						{
							double MUTATION_DIRECTION = (float)rand()/((float)RAND_MAX);
							if(MUTATION_DIRECTION < 0.5 )  // mutation decreases prob defect
							{
								if(Wasps[now][wwk]->getProbDoveGivenDove() >= mutationAmount)
								{
									l->setProbDoveGivenDove( Wasps[now][wwk]->getProbDoveGivenDove() - mutationAmount);
								}
								else
								{
									l->setProbDoveGivenDove(Wasps[now][wwk]->getProbDoveGivenDove() );
								}
							}
							else
							{
								if(Wasps[now][wwk]->getProbDoveGivenDove() <= 1 - mutationAmount)
								{
									l->setProbDoveGivenDove(Wasps[now][wwk]->getProbDoveGivenDove() + mutationAmount );
								}
								else
								{
									l->setProbDoveGivenDove(Wasps[now][wwk]->getProbDoveGivenDove()  );
								}
							} 
						}
						else
						{
							l->setProbDoveGivenDove(Wasps[now][wwk]->getProbDoveGivenDove()  );
						}
						
						double MUTATION_DISCRIMINANT2 = (float)rand()/((float)RAND_MAX);   //// Mutation of DefectGivenCooperate
						if(MUTATION_DISCRIMINANT2 <= probMutation && Wasps[now][wwk]->getName()  <= initWasps)
						{
							double MUTATION_DIRECTION2 = (float)rand()/((float)RAND_MAX);
							if(MUTATION_DIRECTION2 < 0.5 )  // mutation decreases prob defect
							{
								if(Wasps[now][wwk]->getProbDoveGivenHawk() >= mutationAmount)
								{
									l->setProbDoveGivenHawk( Wasps[now][wwk]->getProbDoveGivenHawk() - mutationAmount);
								}
								else
								{
									l->setProbDoveGivenHawk(Wasps[now][wwk]->getProbDoveGivenHawk()  );
								}
							}
							else
							{
								if(Wasps[now][wwk]->getProbDoveGivenHawk() <= 1 - mutationAmount)
								{
									l->setProbDoveGivenHawk( Wasps[now][wwk]->getProbDoveGivenHawk() + mutationAmount);
								}
								else
								{
									l->setProbDoveGivenHawk(Wasps[now][wwk]->getProbDoveGivenHawk() );
								}
							}
						}
						else
						{
							l->setProbDoveGivenHawk(Wasps[now][wwk]->getProbDoveGivenHawk());
						}
						///////// MUTATION OF KPROD /////////////////////////////////
						double MUTATION_DISCRIMINANT3 = (float)rand()/((float)RAND_MAX);   //// Mutation of KPROD
						if(MUTATION_DISCRIMINANT3 <= probMutationKProd )// && Wasps[now][wwk]->getName()  <= initWasps)
						{
							double MUTATION_DIRECTION3 = (float)rand()/((float)RAND_MAX);
							
							double ExtremeMUTATION_DISCRIMINANT = (float)rand()/((float)RAND_MAX);
							
							if(MUTATION_DIRECTION3 < 0.5 )  // mutation decreases KPROD
							{
								if(ExtremeMUTATION_DISCRIMINANT < probExtremeMutation && Wasps[now][wwk]->getKProd() >= extremeMutationKProd)
								{
									l->setKProd( Wasps[now][wwk]->getKProd() - extremeMutationKProd);
								}
								else if(Wasps[now][wwk]->getKProd() >= mutationKProd)
								{
									l->setKProd( Wasps[now][wwk]->getKProd() - mutationKProd);
								}
								else
								{
									l->setKProd(Wasps[now][wwk]->getKProd()  );
								}
							}
							else  //mutation increases KPROD.
							{
								if(ExtremeMUTATION_DISCRIMINANT < probExtremeMutation)
								{
									l->setKProd( Wasps[now][wwk]->getKProd() + extremeMutationKProd);
								}
								else
								{
									l->setKProd(Wasps[now][wwk]->getKProd() + mutationKProd );
								}
							}
							
						}
						else
						{
							l->setKProd(Wasps[now][wwk]->getKProd());
						}
						
						
						///////// MUTATION OF CHI /////////////////////////////////
						double MUTATION_DISCRIMINANT4 = (float)rand()/((float)RAND_MAX);   //// Mutation of CHI
						if(MUTATION_DISCRIMINANT4 <= probChiMutate) //// MUTATE IN CHI
						{
							double MUTATION_DIRECTION4 = (float)rand()/((float)RAND_MAX);
							double MUTATION_DISCRIMINANT5 =  (float)rand()/((float)RAND_MAX);
							if(MUTATION_DIRECTION4 < 0.5 )  // mutation decreases Chi
							{
								
								
								if(MUTATION_DISCRIMINANT5 < probGrossChiMutate)
								{
									if(Wasps[now][wwk]->getChi() >= GrossChi_increment)
									{
										l->setChi( Wasps[now][wwk]->getChi() - GrossChi_increment);
									}
									else 
									{
										l->setChi(Wasps[now][wwk]->getChi()  );
									}
								}
								else 
								{
									if(Wasps[now][wwk]->getChi() >= chi_increment)
									{
										l->setChi( Wasps[now][wwk]->getChi() - chi_increment);
									}
									else
									{
										l->setChi(Wasps[now][wwk]->getChi()  );
									}
									
								}
							}
							else 
							{
								if(MUTATION_DISCRIMINANT5 < probGrossChiMutate)
								{
									l->setChi( Wasps[now][wwk]->getChi() + GrossChi_increment);
								}
								else 
								{
									l->setChi(Wasps[now][wwk]->getChi() + chi_increment );
								}
								
								
							}
						}
						else
						{
							l->setChi(Wasps[now][wwk]->getChi());
						}
						
						
						
						
//////////////////////// END OF MUTATION CODE ////////////////////////////////////////////////////////////////////////////////////////						
									
						l->setNewX(Wasps[now][wwk]->getX() );  // offspring born at site of parental death.
						l->setNewY(Wasps[now][wwk]->getY() );
						
						int lgd = Wasps[now][wwk]->getCell()->getGpos(); //offspring on same grid
						Grids[lgd]->addWaspXY(l,1-now);  //find out what grid and ask wasp to grid for next
						Wasps[1-now].push_back(l); //add to 'next' wasp vector
                    
					}
				}
				delete Wasps[now][wwk]; //we delete THIS wasp after reproduction
			}
		}
		
		for(int gwk=0;gwk<maxGrids;gwk++)
		{
			Grids[gwk]->clearPops(now); ///clear grid pop //if grid.cpp -> in cell.cpp:"clear all elements in wasp vector ON THE CELL"
			//Instate if want to print out grida.
			if(twk%(100)==0)  //if(twk%(timeSteps/10)==0)
			{
				//cout << twk/10 << endl;
				Grids[gwk]->writeGrid(twk, gwk, 1- now);// output wasp location //////////////////////// "writeGrid(twk, gwk, 1- now)" is a command in grid.cpp that writes all the output files [e.g. H, D, Kai] // "Grids[gwk]->writeGrid" writes next grid => can see that in paper (pics)
			}
		}
		Wasps[now].clear();
		now = 1 - now; //next becomes now
	}	
	TOTAL.close(); //closing of output files
	TOTALD.close();
	TOTALH.close();

 ///// CLEAN UP 
    // it is good to get into the habit of deallocating with deletes all space created with new commands
    
    for(int iwk=0;iwk<maxGrids;iwk++)
	{
        Grids[iwk]->destroy(); // AFTER doing the whole simulation we ask grids to destroy their cells
    }
    Grids.clear(); //then we can delete the whole grid
	
	
	int maxWasps=Wasps[now].size();
	for(int iwk=0;iwk<maxWasps;iwk++)
	{
		
		delete Wasps[now][iwk];
		Wasps[now][iwk]= 0;
		
	}
	Wasps[now].clear(); 
	
	
	
	return (EXIT_SUCCESS);
}

	
	
