FROM alpine:3.8 AS builder
ENV ROOT /opt/cflakes
WORKDIR $ROOT
RUN apk update &&\
    apk add --update \
      build-base \
      curl \
      docker
COPY . $ROOT
RUN make

FROM alpine:3.8
ENV ROOT /opt/cflakes
WORKDIR $ROOT
COPY --from=builder $ROOT/bin/cflakes bin/cflakes
CMD ["bin/cflakes"]
