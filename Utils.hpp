// ROOT
#include "TGraph.h"
#include "TH1.h"
#include "TColor.h"
#include "TStyle.h"
#include "TCanvas.h"
#include <TFile.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TH1.h>
#include "Rtypes.h"
// GENERAL
#include <cstddef>
#include <iostream>

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
