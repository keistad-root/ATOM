#!/bin/bash

# 작업 디렉토리로 이동
cd /home/ychoi/ATOM

# 변경 사항이 있는지 확인
if [[ `git status --porcelain` ]]; then
  # Stage all changes
  git add .

  # Commit
  git commit -m "auto commit at $(date +'%Y-%m-%d %H:%M:%S')"

  # push
  git push origin main
fi