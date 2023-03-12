FROM ubuntu:latest

VOLUME /data/

RUN apt-get update \
    && apt-get install -y cmake git \
    && apt-get clean -y

RUN git clone https://github.com/lawlient/notes && \
    cd notes/log/ && mkdir build && cd build && cmake .. && make -j$(nproc) && make install && \
    cd - && rm build -rf

CMD ["/data/services/log_agent/bin/log_agent"]
