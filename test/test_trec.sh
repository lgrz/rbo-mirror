#!/bin/bash

./rbo -p 0.98 -t test/dat/trec/input.SysA test/dat/trec/input.SysB \
    > /tmp/rbo.trec
rm -f /tmp/rbo.sep

for tpc in $(seq 401 410)
do
    echo -n "$tpc " >> /tmp/rbo.sep
    ./rbo -p 0.98 test/dat/trec/SysA.T$tpc test/dat/trec/SysB.T$tpc \
        >> /tmp/rbo.sep
done

diff /tmp/rbo.trec /tmp/rbo.sep > /tmp/rbo.diff

if [ ! -z /tmp/rbo.diff ]
then
    cat /tmp/rbo.diff
    exit 0
fi

rm /tmp/rbo.trec /tmp/rbo.sep /tmp/rbo.diff

exit 1
