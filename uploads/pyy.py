
cache-control: no-cache
Postman-Token: e024e638-d317-4627-a3b3-dc99fea1346c
Content-Type: text/plain
User-Agent: PostmanRuntime/7.3.0
Accept: */*
Host: localhost:1999
accept-encoding: gzip, deflate
content-length: 936
Connection: keep-alive

#!/usr/bin/python3 -u
#
# Flag is in a file called "flag" in cwd.
#
# Quote from Dockerfile:
#   FROM ubuntu:22.04
#   RUN apt-get update && apt-get install -y python3
#
import ast
import sys
import os


def verify_secure(m):
    for x in ast.walk(m):
        match type(x):
        case(ast.Import | ast.ImportFrom | ast.Call):
            print(f"ERROR: Banned statement {x}")
            return False
    return True


abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

print("-- Please enter code (last line must contain only --END)")
source_code = ""
while True:
    line = sys.stdin.readline()
    if line.startswith("--END"):
        break
