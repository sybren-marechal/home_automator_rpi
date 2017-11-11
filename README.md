Home Automator for RPi 3


# Docker

## build

docker build -t home_automator .

or force rebuild

docker build --no-cache -t home_automator .

## running

docker run -d --privileged --restart unless-stopped --name automator home_automator
