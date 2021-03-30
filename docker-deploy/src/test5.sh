#!/bin/bash
./client 127.0.0.1 create.txt
./client 127.0.0.1 5000.txt &
./client 127.0.0.1 5000.txt &
./client 127.0.0.1 5000.txt &
./client 127.0.0.1 5000.txt
# ./client 127.0.0.1 cancel.txt &
# ./client 127.0.0.1 cancel.txt &
# ./client 127.0.0.1 cancel.txt
./client 127.0.0.1 cancel.txt