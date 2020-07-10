FROM alpine:3.7
COPY src/ /usr/share/
COPY Makefile /usr/share/
RUN ls /usr/share/
WORKDIR /usr/share/
RUN make
RUN ls /usr/share/