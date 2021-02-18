#!/bin/bash

run() {
  ts=$(date +%s%N)
  keys=$(echo $1 | sed 's/keys=//' | sed 's/\,/ /g')
  report=$(/etc/trabalhator/keyboard_control /dev/hidg0 $keys)
  te=$(date +%s%N)
  echo '{ "exec": "'$((($te - $ts) / 1000000))'", "keys": "'$keys'", "report": {'$report'} }'
}

run $1 >> /usr/lib/cgi-bin/type.log
