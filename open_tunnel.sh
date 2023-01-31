#!/bin/bash
echo "Opening the tunnel (must be run as root)";
openvpn --mktun --dev tun0;
ip link set tun0 up;
ip addr add 13.42.57.1/24 dev tun0 # directed to aws
# ip addr add 13.42.57.1/0 dev tun0 # directed to aws