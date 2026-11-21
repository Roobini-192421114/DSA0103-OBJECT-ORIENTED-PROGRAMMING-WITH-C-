#!/usr/bin/env bash
# Creates a repository with one commit per task (the assignment requires progressive commits).
# Better: run the relevant commit yourself right after you finish each task.
# Usage: ./git_commit_each_task.sh  &&  git remote add origin <your-url> && git push -u origin main
set -e
[ -d .git ] || git init -b main
git add README.md .gitignore run_all.sh;                         git commit -m "Add README and run script" || true
for t in A2 A3 A4 A5 B1 B2 B3 B4; do
  git add Task_$t;                                               git commit -m "Task $t: code, outputs, notes" || true
done
git add diagrams report 2>/dev/null || true;                      git commit -m "Add diagrams and report" || true
git log --oneline
