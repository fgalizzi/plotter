#include "Rtypes.h"
#include "TStyle.h"
#include "PlotterUtils.hpp"
#include <string>

// The macro plots either TH1 with the same name but contained
// in different files, or TH1 with different name, but in the
// same root file.
std::vector<TString> root_files   =  {
};

// If any subfolder, name can be "subfolder1/th1_name;1"
std::vector<TString> histo_names = {
};

// Very comfy to quickly save the canvas from the GUI
TString canvas_name = "";

// Leave it empty if the th1_names are already descriptive
std::vector<TString> histo_titles = {
};

std::vector<int> color_idxs = {
};

// --- Fit ----------------------------------------------------------
bool single_fit = 0;
// --- Palette ------------------------------------------------------
// Choose among: Baseline, ColdToWarm, WarmToCold
// If unknown, it will use the Baseline palette
std::string palette = "";
// --- Axis title ---------------------------------------------------
TString title_x = "";
TString title_y = "";
// --- Log scale and grid -------------------------------------------
bool log_x  = false;
bool log_y  = false;
bool grid_h = 1;
bool grid_v = 1;
// --- Rebinning ----------------------------------------------------
int rebin = 0; // If rebin<=0 no rebinning
// --- Axis range ---------------------------------------------------
// If (low==0 and up==0) it will set it automatically
bool use_custom_frame = 1;
// double x_axis_low = 0.;
// double x_axis_up  = 0.;
// double y_axis_low = 0.;
// double y_axis_up  = 0.;
double min_y = 1.; // If using log_y, set it > 0
// --- Graph settings -----------------------------------------------
// NB: "hist" remove the fit, if any
std::string draw_option = "hist"; // only marker and errors "E", simple histo and errors "hist E"
int h_line_width = 8; // Line width of the TH1(s)
float fill_alpha = 0.; // Opacity of filling color
bool show_marker  = 1;
float marker_alpha = 1;
// --- Legend settings ----------------------------------------------
int legend_ncolumns = 1;
float margin = 0.14;
const char* entry_opt = "l"; // Options for the legend entries
// --- Dune Marker --------------------------------------------------
TString dune_marker = ""; // Options: "preliminary", "simulation"



// ***************************************************************
// ***************************************************************

void Histo_1D(){
  SetMyStyle();
  gStyle->SetOptFit(0);
  
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

    if (obj->InheritsFrom(TH1::Class())) {
      TH1* h = static_cast<TH1*>(obj);
      if (!h) continue;
      if (rebin>1) h->Rebin(rebin);

      int color_idx = (color_idxs.size()>0) ? color_idxs[i] : i;
      if (single_fit) make_histo_cute(h, h_line_width, -1, fill_alpha, show_marker, marker_alpha);
      else            make_histo_cute(h, h_line_width,  color_list[color_idx%color_list.size()], fill_alpha, show_marker, marker_alpha);
      
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
      if (single_fit) make_histo_cute(eff, line_width, -1, fill_alpha, show_marker);
      else            make_histo_cute(eff, line_width,  color_list[color_idx%color_list.size()], fill_alpha, show_marker);
      
      if (!histo_titles.empty()) eff->SetTitle(histo_titles[i]);

      efficiencies.push_back(eff);
      std::cout << "dd" << std::endl;


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

  TCanvas* gc = new TCanvas(canvas_name, canvas_name, 0, 0, canvas_width, canvas_height);
  gc->cd();
# // Set transparent background

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


  TH1* frame;
    // if (show_marker) h->Draw("hist same E2");
  std::string s_first_draw_option = draw_option;
  std::string s_other_draw_option = "SAME "+s_first_draw_option;
  const char* first_draw_option = s_first_draw_option.c_str();
  const char* other_draw_option = s_other_draw_option.c_str();

  if (use_custom_frame){
    frame = gc->DrawFrame(x_min, y_min, x_max, y_max);
  }
  else if (histos.size()>efficiencies.size()){
    frame = histos[0];
  }

  frame->GetXaxis()->SetTitle(title_x);   frame->GetYaxis()->SetTitle(title_y); 
  frame->GetXaxis()->CenterTitle();       frame->GetYaxis()->CenterTitle();
  frame->GetXaxis()->SetTitleFont(42);    frame->GetYaxis()->SetTitleFont(42);
  frame->GetXaxis()->SetTitleOffset(1.5); frame->GetYaxis()->SetTitleOffset(1.3);
  frame->GetXaxis()->SetTitleSize(0.04);  frame->GetYaxis()->SetTitleSize(0.04);

  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
 
  if (use_custom_frame) frame->Draw();
  else                  frame->Draw(first_draw_option);
  for (auto& h : histos) {
    CenterTitles(h);
    h->Draw(other_draw_option);
  }

  for (auto& eff : efficiencies) {
    eff->Draw(other_draw_option);
    gPad->Update();
    auto* h_axis = eff->GetPaintedGraph()->GetHistogram();
    if (h_axis) {
      h_axis->GetXaxis()->CenterTitle();
      h_axis->GetYaxis()->CenterTitle();
    }
  } 

  auto legend = build_legend(gc, entry_opt,
                             0.7, 0.7, 0.9, 0.9,
                             legend_ncolumns, margin);
  // auto entries = legend->GetListOfPrimitives();
  // auto entry = static_cast<TLegendEntry*>(entries->At(0));
  // entries->Remove(entry);
  legend->Draw();
  gStyle->SetOptStat(0); gStyle->SetOptFit(0); gStyle->SetOptTitle(0);
  gPad->RedrawAxis();
  if (dune_marker == "preliminary") Preliminary();
  gc->Modified(); gc->Update();
  return;
}
