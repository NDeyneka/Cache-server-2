# Cache-server-2

Cache server based on libevent library.

## Features and limitations

Cache server can save cached data and load cached data.

### Interaction protocol

There are three types of requests in the interaction protocol.
- GET - load cached data from server by key. Format: GET KEY=\<key\> 
	where \<key\> is any sequence of non-spacing characters (ASCII 33-126).
- PUT - save cache on a server. Format: PUT KEY=\<key\> VALUE=\<value\> TTL=\<ttl\> 
	where \<key\> is any sequence of non-spacing characters (ASCII 33-126), 
	\<value\> is any sequence of non-spacing characters (ASCII 33-126),
	\<ttl\> is time to live for value in seconds.
- EXIT - special query for closing session.

## Getting started

1. Download all files.

2a. Open solution in a folder libevent-biuld and build solution.

2b. If any problem occures at this stage, try to rebuild solution from files in libevent-master.
- Remove all files in libevent-biuld directory.
- Open command line in libevent-biuld directory.
- Run command 
```
cmake -DEVENT__DISABLE_OPENSSL=on -G "Visual Studio 14 2015" ../libevent-master
```

3. Open solution in a folder CachedServer and run solution.

## Acknowledgements

Used vesrion of libevent library (https://github.com/libevent/libevent, version from 21.04.2019).
Contents from that link were saved locally in directory libevent-master.

## License

This project is licensed under the MIT License.

## Author

Timur Sitdikov