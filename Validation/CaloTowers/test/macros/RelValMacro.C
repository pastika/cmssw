#include "TFile.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TDirectory.h"
#include "TPaveText.h"
#include "TPaveStats.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

#include "CombinedCaloTowers.C"

using namespace std;

void prn(TString s0, TString s1) {
    std::cout << "\t>> " << s0 << ": " << s1 << std::endl;
}

void prn(TString s0, double d) {
    std::cout << "\t>> " << s0 << ": " << d << std::endl;
}

TDirectory* findDirectory( TDirectory *target, std::string s);
void ProcessRelVal(TFile &ref_file, TFile &val_file, ifstream &recstr, TString ref_vers, TString val_vers, int harvest = 0, bool bRBX = false, bool bHD = false);

void RelValMacro(TString ref_vers = "218", TString val_vers = "218", TString rfname, TString vfname, TString inputStream = "InputRelVal.txt") {

    ifstream RelValStream;

    RelValStream.open(InputStream);

    TFile Ref_File(rfname);
    TFile Val_File(vfname);
    
    //File Read 
    FILE * inputFile = NULL;
    if((inputFile = fopen(inputStream, "r")))
    {
        char buff[4096];
        char *c;
        while(!feof(inputFile) && (c = fgets(buff, 4096, inputFile)) != NULL)
        {
            char histName[128], ofileName[128], xAxisTitle[128];
            double xAxisMin, xAxisMax, yAxisMin, yAxisMax;
            char dimFlag[32], statFlag[32], chi2Flag[32], logFlag[32];
            int nRebin, draw, refCol, valCol;
            
            //The following lines allow for comments.  The first comment character (#) will be replaced with a end of string.
            char* k = strchr(buff, '#');
            if(k) *k = '\0';
            //Parse the line
            if(sscanf(buff, "%s %d %s %d %lf %lf %lf %lf %s %s %s %s %d %d %[^\n]", histName, &draw, ofileName, &nRebin, &xAxisMin, &xAxisMax, &yAxisMin, &yAxisMax, dimFlag, statFlag, chi2Flag, logFlag, &refCol, &valCol, xAxisTitle) == 15);
            {
                //Skip is set not to draw
                if(!draw) continue;
                //Make plot
                
            }
        }
        fclose(inputFile);                                                                                                                                                                                                         
    }
    else
    {
        std::cout << "Input file not found!!!" << std::endl;
    }

    ProcessRelVal(Ref_File, Val_File, RelValStream, HD_nHist1, HD_nHist2, HD_nHist2D, HD_nProfInd, HD_nHistTot, ref_vers, val_vers, harvest, false, true);

    ProcessSubDetCT(Ref_File, Val_File, RelValStream, CT_nHist1, CT_nHist2, CT_nProf, CT_nHistTot, ref_vers, val_vers, harvest);

    ProcessRelVal(Ref_File, Val_File, RelValStream, RH_nHist1, RH_nHist2, RH_nHist2D, RH_nProfInd, RH_nHistTot, ref_vers, val_vers, harvest, false, false);

    ProcessRelVal(Ref_File, Val_File, RelValStream, RBX_nHist1, 0, 0, 0, RBX_nHistTot, ref_vers, val_vers, harvest, true, false);


    Ref_File.Close();
    Val_File.Close();

    return;
}

void ProcessRelVal(TFile &ref_file, TFile &val_file, std::string ref_vers, std::string val_vers, std::string histName, std::string outLabel, int nRebin, double xAxisMin, double xAxisMax, double yAxisMin, double yAxisMax,
                   std::string dimSwitch, std::string statSwitch, std::string chi2Switch, std::string logSwitch, int refCol, int valCol, std::string xAxisTitle)
{
    prn("HistName", histName);

    TCanvas* myc = 0;
    TLegend* leg = 0;
    TPaveText* ptchi2 = 0;
    TPaveStats *ptstats_r = 0;
    TPaveStats *ptstats_v = 0;

    TH1F * ref_hist1[nHist1];
    TH2F * ref_hist2[nHist2];
    TProfile * ref_prof[nProfInd];
    TH1D * ref_fp[nProfInd];
    TH2F * ref_hist2D[nHist2D];

    TH1F * val_hist1[nHist1];
    TH2F * val_hist2[nHist2];
    TProfile * val_prof[nProfInd];
    TH1D * val_fp[nProfInd];
    TH2F * val_hist2D[nHist2D];

	//Make sure extra Profile info is also taken care of
    //if (DrawSwitch == 0) {
    //   if (dimSwitch == "TM") 
    //recstr >> ProfileLabel;
    //   continue;
    //}


    // Nasty trick:
    // recovering colors redefined in rootlogon.C (for "rainbow" Palette)
    Float_t r, g, b;
    Float_t saturation = 1;
    Float_t lightness = 0.5;
    Float_t maxHue = 280;
    Float_t minHue = 0;
    Int_t maxPretty = 50;
    Float_t hue;

    for (int j = 0; j < maxPretty; j++) {
        hue = maxHue - (j + 1)*((maxHue - minHue) / maxPretty);
        TColor::HLStoRGB(hue, lightness, saturation, r, g, b);
        TColor *color = (TColor*) (gROOT->GetListOfColors()->At(j + 51));
        color->SetRGB(r, g, b);
     }
     gStyle->SetPalette(1);

    //Format canvas
    if (dimSwitch == "PRwide") {
        gStyle->SetPadLeftMargin(0.06);
        gStyle->SetPadRightMargin(0.03);
        myc = new TCanvas("myc", "", 1200, 600);
    } else myc = new TCanvas("myc", "", 800, 600);
    myc->SetGrid();

    std::string xTitleCheck = xAxisTitle;
    xTitleCheck = xTitleCheck.substr(1, 7);

    //Format pad
    if(logSwitch == "Log" && dimSwitch == "2D")
    {
        myc->SetLogy(0);
        myc->SetLogz(1);
    }
    else if(logSwitch == "Log")
    {
        myc->SetLogy(1);
    }
    
    if (dimSwitch == "1D") 
    {
        //Get histograms from files
        ref_file.cd(RefHistDir);
        TH1* ref_hist1 = (TH1*) gDirectory->Get(histName);

        val_file.cd(ValHistDir);
        TH1* val_hist1 = (TH1*) gDirectory->Get(histName);

        // HACK to change what is embedded in DQM histos
        ref_hist1->GetXaxis()->SetLabelSize(0.04);
        val_hist1->GetXaxis()->SetLabelSize(0.04);
        ref_hist1->GetYaxis()->SetLabelSize(0.04);
        val_hist1->GetYaxis()->SetLabelSize(0.04);
        ref_hist1->GetXaxis()->SetTitleSize(0.045);
        val_hist1->GetXaxis()->SetTitleSize(0.045);

        ref_hist1->GetXaxis()->SetTickLength(-0.015);
        val_hist1->GetXaxis()->SetTickLength(-0.015);
        ref_hist1->GetYaxis()->SetTickLength(-0.015);
        val_hist1->GetYaxis()->SetTickLength(-0.015);

        ref_hist1->GetXaxis()->SetLabelOffset(0.02);
        val_hist1->GetXaxis()->SetLabelOffset(0.02);
        ref_hist1->GetYaxis()->SetLabelOffset(0.02);
        val_hist1->GetYaxis()->SetLabelOffset(0.02);

        ref_hist1->GetXaxis()->SetTitleOffset(1.3);
        val_hist1->GetXaxis()->SetTitleOffset(1.3);


        //Rebin histograms -- has to be done first
        if (nRebin != 1) {
            ref_hist1->Rebin(nRebin);
            val_hist1->Rebin(nRebin);
        }

        //Set the colors, styles, titles, stat boxes and format axes for the histograms
        ref_hist1->SetStats(kTRUE);
        val_hist1->SetStats(kTRUE);

        if (statSwitch != "Stat" && statSwitch != "Statrv") {
            ref_hist1->SetStats(kFALSE);
            val_hist1->SetStats(kFALSE);
        }

        //Min/Max Convetion: Default AxisMin = 0. Default AxisMax = -1.
        //xAxis
        if (xAxisMin == 0) xAxisMin = ref_hist1->GetXaxis()->GetXmin();
        if (xAxisMax < 0) xAxisMax = ref_hist1->GetXaxis()->GetXmax();

        if (xAxisMax > 0 || xAxisMin != 0) {
            ref_hist1->GetXaxis()->SetRangeUser(xAxisMin, xAxisMax);
            val_hist1->GetXaxis()->SetRangeUser(xAxisMin, xAxisMax);
        }
        //yAxis
        if (yAxisMin != 0) ref_hist1->SetMinimum(yAxisMin);
        if (yAxisMax > 0) ref_hist1->SetMaximum(yAxisMax);
        else if (ref_hist1->GetMaximum() < val_hist1->GetMaximum() &&
                val_hist1->GetMaximum() > 0) {
            if (logSwitch == "Log") ref_hist1->SetMaximum(2 * val_hist1->GetMaximum());
            else ref_hist1->SetMaximum(1.05 * val_hist1->GetMaximum());
        }

        //Title
        if (xTitleCheck != "NoTitle") ref_hist1->GetXaxis()->SetTitle(xAxisTitle);

        //Different histo colors and styles
        ref_hist1->SetTitle("");
        ref_hist1->SetLineColor(refCol);
        ref_hist1->SetLineStyle(1);
        ref_hist1->SetMarkerSize(0.02);
        if (statSwitch != "Stat" && statSwitch != "Statrv") ref_hist1->SetLineWidth(2);

        val_hist1->SetTitle("");
        val_hist1->SetLineColor(valCol);
        val_hist1->SetLineStyle(2);
        val_hist1->SetMarkerSize(0.02);
        if (statSwitch != "Stat" && statSwitch != "Statrv") val_hist1->SetLineWidth(2);

        //Legend
        TLegend *leg = new TLegend(0.50, 0.91, 0.84, 0.99, "", "brNDC");
        leg->SetBorderSize(2);
        leg->SetFillStyle(1001);
        leg->AddEntry(ref_hist1, "CMSSW_" + ref_vers, "l");
        leg->AddEntry(val_hist1, "CMSSW_" + val_vers, "l");

        if (chi2Switch == "Chi2") {
            //Draw and save histograms
            ref_hist1->SetFillColor(40);//42 Originally, now 40 which is lgiht brown
            ref_hist1->Draw("hist");
            val_hist1->SetLineStyle(1);
            if (statSwitch == "Statrv") val_hist1->Draw("sames e0");
            else val_hist1->Draw("same e0");

            //Get p-value from chi2 test
            const float NCHI2MIN = 0.01;

            float pval;
            char tempbuff[30];

            pval = ref_hist1->Chi2Test(val_hist1);

            sprintf(tempbuff, "Chi2 p-value: %6.3E", pval);

            ptchi2 = new TPaveText(0.05, 0.92, 0.35, 0.99, "NDC");

            if (pval > NCHI2MIN) ptchi2->SetFillColor(kGreen);
            else ptchi2->SetFillColor(kRed);

            ptchi2->SetTextSize(0.03);
            ptchi2->AddText(tempbuff);
            ptchi2->Draw();
        } else {
            //Draw and save histograms
            ref_hist1->Draw("hist");
            if (statSwitch == "Statrv") val_hist1->Draw("hist sames");
            else val_hist1->Draw("hist same");
        }

        //Stat Box where required
        if (statSwitch == "Stat" || statSwitch == "Statrv") {
            ptstats_r = new TPaveStats(0.85, 0.86, 0.98, 0.98, "brNDC");
            ptstats_r->SetTextColor(refCol);
            ref_hist1->GetListOfFunctions()->Add(ptstats_r);
            ptstats_r->SetParent(ref_hist1->GetListOfFunctions());
            ptstats_v = new TPaveStats(0.85, 0.74, 0.98, 0.86, "brNDC");
            ptstats_v->SetTextColor(valCol);
            val_hist1->GetListOfFunctions()->Add(ptstats_v);
            ptstats_v->SetParent(val_hist1->GetListOfFunctions());

            ptstats_r->Draw();
            ptstats_v->Draw();
        }

        leg->Draw();

        myc->SaveAs(outLabel);
    }
    //Profiles not associated with histograms
    else if (dimSwitch == "PR" || dimSwitch == "PRwide") 
    {
        //Get profiles from files
        ref_file.cd(RefHistDir);
        TProfile* ref_prof = (TProfile*) gDirectory->Get(histName);

        val_file.cd(ValHistDir);
        TProfile* val_prof = (TProfile*) gDirectory->Get(histName);

        // HACK to change what is embedded in DQM histos
        ref_prof->GetXaxis()->SetLabelSize(0.04);
        val_prof->GetXaxis()->SetLabelSize(0.04);
        ref_prof->GetYaxis()->SetLabelSize(0.04);
        val_prof->GetYaxis()->SetLabelSize(0.04);
        ref_prof->GetXaxis()->SetTitleSize(0.045);
        val_prof->GetXaxis()->SetTitleSize(0.045);

        ref_prof->GetXaxis()->SetTickLength(-0.015);
        val_prof->GetXaxis()->SetTickLength(-0.015);
        ref_prof->GetYaxis()->SetTickLength(-0.015);
        val_prof->GetYaxis()->SetTickLength(-0.015);

        ref_prof->GetXaxis()->SetLabelOffset(0.02);
        val_prof->GetXaxis()->SetLabelOffset(0.02);
        ref_prof->GetYaxis()->SetLabelOffset(0.02);
        val_prof->GetYaxis()->SetLabelOffset(0.02);

        ref_prof->GetXaxis()->SetTitleOffset(1.3);
        val_prof->GetXaxis()->SetTitleOffset(1.3);


        //Legend
        leg = new TLegend(0.50, 0.91, 0.84, 0.99, "", "brNDC");
        leg->SetBorderSize(2);
        leg->SetFillStyle(1001);

        //Ordinary profiles
        if (dimSwitch == "PR") 
        {
            ref_prof->SetTitle("");
            ref_prof->SetErrorOption("");

            val_prof->SetTitle("");
            val_prof->SetErrorOption("");

            ref_prof->GetXaxis()->SetTitle(xAxisTitle);

            if (statSwitch != "Stat" && statSwitch != "Statrv") {
                ref_prof->SetStats(kFALSE);
                val_prof->SetStats(kFALSE);
            }

            ref_prof->SetLineColor(41);
            ref_prof->SetLineStyle(1);
            ref_prof->SetLineWidth(1);
            ref_prof->SetMarkerColor(41);
            ref_prof->SetMarkerStyle(21);
            ref_prof->SetMarkerSize(0.8);

            val_prof->SetLineColor(43);
            val_prof->SetLineStyle(1);
            val_prof->SetLineWidth(1);
            val_prof->SetMarkerColor(43);
            val_prof->SetMarkerStyle(22);
            val_prof->SetMarkerSize(1.0);

            if (ref_prof->GetMaximum() < val_prof->GetMaximum() &&
                    val_prof->GetMaximum() > 0) {
                if (logSwitch == "Log") ref_prof->SetMaximum(2 * val_prof->GetMaximum());
                else ref_prof->SetMaximum(1.05 * val_prof->GetMaximum());
            }

            ref_prof->Draw("hist pl");
            val_prof->Draw("hist pl same");

            leg->AddEntry(ref_prof, "CMSSW_" + ref_vers, "pl");
            leg->AddEntry(val_prof, "CMSSW_" + val_vers, "pl");
        }//Wide profiles
        else if (dimSwitch == "PRwide") 
        {
            char temp[128];
            sprintf(temp, "%s_px_v", ref_prof->GetName());
            TH1* ref_fp = ref_prof->ProjectionX();
            TH1* val_fp = val_prof->ProjectionX(temp);

            ref_fp->SetTitle("");
            val_fp->SetTitle("");

            ref_fp->GetXaxis()->SetTitle(xAxisTitle);

            if (statSwitch != "Stat" && statSwitch != "Statrv") {
                ref_fp->SetStats(kFALSE);
                val_fp->SetStats(kFALSE);
            }

            int nbins = ref_fp->GetNbinsX();
            for (int j = 1; j < nbins; j++) {
                ref_fp->SetBinError(j, 0.);
                val_fp->SetBinError(j, 0.);
            }
            ref_fp->SetLineWidth(0);
            ref_fp->SetLineColor(0); // 5 yellow
            ref_fp->SetLineStyle(1);
            ref_fp->SetMarkerColor(2);
            ref_fp->SetMarkerStyle(20);
            ref_fp->SetMarkerSize(0.5);

            val_fp->SetLineWidth(0);
            val_fp->SetLineColor(0); // 45 blue
            val_fp->SetLineStyle(2);
            val_fp->SetMarkerColor(4);
            val_fp->SetMarkerStyle(22);
            val_fp->SetMarkerSize(0.5);

            if (ref_fp->GetMaximum() < val_fp->GetMaximum() &&
                    val_fp->GetMaximum() > 0) {
                if (logSwitch == "Log") ref_fp->SetMaximum(2 * val_fp->GetMaximum());
                else ref_fp->SetMaximum(1.05 * val_fp->GetMaximum());
            }

            ref_fp->Draw("p9");
            val_fp->Draw("p9same");

            leg->AddEntry(ref_fp, "CMSSW_" + ref_vers, "lp");
            leg->AddEntry(val_fp, "CMSSW_" + val_vers, "lp");

        }

        leg->Draw("");

        myc->SaveAs(outLabel);
    }//Timing Histograms (special: read two lines at once)
    else if (dimSwitch == "TM") 
    {
        // This section needs extra attention
        /*
        recstr >> HistName2;

        ref_file.cd(RefHistDir);

        ref_hist2[nh2] = (TH2F*) gDirectory->Get(histName);
        ref_prof = (TProfile*) gDirectory->Get(HistName2);

        ref_hist2[nh2]->SetMarkerStyle(21);
        ref_prof ->SetMarkerStyle(21);
        ref_hist2[nh2]->SetMarkerSize(0.02);
        ref_prof ->SetMarkerSize(0.02);

        val_file.cd(ValHistDir);

        val_hist2[nh2] = (TH2F*) gDirectory->Get(histName);
        val_prof = (TProfile*) gDirectory->Get(HistName2);

        val_hist2[nh2]->SetMarkerStyle(21);
        val_prof ->SetMarkerStyle(21);
        val_hist2[nh2]->SetMarkerSize(0.02);
        val_prof ->SetMarkerSize(0.02);

        // HACK to change what is embedded in DQM histos
        ref_hist2[nh2]->GetXaxis()->SetLabelSize(0.04);
        val_hist2[nh2]->GetXaxis()->SetLabelSize(0.04);
        ref_hist2[nh2]->GetYaxis()->SetLabelSize(0.04);
        val_hist2[nh2]->GetYaxis()->SetLabelSize(0.04);
        ref_hist2[nh2]->GetXaxis()->SetTitleSize(0.045);
        val_hist2[nh2]->GetXaxis()->SetTitleSize(0.045);

        ref_hist2[nh2]->GetXaxis()->SetTickLength(-0.015);
        val_hist2[nh2]->GetXaxis()->SetTickLength(-0.015);
        ref_hist2[nh2]->GetYaxis()->SetTickLength(-0.015);
        val_hist2[nh2]->GetYaxis()->SetTickLength(-0.015);

        ref_hist2[nh2]->GetXaxis()->SetLabelOffset(0.02);
        val_hist2[nh2]->GetXaxis()->SetLabelOffset(0.02);
        ref_hist2[nh2]->GetYaxis()->SetLabelOffset(0.02);
        val_hist2[nh2]->GetYaxis()->SetLabelOffset(0.02);

        ref_hist2[nh2]->GetXaxis()->SetTitleOffset(1.3);
        val_hist2[nh2]->GetXaxis()->SetTitleOffset(1.3);


        //Min/Max Convention: Default AxisMin = 0. Default AxisMax = -1.
        //xAxis
        if (xAxisMin == 0) xAxisMin = ref_hist2[nh2]->GetXaxis()->GetXmin();
        if (xAxisMax < 0) xAxisMax = ref_hist2[nh2]->GetXaxis()->GetXmax();

        if (xAxisMax > 0 || xAxisMin != 0) {
            ref_hist2[nh2]->GetXaxis()->SetRangeUser(xAxisMin, xAxisMax);
            val_hist2[nh2]->GetXaxis()->SetRangeUser(xAxisMin, xAxisMax);
        }
        //yAxis
        if (yAxisMin != 0) ref_hist2[nh2]->SetMinimum(yAxisMin);
        if (yAxisMax > 0) ref_hist2[nh2]->SetMaximum(yAxisMax);
        else if (ref_hist2[nh2]->GetMaximum() < val_hist2[nh2]->GetMaximum() &&
                val_hist2[nh2]->GetMaximum() > 0) {
            if (logSwitch == "Log") ref_hist2[nh2]->SetMaximum(2 * val_hist2[nh2]->GetMaximum());
            else ref_hist2[nh2]->SetMaximum(1.05 * val_hist2[nh2]->GetMaximum());
        }

        //AF
        if (yAxisMax > 0 || yAxisMin != 0) {
            ref_hist2[nh2]->GetYaxis()->SetRangeUser(yAxisMin, yAxisMax);
            val_hist2[nh2]->GetYaxis()->SetRangeUser(yAxisMin, yAxisMax);
        }

        //Legend
        leg = new TLegend(0.50, 0.91, 0.84, 0.99, "", "brNDC");
        leg->SetBorderSize(2);
        leg->SetFillStyle(1001);

        ref_hist2[nh2]->GetXaxis()->SetTitle(xAxisTitle);
        ref_hist2[nh2]->SetStats(kFALSE);

    ref_hist2[nh2]->SetTitle("");
        val_hist2[nh2]->SetTitle("");

        ref_hist2[nh2]->SetMarkerColor(refCol); // rose
        ref_hist2[nh2]->Draw();
        ref_prof->SetLineColor(41);
        ref_prof->Draw("same");

        val_hist2[nh2]->SetMarkerColor(valCol);
        val_hist2[nh2]->Draw("same");
        val_prof->SetLineColor(45);
        val_prof->Draw("same");

        leg->AddEntry(ref_prof, "CMSSW_" + ref_vers, "pl");
        leg->AddEntry(val_prof, "CMSSW_" + val_vers, "pl");

        leg->Draw("");

        myc->SaveAs(outLabel);

        npi++;
        nh2++;
        i++;*/
    } 
    else if (dimSwitch == "2D") 
    {

        myc->SetGrid(0, 0);

        //Get histograms from files
        ref_file.cd(RefHistDir);
        TH2* ref_hist2D = (TH2*) gDirectory->Get(histName);

        val_file.cd(ValHistDir);
        TH2* val_hist2D = (TH2*) gDirectory->Get(histName);

        ref_hist2D->SetStats(kFALSE);
        val_hist2D->SetStats(kFALSE);

        // HACK to change what is embedded in DQM histos
        ref_hist2D->GetXaxis()->SetLabelSize(0.04);
        val_hist2D->GetXaxis()->SetLabelSize(0.04);
        ref_hist2D->GetYaxis()->SetLabelSize(0.04);
        val_hist2D->GetYaxis()->SetLabelSize(0.04);
        ref_hist2D->GetXaxis()->SetTitleSize(0.045);
        val_hist2D->GetXaxis()->SetTitleSize(0.045);

        ref_hist2D->GetXaxis()->SetTickLength(-0.015);
        val_hist2D->GetXaxis()->SetTickLength(-0.015);
        ref_hist2D->GetYaxis()->SetTickLength(-0.015);
        val_hist2D->GetYaxis()->SetTickLength(-0.015);

        ref_hist2D->GetXaxis()->SetLabelOffset(0.02);
        val_hist2D->GetXaxis()->SetLabelOffset(0.02);
        ref_hist2D->GetYaxis()->SetLabelOffset(0.02);
        val_hist2D->GetYaxis()->SetLabelOffset(0.02);

        ref_hist2D->GetXaxis()->SetTitleOffset(1.3);
        val_hist2D->GetXaxis()->SetTitleOffset(1.3);

        ref_hist2D->SetTitle("");
        val_hist2D->SetTitle("");

        // special zoom on HB/HE depth1
        if (n2D == 1) {
            ref_hist2D->GetXaxis()->SetRangeUser(-29., 28.);
            val_hist2D->GetXaxis()->SetRangeUser(-29., 28.);
        }

        //AF
        //Min/Max Convetion: Default AxisMin = 0. Default AxisMax = -1.
        //xAxis
        if (xAxisMax > 0 || xAxisMin != 0) {
            ref_hist2D->GetXaxis()->SetRangeUser(xAxisMin, xAxisMax);
            val_hist2D->GetXaxis()->SetRangeUser(xAxisMin, xAxisMax);
        }
        //yAxis
        if (yAxisMax > 0 || yAxisMin != 0) {
            ref_hist2D->GetYaxis()->SetRangeUser(yAxisMin, yAxisMax);
            val_hist2D->GetYaxis()->SetRangeUser(yAxisMin, yAxisMax);
        }

        TLegend *leg1 = new TLegend(0.50, 0.91, 0.84, 0.99, "", "brNDC");
        leg1->SetBorderSize(2);
        leg1->SetFillStyle(1001);
        leg1->AddEntry(ref_hist2D, "CMSSW_" + ref_vers, "l");

        if (xTitleCheck != "NoTitle") ref_hist2D->GetXaxis()->SetTitle(xAxisTitle);
        ref_hist2D->Draw("colz");
        leg1->Draw();
        myc->SaveAs("ref_" + outLabel);


        TLegend *leg2 = new TLegend(0.50, 0.91, 0.84, 0.99, "", "brNDC");
        leg2->SetBorderSize(2);
        leg2->SetFillStyle(1001);
        leg2->AddEntry(val_hist2D, "CMSSW_" + val_vers, "l");

        if (xTitleCheck != "NoTitle") val_hist2D->GetXaxis()->SetTitle(xAxisTitle);
        val_hist2D->Draw("colz");
        leg2->Draw();
        myc->SaveAs("val_" + outLabel);
    }


    if (myc) delete myc;
    if (leg) delete leg;
    if (ptchi2) delete ptchi2;
    if (ptstats_r) delete ptstats_r;
    if (ptstats_v) delete ptstats_v;
    
    return;
}

TDirectory* findDirectory( TDirectory *target, std::string s)
{
    TDirectory *retval = 0;

    // loop over all keys in this directory                                                                                                                                                                                                  
    TIter nextkey(target->GetListOfKeys());
    TKey *key, *oldkey=0;
    while((key = (TKey*)nextkey()))
    {
        //keep only the highest cycle number for each key                                                                                                                                                                                    
        if (oldkey && !strcmp(oldkey->GetName(),key->GetName())) continue;

        // read object from file                                                                                                                                                                                                             
        target->cd();
        TObject *obj = key->ReadObj();

        if(obj->IsA()->InheritsFrom(TDirectory::Class()))
        {
            // it's a subdirectory                                                                                                                                                                                                           
            //cout << "Found subdirectory " << obj->GetName() << endl;                                                                                                                                                                       
            if(strcmp(s.c_str(), obj->GetName()) == 0) return (TDirectory*)obj;

            if((retval = fileDirectory((TDirectory*)obj, s))) break;

        }
    }
    return retval;
}
