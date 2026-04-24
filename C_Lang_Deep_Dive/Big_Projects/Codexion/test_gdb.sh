make re
./codexion 199 331 100 100 100 5 10 edf &
PID=$!
sleep 1
gdb -batch -ex "thread apply all bt" -p $PID
kill -9 $PID
