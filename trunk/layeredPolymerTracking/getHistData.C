{
	gROOT->Reset();

	// Getting Files
	TFile* fCo60_1 = new TFile("run_0.root","r");
	TFile* fCo60_2 = new TFile("run_1.root","r");

	// Getting Ntuples
	TNtuple* t1 = (TNtuple*) fCo60_1->Get("eventTupleLayer 0");
	TNtuple* t2 = (TNtuple*) fCo60_2->Get("eventTupleLayer 0");

	Float_t eDepGap, eDepAbs, eDepTot;

	// Opening Output File
	FILE *fout = fopen("outData.txt","w");
	if( fout == NULL)
		printf("Could not open output file\n");
	else{
		t1->SetBranchAddress("eDepGap",&eDepGap);
		t1->SetBranchAddress("eDepAbs",&eDepAbs);
		t1->SetBranchAddress("eDepTot",&eDepTot);
		Int_t nbytes = 0;
		for(Int_t i=0; i<t1->GetEntries(); i++)
			nbytes += t1->GetEvent(i);
			fprintf(fout,"%f\n",eDepAbs);
		}
		t2->SetBranchAddress("eDepGap",&eDepGap);
		t2->SetBranchAddress("eDepAbs",&eDepAbs);
		t2->SetBranchAddress("eDepTot",&eDepTot);
		for(Int_t i=0; i<t2->GetEntries(); i++){
			nbytes += t1->GetEvent(i);
			fprintf(fout,"%f\n",eDepAbs);
		}
		fclose(fout);
	}
	fCo60_1->Close();
	fCo60_2->Close();
}
