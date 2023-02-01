# ipOverTelegram

## ❓ About

This is an IP over Telegram project that builds a rudimentary tunneling service using a Telegram Bot.

On the client side, it uses a tunneling technique to intercept an outgoing request on Layer 3. It then encodes that request data in a Natural Language format and sends it to a Telegram chat via a pre-configured bot. The server side then reads the message from the chat, decodes it and completes the request. The response is again encoded and sended via chat to be read by the client side bot and displayed to the user.

This project is part of the INF472D course concerning Network Programming.


## 🏗️ Compiling
This application uses `cURL` to make API requests, and thus, you'll need to have the `libcurl` (or `libcurl-dev`) installed prior to the compilation. 

To compile the code you need to first generate the makefile using CMake: 
```bash
cmake .;
```

And then generate the executables:
```bash
make;
```


## 🏃 Running

You will need to run this application on two different devices, one that will act as a client and the other that will act as the server.


### Server-side
This part needs root privileges to  write on the tunnel. So it is recommended to run everything below as root, with the command:
```bash
sudo su;
```

Now in the root user, execute the app with the correct bot and chat ids, and with the correct IP mask where to re-inject incoming packets.

- export the Environment variables: 
```bash
export TELEGRAM_CHAT_ID=<YOUR_CHAT_ID>
export SERVER_BOT_ID=<YOUR_SERVER_BOT_ID>
```

- setting up the scripts:
```bash
chmod +x ./open_server_tunnel.sh 
./open_server_tunnel.sh <IP_MASK>
```

- executing: 
```bash
./server
```


### Client-side
We don't need root privileges for every stage. So to execute the client side with the correct bot and chat ids, and the correct IP mask where to intercept packets:

- export the Environment variables: 
```bash
export TELEGRAM_CHAT_ID=<YOUR_CHAT_ID>
export CLIENT_BOT_ID=<YOUR_CLIENT_BOT_ID>
```

- setting up the scripts:
```bash
sudo chmod +x ./open_client_tunnel.sh 
sudo ./open_client_tunnel.sh <IP_MASK>
```

- executing: 
```bash
./client
```


### Miscellaneous
To close any of the tunnels (for example TUNNEL_NAME=tun0), run:
```bash
sudo chmod +x ./close_tunnel.sh 
sudo ./close_tunnel.sh <TUNNEL_NAME>
```


## 🏯 Architecture
The code is separated in several modules with 2 main entrance points. 

The files `client.c` and `server.c` define the main entry-points for the applications, whereas the `main.c` file was mostly used during development.

The client-side intercepts the packages from the tunnel `tun0`, encodes them in a natural language format, and sends the message through Telegram. On the other hand, the server-side listens for messages on the Telegram chat, decodes them and re-injects them into the network using the tunnel `tun1`.

The shared packages are:
- `encryptor`: responsable for encrypting and decrypting values, using a shared dictionary;
- `request`: get-request interface to allow for ease communication with the Telegram api. Uses the cURL library;
- `telegram`: interface that encapsulates the main functionalities of the bots in the application;
- `tunnel`: tunnel management/allocation functionalities;

External packages:
- `cURL`: to make easy https requests to the Telegram API;

More information on the packages can be found on their respective header files (in the `include` folder).