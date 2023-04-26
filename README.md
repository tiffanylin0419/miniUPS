# UPS

settings.py改密碼
server.cpp改amazon vcm


## Install

```
sudo apt install gcc
sudo apt install g++
sudo apt install make 
sudo apt install emacs
sudo apt install postgresql postgresql-contrib 
sudo apt-get install libpqxx-dev
sudo apt-get install libpugixml-dev
```

## World setup
```
sudo apt-get install build-essential zlib1g-dev
```
[download world](https://github.com/protocolbuffers/protobuf/releases/tag/v22.2)

[google protocal setup](https://edstem.org/us/courses/32653/discussion/2915749)

```
sudo apt-get install pkg-config
```

[generate code from proto](https://protobuf.dev/reference/cpp/cpp-generated/#string )



## Run

go to world folder and execute `sudo docker-compose up`

go to Amazon folder and execute `./main`

go to UPS folder and execute `./main`


## concepts of command and response
Set: responses that was processed that should not be handled again after received

Map: commands that has not receive ack that requires continue send

List: ack of response that need to send to world

* send:     
    * List: send, remove
    * Map: send
* receive: 
    * ack: remove from Map
    * response: 
        * in set: do nothing, add ack to list
        * not in set: do it and add to set, add ack to list