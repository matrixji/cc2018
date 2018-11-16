#!/bin/sh

export PATH=${PATH}:/opt/build-wrapper-linux-x86:/opt/sonar-scanner-3.2.0.1227-linux/bin
build-wrapper-linux-x86-64 --out-dir bw-output make clean all

sonar-scanner \
  -Dsonar.projectKey=matrixji_cc2018 \
  -Dsonar.organization=matrixji-github \
  -Dsonar.sources=. \
  -Dsonar.cfamily.build-wrapper-output=bw-output \
  -Dsonar.host.url=https://sonarcloud.io \
  -Dsonar.login=d9cb21898d743e04a394a8fd45098f7c7d846c1c

