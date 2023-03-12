FROM ubuntu:latest

VOLUME /data/

RUN git clone --depth=1 https://github.com/lawlient/notes && \
    cd notes/log/ && mkdir build && cd build && cmake .. && make -j$(nproc) && make install && \
    cd - && rm build -rf

CMD ["/data/services/log_agent/bin/log_agent"]
