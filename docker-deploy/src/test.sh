#!/bin/bash
./client 127.0.0.1 create.txt
./client 127.0.0.1 1000.txt &
./client 127.0.0.1 1000.txt &
./client 127.0.0.1 1000.txt &
./client 127.0.0.1 1000.txt
wait
./client 127.0.0.1 cancel.txt