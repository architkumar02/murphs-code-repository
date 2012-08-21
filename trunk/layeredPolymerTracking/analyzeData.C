{
// ROOT Macro for analyzing histogram data

	// Getting the NTuple / Data
	TFile* f = new TFile("/Users/murffer/Desktop/gammaRun.root","r");
	TNtuple* runTuple = gROOT->FindObject("runTuple");
	
	// Getting all of the kinetic energies of the secondaries
	TCanvas *c = new TCanvas;
	c->Divide(1,2);
	c->cd(1);
	//runTuple->SetTitle("Kinetic Energy of Secondaries");
	runTuple->Draw("kinE","(ParentID == 1)");
	//runTuple->GetXaxis()->SetTitle("Intial Kinetic Energy (MeV)");

	c->cd(2);
	//runTuple->SetTitle("");
	runTuple->SetLineColor(kMagenta);
	runTuple->Draw("kinE","(ParentID == 1) && (posZ >= 0 && posZ<= 20)");
	runTuple->SetLineColor(kSpring);
	runTuple->Draw("kinE","(ParentID == 1) && (posZ <= 0 && posZ >= -20)","same");
	//runTuple->GetXaxis()->SetTitle("Intial Kinetic Energy (MeV)");
	f->Close();
}
