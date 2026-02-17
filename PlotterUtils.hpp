// ROOT
#include "RtypesCore.h"
#include "TGraph.h"
#include "TString.h"
#include "TMultiGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TColor.h"
#include "TStyle.h"
#include "TLine.h"
#include "TCanvas.h"
#include <TFile.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TF1.h>
#include <TMath.h>
#include <TEfficiency.h>
#include <TGaxis.h>
#include "TLatex.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "Rtypes.h"
// GENERAL
#include <cstddef>
#include <string>
#include <vector>
#include <iostream>


// --- STYLE --------------------------------------------------------
int line_width = 6;

int canvas_width  = 1200;
int canvas_height = 900;

int   font = 42;
float title_font_size = 0.043;
float label_font_size = 0.04;
float title_offset_x = 1.2;
float label_offset_x = 0.007;
float label_offset_y = 0.01;
float title_offset_y = 1.12;


// --- COLORS -------------------------------------------------------
const TColor kOIBlueGreen(TColor::GetFreeColorIndex(), 0.00, 0.60, 0.50);
const TColor kOIRedPurple(TColor::GetFreeColorIndex(), 0.80, 0.60, 0.70);
const TColor kOIBlue(TColor::GetFreeColorIndex(),      0.00, 0.45, 0.70);
const TColor kOISkyBlue(TColor::GetFreeColorIndex(),   0.35, 0.70, 0.90);
const TColor kOIYellow(TColor::GetFreeColorIndex(),    0.95, 0.90, 0.25);
const TColor kOIOrange(TColor::GetFreeColorIndex(),    0.90, 0.60, 0.00);
const TColor kOIVermilion(TColor::GetFreeColorIndex(), 0.80, 0.40, 0.00);
const TColor kBlack_(TColor::GetFreeColorIndex(), 0., 0., 0.);

// Most used so far
// std::vector<TColor> TColor_list = {kOIOrange, kOISkyBlue, kOIBlueGreen,
//                                   kOIYellow, kOIBlue, kOIVermilion, kOIRedPurple};
// Rainbow - inspired
// std::vector<TColor> TColor_list = {kOIOrange, kOIYellow, kOISkyBlue, kOIBlue, kOIBlueGreen,
//                                     kOIVermilion, kOIRedPurple};
// std::vector<TColor> TColor_list = {kOIRedPurple, kOIVermilion, kOIOrange, kOIYellow,
//                                   kOISkyBlue, kOIBlue, kOIBlueGreen};
// std::vector<TColor> color_list = {kOIBlueGreen, kOIBlueGreen, kOIBlue, kOISkyBlue, kOIYellow,
                                  // kOIOrange, kOIVermilion, kOIRedPurple};
// DUNE
std::vector<TColor> TColor_list = {kOIOrange, kOISkyBlue, kOIVermilion,  
                                  kOIBlueGreen, kOIRedPurple, kOIBlue, kOIYellow, kBlack_};  

// Create Warmo<->Cold Palettes according to DUNE's kOIBlue and kOIRedPurple
std::vector<Int_t> createColdToWarmPalette(Int_t nSteps) {
    std::vector<Int_t> coldToWarmColors;

    // Define color stops (approximate cold to warm sequence)
    const Int_t nStops = 5;
    Double_t stops[nStops]   = {0.0, 0.25, 0.5, 0.75, 1.0};  // Equal-ish spread

    Double_t red[nStops]     = {0.00, 0.35, 0.95, 0.90, 0.80};
    Double_t green[nStops]   = {0.45, 0.70, 0.90, 0.60, 0.40};
    Double_t blue[nStops]    = {0.70, 0.90, 0.25, 0.00, 0.00};

    // Generate gradient color table
    Int_t firstIndex = TColor::CreateGradientColorTable(nStops, stops, red, green, blue, nSteps);

    // Fill the vector with color indices
    for (int i = 0; i < nSteps; ++i) {
        coldToWarmColors.push_back(firstIndex + i);
    }

    return coldToWarmColors;
}

inline std::vector<Int_t> createWarmToColdPalette(Int_t nSteps){
    std::vector<Int_t> warmToColdColors;

    // Define color stops (approximate warm to cold sequence)
    const Int_t nStops = 5;
    Double_t stops[nStops]   = {0.0, 0.20, 0.45, 0.75, 1.0};  // Equal-ish spread

    Double_t red[nStops]     = {0.80, 0.90, 0.95, 0.35, 0.00};
    Double_t green[nStops]   = {0.40, 0.60, 0.90, 0.70, 0.45};
    Double_t blue[nStops]    = {0.00, 0.00, 0.25, 0.90, 0.70};

    // Generate gradient color table
    Int_t firstIndex = TColor::CreateGradientColorTable(nStops, stops, red, green, blue, nSteps);

    // Fill the vector with color indices
    for (int i = 0; i < nSteps; ++i) {
        warmToColdColors.push_back(firstIndex + i);
    }

    return warmToColdColors;
}

inline std::vector<Int_t> set_palette(std::string palette = "Default", int n_colors = 0) {
  if (palette == "ColdToWarm") {
    return createColdToWarmPalette(n_colors);
  } else if (palette == "WarmToCold") {
    return createWarmToColdPalette(n_colors);
  } else {
    std::vector<Int_t> default_colors;
    for (const auto& color : TColor_list) {
      default_colors.push_back(color.GetNumber());
    }
    return default_colors;
  }
}

// --- WATERMARKS AND LABELS ----------------------------------------
inline TLatex * TextLabel(const std::string & text, double xLoc, double yLoc, short color=kBlue,
    ETextAlign hAlign=kHAlignLeft, ETextAlign vAlign=kVAlignTop)
{
  auto txtObj = new TLatex(xLoc, yLoc, text.c_str());
  txtObj->SetTextColor(color);
  txtObj->SetNDC();
  txtObj->SetTextSize(2 / 30.);
  txtObj->SetTextAlign(hAlign + vAlign);
  txtObj->Draw();

  return txtObj;
}
inline TLatex * TextLabel(const std::string & text, ETextAlign align, bool inFrame=true, short color=kBlue)
{
  auto hAlign = static_cast<ETextAlign>(align - (align % 10));
  auto vAlign = static_cast<ETextAlign>(align % 10);
  float xloc = (hAlign == kHAlignRight) ? 0.85 : ((hAlign == kHAlignLeft) ? 0.18 : 0.525);
  float yloc = (vAlign == kVAlignTop) ? (inFrame ? 0.87 : 0.96) : ((vAlign == kVAlignBottom) ? 0.13 : 0.5);
  return TextLabel(text, xloc, yloc, color, hAlign, vAlign);
}
/// Return the "DUNE" part of the watermark string, which may have its own styling
inline std::string DUNEWatermarkString()
{
  return "#font[62]{DUNE}";
}

/// Write a "DUNE Preliminary" tag
inline TLatex* Preliminary(ETextAlign loc=static_cast<ETextAlign>(kHAlignLeft + kVAlignTop), bool inFrame=true)
{
  return TextLabel(DUNEWatermarkString() + " Preliminary", loc, inFrame, kBlack);
}

/// My tag
inline TLatex* MyTag(ETextAlign loc=static_cast<ETextAlign>(kHAlignRight + kVAlignCenter), bool inFrame=true)
{
  auto my_tag = TextLabel("#font[62]{@Federico Galizzi}", loc, inFrame, kBlack);
  my_tag->SetTextSize(0.035);
  my_tag->SetTextAngle(270);
  return my_tag;
}


/// My tag
inline TLatex* CustomTag(std::string custom_tag, ETextAlign loc=static_cast<ETextAlign>(kHAlignRight + kVAlignCenter), bool inFrame=true)
{
  std::string label = "#font[62]{" + custom_tag + "}";
  auto my_tag = TextLabel(label, loc, inFrame, kBlack);
  my_tag->SetTextSize(0.035);
  return my_tag;
}



inline void SetMyStyle(){
  // Axis titles ------------------------------------------
  gStyle->SetTitleFont(font, "xyz");
  gStyle->SetTitleFontSize(title_font_size);
  gStyle->SetTitleOffset(title_offset_x, "x");
  gStyle->SetTitleOffset(title_offset_y, "y");
  
  // Axis labels ------------------------------------------
  gStyle->SetLabelFont(font, "xyz");
  gStyle->SetLabelSize(label_font_size, "xyz");
  gStyle->SetTitleOffset(title_offset_x, "x");
  gStyle->SetLabelOffset(label_offset_y, "y");

  gStyle->SetPadTickX(1); // Ticks on the upper border
  gStyle->SetPadTickY(1);
  gStyle->SetTickLength(0.033, "xyz");
  gStyle->SetNdivisions(506, "xyz");

  // Pad --------------------------------------------------
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.11);
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetPadBottomMargin(0.15);

  // Frame ------------------------------------------------
  gStyle->SetFrameLineWidth(1);

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
                     bool show_marker = false,
                     double marker_alpha = 0.5){
  const TColor black(TColor::GetFreeColorIndex(), 0.8, 0.4, 0.);
  int ncolor;
  if (idx_color==-1) ncolor = 1;
  else               ncolor = idx_color;
  // else               ncolor = TColor_list[idx_color % TColor_list.size()].GetNumber();

  h->SetLineWidth(line_width);
  h->SetLineColor(ncolor);
  h->SetMarkerColor(ncolor);
  h->SetFillColorAlpha(ncolor, fill_alpha);
 
  if (show_marker){
    h->SetMarkerStyle(21);
    h->SetLineColorAlpha(ncolor, marker_alpha);
  }

}

// --- GRAPHS -------------------------------------------------------
template<typename T>
T* g_allign_maximum(T* g, double max_position_percentage){
  std::vector<double> y, x;

  int n = g->GetN();

  for (int i = 0; i < n; ++i) {
    x.push_back(g->GetX()[i]);
    y.push_back(g->GetY()[i]);
  }

  int max_index = std::distance(y.begin(), std::max_element(y.begin(), y.end())); 
  vector_roll(y, -max_index+int(max_position_percentage*double(n)));

  T* g2 = new T(n, &x[0], &y[0]);

  return g2;
}


template<typename T>
void g_normalize(T* g, double scale, bool norm=1){
  double max = TMath::MaxElement(g->GetN(), g->GetY());
  if (!norm) max = 1.;
  for (int i=0; i<g->GetN(); i++){
    g->SetPointY(i, g->GetPointY(i)*scale/max);
    if (g->InheritsFrom(TGraphErrors::Class()) || g->InheritsFrom(TGraphAsymmErrors::Class()))
      g->SetPointError(i, g->GetErrorX(i), g->GetErrorY(i)*scale/max);
  }
}

inline void g_xshift(TGraph* graph, double shift) {
  int n = graph->GetN();
  double* x = graph->GetX();

  for (int i = 0; i < n; ++i) x[i] += shift;
}

inline void g_xrescale(TGraph* graph, double scale) {
  int n = graph->GetN();
  double* x = graph->GetX();

  for (int i = 0; i < n; ++i) x[i] *= scale;
}

// --- LINES ---------------------------------------------------------
inline TLine* make_line(double x1, double y1, double x2, double y2,
                 int color = kBlack, int width = 2, int style = 1) {
  std::cout << x1 << " " << x2 << " " << y1 << " " << y2 << std::endl;
  TLine* line = new TLine(x1, y1, x2, y2);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  line->SetLineStyle(style);
  return line;
}

inline TLine* make_horizontal_line(double y, TMultiGraph* mg,
                            int color = kBlack, int width = 2, int style = 1,
                            double x_min = 0., double x_max = 0.) {
  if (x_min == x_max) {
    x_min = mg->GetXaxis()->GetXmin();
    x_max = mg->GetXaxis()->GetXmax();
  }
  return make_line(x_min, y, x_max, y, color, width, style);
}

inline TLine* make_vertical_line(double x, TMultiGraph* mg,
                          int color = kBlack, int width = 2, int style = 1,
                          double y_min = 0., double y_max = 0.) {
  if (y_min == y_max) {
    y_min = mg->GetYaxis()->GetXmin();
    y_max = mg->GetYaxis()->GetXmax();
  }
  return make_line(x, y_min, x, y_max, color, width, style);
}


// --- LEGENDS -------------------------------------------------------
inline TLegend* build_legend(TCanvas* gc, const char* opt="lpe",
                      double x1=0.7, double y1=0.7, double x2=0.9, double y2=0.9,
                      int n_columns=1, float margin=0.4, float fill_alpha=0.0) {
  TLegend* legend = gc->BuildLegend(x1, y1, x2, y2);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->SetTextSize(0.037);
  legend->SetTextFont(42);
  legend->SetNColumns(n_columns);;
  legend->SetEntrySeparation(0.2);
  legend->SetMargin(margin);
  if (fill_alpha > 0.0) legend->SetFillStyle(1001); // Solid fill
  else                   legend->SetFillStyle(0); // No fill
  legend->SetFillColorAlpha(kWhite, fill_alpha);

  TList* entries = legend->GetListOfPrimitives();

  TIter next(entries);
    TObject* obj;
    while ((obj = next())) {
    TLegendEntry* entry = dynamic_cast<TLegendEntry*>(obj);
    if (entry) {
      entry->SetOption(opt);
    }
  }

  return legend;
}

