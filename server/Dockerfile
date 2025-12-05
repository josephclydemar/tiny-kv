FROM ubuntu:22.04 AS build

RUN apt update && \
  apt install -y build-essential && \
  apt clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN make release


FROM ubuntu:22.04

WORKDIR /app

COPY --from=build /app /app

EXPOSE 5800

CMD [ "/app/build/source.out" ]
