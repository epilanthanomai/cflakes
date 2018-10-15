FROM alpine:3.8
ENV ROOT /opt/cflakes
WORKDIR $ROOT
COPY bin/cflakes $ROOT/bin/cflakes
CMD $ROOT/bin/cflakes
