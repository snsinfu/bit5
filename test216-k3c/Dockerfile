FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
 && apt-get install -y --no-install-recommends net-tools \
 && apt-get clean

CMD ["/bin/sleep", "infinity"]
