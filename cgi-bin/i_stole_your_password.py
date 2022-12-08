#!/usr/bin/python3
import os

pairs = os.environ["QUERY_STRING"].split("&")
first_name = pairs[0].split("=")[1]
password = pairs[1].split("=")[1]

print ("<html><head><title>Sucker</title>")
print ("</head>")
print ("<body>")
# print("os.environ["QUERY_STRING"]")
print(f"<h1>I stole your password, {first_name}!</h1><h2> It's {password} sucker!!!</h2>")
# for key, val in os.environ.items():
# 	print(f"<h2>Key = {key}, Value = {val}\n</h2>")
print ("</body>")
print ("</html>")