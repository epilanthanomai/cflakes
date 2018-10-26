Copyright 2018, Hope Ranker

`cflakes` makes svg snowflakes. We use an algorithm based closely on [Reiter
2005](http://www.patarnott.com/pdf/SnowCrystalGrowth.pdf).

```
$ make
$ bin/cflakes > snowflake.svg
```
or
```
$ docker run epilanthanomai/cflakes > snowflake.svg
```

Please be patient. With default options execution takes around 30s on my
2015 workstation.

Command line options are poorly documented, but you can see them
in the program [usage string](https://github.com/epilanthanomai/cflakes/blob/develop/src/cflakes.c#L30).
