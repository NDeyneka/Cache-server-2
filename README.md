# Cache-server-2

Cache server based on libevent library.

## Features and limitations

Cache server can save cached data and load cached data.

### Interaction protocol

There are three types of requests in the interaction protocol.
- GET - load cached data from server by key. Format: GET KEY=\<key\> 
	where \<key\> is any nonempty sequence of non-spacing characters (ASCII 33-126).
- PUT - save cache on a server. Format: PUT KEY=\<key\> VALUE=\<value\> TTL=\<ttl\> 
	where \<key\> is any nonempty sequence of non-spacing characters (ASCII 33-126), 
	\<value\> is any sequence of non-spacing characters (ASCII 33-126),
	\<ttl\> is time to live for value in seconds.
- EXIT - special query for closing session.

Samples of correct requests:
```
GET KEY=1
PUT KEY=1 VALUE=459abcd TTL=500
EXIT
```

There are several types of responses:
- ERROR|\<Error description\> - occures if given request has invalid format.
	\<Error description\> is a string containing a brief description of while error occured.
- GET FAILURE|\<Failure description\> - occures only for GET requests, when request fails (no such value, etc.)
	\<Failure description\> is a string containing a brief description of while failure occured.
- GET SUCCESS|\<Cached data\> - occures only for GET requests, when request succeeds.
	\<Cached data\> is a string containing cached data.
- PUT SUCCESS|\<Description\> - occures only for PUT requests.
	\<Description\> is a string containing description of data saving from server.
	
Samples of server responses:
```
ERROR|Incorrect request type (must be GET, PUT or EXIT).
GET FAILURE|Value does not exist.
GET SUCCESS|459abcd
PUT SUCCESS|Value successfully saved.
```

Use netcat (nc) or similar utilites to produce such requests.

### Limitations

- Cache storage is based on a simple hand-written hash-table which does not handle collisions.
	There is high probability of collisions in case of ~1000 distinct keys in PUT requests.
- Time to live must well fit in C++ integer data type. Values exceeding one billion are undesirable.

## Getting started

### Run cache server from solution

To run a solution follow steps listed below:

1. Download all files.

2. Open solution in a folder libevent-biuld and build solution.

If any problem occures at this stage, try to rebuild solution from files in libevent-master.
- Remove all files in libevent-biuld directory.
- Open command line in libevent-biuld directory.
- Run command 
```
cmake -DEVENT__DISABLE_OPENSSL=on -G "Visual Studio 14 2015" ../libevent-master
```

3. Open solution in a folder CachedServer and run solution.

4. Install nc program. Open command line and type a command.

```
nc localhost 9876
```

Type following requests in nc:

Request                                  | Response
-----------------------------------------|---------------------------------------------------------
**GET** *KEY=1*                          | **GET FAILURE**\|Value does not exist.
**PUT** *KEY=1 VALUE=459abcd TTL=30*     | **PUT SUCCESS**\|Value successfully saved.
**GET** *KEY=1*                          | **GET SUCCESS**\|459abcd

If you try GET command after 30 seconds (30 is used is PUT request TTL value) you will receive GET FAILURE response as time to live passed.

Request                                  | Response
-----------------------------------------|---------------------------------------------------------
**GET** *KEY=1*                          | **GET FAILURE**\|Value does not exist.

### Unit tests

In the solution above there is a project for unit tests (CacheServerUnitTest).
The project is based on Microsoft Visual Studio C++ Native Unit Test Project.
Unit tests may be run is a classical way using Visual Studio Test Explorer.

### Batch tests 

TODO

## Acknowledgements

Used vesrion of libevent library (https://github.com/libevent/libevent, version from 21.04.2019).
Contents from that link were saved locally in directory libevent-master.

## License

This project is licensed under the MIT License.

## Author

Timur Sitdikov