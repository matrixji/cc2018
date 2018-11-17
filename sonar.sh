#!/bin/sh

if test ! -f main_public.cpp  ; then
    echo please run under sub-folder with \"sh ../sonar.sh\"
    exit 1
fi

export PATH=${PATH}:/opt/build-wrapper-linux-x86:/opt/sonar-scanner-3.2.0.1227-linux/bin
build-wrapper-linux-x86-64 --out-dir bw-output make clean test_main

sonar-scanner \
  -Dsonar.projectKey=cpp-sonar-way-with-cppcheck-63097 \
  -Dsonar.sources=main_public.cpp \
  -Dsonar.cfamily.build-wrapper-output=bw-output \
  -Dsonar.host.url=https://sonarqube.int.net.nokia.com \
  -Dsonar.login=ab8b981ae7fac78c1315a6d9667b45c9361e11c4 \
  -Dsonar.analysis.mode=preview -Dsonar.issuesReport.html.enable=true \
  -Dsonar.projectName=biji-cc-test -Dsonar.projectVersion=1 \
  -X


