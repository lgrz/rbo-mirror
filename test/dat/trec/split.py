#!/usr/bin/python

import sys

if len(sys.argv) != 2:
    sys.stderr.write("USAGE: %s <input>\n" % sys.argv[0])

run = sys.argv[1]
old_tpc = None
tpc_fp = None

for line in open(run):
    (tpc, q, doc, rnk, sim, run) = line.split()
    if tpc != old_tpc:
        if tpc_fp != None:
            tpc_fp.close()
        tpc_fp = open("%s.T%s" % (run, tpc), "w")
    old_tpc = tpc
    tpc_fp.write("%s\n" % doc)

tpc_fp.close()
