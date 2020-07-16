```console
$ docker-compose up
$ docker-compose exec influxdb /bin/bash

# cat > data.txt
...
^D
# influx -import -path=data.txt
# influx
> show databases
> use somedatabase
> show series
> select * from someseries
```
