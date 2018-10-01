'''
Python script to run microbenchmark for different
index structure
'''

import json
import optparse
import os
import subprocess
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

def getThroughput(wl):
    fp = open("tmp").readlines()
    if wl == 'a':
        searchStr = "read/update"
    elif wl == 'c':
        searchStr = "read"
    else:
        searchStr = "insert/scan"

    for line in fp:
        w = line.split()
        if w[0] == searchStr:
            return w[1]

def plotgraph(plot_data, workload, key_type, threads, final_dir):
    fig = plt.figure()
    title = workload + '_' + key_type
    fig.suptitle(title)
    ax = fig.add_subplot(111)
    for keys in plot_data:
        ax.plot(threads, plot_data[keys], marker='o', linestyle='-', label = keys)
    ax.set_xlabel('threads')
    ax.set_ylabel('Mops/s')
    ax.legend(loc = 'upper left')
    fig.savefig(final_dir + title + '.png')


###################

parser = optparse.OptionParser()
parser.add_option("-d", "--dest", default = "temp",
        help = "destination folder")
parser.add_option("-c", "--config", default = "config.json",
        help = "config file")
parser.add_option("-p", "--plot", default = False,
        help = "plot only")

(opts, args) = parser.parse_args()

#Create result directory
result_dir = "./resutls/" + opts.dest + "/"
try:
    os.stat(result_dir)
except:
    os.makedirs(result_dir)

## Make binary
if opts.plot == False:
    print "Building binary"
    status = subprocess.check_output('make clean; make', shell = True)

## Read config files
with open(opts.config) as json_data_file:
    json_data = json.load(json_data_file)

for test in json_data:
    data = json_data[test][0]
    final_dir = result_dir + test + "/"

    try:
        os.stat(final_dir)
    except:
        os.makedirs(final_dir)
    
    for workload in data["workloads"]:
        for key_type in data["key_type"]:
            plot_data = {}
            for index in data["index"]:
                plot_data[index] = []
                log = final_dir + index + "_" + workload + "_" + key_type
                log_file = open(log, "w+")
                
                for threads in data["threads"]:
                    cmd = "./workload " + workload + " " + key_type + " " + index
                    cmd = cmd + " " + str(threads)
                    print cmd
                    os.system(cmd + " >> tmp")
                    thp = getThroughput(workload)
                    print thp
                    os.system("rm -rf tmp")
                    log_file.write(str(threads) + " " + thp)
                    plot_data[index].append(float(thp))
                
                log_file.close()
            
            plotgraph(plot_data, workload, key_type, data["threads"], final_dir)


