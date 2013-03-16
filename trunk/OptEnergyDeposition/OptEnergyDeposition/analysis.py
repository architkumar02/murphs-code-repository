# coding: utf-8
def GetRootFiles(path=os.getcwd(),gammaKey='Co60',neutronKey='neutron'):
    """ GetRootFiles
    
    Gets a list of root files, splitting up into gamma and neutrons
    according to gammaKey and neutronKey
    """
    gammaFiles = list()
    neutronFiles = list()
    for filename in os.listdir(path):
        [filename,ext] = os.path.splitext(filename)
        if ext == '.root':
            particleName = filename.split('_')[0].split('run')[0]
            filename = os.path.join(path,filename)+ext
            if particleName == gammaKey:
                gammaFiles.append(filename)
            elif particleName == neutronKey:
                neutronFiles.append(filename)
            else:
                print 'ROOT file '+filename+' is not reconized'
    return [gammaFiles,neutronFiles]
def PlotFiles(filelist,figureName='EnergyDep.png',histKey='eDepHist',title='Gamma Energy Deposition',xlabel='Energy Deposition (MeV)',ylabel='Frequency'):
    """ PlotFiles
    Plots the files files contained in filelist
    Keywords:
    figureName - name of the figure to save
    histKey - Key of the histogram in the root file
    title - title of the figure
    xlabel - xlabel of the figure
    ylabel - ylabel of the figure
    """
    # Creating a Canvas
    canvas = TCanvas()
    canvas.SetLogy()
    # Adding files to the histogram
    first = True
    for f in filelist:
        f = TFile(f,'r')
        hist = f.Get(histKey)
        if first:
            first = False
            hist.Draw()
            hist.GetXaxis().SetTitle(xlabel)
            hist.GetYaxis().SetTitle(ylabel)
            hist.SetTitle(title)
        else:
            hist.Draw("Same")
    canvas.Update()
    canvas.SaveAs(figureName)