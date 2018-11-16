#!/bin/sh

export PATH=${PATH}:/opt/build-wrapper-linux-x86:/opt/sonar-scanner-3.2.0.1227-linux/bin
build-wrapper-linux-x86-64 --out-dir bw-output make clean all

sonar-scanner \
  -Dsonar.projectKey=matrixji_cc2018 \
  -Dsonar.organization=matrixji-github \
  -Dsonar.sources=. \
  -Dsonar.cfamily.build-wrapper-output=bw-output \
  -Dsonar.host.url=https://sonarcloud.io \
  -Dsonar.login=28d1526d0cc974845b90d1825d4325604d5147a9 \
  -Dhttp.proxyHost=10.144.1.10 -Dhttp.proxyPort=8080 -X
