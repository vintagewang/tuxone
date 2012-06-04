#!/bin/sh
ipcs -q |grep $LOGNAME > w.1.1
cat w.1.1 |awk '{print "ipcrm -q "$2}'> w.1.2
chmod u+x w.1.2
./w.1.2
rm -f w.1.1
rm -f w.1.2

ipcs -m |grep $LOGNAME > w.1.1
cat w.1.1 |awk '{print "ipcrm -m "$2}'> w.1.2
chmod u+x w.1.2
./w.1.2
rm -f w.1.1
rm -f w.1.2

ipcs -s |grep $LOGNAME > w.1.1
cat w.1.1 |awk '{print "ipcrm -s "$2}'> w.1.2
chmod u+x w.1.2
./w.1.2
rm -f w.1.1
rm -f w.1.2
