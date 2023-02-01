#!/bin/bash
echo "Opening the server tunnel (must be run as root)";

openvpn --mktun --dev tun1;
ip link set tun1 up;
ip addr add $1 dev tun1 

echo "Tunnel openned, reinjecting trafic on $1";