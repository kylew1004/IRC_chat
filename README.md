# This is IRC Chat Server Project

> Made by: KyungYub Lew

## 1. Introduction

This is a simple chat server project that can be used by multiple clients. The server is written in C++. You can connect to the server using the client program like Irssi, nc.

## 2. How to use

### 2.1. Server

1. Compile the server program

```bash
$ make
```

2. Run the server program

```
./ircserv <port> <password>
```

### 2.2. Client

1. Connect to the server using the client program

```bash
$ apt install irssi
$ irssi
```

2. Connect to the server

```
/connect localhost <port> <password>
```

## 3. Features

```
JOIN <channel> : Join the channel

PART <channel> : Leave the channel

NICK <nickname> : Change the nickname

LIST : List all the channels

WHO <channel> : List all the users in the channel

PRIVMSG <channel> <message> : Send a message to the channel

PRIVMSG <nickname> <message> : Send a message to the user

TOPIC <channel> <topic> : Change the topic of the channel

KICK <channel> <nickname> : Kick the user from the channel

MODE <channel> <mode> : Change the mode of the channel

- <mode> : +o <nickname> : Give the operator to the user
- <mode> : -o <nickname> : Take the operator from the user
- <mode> : +v <nickname> : Give the voice to the user
- <mode> : -v <nickname> : Take the voice from the user
- <mode> : +b <nickname> : Ban the user
- <mode> : -b <nickname> : Unban the user
- <mode> : +k <password> : Set the password to the channel
- <mode> : -k <password> : Remove the password from the channel
- <mode> : +l <limit> : Set the limit of the channel
- <mode> : -l <limit> : Remove the limit of the channel
- <mode> : +i : Set the invite only mode
- <mode> : -i : Remove the invite only mode
```

## 4. Demo

![IRC_chat_2](https://github.com/kylew1004/IRC_chat/assets/5775698/55221881-e1ae-4014-adb3-b389f5a07513)
