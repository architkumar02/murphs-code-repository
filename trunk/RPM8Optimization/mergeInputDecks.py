with open('SCRIPT.mcnp','r') as i,open('INP.mcnp','w') as o:
    for line in i:
        if line.startswith('c CELLPREPENDTOKEN'):
            with open('cells.txt','r') as f:
                numCells = 0
                for line in f:
                    o.write(line)
                    numCells +=1
        elif line.startswith('c SURFACEPREPENDTOKEN'):
            with open('surfaces.txt','r') as f:
                for line in f:
                    o.write(line)
        elif line.startswith('c TALLYPREPENDTOKEN'):
            with open('tallies.txt','r') as f:
                for line in f:
                    o.write(line)
        elif line.startswith('IMP:N'):
            numCells += 4 # 3 Source, 1 RPM8 Encasing
            o.write('IMP:n 1 '+str(numCells)+' 0           $ Particle Importances within cells')
        else:
            o.write(line)
