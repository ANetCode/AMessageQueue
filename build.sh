#!/bin/bash
autoreconf --install
./configure && make -j8
