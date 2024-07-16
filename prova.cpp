void prova(){
  TFile *_file0 = TFile::Open("Self_FOM.root");
  TH1D* h;
  _file0->GetObject("jitter/TRIGGER_15", h);
  h->Draw();
}
