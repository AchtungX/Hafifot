# Git

Clone the [git exercise repo](https://github.com/AchtungX/git-exercise).
For each exercise N, start from the `ex<N>-start` branch and bring the repo to the same state as in `ex<N>-end`.

To compare the repo's state, compare both `git log --online <branch-name>`, and `git show <commit-hash>` for all existing commits.
You only need to match commit names and content. No need to replicate commit hashes, author details, etc.

!!! idea "Example"
	For exercise 1, compare:
	```
	$ git log --oneline ex2-start
	d03fbc2 file: Add the second half of line 4
	9e01750 file: Add half of line 4
	34332ae file: Add line 3
	...

	$ git log --oneline ex2-end
	5c5162d file: Add line 4
	34332ae file: Add line 3
	...
	```

	Also use `git show` on all commits, e.g. `git show d03fbc2`.

	Alternatively, you can view the commits using github's web interface.

!!! tip
	There are currently 5 exercises. The following commands might help.

	Exercise 1: `git commit --amend`

	Exercise 2: `git rebase --interactive`, with squash and reword commands

	Exercise 3: `git commit --fixup` and `git rebase --interactive --autosquash`, or 
	the fixup `edit` command

	Exercise 4: `git reset --soft`, `git restore --staged`, `git add --patch`

	Exercise 5: `git blame -L<lines>` or `git bisect` (recommended: try both ways!)

