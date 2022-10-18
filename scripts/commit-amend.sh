#!/usr/bin/env bash

cd ..
git add .
git commit --amend --no-edit -a
git push -f
