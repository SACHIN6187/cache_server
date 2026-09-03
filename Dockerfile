FROM gcc:latest

WORKDIR /app

COPY . .

RUN g++ -std=c++17 server.cpp cache.cpp -o cache_server

CMD ["./cache_server"]