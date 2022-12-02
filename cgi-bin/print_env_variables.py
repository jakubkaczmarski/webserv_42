#!/usr/bin/env python3
import os

pairs = os.environ["QUERY_STRING"].split("&")
first_name = pairs[0].split("=")[1]
second_name = pairs[1].split("=")[1]

print ("<html><head><title>Environment variables</title>")
print ("</head>")
print ("<body>")
print(f"<h1>First Name = {first_name}, Seventh Name = {second_name}\n\n\n</h1>")
for key, val in os.environ.items():
	print(f"<h2>Key = {key}, Value = {val}\n</h2>")
print ("</body>")
print ("</html>")