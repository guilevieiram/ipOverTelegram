echo "Opening the tunnel (must be run as root)";
openvpn --mktun --dev tun0;
ip link set tun0 up;
ip addr add $1 dev tun0

echo "Tunnel openned, redirecting trafic from $1";