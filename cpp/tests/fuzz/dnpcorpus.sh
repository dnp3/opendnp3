#/bin/sh
PCAPDIR=pcap/ICS-Security-Tools/
OUTDIR=corpus
find $PCAPDIR -name "*.pcap" | while read i; do tshark -r $i -T fields -e tcp.payload; done | tr -d : > /tmp/dnp3.txt
i=0;
cat /tmp/dnp3.txt | while read line; do i=$((i+1)); echo $line | xxd -r -p > $OUTDIR/$i.dnp; done
ls $OUTDIR/*.dnp | while read i; do hash=`md5 $i | cut -d" " -f4`; mv $i $OUTDIR/$hash.dnp; done
rm /tmp/dnp3.txt
