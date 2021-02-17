#!/bin/bash

type() {
  /etc/trabalhator/keyboard_control /dev/hidg0 $1
}

typelong() {
  text=$1
  for i in $(seq 1 ${#text})
  do
    type "--${text:i-1:1}"
  done
}

open() {
  type "--right-meta --r" 1
  typelong "notepad"
  type "--return" 1
}

run() {
  open
  while true; do
    ts=$(date +%s%N)
    size=$(tr -dc '0-9' < /dev/urandom | head -n 1 | head --bytes 2 ; echo '')
    if [ "$size" != "00" ]; then
      text=$(tr -dc 'a-z' < /dev/urandom | head -c $size ; echo '')
      typelong $text
      for i in $(seq 1 1 $size)
      do
        type "--bckspc"
      done
      te=$(date +%s%N)
      echo '{ "size": "'$size'", "exec": "'$((($te - $ts) / 1000000))'", "text": "'$text'" }'
    fi
  done
}

close() {
  type "--left-ctrl --a" 
  sleep 0.5
  type "--del"
  sleep 0.5
  type "--left-alt --f4"
}

if [ "$1" == "start" ]; then
  run > /usr/lib/cgi-bin/keyboard.log
else
  close > /usr/lib/cgi-bin/keyboard.log
fi

