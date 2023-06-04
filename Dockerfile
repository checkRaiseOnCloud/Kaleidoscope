FROM ubuntu:20.04

RUN apt update
RUN apt install -y wget
RUN apt install -y lsb-release wget software-properties-common gnupg
RUN bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
RUN apt install cmake -y
RUN apt install zlib1g-dev
RUN apt install -y gdb


ENTRYPOINT ["/bin/bash"]