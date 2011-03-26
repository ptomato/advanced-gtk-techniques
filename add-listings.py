#!/usr/bin/env python

import re, fileinput

regex = re.compile('<!--{{{(?P<file>.*):(?P<start>\d+)(?:-(?P<end>\d+))?}}}-->')

for line in fileinput.input():
	m = regex.match(line)

	# Pass the line through if it didn't match the special comment syntax
	if m is None:
		print line,

	# Otherwise, replace it with lines of code from the program
	else:
		with open(m.group('file')) as f:
			lines = f.readlines()

		start = int(m.group('start')) - 1
		if m.group('end') is not None:
			end = int(m.group('end'))
		else:
			end = start + 1

		print '<code mime="text/x-csrc"><![CDATA['
		for l in lines[start:end]:
			print l,
		print ']]></code>'
