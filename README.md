# BittrexCrawler

## docker
```bash
     docker run -d -p 27017:27017 -v ${PWD}/data:/data/db mongo
```

### dependencies
1. apt-get install python python-pip
2. pip install requests pyfunctional pymongo

for cpp
1. git submodule update --init --recursive

### cron installation

clone repo into opt dir
```bash
cd /opt && git clone https://github.com/mrowacz/BittrexCrawler.git
```
and add cron position
```bash
*/2 * * * * /usr/bin/python /opt/BittrexCrawler/BittrexCrawler.py
```

