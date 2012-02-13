#!/usr/bin/env python
#coding: utf8
"""
Creates a static HTML page listing the contents of a directory.
"""

import os
import os.path
import codecs
import zipfile

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
p.download::before {{
	content: '\N{downwards arrow} ';
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
  {linkup}
  <p class="download">
    <a href="{zipfile}">Download all files</a>
  </p>
  <ul>
"""

linkup = \
"""  <p class="linkup">
    <a href="{updir}">Up to higher level directory</a>
  </p>
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

def output_html(topdir, subdir=False):
	files = os.listdir(topdir)
	dirnames = [n for n in files if os.path.isdir(os.path.join(topdir, n))]
	filenames = list(set(files) - set(dirnames))
	dirnames.sort()
	filenames.remove('index.html')
	filenames.remove('contents.zip')
	filenames.sort()

	for dirname in dirnames:
		output_html(os.path.join(topdir, dirname), subdir=True)

	html_outfile = os.path.join(topdir, 'index.html')
	zip_outfile = os.path.join(topdir, 'contents.zip')

	with codecs.open(html_outfile, 'w', encoding='utf-8') as fp:
		linkup_text = linkup.format(updir='../index.html') if subdir else ''
		fp.write(header.format(thisdir=topdir, linkup=linkup_text,
			zipfile='contents.zip'))
		for name in dirnames:
			fp.write(item.format(name=name, link=name + '/index.html', 
				cls="dir"))
		for name in filenames:
			if name.startswith('.'):
				continue
			fp.write(item.format(name=name, link=name, cls="file"))
		fp.write(footer)
	
	with zipfile.ZipFile(zip_outfile, 'w') as zp:
		for root, dirs, files in os.walk(topdir):
			files.remove('index.html')
			files.remove('contents.zip')
			for name in dirs + files:
				zp.write(os.path.join(root, name))

if __name__ == '__main__':
	for name in os.listdir('.'):
		if os.path.isdir(name) and name != 'html' and not name.startswith('.'):
			output_html(name)

