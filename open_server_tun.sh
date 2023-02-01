#!/bin/bash
echo "Opening the tunnel (must be run as root)";
openvpn --mktun --dev tun1;
ip link set tun1 up;
ip addr add 10.8.0.1/24 dev tun1 # directed to aws
# ip addr add 13.42.57.1/0 dev tun0 # directed to aws