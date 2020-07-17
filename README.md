# guitar-solo-part-generator

[![Docker Pulls](https://img.shields.io/docker/pulls/sea5kg/guitar-solo-part-generator.svg)](https://hub.docker.com/r/sea5kg/guitar-solo-part-generator/) [![Github Stars](https://img.shields.io/github/stars/sea-kg/guitar-solo-part-generator.svg?label=github%20%E2%98%85)](https://github.com/sea-kg/guitar-solo-part-generator) [![Github Stars](https://img.shields.io/github/contributors/sea-kg/guitar-solo-part-generator.svg)](https://github.com/sea-kg/guitar-solo-part-generator) [![Github Forks](https://img.shields.io/github/forks/sea-kg/guitar-solo-part-generator.svg?label=github%20forks)](https://github.com/sea-kg/guitar-solo-part-generator/network/members)

Guitar Solo Part Generator

## How to run latest from docker

Example docker-compose.yml 
```
version: '3'

services:
  guitar_solo_part_generator_service:
    image: sea5kg/guitar-solo-part-generator:latest
    environment:
      - GSPG_PORT=1234 # default port 1234
      - GSPG_WEB=/home/user0/web # default web folder
    expose:
      - "1234"
    volumes:
      - "./guitar-solo-part-generator-logs:/home/user0/.logs"
    ports:
      - "1234:1234"
    restart: always
    networks:
      - guitar_solo_part_generator_net

networks:
  guitar_solo_part_generator_net:
    driver: bridge
```

Command:
```
docker-compose up
```

Now you can see web on page: `http://localhost:1234/`

## API:

* GET `http://localhost:1234/api/v1/solo-generate/` - guitar solo part generate, response will be returned json format


