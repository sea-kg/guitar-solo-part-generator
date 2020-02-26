# for-build wsjcpp:v0.0.1
FROM debian:10
# install build requiremenets
RUN apt-get update && apt-get install -y \
  make\
  cmake\
  gcc\
  g++\
  pkg-config

RUN apt-get clean

WORKDIR /root/sources

COPY . /root/sources
RUN mkdir -p tmp.docker && cd tmp.docker && cmake .. && make

FROM debian:10
# install release requiremenets
RUN apt-get update && apt-get install -y \
  locales\
  libpthread-stubs0-dev

RUN sed -i -e "s/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/" /etc/locale.gen && \
  echo 'LANG="en_US.UTF-8"'>/etc/default/locale && \
  dpkg-reconfigure --frontend=noninteractive locales && \
  update-locale LANG=en_US.UTF-8

RUN apt-get clean

RUN useradd -ms /bin/bash user0
WORKDIR /home/user0
USER user0
COPY --from=0 /root/sources/guitar-solo-part-generator /usr/bin/guitar-solo-part-generator
COPY ./rules-sgg.txt /home/user0/rules-sgg.txt
COPY ./gspg-config.yml /home/user0/gspg-config.yml
COPY ./web /home/user0/web
RUN mkdir -p /home/user0/.logs; chown -R user0:user0 /home/user0/.logs

# Deprecated (move to config.yml):
COPY ./rules-sgg.txt /home/user0/rules-sgg.txt
EXPOSE 1234
CMD ["guitar-solo-part-generator", "start-server"]
