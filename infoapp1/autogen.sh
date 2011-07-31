#!/bin/bash

echo "Regenerating autotools files"
autoreconf --force --install || exit 1

echo "Setting up Intltool"
intltoolize --copy --force --automake || exit 1

