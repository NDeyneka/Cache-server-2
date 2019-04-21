# Cache-server-2
Cache server based on libevent library.

# 1. AСKNOWLEDGEMENT

Used vesrion of libevent library (https://github.com/libevent/libevent, version from 21.04.2019).
Contents from the link above used locally in directory libevent-master.

# 2. BUILDING AND INSTALLATION

1. Download all files.

2a. Open solution in a folder libevent-biuld and build solution.

2b. If any problem occures at this stage, try to rebuild solution from files in libevent-master.
- Remove all files in libevent-biuld directory.
- Open command line in libevent-biuld directory.
- Run command 
```
cmake -DEVENT__DISABLE_OPENSSL=on -G "Visual Studio 14 2015" ../libevent-master
```

3a. Open solution in a folder CachedServer and try to run solution.

3b. In case of runtime error try to copy dlls builded in project libevent-biuld to project CachedServer
(bin\Debug to Debug, bin\Release to Release).

