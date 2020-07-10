FROM alpine:3.7
RUN apk add --update make
COPY src/ /usr/share/src/
COPY Makefile /usr/share/
WORKDIR /usr/share/
RUN make
RUN ls -R /usr/share/