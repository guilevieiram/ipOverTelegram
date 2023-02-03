#!/bin/bash
echo "Closing the tunnel $1 (must be run as root)";

ip link set $1 down;
ip link delete $1;

echo "Tunnel $1 closed";