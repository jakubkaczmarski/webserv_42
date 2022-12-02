#!/usr/bin/env python3

import os

#oppening the html for reading
file = open("/workspaces/webserv_42/database/default_index_.html", "r")

# #initialising an empty string, getting the intra login, prepraing the path
replaced_content = ""
intra = os.environ["QUERY_STRING"].split('=')[1]
print(intra)
# new_path = "/home/kis619/webserv_42/database/intraPictures" + intra + '.jpg'

# #looping through the file
# for line in file:

#     #replacing the texts
#     new_line = line.replace("/database/UFF.png", new_path)

#     #concatenate the new string and add an end-line break
#     replaced_content = replaced_content + new_line

    
# #close the file
# file.close()

# #Open file in write mode
# path = "./cgi-bin/" + intra + ".html"
# write_file = os.open(	
# 						path,
# 						os.O_RDWR | os.O_CREAT,
# 						0o777
# 					)
# write_file = open(write_file, "a+")

# print(write_file)

#overwriting the old file contents with the new/replaced content

# write_file.write(replaced_content)

print(os.environ)
print("Content-type: text/html")
# print(replaced_content)
#close the file
# print(write_file.fileno())
# write_file.close()