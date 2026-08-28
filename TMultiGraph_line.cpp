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
  "./projects/Thesis/data/Chapter4/NP04_ST_SelfTriggerResults_3PEeff_Avg.root",
  "./projects/Thesis/data/Chapter4/NP04_ST_SelfTriggerResults_Ch10441.root",
  "./projects/Thesis/data/Chapter4/NP04_ST_SelfTriggerResults_Ch10903.root",
  "./projects/Thesis/data/Chapter4/NP04_ST_SelfTriggerResults_Ch10945.root",
  "./projects/Thesis/data/Chapter4/NP04_ST_SelfTriggerResults_Ch11121.root",
  "./projects/Thesis/data/Chapter4/NP04_ST_SelfTriggerResults_Ch11221.root",
  "./projects/Thesis/data/Chapter4/CB_ST_SelfTriggerResults_Ch10400.root",
  "./projects/Thesis/data/Chapter4/CB_ST_SelfTriggerResults_Ch10402.root",
  "./projects/Thesis/data/Chapter4/CB_ST_SelfTriggerResults_Ch10403.root",
  "./projects/Thesis/data/Chapter4/CB_ST_SelfTriggerResults_3PEeff_Avg.root",
  "./projects/Thesis/data/Chapter4/LabTest_ST_SelfTriggerResults_INTEGRATED.root",
};

// If any subfolder, name can be "subfolder1/trgraph_name;1"
std::vector<TString> tgraph_names =  {
  "averaged_graph;1",
  "g_effAt3peFit_ThrCal_it_10441;1",
  "g_effAt3peFit_ThrCal_it_10903;1",
  "g_effAt3peFit_ThrCal_it_10945;1",
  "g_effAt3peFit_ThrCal_it_11121;1",
  "g_effAt3peFit_ThrCal_it_11221;1",
  "g_effAt3peFit_ThrCal_it_10400;1",
  "g_effAt3peFit_ThrCal_it_10402;1",
  "g_effAt3peFit_ThrCal_it_10403;1",
  "averaged_graph;1",
  "g_effAt3peFit_ThrCal_it_7;1",
};

// Very comfy to quickly save the canvas from the GUI
TString canvas_name = "./projects/Thesis/plots/Chapter4/ST_Comparison_TriggerEff3PE_vs_PE";

// Leave it empty if the tgraph_names are already descriptive
std::vector<TString> tgraph_titles = {
  "NP04 Avg.",
  "NP04 #upoint 10441",
  "NP04 #upoint 10903",
  "NP04 #upoint 10945",
  "NP04 #upoint 11121",
  "NP04 #upoint 11221",
  "CB #upoint 10400",
  "CB #upoint 10402",
  "CB #upoint 10403",
  "CB Avg.",
  "Integrated"
};

// ******************************************************************
// ******************************************************************

// --- Canvas -------------------------------------------------------
TString canvas_title = "";
bool horizontal_canvas = 0; // True 16:9, False 4:3
int canvas_fill_style = 0;  // 0 = transparent, 1 = solid
// --- Palette ------------------------------------------------------
// Choose among: Baseline, ColdToWarm, WarmToCold, DuneWhite
// If unknown, it will use the Baseline palette
std::string palette = "WarmToCold";
// --- Axis title ---------------------------------------------------
TString title_x = "Threshold [PE]";
TString title_y = "Trigger Efficiency";
// --- Log scale and grid -------------------------------------------
bool log_x  = 0;
bool log_y  = 0;
bool grid_h = 0;
bool grid_v = 0;
// --- Axis range ---------------------------------------------------
// If (low==0 and up==0) it will set it automatically
double x_axis_low = 0.005;
double x_axis_up  = 5.1;
double y_axis_low = 0.005;
double y_axis_up  = 1.1;
// --- Manipulation -------------------------------------------------
bool normalize = 0; // Normalize the TGraph(s) amplitude
bool norma = 0;
double scale = 16;
bool allign = 0;    // Allign the TGraph(s) to their maximum
double max_position_percentage = 0.15;
double x_rescale = 1.; // Rescale the x-axis of the TGraph(s) by this factor
double x_shift   = 0.; // Apply offset to x-axis of the TGraph(s)
std::vector<double> x_shifts = {}; // Apply x-shift to i-th graph
bool subtract_first_point_yvalue = 0; // Subtract the y-value of the first point to all the points of the TGraph(s)
// --- Graph settings -----------------------------------------------
// Draw Option Ref: https://root.cern.ch/doc/master/classTGraphPainter.html
const char* draw_option = "pe"; // Generally, lp for TGraphs and pe for TGraphErrors
int g_line_width = 8; // Line width of the TGraph(s)
int line_style = 1; // Line style of the TGraph(s)
float line_opacity = 0.6; // Opacity of the lines in the TGraph(s)
float end_error_size = 5.2;
float marker_size = 0.5; // Size of the markers in the TGraph(s)
int marker_style = 20; // Style of the markers in the TGraph(s)
bool disentangle_point_and_line = 1; // If true, the line and the points will be drawn separately, with different styles
std::vector<int> color_indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1}; // Indices of the colors to use from the palette
std::vector<int> marker_styles = {}; // see: https://root.cern.ch/doc/master/classTAttMarker.html
std::vector<float> marker_sizes  = {0., 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0., 0.5}; // If empty, it will use the default marker size
std::vector<int> line_widths = {5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2};
std::vector<int> line_styles = {};
std::vector<float> line_opacities = {0.3, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.3, 0.6};
// --- Legend settings ----------------------------------------------
int legend_ncolumns = 1;
float legend_fill_alpha = 1.0; // Fill alpha of the legend
float margin = 0.3;
const char* entry_opt = "p"; // Options for the legend entries
float legend_marker_size = 1.7;
double legend_x1 = 0.69;
double legend_x2 = legend_x1+0.14;
double legend_y1 = 0.45;
double legend_y2 = legend_y1+0.43;
// --- Dune Marker --------------------------------------------------
std::string dune_marker = ""; // Options: "preliminary", "simulation"
// --- Horizontal lines ---------------------------------------------
std::vector<double> h_lines_y = {};
std::vector<double> h_lines_x1 = {};
std::vector<double> h_lines_x2 = {};
// --- Vertical lines -----------------------------------------------
std::vector<double> v_lines_x = {1.5};
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

  // TGraph* g[n_graph];
  TGraphErrors* g[n_graph];
  TMultiGraph* mg = new TMultiGraph();

  std::vector<Int_t> color_list = set_palette(palette, n_graph);
  std::vector<TGraph*> g_lines;
 
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
    if (subtract_first_point_yvalue) g_subract_first_point_yvalue(g[i]);
    if (allign) g[i] =     g_allign_maximum(g[i], max_position_percentage);
    if (normalize)         g_normalize(g[i], scale, norma);
    if (x_rescale!=0.)     g_xrescale(g[i], x_rescale);
    if (x_shift!=0.)       g_xshift(g[i], x_shift);
    if (x_shifts.size()>0) g_xshift(g[i], x_shifts[i]);
;

    // Modify the Graph according to the settings
    g[i]->SetFillStyle(0);

    if   (line_styles.size()>0 && !disentangle_point_and_line) g[i]->SetLineStyle(line_styles[i]);
    else if (disentangle_point_and_line) g[i]->SetLineStyle(0);
    else g[i]->SetLineStyle(line_style);

    if (line_widths.size()>0) g[i]->SetLineWidth(line_widths[i]);
    else g[i]->SetLineWidth(g_line_width);

    if (color_indices.size()==0){
      g[i]->SetLineColor(color_list[i%color_list.size()]);
      g[i]->SetMarkerColor(color_list[i%color_list.size()]);
    }
    else {
      g[i]->SetLineColor(color_list[color_indices[i]%color_list.size()]);
      g[i]->SetMarkerColor(color_list[color_indices[i]%color_list.size()]);
      if (color_indices[i] < 0) {
        g[i]->SetLineColor(kBlack);
        g[i]->SetMarkerColor(kBlack);
      }
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
    
    TGraph* g_line;
    if (disentangle_point_and_line){
      g_line = dynamic_cast<TGraph*>(g[i]->Clone());
      g_line->SetLineStyle(line_styles.size() > 0 ? line_styles[i] : line_style);
      g_line->SetLineWidth(line_widths.size() > 0 ? line_widths[i] : g_line_width);
      g_line->SetLineColorAlpha(g_line->GetLineColor(), line_opacities.size() > 0 ? line_opacities[i] : line_opacity);
      g_line->SetMarkerStyle(0);
      g_lines.push_back(g_line);
    }

    input_file.Close();
  }
 
  if (horizontal_canvas){
    canvas_width = 1600;
    title_offset_y *= 0.8;
    title_offset_x *= 0.9;
    canvas_left_margin /= 1.2;
  }
  TCanvas* gc = new TCanvas(canvas_name, canvas_name, 0, 0, canvas_width, canvas_height);
  gc->SetTopMargin(canvas_top_margin);   gc->SetBottomMargin(canvas_bottom_margin);
  gc->SetLeftMargin(canvas_left_margin); gc->SetRightMargin(canvas_right_margin);
  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  gc->SetFillStyle(canvas_fill_style);
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
      lines.push_back(make_horizontal_line(h_lines_y[i], mg, kGray+2, 2, 7));
    }
    else if (h_lines_x1.size() > i && h_lines_x2.size() > i) {
      lines.push_back(make_horizontal_line(h_lines_y[i], mg, kGray+2, 2, 7,
                                           h_lines_x1[i], h_lines_x2[i]));
    }
    else {
      std::cout << "Warning: Inconsistent number of horizontal line coordinates. Skipping line " << i << std::endl;
      continue;
    }
  }

  for (size_t i=0; i<v_lines_x.size(); i++) {
    if (v_lines_y1.size() == 0 && v_lines_y2.size() == 0) {
      lines.push_back(make_vertical_line(v_lines_x[i], mg, kGray+2, 2, 7));
    }
    else if (v_lines_y1.size() > i && v_lines_y2.size() > i) {
      lines.push_back(make_vertical_line(v_lines_x[i], mg, kGray+2, 2, 7,
                                         v_lines_y1[i], v_lines_y2[i]));
    }
    else {
      std::cout << "Warning: Inconsistent number of vertical line coordinates. Skipping line " << i << std::endl;
      continue;
    }
  }


  auto legend = build_legend(gc, entry_opt,
                             legend_x1, legend_y1, legend_x2, legend_y2,
                             legend_ncolumns, margin, legend_fill_alpha, legend_marker_size, mg);
  legend->Draw();
  
  for (auto& line : lines) line->Draw();
  for (auto* g_line : g_lines)
    g_line->Draw("L");

  if (dune_marker == "preliminary") Preliminary();
  else if (dune_marker == "mytag") MyTag();
  else if (dune_marker != "") CustomTag(dune_marker);
  gc->Modified(); gc->Update();
}
