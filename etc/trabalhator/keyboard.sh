#!/bin/bash

type() {
  /etc/trabalhator/keyboard_control /dev/hidg0 $1
  sleep $2
}

typelong() {
  text=$1
  for i in $(seq 1 ${#text})
  do
    type "-${text:i-1:1}" $2
  done
}

open() {
  ts=$(date +%s%N)
  type "-rightmeta -r"  0.5
  typelong "notepad" 0.01
  type "-return" 1
  te=$(date +%s%N)
  echo '"size": "10", "exec": "'$((($te - $ts) / 1000000))'", "text": "meta r notepad return"'
}

run() {
  open
  while true; do
    ts=$(date +%s%N)
    size=$(tr -dc '0-9' < /dev/urandom | head -n 1 | head --bytes 2 ; echo '')
    if [ "$size" != "00" ]; then
      text=$(tr -dc 'a-z' < /dev/urandom | head -c $size ; echo '')
      typelong $text 0.01
      for i in $(seq 1 1 $size)
      do
        type "-bckspc" 0.01
      done
      te=$(date +%s%N)
      echo '"size": "'$size'", "exec": "'$((($te - $ts) / 1000000))'", "text": "'$text'"'
    fi
  done
}

close() {
  ts=$(date +%s%N)
  type "-leftctrl -a" 0.5
  type "-del" 0.5
  type "-leftalt -f4" 0.5
  te=$(date +%s%N)
  echo '"size": "5", "exec": "'$((($te - $ts) / 1000000))'", "text": "ctrl a del alt f4"'
}

if [ "$1" == "start" ]; then
  run > /usr/lib/cgi-bin/keyboard.log
else
  close > /usr/lib/cgi-bin/keyboard.log
fi

