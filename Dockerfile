FROM debian:stable
ENV ROOT /opt/cflakes
WORKDIR $ROOT
RUN apt-get update -y && apt-get install -y \
  build-essential \
  curl
COPY . $ROOT
RUN make
CMD $ROOT/bin/cflakes
