#!/bin/bash

set -e

# This script is run by the CI setup to autoupdate our documentation

git config --global user.name "$GIT_USERNAME"
git config --global user.email $GIT_EMAIL
mkdir -p api_docs
git clone -b gh-pages git://github.com/robojackets/robocup-firmware api_docs/html
make apidocs
cd api_docs/html
git add --all
git commit -m 'auto-updated api docs'
git push https://$GH_TOKEN@github.com/robojackets/robocup-firmware gh-pages
