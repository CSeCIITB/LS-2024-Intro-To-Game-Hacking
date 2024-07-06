# Section 1: Setting up the Game and Server

## Host machine

This course assumes that you have access to a Linux machine. It is recommended to perform a dual-boot if you intend to try this on a [Windows](https://youtu.be/QKn5U2esuRk?si=an1jyQDlvp4RmTzD) or a [Mac](https://www.youtube.com/watch?v=aXh_d1OixRI). A virtual machine may not suffice as the game requires significant resources to run. Additionally, a WSL will not work due to graphic issues.

## Install Server

After setting up the host machine, ensure that the following requirements are met on your device.

### Requirements

From the official README:

* At least 2GB of RAM, more RAM will allow more instances to be run on a single machine
* The Game Server does not need any graphics hardware and runs purely on console. It is known to run well on Amazon AWS and Digital Ocean VPS instances.
* The Game Server requires a lot of RAM to run, but uses fork and copy-on-write memory to allow many instances to run on a single host.  
* For a server with 2GB of RAM, it is not recommended to run more than 5 instances, but a server with 8GB of RAM can typically run as many as the CPU can handle. 
* It is recommended to use 2-3 instances per CPU core if you have sufficient RAM.  You may be able to run 4-5 instances per core, but doing so may introduce slight lag.

* The files for the client and server are over 2GB as well, so several GB of free disk space are required.

Now follow these instructions to build and deploy your own server.

### Docker

First install `docker` and `docker-compose` on your host system. It makes it easy to run and tear down a server, without making changes to the actual host system.

Second, gather all necessary files:

Download the Server Files from [here](https://iitbacin-my.sharepoint.com/:u:/g/personal/22b0995_iitb_ac_in/ETEmMpV1LJBHhIThBW3WytEBY6KtD6ycnILd8jtqdjBfsw?e=YKpyuh) and extract them.

In order to run the server, `docker` and `docker-compose` have to be installed. Docker is moving fast, so it's a good idea to follow the current official steps for installation (which could also include to remove an older system version of docker): 

* Docker CE Ubuntu: [https://docs.docker.com/install/linux/docker-ce/ubuntu/](https://docs.docker.com/install/linux/docker-ce/ubuntu/). 
* `docker-compose`: [https://docs.docker.com/compose/install/](https://docs.docker.com/compose/install/)
* make sure the current user is part of the `docker` group with: `sudo usermod -a -G docker $USER`. restart or re-login and verify with `id` that the user is part of the docker group.

Then simply build the image and launch the master and game server:

```bash
docker-compose build
docker-compose up
```

`docker-compose up` can also run in detached/background mode with `-d`.

## Install Client

First download the client from the official website here: http://www.pwnadventure.com/#downloads

To get a client connected to the new server, the `server.ini` for the client has to be modified. The server launched with docker expects that hostnames `master.pwn3` and `game.pwn3` are being used (These could theoretically be changed in the docker/setup files). 

The `server.ini` for the client has to look something like this:

```
[MasterServer]
Hostname=master.pwn3
Port=3333

[GameServer]
Hostname=game.pwn3
Port=3000
Username=
Password=
Instances=
```

Make sure that the client can reach these hosts, for example by adding them to the `/etc/hosts` file. In this example the server is running on `127.0.0.1` and the entry for them would be:

```
127.0.0.1  master.pwn3
127.0.0.1  game.pwn3
```

**Warning:** Using an IP as `Hostname` in the `server.ini` does not work!

To stop the server, simply type `docker-compose down`.

**Warning:** The database file is not persistent - taking down the container resets everything. So backup first.

### Note:
If you get an error saying libssl.so.1.0.0 library not found, then refer to [this answer](https://askubuntu.com/a/1331642).

## Running the Game

If everything was done correctly, you can run the game by doing the following:

Start the server:
```bash
docker-compose up -d
```
Start the Client by running the game binary:
```bash
cd ./PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Binaries/Linux
./PwnAdventure3-Linux-Shipping
```
This should open the game's home screen.

![Home Screen](image.png)

If you get this window by clicking on `Play Game` then the server is set up correctly.

![login](image-1.png)

Now simply register a new account and login with those credentials to start the game!

## Troubleshooting

### Error: docker-compose build

```bash
$ docker-compose build
Building init
ERROR: Error processing tar file(exit status 1): write /client/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Content/Paks/Characters.pak: no space left on device
```

A: Get more disk space.

```bash
$ docker-compose build
Building init
ERROR: Couldn't connect to Docker daemon at http+docker://localunixsocket - is it running?
```

A: Your user is probably not part of the `docker` group or docker service not running. `sudo usermod -a -G docker pwn3`, verify with `id`. Or `service docker restart`.

### File Integrity

Check if the archive is corrupted

```bash
$ md5sum pwn3.tar.gz
d3f296461fa57996018ce0e4e5a653ee  pwn3.tar.gz
$ sha1sum pwn3.tar.gz
022bd5174286fd78cd113bc6da6d37ae9af1ae8e  pwn3.tar.gz
```

### PwnAdventure3 Client Errors

**Connection Error: Unable to connect to master server**

This probably means that the MasterServer is not reachable.

* Client issues:
  * Check the `[MasterServer]` entry in the client's `server.ini`
  * Can you ping `master.pwn3` from the host from your system?
  * Is the IP correct in the `/etc/hosts` file?
* Server issues:
  * Is the server not running and listening on port `3333`? 
  * Check with `sudo netstat -tulpn`
    * Is the master server listening: `tcp6 0 0 :::3333 :::* LISTEN 31913/docker-proxy`
  * Check `docker ps` if the two containers are up
    * master server running? `880f93374070 pwn3server "/opt/pwn3/setup/mas…" 0.0.0.0:3333->3333/tcp, 5432/tcp pwnadventure3_master_1`

**Waiting in connection queue...**

This means the MasterServer *is* reachable and is waiting now for a free GameServer that can be given to the client. This probably means that no GameServer is running, or was not able to connect to the MasterServer.

* Server issues:
  * Is a game server running and listening on port `3000-3005`? 
  *  Check listening processes with `sudo netstat -tulpn`
    * `tcp6 0 0 :::3000 :::* LISTEN 32160/docker-proxy`
  * Is `pwnadventure3_game_1` container running? Check with `docker ps -a`
    * `84343f81034f pwn3server "/opt/pwn3/setup/gam…" 0.0.0.0:3000-3010->3000-3010/tcp, 5432/tcp pwnadventure3_game_1`
  * do you see the following line in the log from `docker-compose up`: `line 1: 7 Killed ./PwnAdventure3Server; pwnadventure3_game_1 exited with code 137`
      * GET MORE RAM!

### Docker versions

These versions were used during testing as a host:

```bash
$ uname -a
Linux ubuntu 4.4.0-31-generic #50~14.04.1-Ubuntu SMP Wed Jul 13 01:07:32 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux
$ docker-compose version
docker-compose version 1.19.0, build 9e633ef
docker-py version: 2.7.0
CPython version: 2.7.13
OpenSSL version: OpenSSL 1.0.1t  3 May 2016
$ docker --version
Docker version 17.12.1-ce, build 7390fc6
```
