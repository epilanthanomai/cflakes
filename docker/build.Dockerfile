FROM alpine:3.8
RUN apk update &&\
    apk add --update \
      build-base \
      curl \
      docker