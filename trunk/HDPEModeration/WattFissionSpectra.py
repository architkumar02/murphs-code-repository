# coding: utf-8
import numpy as np
import matplotlib.pyplot as plt

# Watt Fission Spectra
def WattFission(E):
    return np.exp(-E/a)*np.sinh(np.sqrt(b*E))

E = np.linspace(0,5,1000)

# MCNP
a = 1.025
b = 2.926
MCNP = WattFission(E)

# MCNPX
a = 1.18
b = 1.03419
MCNPX = WattFission(E)

# Plotting
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(E,MCNP,'k',E,MCNPX,'k:')
ax.grid(True)
ax.set_ylabel('Watt Fission Spectra (frequency)')
ax.set_xlabel('Energy (MeV)')
leg = ax.legend(('MCNP5','MCNPX'))
fig.show()
fig.savefig('WattFissionComparionsCf252.png')

