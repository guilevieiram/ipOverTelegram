#!/bin/bash
echo "Closing the tunnel (must be run as root)";
ip link set tun0 down;