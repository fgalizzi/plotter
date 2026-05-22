#include "RtypesCore.h"
#include "PlotterUtils.hpp"
#include <vector>
  
// The macro plots either TGraphs with the same name but contained
// in different files, or TGraphs with different name, but in the
// same root file.
// You can easily modify the macro to plot TGraph / TGraphErrors

// ******************************************************************
// *** Files, TGraphs and settings **********************************
std::vector<TString> root_files   =  {
  "~/PhD/LowEnergy/flashmatch/data/testing_MLL_FM/nuecc/MLL_LightGBM_efficiency_dune10kt_charge_preselected.root",
  // "~/PhD/LowEnergy/flashmatch/data/testing_MLL_FM/nuecc/MLL_Parametrizer_dune10kt_true_energy.root",
  // "~/PhD/LowEnergy/flashmatch/data/testing_MLL_FM/nuees/MLL_Parametrizer_dune10kt_charge.root",
  // "~/PhD/LowEnergy/flashmatch/data/testing_MLL_FM/nuees/MLL_Parametrizer_dune10kt_true_energy.root",
};

// If any subfolder, name can be "subfolder1/trgraph_name;1"
std::vector<TString> tgraph_names =  {
  "g_par2;1",
};

// Very comfy to quickly save the canvas from the GUI
TString canvas_name = "./projects/FM_MLL/NueCC_ES_Charge_Etrue_calibration";

// Leave it empty if the tgraph_names are already descriptive
std::vector<TString> tgraph_titles = {
  "#nu_{e} CC Charge",
  "#nu_{e} CC True Energy",
  "#nu_{e} ES Charge",
  "#nu_{e} ES True Energy",
};

// ******************************************************************
// ******************************************************************

// --- Canvas -------------------------------------------------------
TString canvas_title = "";
bool horizontal_canvas = 0; // True 16:9, False 4:3
// --- Palette ------------------------------------------------------
// Choose among: Baseline, ColdToWarm, WarmToCold, DuneWhite
// If unknown, it will use the Baseline palette
std::string palette = "";
// --- Axis title ---------------------------------------------------
TString title_x = "Expected PE";
TString title_y = "#sigma [PE]";
// --- Log scale and grid -------------------------------------------
bool log_x  = 0;
bool log_y  = 0;
bool grid_h = 0;
bool grid_v = 0;
// --- Axis range ---------------------------------------------------
// If (low==0 and up==0) it will set it automatically
double x_axis_low = 0.;
double x_axis_up  = 0.;
double y_axis_low = 0.;
double y_axis_up  = 0.;
// --- Manipulation -------------------------------------------------
bool normalize = 0; // Normalize the TGraph(s) amplitude
bool norma = 0;
double scale = .3;
bool allign = 0;    // Allign the TGraph(s) to their maximum
double max_position_percentage = 0.15;
double x_rescale = 1.; // Rescale the x-axis of the TGraph(s) by this factor
double x_shift   = 0.; // Apply offset to x-axis of the TGraph(s)
std::vector<double> x_shifts = {}; // Apply x-shift to i-th graph
// --- Graph settings -----------------------------------------------
const char* draw_option = "lpe"; // Generally, lp for TGraphs and pe for TGraphErrors
int g_line_width = 1; // Line width of the TGraph(s)
int line_style = 1; // Line style of the TGraph(s)
float end_error_size = 5.7;
float marker_size = 0.7; // Size of the markers in the TGraph(s)
int marker_style = 20; // Style of the markers in the TGraph(s)
std::vector<int> color_indices = {}; // Indices of the colors to use from the palette
std::vector<int> marker_styles = {}; // see: https://root.cern.ch/doc/master/classTAttMarker.html
std::vector<float> marker_sizes  = {};
std::vector<int> line_styles   = {};
// --- Legend settings ----------------------------------------------
int legend_ncolumns = 1;
float legend_fill_alpha = 1.0; // Fill alpha of the legend
float margin = 0.2;
const char* entry_opt = "l"; // Options for the legend entries
double legend_x1 = 0.56;
double legend_y1 = 0.68;
double legend_x2 = 0.82;
double legend_y2 = 0.85;
// --- Dune Marker --------------------------------------------------
std::string dune_marker = ""; // Options: "preliminary", "simulation"
// --- Horizontal lines ---------------------------------------------
std::vector<double> h_lines_y = {};
std::vector<double> h_lines_x1 = {};
std::vector<double> h_lines_x2 = {};
// --- Vertical lines -----------------------------------------------
std::vector<double> v_lines_x = {};
std::vector<double> v_lines_y1 = {};
std::vector<double> v_lines_y2 = {};

// ***************************************************************
// ***************************************************************

void TMultiGraph_line(){
  SetMyStyle();
  gStyle->SetEndErrorSize(end_error_size);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  bool loop_on_files, loop_on_tgraphs;
  size_t n_graph = 0;

  if(root_files.size()>tgraph_names.size()){
    n_graph = root_files.size();
    loop_on_files = true;
    loop_on_tgraphs = false;
  }
  else if(root_files.size()<tgraph_names.size()){
    n_graph = tgraph_names.size();
    loop_on_files = false;
    loop_on_tgraphs = true;
  }
  else{
    std::cout << "All true" << std::endl;
    n_graph = root_files.size();
    loop_on_files = true;
    loop_on_tgraphs = true;
  }

  TGraph* g[n_graph];
  // TGraphErrors* g[n_graph];
  TMultiGraph* mg = new TMultiGraph();

  std::vector<Int_t> color_list = set_palette(palette, n_graph); 
 
  for(size_t i=0; i<n_graph; i++){
    TString file_name;
    if(loop_on_files==true) file_name = root_files[i];
    else file_name = root_files[0];
    
    TFile input_file(file_name, "READ");

    TString tg_name;
    if(loop_on_tgraphs==true){
      tg_name = tgraph_names[i];
    }
    else{
      tg_name = tgraph_names[0];
    }
    std::cout << i << std::endl;
    input_file.GetObject(tg_name, g[i]);


    if (g[i]==nullptr) {
      std::cout << "TGraph " << tg_name << " not found in file " << file_name << std::endl;
      continue;
    }
    std::cout << "TGraph " << g[i]->GetName() << std::endl;

    // Manipulation
    if (allign) g[i] =     g_allign_maximum(g[i], max_position_percentage);
    if (normalize)         g_normalize(g[i], scale, norma);
    if (x_rescale!=0.)     g_xrescale(g[i], x_rescale);
    if (x_shift!=0.)       g_xshift(g[i], x_shift);
    if (x_shifts.size()>0) g_xshift(g[i], x_shifts[i]);
;

    // Modify the Graph according to the settings
    g[i]->SetLineWidth(g_line_width);
    g[i]->SetFillStyle(0);

    if   (line_styles.size()>0) g[i]->SetLineStyle(line_styles[i]);
    else g[i]->SetLineStyle(line_style);

    if (color_indices.size()==0){
      g[i]->SetLineColor(color_list[i%color_list.size()]);
      g[i]->SetMarkerColor(color_list[i%color_list.size()]);
    }
    else {
      g[i]->SetLineColor(color_list[color_indices[i]%color_list.size()]);
      g[i]->SetMarkerColor(color_list[color_indices[i]%color_list.size()]);
    }
    
    if (marker_styles.size()==0)
      g[i]->SetMarkerStyle(marker_style);
    else
      g[i]->SetMarkerStyle(marker_styles[i]);

    if (marker_sizes.size()==0)
      g[i]->SetMarkerSize(marker_size);
    else
      g[i]->SetMarkerSize(marker_sizes[i]);

    
    if (tgraph_titles.size()>0) g[i]->SetTitle(tgraph_titles[i]); 
    
    CenterTitles(g[i]);
    mg->Add(g[i], draw_option);
    input_file.Close();
  }
 
  if (horizontal_canvas){
    canvas_width = 1600;
    title_offset_y *= 0.7;
  }
  TCanvas* gc = new TCanvas(canvas_name, canvas_name, 0, 0, canvas_width, canvas_height);
  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  if (!grid_v) gc->SetTickx(0);
  if (!grid_h) gc->SetTicky(0);

  mg->SetTitle(canvas_title);
  mg->GetXaxis()->CenterTitle();                  mg->GetYaxis()->CenterTitle();
  mg->GetXaxis()->SetTitle(title_x);              mg->GetYaxis()->SetTitle(title_y);
  mg->GetXaxis()->SetTitleFont(font);             mg->GetYaxis()->SetTitleFont(font);
  mg->GetXaxis()->SetTitleSize(title_font_size);  mg->GetYaxis()->SetTitleSize(title_font_size);
  mg->GetXaxis()->SetTitleOffset(title_offset_x); mg->GetYaxis()->SetTitleOffset(title_offset_y);
  mg->GetXaxis()->SetLabelFont(font);             mg->GetYaxis()->SetLabelFont(font);
  mg->GetXaxis()->SetLabelSize(label_font_size);  mg->GetYaxis()->SetLabelSize(label_font_size);
  mg->GetXaxis()->SetLabelOffset(label_offset_x); mg->GetYaxis()->SetLabelOffset(label_offset_y);
  mg->GetXaxis()->SetTickLength(0.02);            mg->GetYaxis()->SetTickLength(0.02);

  mg->Draw("ape");
 
  if(x_axis_low!=0. || x_axis_up!=0.){
    std::cout << "Setting x-axis range: " << x_axis_low << " - " << x_axis_up << std::endl;
    mg->GetXaxis()->SetRangeUser(x_axis_low, x_axis_up);
    for (size_t i=0; i<h_lines_y.size(); i++) {
      if (h_lines_x1.size() == 0 && h_lines_x2.size() == 0) {
        h_lines_x1.push_back(x_axis_low);
        h_lines_x2.push_back(x_axis_up);
      }
    }
  }
  if(y_axis_low!=0. || y_axis_up!=0.){
    std::cout << "Setting y-axis range: " << y_axis_low << " - " << y_axis_up << std::endl;
    for (size_t i=0; i<v_lines_x.size(); i++) {
      if (v_lines_y1.size() == 0 && v_lines_y2.size() == 0) {
        v_lines_y1.push_back(y_axis_low);
        v_lines_y2.push_back(y_axis_up);
      }
    }
    mg->GetYaxis()->SetRangeUser(y_axis_low, y_axis_up);
  }

  gc->Modified(); gc->Update();
  
  std::vector<TLine*> lines;
  for (size_t i=0; i<h_lines_y.size(); i++) {
    if (h_lines_x1.size() == 0 && h_lines_x2.size() == 0) {
      lines.push_back(make_horizontal_line(h_lines_y[i], mg, kBlack, 2, 2));
    }
    else if (h_lines_x1.size() > i && h_lines_x2.size() > i) {
      lines.push_back(make_horizontal_line(h_lines_y[i], mg, kBlack, 2, 2,
                                           h_lines_x1[i], h_lines_x2[i]));
    }
    else {
      std::cout << "Warning: Inconsistent number of horizontal line coordinates. Skipping line " << i << std::endl;
      continue;
    }
  }

  for (size_t i=0; i<v_lines_x.size(); i++) {
    if (v_lines_y1.size() == 0 && v_lines_y2.size() == 0) {
      lines.push_back(make_vertical_line(v_lines_x[i], mg, kBlack, 2, 2));
    }
    else if (v_lines_y1.size() > i && v_lines_y2.size() > i) {
      lines.push_back(make_vertical_line(v_lines_x[i], mg, kBlack, 2, 2,
                                         v_lines_y1[i], v_lines_y2[i]));
    }
    else {
      std::cout << "Warning: Inconsistent number of vertical line coordinates. Skipping line " << i << std::endl;
      continue;
    }
  }


  auto legend = build_legend(gc, entry_opt,
                             legend_x1, legend_y1, legend_x2, legend_y2,
                             legend_ncolumns, margin, legend_fill_alpha);
  legend->Draw();
  
  for (auto& line : lines) line->Draw();

  if (dune_marker == "preliminary") Preliminary();
  else if (dune_marker == "mytag") MyTag();
  else if (dune_marker != "") CustomTag(dune_marker);
  gc->Modified(); gc->Update();
}
