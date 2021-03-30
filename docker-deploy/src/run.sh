#!/bin/bash
#sudo su - postgres & psql & CREATE DATABASE exchange;
make clean
make all
echo 'start running server...'
./server &
while true ; do continue ; done
