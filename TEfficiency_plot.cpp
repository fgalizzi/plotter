#include "TFile.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include "PlotterUtils.hpp"

TString root_file   =  "./projects/NP04_PDS_article/SelfTrigger/AnaST_Ch_11121.root";
TString teff_name   =  "Run_32099/he_efficiency_nofit;1";
TString canvas_name = "./projects/NP04_PDS_article/SelfTrigger/EffHistoeee";

// --- Axis title ---------------------------------------------------
TString title_x = "PE";
TString histo_title_y = "Counts";
TString eff_title_y = "Efficiency";
// --- Axis range ---------------------------------------------------
// If (low==0 and up==0) it will set it automatically
// bool use_custom_topFrame = 1;
double x_axis_low = 0.;
double x_axis_up  = 0.;
double y_axis_low = 0.;
double y_axis_up  = 0.;
double min_y = 1.; // If using log_y, set it > 0
// --- Log scale and grid -------------------------------------------
bool log_x  = false;
bool log_y  = false;
bool hgrid_h = 0;
bool hgrid_v = 1;
// --- Graph settings -----------------------------------------------
// NB: "hist" remove the fit, if any
std::string draw_option = "h"; // only marker and errors "E", simple histo and errors "hist E"
int h_line_width = 1; // Line width of the TH1(s)
float fill_alpha = 1.; // Opacity of filling color
bool show_marker  = 1;
double marker_alpha = 1.;


void TEfficiency_plot(){
  SetMyStyle();
  
  font = 43;
  title_font_size = 35;
  label_font_size = 35;
  title_offset_x = 0.8;
  label_offset_x = 0.01;
  label_offset_y = 0.01;
  title_offset_y = 1.7;
  
  std::vector<Int_t> color_list = set_palette("", 2); 
  double x_min = 1e100;
  double x_max = -1e100;
  double y_min = 1.e100;
  double y_max = -1.e100;
  
  TFile* input_file = TFile::Open(root_file);
  if (!input_file || input_file->IsZombie()) {
    std::cout << "Error opening file: " << root_file << std::endl;
    return;
  }
  TEfficiency* teff = nullptr;
  input_file->GetObject(teff_name, teff);
  if (!teff) {
    std::cout << "Error: TEfficiency object " << teff_name << " not found in file " << root_file << std::endl;
    input_file->Close();
    return;
  }

  TH1D* h_total = (TH1D*)teff->GetTotalHistogram();
  make_histo_cute(h_total,  h_line_width, color_list[0], fill_alpha, show_marker, marker_alpha);
  TH1D* h_passed = (TH1D*)teff->GetPassedHistogram();
  make_histo_cute(h_passed, h_line_width, color_list[1], fill_alpha, show_marker, marker_alpha);

  TCanvas* gc = new TCanvas(canvas_name, canvas_name, 0, 0, canvas_width, canvas_height);
  TPad *topPad = new TPad("topPad", "", 0.005, 0.3, 0.995, 0.995);
  TPad *bottomPad = new TPad("bottomPad", "", 0.005, 0.005, 0.995, 0.3);
  topPad->Draw();
  bottomPad->Draw();
  topPad->SetBottomMargin(0.01);
  bottomPad->SetTopMargin(0.01);
  
  topPad->cd();
  TH1* topFrame;
  if (x_min > h_total->GetXaxis()->GetXmin()) x_min = h_total->GetXaxis()->GetXmin();
  if (x_max < h_total->GetXaxis()->GetXmax()) x_max = h_total->GetXaxis()->GetXmax();
  if (y_min > h_total->GetMinimum()) y_min = h_total->GetMinimum();
  if (y_max < h_total->GetMaximum()) y_max = h_total->GetMaximum()*1.05;
  if (x_axis_low!=0. || x_axis_up!=0.) {
    x_min = x_axis_low;
    x_max = x_axis_up;
  }
  if (y_axis_low!=0. || y_axis_up!=0.) {
    y_min = y_axis_low;
    y_max = y_axis_up;
  }
  topFrame = topPad->DrawFrame(x_min, y_min, x_max, y_max);
  topFrame->GetXaxis()->CenterTitle();                  topFrame->GetYaxis()->CenterTitle();
  topFrame->GetXaxis()->SetTitle("");                   topFrame->GetYaxis()->SetTitle(histo_title_y);
  topFrame->GetXaxis()->SetTitleFont(font);             topFrame->GetYaxis()->SetTitleFont(font);
  topFrame->GetXaxis()->SetTitleSize(0);                topFrame->GetYaxis()->SetTitleSize(title_font_size);
  topFrame->GetXaxis()->SetTitleOffset(title_offset_x); topFrame->GetYaxis()->SetTitleOffset(title_offset_y);
  topFrame->GetXaxis()->SetLabelFont(font);             topFrame->GetYaxis()->SetLabelFont(font);
  topFrame->GetXaxis()->SetLabelSize(0);                topFrame->GetYaxis()->SetLabelSize(label_font_size);
  topFrame->GetXaxis()->SetLabelOffset(label_offset_x); topFrame->GetYaxis()->SetLabelOffset(label_offset_y);
  topPad->SetLogy(log_y); topPad->SetLogx(log_x);
  topPad->SetGrid(hgrid_v, hgrid_h);
  
  
  h_total->Draw("hist same");
  h_passed->Draw("hist same");

  bottomPad->cd();
  TH1* bottomFrame;
  if (x_min > h_total->GetXaxis()->GetXmin()) x_min = h_total->GetXaxis()->GetXmin();
  bottomFrame = bottomPad->DrawFrame(x_min, -0.1, x_max, 1.05);
  bottomFrame->GetXaxis()->CenterTitle();                  bottomFrame->GetYaxis()->CenterTitle();
  bottomFrame->GetXaxis()->SetTitle(title_x);              bottomFrame->GetYaxis()->SetTitle(eff_title_y);
  bottomFrame->GetXaxis()->SetTitleFont(font);             bottomFrame->GetYaxis()->SetTitleFont(font);
  bottomFrame->GetXaxis()->SetTitleSize(title_font_size);  bottomFrame->GetYaxis()->SetTitleSize(title_font_size);
  bottomFrame->GetXaxis()->SetTitleOffset(title_offset_x); bottomFrame->GetYaxis()->SetTitleOffset(title_offset_y);
  bottomFrame->GetXaxis()->SetLabelFont(font);             bottomFrame->GetYaxis()->SetLabelFont(font);
  bottomFrame->GetXaxis()->SetLabelSize(label_font_size);  bottomFrame->GetYaxis()->SetLabelSize(label_font_size);
  bottomFrame->GetXaxis()->SetLabelOffset(label_offset_x); bottomFrame->GetYaxis()->SetLabelOffset(label_offset_y);
  bottomPad->SetLogy(log_y); bottomPad->SetLogx(log_x);
  bottomPad->SetGrid(hgrid_v, hgrid_h);
  bottomFrame->SetNdivisions(505, "y");
  teff->Draw("same");

  return;
}
