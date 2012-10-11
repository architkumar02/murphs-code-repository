{
    // ROOT Macro for analyzing histogram data

    // Getting the files
    TFile *f25 = new TFile("cs137_25um.root");
    TFile *f50 = new TFile("cs137_50um.root");
    TFile *f100 = new TFile("cs137_100um.root");
    TFile *f1mm = new TFile("cs137_1mm.root");
    TFile *f1cm = new TFile("cs137_1cm.root");

    // Getting the Histograms
    TH1F* h25 = (TH1F*) f25->Get("totEventEDepGap_00");
    TH1F* h50 = (TH1F*) f50->Get("totEventEDepGap_00");
    TH1F* h100 = (TH1F*) f100->Get("totEventEDepGap_00");
    TH1F* h1mm = (TH1F*) f1mm->Get("totEventEDepGap_00");
    TH1F* h1cm = (TH1F*) f1cm->Get("totEventEDepGap_00");

    // Plotting
    gStyle->SetOptStat(0);
    h25->Draw();
    h50->Draw("same");
    h100->Draw("same");
    h1mm->Draw("same");
    h1cm->Draw("same");
    
    TAxis *xaxis = h25->GetXaxis();
    xaxis->SetRangeUser(0,0.67);
    
    gPad->SetLogy();
}
