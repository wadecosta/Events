#!/usr/bin/env sh

[ -d "../data/" ] && echo "Directory exists."

[ ! -d "../data" ] && echo "Directory doesn't exists." && mkdir ../data/
