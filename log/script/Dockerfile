FROM ubuntu:latest

RUN apt-get update \
    && apt-get install -y cmake git \
    && apt-get clean -y

RUN git clone https://github.com/lawlient/notes && \
    cd notes/log/ && mkdir build && cd build && cmake .. && make -j$(nproc) && make install && \
    cd / && rm notes -rf

CMD ["log_agent"]
