FROM ubuntu:24.04 as builder 

RUN apt update && apt install -y build-essential cmake python3 pipx
RUN pipx ensurepath && pipx install conan

ENV PATH="/root/.local/bin:${PATH}"

RUN conan profile detect --force


WORKDIR /app 
COPY conanfile.py .
COPY cmake/ ./cmake/
COPY src/ ./src/
COPY CMakeLists.txt .
COPY tests/ ./tests/

WORKDIR /app/build

RUN conan install .. --build=missing -of=. -s build_type=Release -s compiler.cppstd=20
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE="./build/Release/generators/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
RUN grep CXX_FLAGS CMakeFiles/BookingCore.dir/flags.make
RUN cmake --build . -j$(nproc)

FROM ubuntu:24.04 as runner

RUN apt update

WORKDIR /app 

COPY --from=builder /app/build/BookingServer .

EXPOSE 8080


CMD [ "./BookingServer" ]