# Container for building and testing cmake projects v3.16
FROM ubuntu:18.04
MAINTAINER Arvind Rao



# get the latest cmake release
RUN apt-get -y update
RUN apt-get -y install software-properties-common git wget clang-10
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add -
RUN apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'

RUN apt-get -y install build-essential cmake qtbase5-dev

COPY entrypoint.sh /entrypoint.sh
RUN ls -l
RUN find ./ trie.h
RUN echo "$GITHUB_WORKSPACE"
COPY "$GITHUB_WORKSPACE" /hamt
RUN chmod +x entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]