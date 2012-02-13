#!/usr/bin/env python
#coding: utf8
"""
Creates a static HTML page listing the contents of a directory.
"""

import os
import os.path
import codecs

header = \
u"""<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
                         "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
  <title>Index of {thisdir}</title>
  <style type="text/css">
body {{
	font-family: sans-serif;
	max-width: 600px;
	margin: 0 auto;
}}
a {{
	text-decoration: none;
	color: #1f609f;
}}
a:visited {{
	color: #9f1f6f;
}}
a:hover {{ 
	text-decoration: underline; 
}}
h1 {{
	border-bottom: solid 1px;
	border-width: thin;
	border-color: #babdb6;
}}
p.linkup::before {{
	content: '\N{black up-pointing triangle} ';
}}
li.dir::before {{
	content: '\N{black right-pointing triangle} ';
}}
ul {{
	list-style-type: none;
}}
  </style>
</head>
<body>
  <h1>Index of {thisdir}</h1>
  <p class="linkup">
    <a href="{updir}">Up to higher level directory</a>
  </p>
  <ul>
"""

item = \
"""<li class="{cls}">
  <a href="{link}">{name}</a>
</li>
"""

footer = """
</ul>
</body>
</html>"""

def output_html(topdir):
	for thisdir, dirnames, filenames in os.walk(topdir):
		for dirname in dirnames:
			output_html(os.path.join(thisdir, dirname))
		
		html_outfile = os.path.join(thisdir, 'index.html')
		with codecs.open(html_outfile, 'w', encoding='utf-8') as fp:
			parent_path = os.path.join(thisdir, '..')
			fp.write(header.format(thisdir=thisdir, updir=parent_path))
			for name in dirnames + filenames:
				filepath = os.path.join(thisdir, name)
				fp.write(item.format(name=name, link=filepath,
					cls="file" if name in filenames else "dir"))
			fp.write(footer)

if __name__ == '__main__':
	for name in os.listdir('.'):
		if os.path.isdir(name) and name != 'html' and not name.startswith('.'):
			output_html(name)

