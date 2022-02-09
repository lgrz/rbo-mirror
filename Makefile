CC=gcc
CFLAGS=-Wall -g
#CFLAGS=-Wall -O3
LDLIBS=-lm

# For make depend
DEPFILE	 = .depends
DEPTOKEN = '\# MAKEDEPENDS'
###
# makedepends related settings
# -Y		Don't search standard library paths, this is excessive
# -f		output dependencies to this file
# -s		use this token string
###
DEPFLAGS = -Y -f $(DEPFILE) -s $(DEPTOKEN) 
SRC=rbo.c rbo_main.c hashset.c sset.c rnkf.c rnkf_pr.c \
    log.c args.c rbocalc.c ao.c aocalc.c util.c metric.c \
    rnkhash.c tau.c strarr.c taucalc.c trec.c \
    \
    test/tutil.c \
    test/test_rbo_base.c test/test_sset.c test/test_rbo_extr.c \
    test/test_rnkf.c test/test_args.c test/test_rbocalc.c \
    test/test_ao.c test/test_aocalc.c test/test_rnkhash.c \
    test/test_tau.c test/test_strarr.c test/test_read_lines.c \
    test/test_taucalc.c test/test_cli.c \
    \

#    perf/rnd.c perf/perf.c

COMP_TESTS=test/test_sset test/test_rbo_base test/test_rbo_extr \
      test/test_rnkf test/test_rnkf_pr test/test_args test/test_rbocalc \
      test/test_ao test/test_aocalc test/test_rnkhash test/test_tau \
      test/test_strarr test/test_read_lines test/test_taucalc \
      test/test_cli
TESTS=${COMP_TESTS} test/test_trec.sh

#PERF=perf/perf

all: rbo ${TESTS} 
        
        # ${PERF}

rbo: rbo_main.o rbo.o hashset.o sset.o rnkf.o rnkf_pr.o log.o args.o \
    rbocalc.o util.o metric.o rnkhash.o strarr.o taucalc.o tau.c aocalc.o \
    ao.o trec.o

clean:
	rm -f rbo *.o $(COMP_TESTS) test/*.o

test/test_sset: sset.o hashset.o test/test_sset.o

test/test_rbo_base: rbo.o sset.o hashset.o test/test_rbo_base.o

test/test_rbo_extr: rbo.o sset.o hashset.o test/test_rbo_extr.o

test/test_rnkf: rnkf.o test/test_rnkf.o

test/test_rnkf_pr: rnkf_pr.o test/test_rnkf_pr.o

test/test_args: args.o log.o metric.o test/test_args.o

test/test_rbocalc: rbocalc.o rnkf.o rnkf_pr.o rbo.o sset.o hashset.o log.o \
    trec.o \
    test/test_rbocalc.o test/tutil.o

test/test_ao: ao.o hashset.o sset.o test/test_ao.o

test/test_aocalc: aocalc.o ao.o rnkf_pr.o sset.o hashset.o log.o \
    test/test_aocalc.o test/tutil.o

test/test_rnkhash: rnkhash.o test/test_rnkhash.o

test/test_tau: tau.o rnkhash.o log.o test/test_tau.o

test/test_strarr: strarr.o test/test_strarr.o test/tutil.o

test/test_read_lines: strarr.o util.o log.o test/test_read_lines.o test/tutil.o

test/test_taucalc: tau.o taucalc.o strarr.o util.o log.o rnkhash.o \
    test/test_taucalc.o test/tutil.o

test/test_cli: log.o metric.o test/test_cli.o test/tutil.o

perf/perf: perf/perf.o perf/rnd.o rbocalc.o rbo.o rnkf.o rnkf_pr.o sset.o \
    hashset.o log.o

.PHONY: test

test: ${TESTS}
	@for test in ${TESTS};            \
	do                                \
	        echo Running $$test ...;  \
		$$test;                   \
	done                 

depend:
	rm -f $(DEPFILE)
	make $(DEPFILE)


$(DEPFILE):
	@echo $(DEPTOKEN) > $(DEPFILE)
	makedepend $(DEPFLAGS) -- $(CFLAGS) -- $(SRC) 2> /dev/null 1>&2

sinclude $(DEPFILE)

# DO NOT DELETE
