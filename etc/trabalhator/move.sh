#!/bin/bash

run() {
  ts=$(date +%s%N)

  move=$(echo $1 | sed 's/coords=//' | sed 's/\%22//g' | sed 's/{//g' | sed 's/}//g' | sed 's/x//g' | sed 's/y//g' |  sed 's/://g' | sed 's/,/\n/g')

  b=${move[0]}
  if [ "$b" = "b1" ]; then
    b="01"
  elif [ "$b" = "b2" ]; then
    b="02"
  elif [ "$b" = "b3" ]; then
    b="04"
  else
    b="00"
  fi
  
  x=${move[1]}
  if [ $x -ne 0 ]; then
    if [ $x -gt 0 ]; then
      if [ $x -gt 127 ]; then
        x=127
      fi
    else
      if [ $x -lt -127 ]; then
        x=-127
      fi
      x=$((256+$x))
    fi
  fi
  x=$(echo "obase=16; ibase=10; $x" | bc | sed 's/-//g')

  y=${move[2]}
  if [ $y -ne 0 ]; then
    if [ $y -gt 0 ]; then
      if [ $y -gt 127 ]; then
        y=127
      fi
    else
      if [ $y -lt -127 ]; then
        y=-127
      fi
      y=$((256+$y))
    fi
  fi
  y=$(echo "obase=16; ibase=10; $y" | bc | sed 's/-//g')

  echo -ne "\\x$b\\x$x\\x$y" > /dev/hidg1

  if [ $x -eq 0 ] && [ $y == 0 ]; then
    echo -ne "\\x00\\x00\\x00" > /dev/hidg1
  fi

  te=$(date +%s%N)

  echo '{ "exec": "'$((($te - $ts) / 1000000))'", "b": "'$b'", "x": "'$x'", "y": "'$y'" }'
}

run $1 > /usr/lib/cgi-bin/move.log
