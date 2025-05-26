// ROOT
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TH1.h"
#include "TColor.h"
#include "TStyle.h"
#include "TCanvas.h"
#include <TFile.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TH1.h>
#include <TMath.h>
#include <TEfficiency.h>
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "Rtypes.h"
// GENERAL
#include <cstddef>
#include <vector>
#include <iostream>


// --- STYLE --------------------------------------------------------
int line_width = 6;

int canvas_width = 1200;
int canvas_height = 900;

const TColor kOIOrange(TColor::GetFreeColorIndex(), 0.90, 0.60, 0);
const TColor kOISkyBlue(TColor::GetFreeColorIndex(), 0.35, 0.70, 0.90);
const TColor kOIBlueGreen(TColor::GetFreeColorIndex(), 0, 0.60, 0.50);
const TColor kOIYellow(TColor::GetFreeColorIndex(), 0.95, 0.90, 0.25);
const TColor kOIBlue(TColor::GetFreeColorIndex(), 0, 0.45, 0.70);
const TColor kOIVermilion(TColor::GetFreeColorIndex(), 0.80, 0.40, 0);
const TColor kOIRedPurple(TColor::GetFreeColorIndex(), 0.80, 0.60, 0.70);

std::vector<TColor> color_list = {kOIOrange, kOISkyBlue, kOIBlueGreen,
                                  kOIYellow, kOIBlue, kOIVermilion, kOIRedPurple};

void SetMyStyle(){
  // Axis titles ------------------------------------------
  gStyle->SetTitleFont(42, "xyz");
  gStyle->SetTitleFontSize(0.04);
  
  // Axis labels ------------------------------------------
  gStyle->SetLabelFont(42, "xyz");
  gStyle->SetLabelSize(0.04, "xyz");
  gStyle->SetLabelOffset(0.01, "y");

  gStyle->SetPadTickX(1); // Ticks on the upper border
  gStyle->SetPadTickY(1);
  gStyle->SetTickLength(0.033, "xyz");
  gStyle->SetNdivisions(506, "xyz");

  // Pad --------------------------------------------------
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadRightMargin(0.10);
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetPadBottomMargin(0.15);

  // Frame ------------------------------------------------
  gStyle->SetFrameLineWidth(3);

  // Palette 
  gStyle->SetPalette(kSunset); 
  gStyle->SetNumberContours(255);
}

inline void CenterTitles(TH1 *histo)
  {
    histo->GetXaxis()->CenterTitle();
    histo->GetYaxis()->CenterTitle();
    histo->GetZaxis()->CenterTitle();
  }

inline void CenterTitles(TGraph *histo)
  {
    histo->GetXaxis()->CenterTitle();
    histo->GetYaxis()->CenterTitle();
  }



template<typename T>
void vector_roll(std::vector<T>& vec, int roll){
  if (roll > 0) std::rotate(vec.begin(), vec.begin()+vec.size()-roll, vec.end());
  if (roll < 0) std::rotate(vec.begin(), vec.begin()-roll, vec.end());
  return;
}

// --- HISTOS -------------------------------------------------------
template<typename T>
void make_histo_cute(T h, int line_width = 6,
                        int idx_color = 1,
                        double fill_alpha = 0.,
                        bool show_marker = false){
        h->SetLineWidth(line_width);
        h->SetLineColor(color_list[idx_color % color_list.size()].GetNumber());
        h->SetMarkerColor(color_list[idx_color % color_list.size()].GetNumber());
        h->SetFillColorAlpha(color_list[idx_color % color_list.size()].GetNumber(), fill_alpha);

        if (show_marker){
          h->SetMarkerStyle(21);
          h->SetLineColorAlpha(color_list[idx_color % color_list.size()].GetNumber(),  0.5);
        }

      }

// --- GRAPHS -------------------------------------------------------
TGraph* g_allign_maximum(TGraph* g, double max_position_percentage){
  std::vector<double> y, x;

  int n = g->GetN();

  for (int i = 0; i < n; ++i) {
    x.push_back(g->GetX()[i]);
    y.push_back(g->GetY()[i]);
  }

  int max_index = std::distance(y.begin(), std::max_element(y.begin(), y.end())); 
  vector_roll(y, -max_index+int(max_position_percentage*double(n)));

  TGraph* g2 = new TGraph(n, &x[0], &y[0]);

  return g2;
}

void g_normalize(TGraph* g, double norm=1.){
  double max = TMath::MaxElement(g->GetN(), g->GetY());
  for (int i=0; i<g->GetN(); i++) g->SetPointY(i, g->GetPointY(i)*norm/max);
}
