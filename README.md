# avid

## How to build

### Clone mysql-server from github
```bash
$ git clone git@github.com:mysql/mysql-server.git
$ cd mysql-server
```

### Clone this repository
```bash
mysql-server/$ cd storage/
mysql-server/storage$ git clone git@github.com:lrf141/avid.git
```

### Build MySQL Server
```bash
mysql-server$ mkdir build && cd $_
mysql-server/build$ cmake ../ -DCMAKE_BUILD_TYPE=Debug -DWITH_BOOST=./boost -DDOWNLOAD_BOOST=1
```

### (Option) Run mysql-test-run
```bash
mysql-server/storage/avid$ ./mtr/setup-test.sh
mysql-server/storage/avid$ cd ../../build/mysql-test
mysql-server/build/mysql-test$ ./mtr --suite avid
```
