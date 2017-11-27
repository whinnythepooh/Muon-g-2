/* 
*  Cleaned up version of fitSpectrum; compile as you have time
*  1: oneCaloAllXtal_bestFit (fitSpectrum1,2)
*  2: allCaloXtal_bestFit (fitSpectrum3)
*  3: allCaloXtal_fittedEnergyPlots_byXtal (fitSpectrum4,edited)
*  4: allCaloXtal_linePlotTrend (fitSpectrum5)
*/
#include "TCanvas.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TFile.h"
#include "TTree.h"
#include "TColor.h"
#include "TMath.h"
//C++ file reading packages
#include <iostream>
#include <fstream>
#include <string> //string constuctor
#include <cstring>
using namespace std;

void oneCaloAllXtal_bestFit(){ //macro for printing out 4 best fit lines on the histograms and getting a PDF out of only one Calo (w/ UI)

    TFile *file = new TFile("gm2ringsim_truth_32M.root");	//read ROOT file; ----- change as needed -----

    ofstream myfile;	//opening file; ----- change as needed -----
    myfile.open ("fit_results_junk.txt");

    TH1D *xtalHit;	//make a new histogram
    TLegend *leg;

    TCanvas *c1 = new TCanvas("c1","c1",1800,1200);	    //open a new canvas
    c1->Print("plots_bestfit_one_only.pdf("); //print PDF

    //which calo and xtal are analyzed w/ user input
    int n1;
    int n2;
    //printf("Cycling through each Xtal in a Calo.\n"); //----- change as needed -----
    printf("Cycling through each Calo for a Xtal.\n"); //----- change as needed -----
    printf("\n");
    printf("What Calo number to start??\n");
    scanf("%d", &n1);
    printf("What Xtal number?\n");
    scanf("%d", &n2);
    int calo = n1; // UI
    int xtal = n2; // UI

    int eEnd = 2600; //end fit energy

    //myfile<<"Calo "<<calo<<endl; //----- change as needed -----
    myfile<<"Xtal "<<xtal<<endl; //----- change as needed -----

    //----- loop over the entire calo; increment is 'xtal'; change if needed -----
    for (int calo=0; calo<24; calo++){

        //xtal hit data here
        xtalHit = (TH1D*) file->Get(Form("truthAnalyzer/calo%.2dxtalHit%.2d",calo,xtal));
        //myfile<<"Xtal: "<<xtal<<endl; //----- change as needed -----
        myfile<<"Calo: "<<calo<<endl; //----- change as needed -----

        //naming x,y-axis
        xtalHit->GetXaxis()->SetRangeUser(0,3000); //x-axis length
        xtalHit->GetYaxis()->SetRangeUser(0.5,6000); //y-axis length
        xtalHit->GetXaxis()->SetLabelSize(0.04); 
        xtalHit->GetYaxis()->SetLabelSize(0.04);
        xtalHit->GetXaxis()->SetTitle("E [MeV]");
        xtalHit->GetYaxis()->SetTitle("Events");
        c1->SetLogy();

        //draw histogram
        xtalHit->Draw();

        //setting up starting and increment energy levels
        int eStart = 300; //starting energy; arbitrary
        int eRange = 200; //increment
        int eInc[4]; //array of increment
        double energies[4]; //array to store x-intercept energy

        myfile<<"Range (MeV): "<<"               E (MeV): "<<endl;

        //loop over the entire xtal data, starting from eStart 
        for (int i=0; i<4; i++){

            eInc[i] = eRange*(1+i); //energy increment 

            TF1 *fit = new TF1("fit","[0]+[1]*x",eStart+eInc[i],eEnd); //using TFit function; p0 + p1*x
            xtalHit->Fit("fit","R");

            energies[i] = -fit->GetParameter(0)/fit->GetParameter(1);

            //to check if it works; this had been omitted from other fitSpectrum codes
            cout<<"The crossing point on x-axis is: "<<energies[i]<<"MeV"<<endl;
            cout<<endl;

            myfile<<"Start E: "<<eStart+eInc[i]<<" to "<<eEnd<<" --> "<<energies[i]<<endl;
        }

        TF1 *fit1 = new TF1("fit1","[0]+[1]*x",eStart+eInc[0],eEnd);
       	TF1 *fit2 = new TF1("fit2","[0]+[1]*x",eStart+eInc[1],eEnd);
       	TF1 *fit3 = new TF1("fit3","[0]+[1]*x",eStart+eInc[2],eEnd);
       	TF1 *fit4 = new TF1("fit4","[0]+[1]*x",eStart+eInc[3],eEnd);

       	fit1->SetLineColor(840); //kTeal
       	xtalHit->Fit("fit1","REMQ");
       	fit2->SetLineColor(880); //kViolet
       	xtalHit->Fit("fit2","REMQ+");
       	fit3->SetLineColor(632); //kRed
       	xtalHit->Fit("fit3","REMQ+");
       	fit4->SetLineColor(820); //kSpring
       	xtalHit->Fit("fit4","REMQ+");

        myfile<<endl;

        c1->Print("plots_bestfit_one_only.pdf");
    }

    //xtalHit1 = (TH1D*) file->Get(Form("truthAnalyzer/calo%.2dxtalHit%.2d",16,15));
    //xtalHit1->Draw();
    //xtalHit2 = (TH1D*) file->Get(Form("truthAnalyzer/calo%.2dxtalHit%.2d",17,15));
    //xtalHit2->Draw("same");
    
    myfile.close();

    c1->Print("plots_bestfit_one_only.pdf)");

}

void allCaloXtal_bestFit(){ //macro for printing out 4 best fit lines on the histograms and getting a PDF out

    TFile *file = new TFile("gm2ringsim_truth_32M.root");	//read ROOT file; ----- change as needed -----

    ofstream myfile;	//opening file; ----- change as needed -----
    myfile.open ("fit_results_junk.txt");

    TH1D *xtalHit; //make a new histogram
    
 	TCanvas *c1 = new TCanvas("c1","c1",1800,1200); 	//open a new canvas
    c1->Print("plots_bestfit.pdf("); //print PDF

    int eEnd = 2600; //end fit energy

    //which calo and xtal are analyzed
    int calo = 0; // starting from calo 0
    int xtal = 0; // starting from xtal 0

    //----- loop over the entire calo; increment is 'xtal' -----
    for (int calo=0; calo<24; calo++){

    	myfile<<"Calo: "<<calo<<endl; //------> prints Calo number

    	for (int xtal=0; xtal<54; xtal++){

        	//xtal hit data here (declared from line 19)
        	xtalHit = (TH1D*) file->Get(Form("truthAnalyzer/calo%.2dxtalHit%.2d",calo,xtal));

        	myfile<<"Xtal: "<<xtal<<endl; //------> prints Xtal number

        	//naming x,y-axis for best line fits and getting the histograms out
        	xtalHit->GetXaxis()->SetRangeUser(0,3000); //x-axis length
       		xtalHit->GetYaxis()->SetRangeUser(0.5,6000); //y-axis length
        	xtalHit->GetXaxis()->SetLabelSize(0.04); 
        	xtalHit->GetYaxis()->SetLabelSize(0.04);
        	xtalHit->GetXaxis()->SetTitle("E [MeV]");
        	xtalHit->GetYaxis()->SetTitle("Events");
        	c1->SetLogy();

        	//draw histogram
        	xtalHit->Draw();

        	//setting up starting and increment energy levels
        	double eStart = 300.0; //starting energy
        	double eRange = 200.0; //increment
        	double eInc[4]; //array of increment
        	double energies[4]; //array to store x-intercept energy

        	myfile<<"Range (MeV): "<<"            E (MeV): "<<endl; //------> headers

        	//loop over the entire xtal data, starting from eStart 
        	for (int i=0; i<4; i++){

            	eInc[i] = eRange*(1+i); //energy increment 


            	TF1 *fit = new TF1("fit","[0]+[1]*x",eStart+eInc[i],eEnd); //using TFit function; p0 + p1*x
            	xtalHit->Fit("fit","R");

            	energies[i] = - fit->GetParameter(0)/fit->GetParameter(1);

            	myfile<<"Start E: "<<eStart+eInc[i]<<" to "<<eEnd<<" --> "<<energies[i]<<endl; //------> headers 2

        		}

        	TF1 *fit1 = new TF1("fit1","[0]+[1]*x",eStart+eInc[0],eEnd);
        	TF1 *fit2 = new TF1("fit2","[0]+[1]*x",eStart+eInc[1],eEnd);
        	TF1 *fit3 = new TF1("fit3","[0]+[1]*x",eStart+eInc[2],eEnd);
        	TF1 *fit4 = new TF1("fit4","[0]+[1]*x",eStart+eInc[3],eEnd);

        	fit1->SetLineColor(840); //kTeal
        	xtalHit->Fit("fit1","R");
        	fit2->SetLineColor(880); //kViolet
        	xtalHit->Fit("fit2","R+");
        	fit3->SetLineColor(632); //kRed
        	xtalHit->Fit("fit3","R+");
        	fit4->SetLineColor(820); //kSpring
        	xtalHit->Fit("fit4","R+");

        	myfile<<endl;

        	c1->Print("plots_bestfit.pdf");//Print PDF

    	}


    }

    c1->Print("plots_bestfit.pdf)"); //Print PDF

}

void allCaloXtal_fittedEnergyPlots_byXtal(){

	//read ROOT file
    TFile *file = new TFile("gm2ringsim_truth_32M.root"); // --- change as needed ---

    //opening file
    ofstream myfile;
    myfile.open ("fit_results_junk.txt"); // --- change as needed ---

    //make a new histogram
    TH1D *xtalHit;
    TGraph *g[24]; //for MultiGraph
    TMultiGraph *mg = new TMultiGraph(); //multiGraph function in ROOT
    TLegend *leg;

    //open a new canvas
    TCanvas *c1 = new TCanvas("c1","c1",2000,1700);
    c1->Print("plots2.pdf("); //print PDF

    //which calo and xtal are analyzed
    int calo = 0; // calo 0
    int xtal = 0; // starting from xtal 0

    //setting up x,y-axis for fitEnergies graph: 54*4 for per Calo, and 24*4 for per Xtal ***
    double fitEnergies[96];

    double resultEnergies[24][4]; //24x4 matrix; used to loop through this from fitEnergies
    int resultIndex = 0; //for resultEnergies

    double endEnergies[4] = {500.0, 700.0, 900.0, 1100.0}; //remember to change this too!

    int index = 0; //index to rotate through fitEnergies in for-loop

    int xtalNumber = -1; //for naming the graphs later

    int markerStyle[4] = {20, 21, 22, 23};
    int styleIndex = 0;

    int markerColor[6] = {416, 432, 600, 616, 632, 800};
    int colorIndex = 0;

    //----- loop over the entire calo; increment is 'xtal' -----
    for (int xtal=0; xtal<54; xtal++){

    	myfile<<"Xtal: "<<xtal<<endl; //------> prints Xtal number

    	for (int calo=0; calo<24; calo++){

        	//xtal hit data here (declared from line 19)
        	xtalHit = (TH1D*) file->Get(Form("truthAnalyzer/calo%.2dxtalHit%.2d",calo,xtal));

        	myfile<<"Calo: "<<calo<<endl; //------> prints Calo number

        	//setting up starting and increment energy levels
        	double eStart = 300.0; //starting energy
        	double eRange = 200.0; //increment
        	double eInc[4]; //array of increment
        	double energies[4]; //array to store x-intercept energy

        	myfile<<"Range (MeV): "<<"            E (MeV): "<<endl; //------> headers

        	//loop over the entire xtal data, starting from eStart 
        	for (int i=0; i<4; i++){

            	eInc[i] = eRange*(1+i); //energy increment 

            	TF1 *fit = new TF1("fit","[0]+[1]*x",eStart+eInc[i],2600); //using TFit function; p0 + p1*x; 2600 is the arbitrary endpoint
            	xtalHit->Fit("fit","R");

            	energies[i] = - fit->GetParameter(0)/fit->GetParameter(1);

            	myfile<<"Start E: "<<eStart+eInc[i]<<" to 2600"<<" --> "<<energies[i]<<endl; //------> headers 2

                fitEnergies[index] = energies[i];
                index++;

                //rotating through fitEnergies[i] 
                if(index==96){
                    index=0;
                }else{
                    index=index;
                    }

                }

    	   }

        //looping through resultEnergies
        for (int j=0; j<96; j+=4){
            resultEnergies[resultIndex][0] = fitEnergies[j];
            resultEnergies[resultIndex][1] = fitEnergies[j+1];
            resultEnergies[resultIndex][2] = fitEnergies[j+2];
            resultEnergies[resultIndex][3] = fitEnergies[j+3];
            resultIndex++;
            if(resultIndex==25){
                resultIndex=0;
            }else{
            resultIndex=resultIndex;
            }
        }

        //to change graph name
        char xtalGraph[15];
        char xtalN[3];
        xtalNumber++;
        strcpy(xtalGraph, "Endpoint Energies for Xtal ");
        sprintf(xtalN, "%d", xtalNumber); //"string print function -> sprintf"
        strcat(xtalGraph, xtalN);

        leg = new TLegend(0.1, 0.65, 0.35, 0.9);
        leg->SetFillColor(0);

        //using MultiGraph
        for (int k=0; k<24; k++){
            char str[10];
            strcpy(str, "Calo ");
            char kk[5]; //char to replace 'k'; in order to get 'k' into the string name
            sprintf(kk, "%d", k);
            strcat(str, kk);

            //if-loop to change through styles and color
            g[k] = new TGraph(4, endEnergies, resultEnergies[k]);
            g[k]->SetMarkerStyle(markerStyle[styleIndex]);
            g[k]->SetMarkerColor(markerColor[colorIndex]);
            g[k]->SetLineColor(markerColor[colorIndex]);
            colorIndex++;
            if(colorIndex==6){	//if-loop to obtain 6 different colors and 4 different shapes
            	colorIndex=0;
            	styleIndex++;
            	if(styleIndex==4){
            		styleIndex=0;
            	}else{
            		styleIndex=styleIndex;
            	}
            }else{
            	colorIndex=colorIndex;
            }

            mg->Add(g[k]);
            leg->AddEntry(g[k], str);
        }

        mg->Draw("APL");
        mg->GetXaxis()->SetRangeUser(0,3000); //x-axis length
        mg->GetYaxis()->SetRangeUser(1000,3300); //y-axis length
        mg->GetXaxis()->SetLabelSize(0.04); 
        mg->GetYaxis()->SetLabelSize(0.04);
        mg->GetXaxis()->SetTitle("eStart (with eEnd = 2600)[MeV]");
        mg->GetYaxis()->SetTitle("Fitted Endpoint Energy [MeV]");
        mg->SetTitle("Endpoint Energy vs Different eEnd");

        leg->SetHeader(xtalGraph);
        leg->SetNColumns(6);
        leg->DrawClone("Same");

        c1->Print("plots2.pdf");

        //loop to reset the graph so that the multiGraph won't overload with previous instances of graphs
        for (int m=0; m<24; m++){
            g[m]->Set(0);
            mg->Add(g[m]);
        }

    }

    c1->Print("plots2.pdf)"); //Print PDF

}
void allCaloXtal_linePlotTrend(){ 
//prints out a line plot of the trend of fitted energy in a specific interval (user input) for all Xtals, Calo; no txt file

    TFile *file = new TFile("gm2ringsim_truth_32M.root"); // --- change as needed ---

    //user input
    int n1;
    int n2;
    printf("Cycling through each Calo for a Xtal.\n");
    printf("\n");
    printf("What is the starting energy? [MeV] \n");
    scanf("%d", &n1);
    printf("What is the end energy? [MeV] \n");
    scanf("%d", &n2);

    //make a new histogram
    TH1D *xtalHit;
    TLegend *leg;
    TGraph *graph;

    //open a new canvas
    TCanvas *c1 = new TCanvas("c1","c1",2000,1700);
    c1->Print("plots2.pdf("); //print PDF

    //which calo and xtal are analyzed
    int calo; // calo 0
    int xtal; // starting from xtal 0

    int startEnergy = n1;
    int endEnergy = n2;

    //storing arrays
    double caloNumbers[54];
    double fitEnergies[54];

    for (int i=0;i<54;i++){
        caloNumbers[i] = i*(1.0);
    }

    for (calo=0; calo<24; calo++){

        for (xtal=0; xtal<54; xtal++){

            xtalHit = (TH1D*) file->Get(Form("truthAnalyzer/calo%.2dxtalHit%.2d",calo,xtal));

            TF1 *fit = new TF1("fit","[0]+[1]*x",startEnergy,endEnergy); //using TFit function; p0 + p1*x; 2600 is the arbitrary endpoint
            
            xtalHit->Fit("fit","R");

            fitEnergies[xtal] = - fit->GetParameter(0)/fit->GetParameter(1);
        }


        char title[45];
        strcpy(title, "Fitted Energies vs. xtalNumber in Calo ");
        char number[5];
        sprintf(number, "%d", calo);
        strcat(title,number);

        graph = new TGraph(54, caloNumbers, fitEnergies);
        graph->SetLineColor(kRed);
        graph->Draw("APL");
        graph->GetXaxis()->SetRangeUser(0,55); //x-axis length
        graph->GetYaxis()->SetRangeUser(1500,2800); //y-axis length
        graph->GetXaxis()->SetLabelSize(0.04); 
        graph->GetYaxis()->SetLabelSize(0.04);
        graph->GetXaxis()->SetTitle("xtalNumber");
        graph->GetYaxis()->SetTitle("Fitted Endpoint Energy (500-2600) [MeV]");
        graph->SetTitle(title);

        c1->Print("plots2.pdf");

    }

    c1->Print("plots2.pdf)");

}