#!/usr/bin/python
import numpy as np
import pylab
def Compton(Ei,theta):
    """
    Compton Scatter of a photon off an electron
    
    Keywords:
    Ei - Inital Photon Energy
    theta - Scattering angle
    
    Returns
    Ef - final energy of the photon
    Ee - final energy of the electron
    """
    m  = 0.5109989 # Mass Electron (MeV)
    Ee = Ei - Ei*m/(m+Ei*(1-np.cos(theta)))
    Ef = Ei - Ee
    return (Ef,Ee)

def diffXSElectrons(Ei,theta):
    """
    Differnetial cross section for scatter of electrons by the Compton Effect
    Keywords:
        Ei - Initial Photon Energy
        theta - scattering angle (compton)
    """
    # Contants
    r = 2.81794E-13 # Classical Electron Radius (cm)
    m  = 0.5109989 # Mass Electron (MeV)
    # Computing Final Photon and Electron Energy
    Ef,Ee = Compton(Ei,theta)
    ratio = np.divide(Ef,Ei)
    g = 0.5*np.power(ratio,2)*(ratio+1/ratio-np.power(np.sin(theta),2))
    firstTerm = 2*np.pi*np.power(r,2)*np.sin(theta)*g
    secTermNum = np.power(1+Ei/m*(1-np.cos(theta)),2)*m
    secTermDen = np.power(Ei,2)*np.sin(theta)
    return firstTerm*secTermNum/secTermDen

def PopulationMedian(pop,x):
    r = list()
    for i in range(len(pop)-1):
        r.append(np.sum(pop[0:i])/np.sum(pop[i:-1]))
    idx = (np.abs(np.array(r)-1.0)).argmin()
    print 'Population Ratio is: ',r[idx]
    return x[idx]

def PlotData(log=False):
    theta = np.linspace(0,np.pi,1000)
    if log:
      theta = np.logspace(-1.5,np.log10(np.pi),100)
    theta = np.delete(theta,[0,np.pi],None)
    ECo60 = np.array([1.117,1.332])
    Ef0,Ee0 = Compton(ECo60[0],theta)
    Ef1,Ee1 = Compton(ECo60[1],theta)
    dSdE0 = diffXSElectrons(ECo60[0],theta)
    dSdE1 = diffXSElectrons(ECo60[1],theta)

    # Looking for the median
    print 'Median Energy of {} MeV Photon is {}\n'.format(ECo60[0],PopulationMedian(dSdE0,Ee0))
    print 'Median Energy of {} MeV Photon is {}\n'.format(ECo60[1],PopulationMedian(dSdE1,Ee1))

    # Energy Distribution
    pylab.figure()
    pylab.plot(theta,Ee0,theta,Ee1)
    pylab.grid(True)
    pylab.xlabel('Scattering Angle')
    pylab.ylabel("Electron Energy (MeV)")
    pylab.legend(["1.117","1.332"])
    if log:
      pylab.yscale('log')
      pylab.xscale('log')
      pylab.savefig('LogComptonElectronEnergies.png')
    else:
      pylab.savefig('ComptonElectronEnergies.png')

    # Scattering Distribution
    pylab.figure()
    pylab.plot(Ee0,dSdE0,Ee1,dSdE1)
    pylab.grid(True)
    pylab.xlabel('Electron Energy (MeV)')
    pylab.ylabel('Compton Scattering Cross Section')
    pylab.legend(["1.117","1.332"])
    if log: 
      pylab.yscale('log')
      pylab.xscale('log')
      pylab.savefig('LogComptonScatteringXS.png')
    else:
      pylab.savefig('ComptonScatteringXS.png')

def main():
  PlotData(log=False)
  PlotData(log=True)
if __name__ == '__main__':
    main()
#plot(theta,Ee)
