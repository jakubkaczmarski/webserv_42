#!/usr/bin/python3
# print("Content-Type=text/html")
# print("Content-Lenght=150\r\n\r\n")
import os

name = os.environ["QUERY_STRING"].split("=")[1] 
print("<html>")
print(f"<h1> Siemanko, {name}! </h1>")

print("</html>")