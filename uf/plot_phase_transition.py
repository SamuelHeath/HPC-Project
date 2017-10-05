#!/usr/bin/python3

'''''''''
A python script that generates plots of number of percolation out of 50 tries
from ranging probabilities 0.0 to 1.0 at increments of 0.01 for a lattice
of size 128*128.
We can use this to estimate the critical threshold probabilities for bond
and site percolation
'''''''''

import sys;
import subprocess;
from os import system, remove;



# generate a .dat file containing the output from the percolation porgram.
# the output is then plotted using gnu-plot.
# to plot the graph use gnuplot:
# gnuplot '.plot_pref.gp' --persist
def generate_data(perc_type):
    plotPref = open('.plot_pref.gp', 'w') 
    f = open('.phase_transition.gp', 'w')

    if(perc_type == 's'):
        plotPref.write("set title 'Site percolation : percolation probability vs. % of percolations'\n")
        plotPref.write("set xlabel 'site probability'\n")
    else:
        plotPref.write("set title 'Bond percolation : percolation probability vs. % of percolations'\n")
        plotPref.write("set xlabel 'bond probability'\n")
    plotPref.write("set ylabel 'percolation found (%)'\n")
    #plotPref.write("set format x '%.2f'\n")
    plotPref.write("plot '.phase_transition.gp' using 1:3 with lp\n")
    plotPref.close()
    prob = 0.0
    for i in range (0, 100):
        numberOfPercolations = 0
        for j in range (0,100):
            exe = "./percolation -l 128 -p "  + str(prob) +" -" + str(perc_type)
            out =  str(subprocess.check_output(exe, shell=True))
            lines = out.count('[#]'); #if this is 1, only largest cluster was found successfully.
            if(lines > 1):
                numberOfPercolations+=1
        outstring = str(prob) + " , " + str(numberOfPercolations)+"\n"
        f.write(outstring);
        prob+=0.01
    f.close()


if __name__ == '__main__':
    percol_type = input("Enter percolation type site or bond.. [s/b]?\n")
    if(not ((percol_type == "s") or (percol_type == "b"))):
        sys.stderr.write("ERROR: wrong input! must be s or b\n")
        sys.exit(1);
    generate_data(percol_type)