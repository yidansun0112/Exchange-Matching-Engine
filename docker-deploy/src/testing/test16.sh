#!/bin/bash
./client 127.0.0.1 ./testing/create.txt
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
./client 127.0.0.1 ./testing/query.txt &
wait
./client 127.0.0.1 ./testing/cancel.txt
