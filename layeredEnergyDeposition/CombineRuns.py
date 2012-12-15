# coding: utf-8
def CombineStats(filelist):
    print 'Combining the statisitcs'
    for f in filelist:
        print f
def CompressRuns(dataDir='output'):
    runs = dict()
    for fileName in os.listdir(dataDir):
        filePath = os.path.join(dataDir,fileName)
        #Only want the text files
        if filePath.endswith('.txt'):
            print 'Processing file ',filePath
            # Need to break up by run
            runNumber = fileName.split('_')[1]
            # Add to run dicitonary
            if runNumber not in runs.keys():
                runs[runNumber] = list()
            runs[runNumber].append(filePath)
    # Now we need to compine the statisitcs for each run
    for key in runs.keys():
        print 'Combining the statisitcs of run',key
        CombineStats(runs[key])