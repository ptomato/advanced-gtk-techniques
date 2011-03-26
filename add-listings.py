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

		lines = lines[start:end]

		# Remove any indentation common to the whole block
		while all([l.startswith(' ') for l in lines if l != '\n']) \
			or all([l.startswith('\t') for l in lines if l != '\n']):
			lines = [l[1:] for l in lines if l != '\n']

		print '<code mime="text/x-csrc"><![CDATA['
		for l in lines:
			print l,
		print ']]></code>'
