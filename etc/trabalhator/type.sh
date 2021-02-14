#!/bin/bash


run() {
  ts=`date +%s%N`
  keys=`echo $1 | sed 's/keys=//' | sed 's/\,/ /g' `
  echo $keys | /home/wesley/usb-otg/hid_gadget_test /dev/hidg0 > /dev/null 2>&1
  sleep 0.005
  te=`date +%s%N`
  echo '{ "exec": "'$((($te - $ts) / 1000000))'", "keys": "'$keys'" }'
}

run $1 > /usr/lib/cgi-bin/type.log
