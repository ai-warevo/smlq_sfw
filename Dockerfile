FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# 1. Ставим зависимости и компилируем TDLib в систему (кэшируется 1 раз)
RUN apt-get update && apt-get install -y \
    make git zlib1g-dev libssl-dev gperf cmake \
    clang-18 libc++-18-dev libc++abi-18-dev \
    && rm -rf /var/lib/apt/lists/* \
    && git clone --depth 1 https://github.com/tdlib/td.git /td_build \
    && mkdir /td_build/build && cd /td_build/build \
    && CXXFLAGS="-stdlib=libc++" CC=/usr/bin/clang-18 CXX=/usr/bin/clang++-18 cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=/usr/local .. \
    && cmake --build . --target install -j$(nproc) \
    && rm -rf /td_build

# 2. Копируем исходники и собираем через CMake


WORKDIR /app
COPY CMakeLists.txt main.cpp ./

RUN mkdir build && cd build \
    && CXXFLAGS="-stdlib=libc++" CC=/usr/bin/clang-18 CXX=/usr/bin/clang++-18 cmake -DCMAKE_BUILD_TYPE=Release .. \
    && cmake --build . -j$(nproc)

VOLUME /app/tdlib_db

CMD ["./build/lox_bot"]
