# CPP Project

News-server and client implementation of C++ group project for LTH course `EDAF50`. Members:
- Victor Truong ()
- ...
- ...
- ...

## building with make

Running `make` (equivalent to `make all`) in the main directory builds the the server and client into executables that can be found in `bin/` 
- `make server` - builds the two server executables (in-memory and disk)
- `make client` - builds the client executable
- `make test_db` - unit tests for database
- `make error_client` - faulty client for testing protocol violations

For cleaning, use `make clean`.

## running the application

First, open two terminal windows, one for the server and one for the client.

In the first one, start the server with `bin/server_mem <port>` (for in-memory storage) or `bin/server_disc <port>` (for persistent disk storage), e.g.,

```
bin/server_mem 1337
# OR:
bin/server_disc 1337
```

In the other one, start the client with `client <server> <port>`, e.g.,

```
bin/client localhost 1337
```
- Multiple client instances can be active at the same time, open other terminal windows for more clients and run the same command for each.