# #!/usr/bin/env python
# import cgi
#
# print("<!DOCTYPE html>")
#
# form = cgi.FieldStorage()
# name = form.getvalue("name")
#
# print("<html>")
# print("<head><title>CGI Example</title></head>")
# print("<body>")
# if name:
# print(f"<h1>Hello, {name}!</h1>")
# else:
# print("<h1>No name provided.</h1>")
#
# print("</body>")
# print("</html>")
#

#!/usr/bin/env python
import cgi

print("Content-type: text/html\n")  # Content-type header for HTML response

print("<!DOCTYPE html>")
print("<html lang='en'>")
print("<head>")
print("    <meta charset='UTF-8'>")
print("    <meta name='viewport' content='width=device-width, initial-scale=1.0'>")
print("    <title>Cyberpunk CGI Result</title>")
print("    <style>")
print("        body {")
print("            background-color: #0f0f0f;")
print("            color: #00ff00;")
print("            font-family: 'Courier New', monospace;")
print("            margin: 0;")
print("            padding: 20px;")
print("            box-sizing: border-box;")
print("        }")
print("        h1 {")
print("            color: #29b6f6;")
print("        }")
print("    </style>")
print("</head>")
print("<body>")

form = cgi.FieldStorage()
name = form.getvalue("name")

if name:
    print(f"<h1>Hello, {name}!</h1>")
else:
    print("<h1>No name provided.</h1>")

print("</body>")
print("</html>")