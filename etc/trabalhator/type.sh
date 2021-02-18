#!/bin/bash

echo "$1" > /tmp/type.log

ts=$(date +%s%N)
keys=$(echo $1 | sed 's/keys=//' | sed 's/\,/ /g')
report=$(/etc/trabalhator/keyboard_control /dev/hidg0 $keys)
te=$(date +%s%N)
echo '{ "type": { "exec": "'$((($te - $ts) / 1000000))'", "keys": "'$keys'", "report": {'$report'} } }' > /usr/lib/cgi-bin/type.log
