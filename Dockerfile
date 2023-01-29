#building build enviromental
FROM ubuntu:20.04 AS log_collector_build_env
ARG DEBIAN_FRONTEND=noninteractive
RUN echo "deb http://security.ubuntu.com/ubuntu focal-security main" >>  /etc/apt/sources.list.d/focal-security.list
RUN apt-get update &&  \
    apt-get install -y  make  \
                        apt-utils\
                        cmake \
                        g++ \
                        gcc \
                        automake \
                        build-essential \
                        libssl-dev \
                        libssl1.1 \
                        gdb

RUN echo /usr/local/lib > /etc/ld.so.conf.d/usr_local.conf

RUN mkdir -p /tmp/build_deps

COPY libs/mysql         /tmp/build_deps/mysql
COPY libs/sqlite        /tmp/build_deps/sqlite
COPY libs/soci          /tmp/build_deps/soci
COPY libs/boost_1_81_0  /tmp/build_deps/boost

#building sqlite3
RUN cd /tmp/build_deps/sqlite && ./configure && make install

#installing mysql
RUN cd /tmp/build_deps/mysql && dpkg -i mysql-common_8.0.31-1ubuntu20.04_amd64.deb && \
                                dpkg -i mysql-community-client-plugins_8.0.31-1ubuntu20.04_amd64.deb && \
                                dpkg -i libmysqlclient21_8.0.31-1ubuntu20.04_amd64.deb && \
                                dpkg -i libmysqlclient-dev_8.0.31-1ubuntu20.04_amd64.deb
RUN mv /usr/include/mysql /usr/include/mysql_ && cp -rf /usr/include/mysql_/* /usr/include/

#installing postgresql
RUN apt-get install -y libpq-dev
RUN mv /usr/include/postgresql/ /usr/include/postgresql_ && cp -rf /usr/include/postgresql_/* /usr/include/

#building SOCI
RUN mkdir -p /tmp/build_deps/soci/build && cd /tmp/build_deps/soci/build && cmake ../ && make install

#building boost
RUN cd /tmp/build_deps/boost && ./bootstrap.sh && ./b2 --with=all -j 8 install

#building app
FROM log_collector_build_env as log_collector_building
RUN mkdir -p /usr/src/log_collector
COPY src /usr/src/log_collector
WORKDIR /usr/src/log_collector
RUN mkdir -p build && cd build && ls -la ../ && cmake ../ && make


FROM scratch AS log_collector_packages_export
COPY --from=log_collector_building /usr/src/log_collector/build/log_collector/testbin1/testbin1 /

