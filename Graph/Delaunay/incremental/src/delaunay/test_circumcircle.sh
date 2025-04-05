#!/usr/local/bin/zsh

cc \
  -Wall -Wextra -Wpedantic \
  -I../../include \
  -DTEST_CIRCUMCIRCLE \
  circumcircle.c \
  -o a.out \
  -lm
