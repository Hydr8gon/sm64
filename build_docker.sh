#!/bin/sh
docker build -t sm64dsi .
docker run --rm --mount type=bind,source="$(pwd)",destination=/sm64 sm64dsi make VERSION=us -j$(nproc) "$@"
