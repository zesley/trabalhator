#!/bin/bash

move() {
  echo -ne "\\x00\\$1\\$2" > /dev/hidg1
  sleep $3
}

coord() {
  echo $(hexdump -n 1 -v -e '"\\x" "%02X"' /dev/urandom)
}

run() {
  cin=0
  while true; do
    ts=$(date +%s%N)

    # random
    #x=`coord`
    #y=`coord`

    # 1px square
    x="x00"
    y="x00"
    if [ $cin == 0 ]; then
      x="x01"
      cin=1
    elif [ $cin == 1 ]; then
      y="x01"
      cin=2
    elif [ $cin == 2 ]; then
      x="xff"
      cin=3
    elif [ $cin == 3 ]; then
      y="xff"
      cin=0
    fi

    move "$x" "$y" 1

    x=$(echo "obase=10; ibase=16; $(echo $x | tr '[a-z]' '[A-Z]' | sed 's/[x-X]//')" | bc)
    y=$(echo "obase=10; ibase=16; $(echo $y | tr '[a-z]' '[A-Z]' | sed 's/[x-X]//')" | bc)
    
    if [ $x -ge 127 ]; then
      x=$(($x - 256))
    fi
    if [ $y -ge 127 ]; then
      y=$(($y - 256))
    fi

    te=`date +%s%N`

    echo '{ "exec": "'$((($te - $ts) / 1000000))'", "x": "'$x'", "y": "'$y'" }'
  done
}

stop() {
  move  "x00" "x00" 0.25
}

if [ "$1" == "start" ]; then
  run > /usr/lib/cgi-bin/mouse.log
else
  stop > /usr/lib/cgi-bin/mouse.log
fi
