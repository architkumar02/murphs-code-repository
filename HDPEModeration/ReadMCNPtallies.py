# This script reads all tally data from a specified MCNP output file and spits each tally in its own *.csv file

input_name = "tesu.o"
project_name = "Test" # Filename prefix for the output files

############################
### FUNCTION DEFINITIONS ###
############################

def findTallies(filename):
	tally_file = open(filename,"r")
	line_index = 0
	tally_starts = [] # List of indices that mark the beginning of a tally
	tally_ends = [] # List of indices that mark the end of a tally
	for line in tally_file:
		if line.startswith("      energy"):
			tally_starts.append(line_index)
		if line.startswith("      total") and tally_starts != []:
			tally_ends.append(line_index)
		line_index = line_index + 1
	return tally_starts,tally_ends

# We want to make sure our filenames have no newlines, excessive spaces, or special characters like colons
def formatTitle(base,toAdd):
	toAdd = toAdd.replace("\n","")
	toAdd = toAdd.replace(" ","")
	toAdd = toAdd.replace(":","")
	if toAdd != "": base = base + "_" + toAdd
	return base

def formatData(string):
	string = string.replace("    ","")
	string = string.replace("   ",",")
	string = string.replace(" ",",")
	return string

def readTallies(filename,tally_starts,tally_ends):
	tally_file = open(filename,"r")
	line_index = 0
	readTallyFlag = 0
	tally_title = ""; output_string = ""; particle = ""
	
	for line in tally_file:
		if line_index in tally_ends:
			# Time to write the tally data to a file and reset all flags
			output_file = open(project_name + "_" + tally_title + ".csv","w")
			output_file.write(output_string)

			output_file.close()
			tally_title = ""
			readTallyFlag = 0
			output_string = ""
			
		if readTallyFlag == 1:
			line = formatData(line)
			output_string = output_string + line

		if line.startswith("           particle(s):") == 1: #This is the line above the tally that tells what particle type is tallied
                        particle = line.split(":")
                        particle = particle[1].replace("\n","")
                        particle = particle.replace(" ","")
		if line.find("tally  ") > -1: #This is the line above the tally that tells what particle type is tallied
                        tallyType = line.split()
                        tallyType = tallyType[1]

		# We'll use the tallied surface or cell to identify the output filename
		if (line_index+1) in tally_starts: tally_title = formatTitle(tally_title,line) + "_" + particle + "_" + "F" + tallyType + "_tally"
		if line_index in tally_starts: readTallyFlag = 1
				
		line_index = line_index + 1

#################
### SCRIPTING ###
#################

# First, we will find all the line indices marking the beginnings and ends of each tally
(tally_starts,tally_ends) = findTallies(input_name)

# Next we will read the tallies themselves
# The header lines for each tally are identifiers for the output filenames
readTallies(input_name,tally_starts,tally_ends)
