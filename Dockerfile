FROM archlinux:latest

# VOLUME output

RUN pacman --noconfirm -Sy \
    llvm \
    cmake \
    gcc \
    antlr4 \
    antlr4-runtime \
    jdk-openjdk \
    make \
    lld

COPY . .

RUN cmake . && make

CMD [ "cp", "./fcc", "/output" ]
