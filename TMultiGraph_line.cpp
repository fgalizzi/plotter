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
};

// If any subfolder, name can be "subfolder1/trgraph_name;1"
std::vector<TString> tgraph_names =  {
};

// Very comfy to quickly save the canvas from the GUI
TString canvas_name = "";

// Leave it empty if the tgraph_names are already descriptive
std::vector<TString> tgraph_titles = {
};

// ******************************************************************
// ******************************************************************

// --- Palette ------------------------------------------------------
// Choose among: Baseline, ColdToWarm, WarmToCold
// If unknown, it will use the Baseline palette
std::string palette = "";
// --- Axis title ---------------------------------------------------
TString title_x = "";
TString title_y = "";
// --- Log scale and grid -------------------------------------------
bool log_x  = 0;
bool log_y  = 0;
bool grid_h = 1;
bool grid_v = 1;
// --- Axis range ---------------------------------------------------
// If (low==0 and up==0) it will set it automatically
double x_axis_low = 0.;
double x_axis_up  = 0.;
double y_axis_low = 0.;
double y_axis_up  = 0.;
// --- Manipulation -------------------------------------------------
bool normalize = 0; // Normalize the TGraph(s) amplitude
bool norma = 0;
double scale = 1.;
bool allign = 0;    // Allign the TGraph(s) to their maximum
double max_position_percentage = 0.15;
double x_rescale = 1.; // Rescale the x-axis of the TGraph(s) by this factor
double x_shift   = 0.; // Apply offset to x-axis of the TGraph(s)
std::vector<double> x_shifts = {}; // Apply x-shift to i-th graph
// --- Graph settings -----------------------------------------------
const char* draw_option = "pe"; // Generally, lp for TGraphs and pe for TGraphErrors
int g_line_width = 2; // Line width of the TGraph(s)
int line_style = 1; // Line style of the TGraph(s)
float end_error_size = 5.7;
float marker_size = 1.; // Size of the markers in the TGraph(s)
int marker_style = 20; // Style of the markers in the TGraph(s)
std::vector<int> color_indices = {}; // Indices of the colors to use from the palette
std::vector<int> marker_styles = {}; // see: https://root.cern.ch/doc/master/classTAttMarker.html
std::vector<float> marker_sizes  = {};
std::vector<int> line_styles   = {};
// --- Legend settings ----------------------------------------------
int legend_ncolumns = 1;
float legend_fill_alpha = 1.0; // Fill alpha of the legend
float margin = 0.2;
const char* entry_opt = "pe"; // Options for the legend entries
// --- Dune Marker --------------------------------------------------
TString dune_marker = ""; // Options: "preliminary", "simulation"

// ***************************************************************
// ***************************************************************

void TMultiGraph_line(){
  SetMyStyle();
  gStyle->SetEndErrorSize(end_error_size);

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

  TGraphErrors* g[n_graph];
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
  
  TCanvas* gc = new TCanvas(canvas_name, canvas_name, 0, 0, canvas_width, canvas_height);
  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);

  mg->SetTitle("");
  mg->GetXaxis()->CenterTitle();                  mg->GetYaxis()->CenterTitle();
  mg->GetXaxis()->SetTitle(title_x);              mg->GetYaxis()->SetTitle(title_y);
  mg->GetXaxis()->SetTitleFont(font);             mg->GetYaxis()->SetTitleFont(font);
  mg->GetXaxis()->SetTitleSize(title_font_size);  mg->GetYaxis()->SetTitleSize(title_font_size);
  mg->GetXaxis()->SetTitleOffset(title_offset_x); mg->GetYaxis()->SetTitleOffset(title_offset_y);
  mg->GetXaxis()->SetLabelFont(font);             mg->GetYaxis()->SetLabelFont(font);
  mg->GetXaxis()->SetLabelSize(label_font_size);  mg->GetYaxis()->SetLabelSize(label_font_size);
  mg->GetXaxis()->SetLabelOffset(label_offset_x); mg->GetYaxis()->SetLabelOffset(label_offset_y);

  mg->Draw("ape");
  // line->Draw("same");
 
  if(x_axis_low!=0. || x_axis_up!=0.){
    std::cout << "Setting x-axis range: " << x_axis_low << " - " << x_axis_up << std::endl;
    mg->GetXaxis()->SetRangeUser(x_axis_low, x_axis_up);
  }
  if(y_axis_low!=0. || y_axis_up!=0.){
    std::cout << "Setting y-axis range: " << y_axis_low << " - " << y_axis_up << std::endl;
    mg->GetYaxis()->SetRangeUser(y_axis_low, y_axis_up);
  }

  auto legend = build_legend(gc, entry_opt,
                             0.7, 0.7, 0.9, 0.9,
                             legend_ncolumns, margin, legend_fill_alpha);

  legend->Draw();
  if (dune_marker == "preliminary") Preliminary();
  gc->Modified(); gc->Update();
}
