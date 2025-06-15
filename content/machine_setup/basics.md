# Basics

## apt

```sh
sudo apt update
sudo apt upgrade
```

## python and pip

```sh
sudo apt install -y \
  python3.12 \
  python3-pip
```

Make sure python is installed, and that you can create virtualenvs:
```sh
# python installation sanity
$ python -V
Python 3.12.3
$ pip -V
pip 24.0 from /usr/lib/python3/dist-packages/pip (python 3.12)

# working with virtualenv
$ python -m venv /tmp/foo
$ source /tmp/foo/bin/activate.fish  # no `.fish` if using bash or zsh
(foo) $ which python
/tmp/foo/bin/python
(foo) $ pip install rich
Collecting rich
[...]
(foo) $ deactivate
$ rm -r /tmp/foo
```

## Common Utils
```sh
sudo apt install -y \
  git \
  moreutils \
  sublime-text \
  pipx \
  batcat \
  micro \
  fzf \
  plocate
```

## CI Tools
```sh
sudo apt install -y \
  gitlab-ci-multi-runner \
  jenkins \
  travis \
  circleci-cli
```

## Other Tools
```sh
sudo apt install -y \
  docker.io \
  docker-compose \
  kubectl \
  minikube
```

## IDEs
TODO Pycharm, CLion, VisualStudio, Sublime
```