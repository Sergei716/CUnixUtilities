FROM ubuntu:24.04

RUN apt update && \
    apt install vim -y && \
    apt install build-essential -y

WORKDIR /project

RUN mkdir -p src/cat src/grep

COPY src/cat src/cat/.
COPY src/grep src/grep/.
COPY src/Makefile src/.
COPY README.md .
COPY dockerfile .

CMD ["bash"]
