#include "Rtypes.h"
#include "TEfficiency.h"
#include "TStyle.h"
#include "PlotterUtils.hpp"
#include "TVirtualPad.h"
#include "TPaveStats.h"
#include <string>



// The macro plots either TH1 with the same name but contained
// in different files, or TH1 with different name, but in the
// same root file.
std::vector<TString> root_files   =  {
  "~/PhD/LowEnergy/flashmatch/data/testing_MLL_FM/nuecc/MLL_LightGBM_efficiency_dune10kt_charge_preselected.root",
};

// If any subfolder, name can be "subfolder1/th1_name;1"
std::vector<TString> histo_names = {
  "he_eff_drift_model_e_6;1",
  "he_eff_drift_model_e_6;1",
  "he_eff_drift_model_e_8;1",
  "he_eff_drift_model_e_10;1",
  "he_eff_drift_model_e_12;1",
  "he_eff_drift_model_e_14;1",
  "he_eff_drift_model_e_16;1",
  "he_eff_drift_model;1",
};

// Very comfy to quickly save the canvas from the GUI
TString canvas_name = "./projects/FM_MLL/FDHD_Central_NueCC_EffvsDrift_EnergyScan_ChargeCalib_Preselected_GBDT";

// Leave it empty if the th1_names are already descriptive
std::vector<TString> histo_titles = {
  "MeV",
  " 6",
  " 8",
  " 10",
  " 12",
  " 14",
  " 16",
  " [4-30]",
};

std::vector<int> color_idxs = {8, 7, 0, 1, 2, 3, 4, 5
};

// --- Canvas -------------------------------------------------------
TString canvas_title = "GBDT";
bool horizontal_canvas = 0; // True 16:9, False 4:3
// --- Fit ----------------------------------------------------------
bool single_fit = 0;
// --- Palette ------------------------------------------------------
// Choose among: Baseline, ColdToWarm, WarmToCold, DuneWhite
// If unknown, it will use the Baseline palette
std::string palette = "DuneWhite";
// --- Axis title ---------------------------------------------------
TString title_x = "Drift Distance [cm]";
TString title_y = "Matching Efficiency";
// --- Log scale and grid -------------------------------------------
bool log_x  = 0;
bool log_y  = 0;
bool grid_h = 0;
bool grid_v = 0;
// --- Convert efficiency to histogram ------------------------------
bool convert_eff_to_histo = 0;
// --- Rebinning ----------------------------------------------------
int rebin = 0; // If rebin<=0 no rebinning
// --- Axis range ---------------------------------------------------
// If (low==0 and up==0) it will set it automatically
bool use_custom_frame = 1;
double x_axis_low = 5.;
double x_axis_up  = 360.;
double y_axis_low = 0.;
double y_axis_up  = 1.05;
double min_y = 1.; // If using log_y, set it > 0
// --- Graph settings -----------------------------------------------
// Draw Option Ref: https://root.cern.ch/doc/master/classTGraphPainter.html
// NB: "hist" remove the fit, if any
// To use "hist" on TEfficiency, set convert_eff_to_histo = 1
std::string draw_option = "hist"; // only marker and errors "E", simple histo and errors "hist E"
int h_line_width = 2; // Line width of the TH1(s)
float fill_alpha = 0.3; // Opacity of filling color
bool show_marker  = 1;
double marker_size = 0.5;
double marker_alpha = 1.;
bool force_black_line = 0; // Force black line color for all histos, ignoring the palette
// --- Legend settings ----------------------------------------------
// Draw Option Ref: https://root.cern.ch/doc/master/classTGraphPainter.html
const char* entry_opt = "l"; // Options for the legend entries
int legend_ncolumns = 1;
float legend_fill_alpha = 1.0; // Fill alpha of the legend
float margin = 0.24;
double legend_x1 = 0.18;
double legend_y1 = 0.17;
double legend_x2 = 0.35;
double legend_y2 = 0.42;
bool plot_legend = 1;
// --- OptStat ------------------------------------------------------
bool  show_stat_box = 1;
float stat_box_alpha = 1.0; // Opacity of the stat box
// --- Dune Marker --------------------------------------------------
TString dune_marker = ""; // Options: "preliminary", "simulation"
                               // "mytag"



// ***************************************************************
// ***************************************************************

void Histo_1D(){
  SetMyStyle();
  // gStyle->SetOptFit(1111);
  
  std::vector<Int_t> color_list = set_palette(palette, histo_names.size()); 
  
  double x_min = 1e100;
  double x_max = -1e100;
  double y_min = 1.e100;
  double y_max = -1.e100;

  bool loop_on_files, loop_on_tgraphs;
  size_t n_graph = 0;
  if (root_files.size()>histo_names.size()){
    std::cout << "Looping on files" << std::endl;
    n_graph = root_files.size();
    loop_on_files = true;
    loop_on_tgraphs = false;
  }
  else if (root_files.size()<histo_names.size()){
    std::cout << "Looping on tgraphs" << std::endl;
    n_graph = histo_names.size();
    loop_on_files = false;
    loop_on_tgraphs = true;
  }
  else{
    std::cout << "Looping on both" << std::endl;
    n_graph = root_files.size();
    loop_on_files = true;
    loop_on_tgraphs = true;
  }

  std::vector<TH1*> histos;
  std::vector<TEfficiency*> efficiencies;

  for (size_t i = 0; i < n_graph; i++) {
    TString file_name = loop_on_files ? root_files[i] : root_files[0];
    TFile* input_file = TFile::Open(file_name);
    if (!input_file || input_file->IsZombie()) continue;

    TString obj_name = loop_on_tgraphs ? histo_names[i] : histo_names[0];
    TObject* obj = input_file->Get(obj_name);
    if (!obj) {
      std::cout << "Object " << obj_name << " not found in " << file_name << std::endl;
      continue;
    }

    if (obj->InheritsFrom(TH1::Class()) || convert_eff_to_histo) {
      TH1* h = nullptr;
      if (obj->InheritsFrom(TH1::Class())) {
        h = static_cast<TH1*>(obj);
      }
      else if (obj->InheritsFrom(TEfficiency::Class())) {
        TEfficiency* eff = static_cast<TEfficiency*>(obj);
        h = get_histo_from_eff(eff);
      }

      if (!h) continue;
      if (rebin>1) h->Rebin(rebin);

      int color_idx = (color_idxs.size()>0) ? color_idxs[i] : i;
      if (single_fit) make_histo_cute(h, h_line_width, -1,
                                      fill_alpha, show_marker, marker_size, marker_alpha);
      else            make_histo_cute(h, h_line_width,  color_list[color_idx%color_list.size()],
                                      fill_alpha, show_marker, marker_size, marker_alpha);

      if (force_black_line) h->SetLineColor(kBlack);
      
      if (!histo_titles.empty()) h->SetTitle(histo_titles[i]);

      histos.push_back(h);

      if (x_min > h->GetXaxis()->GetXmin()) x_min = h->GetXaxis()->GetXmin();
      if (x_max < h->GetXaxis()->GetXmax()) x_max = h->GetXaxis()->GetXmax();
      if (y_min > h->GetMinimum()) y_min = h->GetMinimum();
      if (y_max < h->GetMaximum()) y_max = h->GetMaximum();
    }
    else if (obj->InheritsFrom(TEfficiency::Class())) {
      TEfficiency* eff = static_cast<TEfficiency*>(obj);
      if (!eff) continue;


      int color_idx = (color_idxs.size()>0) ? color_idxs[i] : i;
      if (single_fit) make_histo_cute(eff, h_line_width, -1,
                                      fill_alpha, show_marker, marker_size, marker_alpha);
      else            make_histo_cute(eff, h_line_width,  color_list[color_idx%color_list.size()],
                                      fill_alpha, show_marker, marker_size, marker_alpha);
      
      if (!histo_titles.empty()) eff->SetTitle(histo_titles[i]);

      efficiencies.push_back(eff);

      auto* htot = eff->GetTotalHistogram();
      if (htot) {
        if (x_min > htot->GetXaxis()->GetXmin()) x_min = htot->GetXaxis()->GetXmin();
        if (x_max < htot->GetXaxis()->GetXmax()) x_max = htot->GetXaxis()->GetXmax();
      }

      y_min = 0.0;
      y_max = 1.05;
    }
  }

  std::cout << x_min << " " << x_max << " " << y_min << " " << y_max << std::endl;
  std::cout << "Eff size: " << efficiencies.size() << std::endl;
  std::cout << "Histo size: " << histos.size() << std::endl;

  std::cout << "Canvas name: " << canvas_name << std::endl;
  if (horizontal_canvas){
    canvas_width = 1600;
    title_offset_y *= 0.7;
  }
  TCanvas* gc = new TCanvas(canvas_name, canvas_name, 0, 0, canvas_width, canvas_height);
  gc->cd();

  if (y_min == 0 && log_y) y_min = min_y;
  
  if (!log_x) {
    double dx = x_max - x_min;
    x_min = x_min - dx*0.04;
    x_max = x_max + dx*0.04;
  }
  else {
    x_min = x_min - abs(x_min*0.55);
    x_max = x_max + abs(x_max*0.55);
  }

  if (!log_y) {
    double dy = y_max - y_min;
    if (y_min != 0) y_min = y_min - dy*0.05;
    y_max = y_max + dy*0.05;
  }
  else {
    if (y_min != 0.) y_min = y_min - abs(y_min*0.55);
    y_max = y_max + abs(y_max*0.55);
  }


  std::cout << "Frame" << std::endl;
  TH1* frame;
    // if (show_marker) h->Draw("hist same E2");
  std::string s_first_draw_option = draw_option;
  std::string s_other_draw_option = "SAME "+s_first_draw_option;
  const char* first_draw_option = s_first_draw_option.c_str();
  const char* other_draw_option = s_other_draw_option.c_str();

  if (use_custom_frame){
    std::cout << "Using custom frame" << std::endl;
    if (x_axis_low!=0. || x_axis_up!=0.) {
      x_min = x_axis_low;
      x_max = x_axis_up;
    }
    if (y_axis_low!=0. || y_axis_up!=0.) {
      y_min = y_axis_low;
      y_max = y_axis_up;
    }
    frame = gc->DrawFrame(x_min, y_min, x_max, y_max);
  }
  else if (single_fit) {
    gStyle->SetOptStat(1110);
    frame = static_cast<TH1*>(histos[0]->Clone("frame_histo"));
    frame->SetDirectory(nullptr);
  }
  else if (histos.size()>efficiencies.size()){
    frame = histos[0];
  }

  std::cout << "Framing" << std::endl;
  frame->SetTitle(canvas_title);
  frame->GetXaxis()->CenterTitle();                  frame->GetYaxis()->CenterTitle();
  frame->GetXaxis()->SetTitle(title_x);              frame->GetYaxis()->SetTitle(title_y);
  frame->GetXaxis()->SetTitleFont(font);             frame->GetYaxis()->SetTitleFont(font);
  frame->GetXaxis()->SetTitleSize(title_font_size);  frame->GetYaxis()->SetTitleSize(title_font_size);
  frame->GetXaxis()->SetTitleOffset(title_offset_x); frame->GetYaxis()->SetTitleOffset(title_offset_y);
  frame->GetXaxis()->SetLabelFont(font);             frame->GetYaxis()->SetLabelFont(font);
  frame->GetXaxis()->SetLabelSize(label_font_size);  frame->GetYaxis()->SetLabelSize(label_font_size);
  frame->GetXaxis()->SetLabelOffset(label_offset_x); frame->GetYaxis()->SetLabelOffset(label_offset_y);
  frame->GetXaxis()->SetTickLength(0.02);            frame->GetYaxis()->SetTickLength(0.02);


  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  if (!grid_v) gc->SetTickx(0);
  if (!grid_h) gc->SetTicky(0);

  if (use_custom_frame) frame->Draw();
  else                  frame->Draw(first_draw_option);

  std::cout << "Drawing histos" << std::endl;
  if (!single_fit) {
    for (auto& h : histos) {
      CenterTitles(h);
      h->Draw(other_draw_option);
    }
  }

  std::cout << "Drawing efficiencies" << std::endl;
  for (auto& eff : efficiencies) {
    eff->Draw("SAME LE5");
    if (!use_custom_frame){
      gPad->Update();
      auto* h_axis = eff->GetPaintedGraph();
      if (h_axis) {
        h_axis->GetXaxis()->CenterTitle();
        h_axis->GetYaxis()->CenterTitle();
      }
    }
  } 

  std::cout << "Drawing legend" << std::endl;
  if (plot_legend){
    auto legend = build_legend(gc, entry_opt,
                               legend_x1, legend_y1, legend_x2, legend_y2,
                               legend_ncolumns, margin, legend_fill_alpha);
    auto entries = legend->GetListOfPrimitives();
    auto entry = static_cast<TLegendEntry*>(entries->At(0));
    entries->Remove(entry);
    legend->Draw();
  }
  // gStyle->SetOptStat(0); gStyle->SetOptFit(0); gStyle->SetOptTitle(0);
  if (show_stat_box){
    gStyle->SetOptStat(1110);
    // delete box boarder
    gPad->Update();
    TPaveStats* stat_box = static_cast<TPaveStats*>(gPad->GetPrimitive("stats"));
    if (stat_box) {
      stat_box->SetBorderSize(0);
      stat_box->SetFillColorAlpha(kWhite, stat_box_alpha);
    }
  }
  gPad->RedrawAxis();
  if (dune_marker == "preliminary") Preliminary();
  if (dune_marker == "mytag") MyTag();
  gc->Modified(); gc->Update();
  return;
}
